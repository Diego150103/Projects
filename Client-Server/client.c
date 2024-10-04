#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>

// Implementation of the functions
void connect_server(int *sd, struct sockaddr_in *sin_addr, const char *ipaddress, int portNumber) {
    int rc;

    // Create a socket
    *sd = socket(AF_INET, SOCK_STREAM, 0);
    if (*sd < 0) {
        perror("error opening stream socket");
        exit(1);
    }

    // Add server address information
    sin_addr->sin_family = AF_INET;
    sin_addr->sin_port = htons(portNumber);

    // Convert IP address
    rc = inet_pton(AF_INET, ipaddress, &(sin_addr->sin_addr));
    if (rc < 0) {
        printf("failed to convert ip address\n");
        exit(1);
    }

    // Establish connection with server
    rc = connect(*sd, (struct sockaddr *) sin_addr, sizeof(struct sockaddr_in));
    if (rc < 0) {
        close(*sd); // Clean up
        perror("error connecting stream socket");
        exit(1);
    }
}

// Function to send the string to the server
void send_string(int sd, const char *inputString) {
    int rc;
    int stringLength = strlen(inputString);
    int convertLength = htonl(stringLength);

    // Send the length of the string
    rc = write(sd, &convertLength, sizeof(int));
    if (rc < sizeof(int)) {
        printf("error sending string length\n");
        exit(1);
    }

    // Send the string
    rc = write(sd, inputString, stringLength);
    if (rc < stringLength) {
        printf("error sending string\n");
        exit(1);
    }
}

void receive_echo(int sd) {
    int rc;
    int echoLength;
    char echoString[256];

    // Receive the length of the echoed string
    rc = read(sd, &echoLength, sizeof(int));
    if (rc < sizeof(int)) {
        printf("error receiving echo length\n");
        exit(1);
    }
    echoLength = ntohl(echoLength);

    printf("I am expecting you to send %d bytes\n", echoLength);

    // Receive the echoed string
    rc = read(sd, echoString, echoLength);
    if (rc < echoLength) {
        printf("error receiving echo string\n");
        exit(1);
    }
    echoString[echoLength] = '\0'; // Null-terminate the received string

    printf("received the following string '%s'\n", echoString);

}

// Main function
int main(int argc, char *argv[]) {
    int sd;
    struct sockaddr_in sin_addr;

    if (argc < 3) {
        printf("Usage is: client <portNumber> <ipaddress>\n");
        exit(1);
    }

    int portNumber = atoi(argv[1]);

    // Connect to server
    connect_server(&sd, &sin_addr, argv[2], portNumber);

    char inputString[256];
    printf("Enter a string (no more than 255 characters): ");
    fgets(inputString, 256, stdin);
    inputString[strcspn(inputString, "\n")] = '\0';

    // Send string
    send_string(sd, inputString);

    // Receive echo
    receive_echo(sd);

    close(sd);
    return 0;
}