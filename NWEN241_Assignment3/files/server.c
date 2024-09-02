/**
 * Skeleton file for server.c
 * 
 * You are free to modify this file to implement the server specifications
 * as detailed in Assignment 3 handout.
 * 
 * As a matter of good programming habit, you should break up your imple-
 * mentation into functions. All these functions should be contained in this
 * file as you are only allowed to submit this file.
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <ctype.h>
#define BUFFER_SIZE 1024
// Include necessary header files

/**
 * The main function should be able to accept a command-line argument
 * argv[0]: program name
 * argv[1]: port number
 * 
 * Read the assignment handout for more details about the server program
 * design specifications.
 */ 
void send_response(int client_sockfd, const char *response) {
    if (send(client_sockfd, response, strlen(response), 0) < 0) {
        perror("Send failed");
        //close(client_sockfd);
    }
}

int main(int argc, char *argv[]) {
    int port, sockfd, client_sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;

    // Check command-line arguments
    if (argc != 2) {
        printf("Usage: %s <port number>\n", argv[0]);
        return -1;
    }

    // Get port number from command-line argument
    port = atoi(argv[1]);

    // Check if the port number is valid
    if (port < 1024) {
        printf("Port number must be greater than or equal to 1024.\n");
        return -1;
    }

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Error creating socket");
        return -1;
    }
    printf("Socket created\n");

      // Initialize server address struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding socket");
        close(sockfd);
        return -1;
    }
    printf("Bind successful\n");

    // Listen for incoming connections
    if (listen(sockfd, SOMAXCONN) < 0) {
        perror("Listen failed");
        close(sockfd);
        return -1;
    }
    printf("Listen successful\n");

    // Main server loop
    while (1) {
        // Accept incoming connection
        client_len = sizeof(client_addr);
        client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
        if (client_sockfd < 0) {
            perror("Accept failed");
            close(sockfd);
            return -1;
        }
        printf("Accept successful\n");

        // Send hello message to client
        const char *hello_msg = "HELLO\n";
        if (send(client_sockfd, hello_msg, strlen(hello_msg), 0) < 0) {
            perror("Send failed");
            close(client_sockfd);
            close(sockfd);
            return -1;
        }

        char buffer[BUFFER_SIZE];
        while (1) {
            // Receive data from client
            int bytes_received = recv(client_sockfd, buffer, BUFFER_SIZE - 1, 0);
            if (bytes_received == -1) {
                perror("Receive failed");
                close(client_sockfd);
                close(sockfd);
                return -1;
            } else if (bytes_received == 0) {
                printf("Client disconnected\n");
                close(client_sockfd);
                break;
            } else {
                buffer[bytes_received] = '\0'; // Null-terminate the string
                for (int i = 0; buffer[i]; i++) {
                    buffer[i] = tolower(buffer[i]); // Convert to lowercase
                }

                // Check received command
                if (strncmp(buffer, "bye", 3) == 0) {
                    printf("Client sent BYE. Closing connection.\n");
                    close(client_sockfd);
                    break;
                } else if (strncmp(buffer, "get", 3) == 0) { // Code for handling "get" operation
                    char filename[BUFFER_SIZE]; // Extract filename from the received command
                    int num_args = sscanf(buffer, "%*s %s", filename);

                    if (num_args != 1 || strlen(filename) == 0) {  // Validate filename
                        const char *response = "SERVER 500 Get Error\n";
                        send_response(client_sockfd, response);
                    } else {
                        FILE *file = fopen(filename, "r");  // Open the requested file for reading
                        if (file == NULL) {
                            const char *response = "SERVER 404 Not Found\n";
                            send_response(client_sockfd, response);
                        } else {
                            const char *response_header = "SERVER 200 OK\n\n";
                            send_response(client_sockfd, response_header);

                            char file_buffer[BUFFER_SIZE];  // Read file content and send it to the client
                            while (fgets(file_buffer, BUFFER_SIZE, file) != NULL) {
                                send_response(client_sockfd, file_buffer);
                            }
                             // Send end of file response
                            send_response(client_sockfd, "\n\n");
                            fclose(file);
                        }
                    }
                } else if (strncmp(buffer, "put", 3) == 0) {// Code for handling "put" operation
                    char filename[BUFFER_SIZE];
                    int num_args = sscanf(buffer, "%*s %s", filename);

                    if (num_args != 1 || strlen(filename) == 0) {// Validate filename
                        const char *response = "SERVER 502 Command Error\n";
                        send_response(client_sockfd, response);
                    } else {
                        FILE *file = fopen(filename, "w");
                        if (file == NULL) {
                            const char *response = "SERVER 501 Put Error\n";
                            send_response(client_sockfd, response);
                        } else { // Open the file for writing
                            char file_buffer[BUFFER_SIZE];   // Read data from client and write it to the file
                            int empty_lines_received = 0;

                            while (1) {
                                int bytes_received = recv(client_sockfd, file_buffer, BUFFER_SIZE - 1, 0);
                                if (bytes_received == -1) {
                                    perror("Receive failed");
                                    fclose(file);
                                    close(client_sockfd);
                                    close(sockfd);
                                    return -1;
                                } else if (bytes_received == 0) {
                                    printf("Client disconnected\n");
                                    fclose(file);
                                    close(client_sockfd);
                                    break;
                                } else {
                                    file_buffer[bytes_received] = '\0'; // Null-terminate the string

                                    // Check for consecutive empty lines
                                    if (strlen(file_buffer) == 1 && file_buffer[0] == '\n') {
                                        empty_lines_received++;
                                        if (empty_lines_received >= 2) {
                                            break;
                                        }
                                    } else {
                                        empty_lines_received = 0; // Reset if non-empty line received
                                    }

                                    // Write received data to file
                                    if (fputs(file_buffer, file) == EOF) {
                                        perror("Write to file failed");
                                        fclose(file);
                                        close(client_sockfd);
                                        close(sockfd);
                                        return -1;
                                    }
                                }
                            }

                            fclose(file);  // Close the file after writing

                            const char *response = "SERVER 201 Created\n"; // Send success response
                            send_response(client_sockfd, response);
                        }
                    }
                } else {
                    const char *response = "SERVER 502 Command Error\n";  // Send command error response to the client
                    send_response(client_sockfd, response);  
                }
            }
        }
    }

    close(sockfd);

    return 0;
}