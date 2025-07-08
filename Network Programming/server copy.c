#define _DEFAULT_SOURCE
#include <cjson/cJSON.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/select.h>

// Increase MAXPEERS if needed.
#define MAXPEERS 100  // Max of peers per file.

// Structure of File 
struct FileInfo {
    char filename[100];
    long fileSize;            // New field to store file size.
    char fullFileHash[65];    
    char clientIP[MAXPEERS][INET_ADDRSTRLEN];
    int clientPort[MAXPEERS];
    int numberOfPeers;
    struct FileInfo *next;    // Pointer for linked list traversal
};

// Global head pointer for the linked list of FileInfo nodes.
struct FileInfo *fileListHead = NULL;

/*
 Register the file from a given JSON given the client's IP, port, and information.
 */
void register_file(const char *jsonString, struct sockaddr_in client_addr) {
    // Parse the JSON string.
    cJSON *json = cJSON_Parse(jsonString);
    if (!json) {
        printf("Error parsing JSON.\n");
        return;
    }
    
    // Extract "filename" and "fullFileHash" from JSON.
    cJSON *filenameItem = cJSON_GetObjectItem(json, "filename");
    cJSON *fullFileHashItem = cJSON_GetObjectItem(json, "fullFileHash");
    if (!cJSON_IsString(filenameItem) || !cJSON_IsString(fullFileHashItem)) {
        printf("Invalid JSON format.\n");
        cJSON_Delete(json);
        return;
    }
    
    // Extract fileSize from JSON.
    cJSON *fileSizeItem = cJSON_GetObjectItem(json, "fileSize");
    long fileSize = 0;
    if (fileSizeItem && cJSON_IsNumber(fileSizeItem)) {
        fileSize = (long)fileSizeItem->valuedouble;
    }
    
    const char *filename = filenameItem->valuestring;
    const char *fullFileHash = fullFileHashItem->valuestring;
    
    // Check if file is already registered.
    struct FileInfo *current = fileListHead;
    while (current != NULL) {
        if (strcmp(current->fullFileHash, fullFileHash) == 0) {
            // File already exists; check for duplicate client registration.
            char clientIPStr[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(client_addr.sin_addr), clientIPStr, INET_ADDRSTRLEN);
            int clientPort = ntohs(client_addr.sin_port);
            int alreadyRegistered = 0;
            for (int i = 0; i < current->numberOfPeers; i++) {
                if (strcmp(current->clientIP[i], clientIPStr) == 0 && current->clientPort[i] == clientPort) {
                    alreadyRegistered = 1;
                    break;
                }
            }
            if (!alreadyRegistered) {
                if (current->numberOfPeers < MAXPEERS) {
                    strcpy(current->clientIP[current->numberOfPeers], clientIPStr);
                    current->clientPort[current->numberOfPeers] = clientPort;
                    current->numberOfPeers++;
                    printf("Added new peer for file %s (hash: %s). Total peers: %d\n", 
                           filename, fullFileHash, current->numberOfPeers);
                } else {
                    printf("Max peers reached for file %s (hash: %s). Cannot add more peers.\n",
                           filename, fullFileHash);
                }
            } else {
                printf("Client already registered for file %s (hash: %s).\n", filename, fullFileHash);
            }
            cJSON_Delete(json);
            return;
        }
        current = current->next;
    }
    
    // File not found; create a new FileInfo node.
    struct FileInfo *newNode = (struct FileInfo *)malloc(sizeof(struct FileInfo));
    if (!newNode) {
        printf("Memory allocation failed for new FileInfo node.\n");
        cJSON_Delete(json);
        return;
    }
    strncpy(newNode->filename, filename, sizeof(newNode->filename) - 1);
    newNode->filename[sizeof(newNode->filename) - 1] = '\0';
    strncpy(newNode->fullFileHash, fullFileHash, sizeof(newNode->fullFileHash) - 1);
    newNode->fullFileHash[sizeof(newNode->fullFileHash) - 1] = '\0';
    newNode->fileSize = fileSize;
    newNode->numberOfPeers = 1;
    
    // Save client's IP and port.
    char clientIPStr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(client_addr.sin_addr), clientIPStr, INET_ADDRSTRLEN);
    strcpy(newNode->clientIP[0], clientIPStr);
    newNode->clientPort[0] = ntohs(client_addr.sin_port);
    
    // Insert new node at the head of the linked list.
    newNode->next = fileListHead;
    fileListHead = newNode;
    
    printf("Registered new file: %s (hash: %s, size: %ld) from client %s:%d\n", 
           filename, fullFileHash, fileSize, clientIPStr, newNode->clientPort[0]);
    
    cJSON_Delete(json);
}

