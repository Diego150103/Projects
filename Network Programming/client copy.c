#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <openssl/evp.h>
#include <cjson/cJSON.h>
#include <dirent.h>
#include <openssl/sha.h>
#include <sys/stat.h>

/*
 Converts a binary SHA-256 hash to its hexadecimal string representation.
*/
void hash_to_hex(const unsigned char *hash, unsigned int hash_len, char *hex_str) {
    for (unsigned int i = 0; i < hash_len; i++) {
        sprintf(hex_str + i*2, "%02x", hash[i]);
    }
    hex_str[hash_len*2] = '\0';
}

/*
 Computes the SHA-256 hash of a given data buffer.
 Returns 1 on success, 0 on failure.
*/
int compute_sha256(const unsigned char *data, size_t data_len, unsigned char *hash, unsigned int *hash_len) {
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if (!mdctx) { printf("Failed to create EVP_MD_CTX\n"); return 0; }
    if (EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL) != 1) {
        printf("EVP_DigestInit_ex failed\n");
        EVP_MD_CTX_free(mdctx);
        return 0;
    }
    if (EVP_DigestUpdate(mdctx, data, data_len) != 1) {
        printf("EVP_DigestUpdate failed\n");
        EVP_MD_CTX_free(mdctx);
        return 0;
    }
    if (EVP_DigestFinal_ex(mdctx, hash, hash_len) != 1) {
        printf("EVP_DigestFinal_ex failed\n");
        EVP_MD_CTX_free(mdctx);
        return 0;
    }
    EVP_MD_CTX_free(mdctx);
    return 1;
}

/*
 Computes the SHA-256 hash for an entire file, reading it in chunks.
 Returns 1 on success, 0 on failure.
*/
int compute_sha256_for_file(const char *filename, unsigned char *hash, unsigned int *hash_len) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) { printf("Error opening file: %s\n", filename); return 0; }
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if (!mdctx) { printf("Failed to create EVP_MD_CTX\n"); fclose(fp); return 0; }
    if (EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL) != 1) {
        printf("EVP_DigestInit_ex failed for file: %s\n", filename);
        EVP_MD_CTX_free(mdctx);
        fclose(fp);
        return 0;
    }
    unsigned char buf[4096];
    size_t n;
    while ((n = fread(buf,1,sizeof(buf),fp)) > 0) {
        if (EVP_DigestUpdate(mdctx, buf, n) != 1) {
            printf("EVP_DigestUpdate failed for file: %s\n", filename);
            EVP_MD_CTX_free(mdctx);
            fclose(fp);
            return 0;
        }
    }
    if (EVP_DigestFinal_ex(mdctx, hash, hash_len) != 1) {
        printf("EVP_DigestFinal_ex failed for file: %s\n", filename);
        EVP_MD_CTX_free(mdctx);
        fclose(fp);
        return 0;
    }
    EVP_MD_CTX_free(mdctx);
    fclose(fp);
    return 1;
}

