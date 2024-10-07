#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>  // Windows-specific socket header
#pragma comment(lib, "ws2_32.lib")  // Link ws2_32.lib for Winsock

#define BUFFER_SIZE 1024

// Function to handle errors
void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    WSADATA wsa;
    SOCKET sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    int portno;

    // Check for correct number of arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <hostname> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Get port number and validate it
    portno = atoi(argv[2]);
    if (portno <= 0) {
        fprintf(stderr, "Invalid port number\n");
        exit(EXIT_FAILURE);
    }

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code : %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    // 1. Create a socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    // 2. Set up the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);  // Convert hostname to IP
    server_addr.sin_port = htons(portno);

    // 3. Establish connection to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Connection failed. Error Code : %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    printf("Connected to %s on port %d.\n", argv[1], portno);

    // 4. Main loop to handle user input and server response
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);

        // 5. Read user input from the console
        printf("> ");
        fgets(buffer, BUFFER_SIZE - 1, stdin);

        // Remove the trailing newline character that fgets captures
        buffer[strcspn(buffer, "\n")] = 0;

        // 6. Check if user wants to quit
        if (strcmp(buffer, "quit") == 0) {
            printf("Terminating connection.\n");
            break;
        }

        // 7. Send the user input to the server
        if (send(sockfd, buffer, strlen(buffer), 0) < 0) {
            printf("Send failed. Error Code : %d\n", WSAGetLastError());
            break;
        }

        // 8. Clear the buffer and receive the response from the server
        memset(buffer, 0, BUFFER_SIZE);
        int n = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
        if (n < 0) {
            printf("Receive failed. Error Code : %d\n", WSAGetLastError());
            break;
        }

        // 9. Print the server's response
        printf("%s\n", buffer);
    }

    // 10. Close the connection and cleanup
    closesocket(sockfd);
    WSACleanup();
    return 0;
}