/*
 Display registered files.
 */
void print_file_list(void) {
    printf("\n--- Registered Files ---\n");
    struct FileInfo *current = fileListHead;
    while (current != NULL) {
        printf("Filename: %s\n", current->filename);
        printf("Full File Hash: %s\n", current->fullFileHash);
        printf("File Size: %ld B\n", current->fileSize);
        printf("Number of Peers: %d\n", current->numberOfPeers);
        for (int i = 0; i < current->numberOfPeers; i++) {
            printf("  Peer %d: %s:%d\n", i + 1, current->clientIP[i], current->clientPort[i]);
        }
        printf("\n");
        current = current->next;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: server <portNumber> [interface]\n");
        exit(1);
    }
    
    int rc;
    int port = atoi(argv[1]);
    
    // Create UDP socket.
    int sd;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0) {
        perror("Error opening socket");
        exit(1);
    }
    
    // Allow multiple uses of the port.
    int reuse = 1;
    rc = setsockopt(sd, SOL_SOCKET, SO_REUSEPORT, (char *)&reuse, sizeof(reuse));
    if (rc < 0) {
        printf("Error setting multiple uses on port\n");
    }
    
    // Configure server address.
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
    // Bind the socket.
    rc = bind(sd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (rc < 0) {
        perror("Error binding socket");
        close(sd);
        exit(1);
    }
    
    // Set up multicast group.
    const char *multicast_group = "239.128.1.1";
    const char *iface = (argc >= 3) ? argv[2] : NULL;
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(multicast_group);
    if (iface) {
        mreq.imr_interface.s_addr = inet_addr(iface);
    } else {
        mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    }
    if (setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
        perror("setsockopt (IP_ADD_MEMBERSHIP)");
        close(sd);
        exit(-1);
    }
    
    printf("Server listening for multicast messages on %s and port: %d...\n\n", multicast_group, port);
    
    // Prepare for select-based I/O multiplexing.
    fd_set read_fds;
    int max_fd = sd;
    struct timeval tv;
    
    // Main loop
    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(sd, &read_fds);
     
        tv.tv_sec = 1;  
        tv.tv_usec = 0;
        
        int activity = select(max_fd + 1, &read_fds, NULL, NULL, &tv);
        if (activity < 0) {
            perror("select error");
            break;
        }
        
        if (FD_ISSET(sd, &read_fds)) {
            char buffer[5000];
            memset(buffer, 0, sizeof(buffer));
            socklen_t client_addr_len = sizeof(client_addr);
            int n = recvfrom(sd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_addr_len);
            if (n < 0) {
                perror("Failed to receive message");
                continue;
            }
            
            // Parse the received JSON to check the requestType.
            cJSON *json = cJSON_Parse(buffer);
            if (json) {
                cJSON *reqItem = cJSON_GetObjectItem(json, "requestType");
                if (reqItem && cJSON_IsString(reqItem)) {
                    if (strcmp(reqItem->valuestring, "query") == 0) {
                        // Handle query request: send a queryResponse with list of files.
                        cJSON *responseJson = cJSON_CreateObject();
                        cJSON_AddStringToObject(responseJson, "requestType", "queryResponse");
                        cJSON *filesArray = cJSON_CreateArray();
                        struct FileInfo *current = fileListHead;
                        while (current != NULL) {
                            cJSON *fileObj = cJSON_CreateObject();
                            cJSON_AddStringToObject(fileObj, "filename", current->filename);
                            cJSON_AddNumberToObject(fileObj, "fileSize", (double)current->fileSize);
                            cJSON_AddStringToObject(fileObj, "fullFileHash", current->fullFileHash);
                            cJSON_AddItemToArray(filesArray, fileObj);
                            current = current->next;
                        }
                        cJSON_AddItemToObject(responseJson, "files", filesArray);
                        char *jsonString = cJSON_Print(responseJson);
                        if (jsonString) {
                            sendto(sd, jsonString, strlen(jsonString), 0, (struct sockaddr *)&client_addr, client_addr_len);
                            free(jsonString);
                        }
                        cJSON_Delete(responseJson);
                    } else {
                        
                        register_file(buffer, client_addr);
                    }
                } else {
                    
                    register_file(buffer, client_addr);
                }
                cJSON_Delete(json);
            } else {
                printf("Received invalid JSON, ignoring.\n");
            }
            print_file_list();
        }
        
    }
    
    close(sd);
    return 0;
}