/*
 Sends a multicast "query" request, blocks in select() on the UDP socket,
 receives exactly one queryResponse, parses it, and prints the table.
*/
void perform_query(int sd, int port) {
    // Build and send the query JSON
    cJSON *q = cJSON_CreateObject();
    cJSON_AddStringToObject(q, "requestType", "query");
    char *qs = cJSON_Print(q);
    cJSON_Delete(q);
    if (!qs) {
        printf("Failed to create query JSON\n");
        return;
    }
    struct sockaddr_in maddr = {0};
    maddr.sin_family = AF_INET;
    maddr.sin_port   = htons(port);
    inet_pton(AF_INET, "239.128.1.1", &maddr.sin_addr);
    sendto(sd, qs, strlen(qs), 0, (struct sockaddr*)&maddr, sizeof(maddr));
    free(qs);

    // Wait for exactly one response
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(sd, &fds);
    select(sd+1, &fds, NULL, NULL, NULL);

    if (FD_ISSET(sd, &fds)) {
        char buf[5000] = {0};
        struct sockaddr_in src; socklen_t sl = sizeof(src);
        int len = recvfrom(sd, buf, sizeof(buf)-1, 0, (struct sockaddr*)&src, &sl);
        if (len > 0) {
            buf[len] = '\0';
            cJSON *r = cJSON_Parse(buf);
            if (r) {
                cJSON *rt = cJSON_GetObjectItem(r,"requestType");
                if (rt && cJSON_IsString(rt) && strcmp(rt->valuestring,"queryResponse")==0) {
                    cJSON *arr = cJSON_GetObjectItem(r,"files");
                    if (arr && cJSON_IsArray(arr)) {
                        printf("\nFiles Available Across Servers:\n");
                        printf("------------------------------------------------\n");
                        printf("Choice | File Name             | Size     | Full Hash\n");
                        printf("------------------------------------------------\n");
                        int idx=1;
                        cJSON *fi;
                        cJSON_ArrayForEach(fi, arr) {
                            cJSON *fn = cJSON_GetObjectItem(fi,"filename");
                            cJSON *fs = cJSON_GetObjectItem(fi,"fileSize");
                            cJSON *fh = cJSON_GetObjectItem(fi,"fullFileHash");
                            if (fn&&cJSON_IsString(fn)&&
                                fs&&cJSON_IsNumber(fs)&&
                                fh&&cJSON_IsString(fh))
                            {
                                printf("%-7d| %-21s| %-9ld B| %s\n",
                                       idx++,
                                       fn->valuestring,
                                       (long)fs->valuedouble,
                                       fh->valuestring);
                            }
                        }
                        printf("------------------------------------------------\n");
                    }
                }
                cJSON_Delete(r);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage is client: <portNumber> <ipaddress> <directory>\n");
        exit(1);
    }
    int  port      = atoi(argv[1]);
    char *serverIP = argv[2];
    char *directory= argv[3];

    // Create UDP socket
    int sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0) { perror("socket"); exit(1); }
    struct sockaddr_in serv = {0};
    serv.sin_family = AF_INET;
    serv.sin_port   = htons(port);
    inet_pton(AF_INET, serverIP, &serv.sin_addr);

    // Ensure CHUNKS dir
    if (mkdir("CHUNKS",0755)==0) printf("Created CHUNKS directory\n");

    // Register each file in directory
    DIR *d = opendir(directory);
    if (!d) { perror("opendir"); exit(1); }
    struct dirent *e;
    while ((e = readdir(d)) != NULL) {
        if (!strcmp(e->d_name,".") || !strcmp(e->d_name,"..") || !strcmp(e->d_name,"CHUNKS"))
            continue;
        char path[1024];
        snprintf(path,sizeof(path), "%s/%s",directory,e->d_name);

        FILE *fp = fopen(path,"rb");
        if (!fp) { perror("fopen"); continue; }
        fseek(fp,0,SEEK_END);
        long filesize = ftell(fp);
        rewind(fp);

        const size_t CHUNK = 500*1024;
        unsigned char *buf = malloc(CHUNK);
        char **hashes = NULL;
        int numh=0;
        size_t r;
        while ((r = fread(buf,1,CHUNK,fp))>0) {
            unsigned char hbin[EVP_MAX_MD_SIZE]; unsigned int hlen=0;
            if (!compute_sha256(buf,r,hbin,&hlen)) break;
            char *hstr = malloc(hlen*2+1);
            hash_to_hex(hbin,hlen,hstr);

            char chunkfile[256];
            snprintf(chunkfile,sizeof(chunkfile),"./CHUNKS/%s",hstr);
            FILE *cf = fopen(chunkfile,"wb");
            if (cf) { fwrite(buf,1,r,cf); fclose(cf); }

            hashes = realloc(hashes,(numh+1)*sizeof(char*));
            hashes[numh++] = hstr;
        }
        free(buf); fclose(fp);

        unsigned char fbin[EVP_MAX_MD_SIZE]; unsigned int flen=0;
        compute_sha256_for_file(path,fbin,&flen);
        char fstr[flen*2+1];
        hash_to_hex(fbin,flen,fstr);

        cJSON *j = cJSON_CreateObject();
        cJSON_AddStringToObject(j,"filename",e->d_name);
        cJSON_AddNumberToObject(j,"fileSize",filesize);
        cJSON *arr = cJSON_CreateArray();
        for (int i=0;i<numh;i++){
            cJSON_AddItemToArray(arr,cJSON_CreateString(hashes[i]));
            free(hashes[i]);
        }
        cJSON_AddItemToObject(j,"chunk_hashes",arr);
        cJSON_AddStringToObject(j,"fullFileHash",fstr);

        char *js = cJSON_Print(j);
        cJSON_Delete(j);
        if (js) {
            sendto(sd,js,strlen(js),0,(struct sockaddr*)&serv,sizeof(serv));
            free(js);
        }
        free(hashes);
    }
    closedir(d);

    // Menu loop
    while (1) {
        printf("\nSelect an option:\n1. View Available Files\n2. Exit\n> ");
        int c = getchar();
        if (c=='1') {
            while (getchar()!='\n');
            perform_query(sd,port);
        }
        else if (c=='2') break;
        else {
            printf("Invalid.\n");
            while (getchar()!='\n');
        }
    }

    close(sd);
    return 0;
}