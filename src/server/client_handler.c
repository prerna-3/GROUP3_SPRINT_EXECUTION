/***************************************************************************
 *   FILENAME : client_handler.c
 *   Owner : Group 3            Date : 15/10/24
 *
 *   DESCRIPTION:  This function handles client communication by receiving commands for registration, login, and message broadcasting.
                   It manages client authentication, message broadcasting to other clients
 *
 *   REVISION HISTORY:
 *
 *   Name : Pranavya Deepthi Dachepalli             Date : 16/10/24
 *   Reason : created function for handling client and server communication.
 *
 *   Name : Priyanka Solanki, Shaista Parveen       Date : 17/10/24
 *   Reason : integrating logger file, and error handling
 *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "/home2/user15/GROUP3_SPRINT_EXECUTION/GROUP3_SPRINT_EXECUTION/include/functions.h"
#include "/home2/user15/GROUP3_SPRINT_EXECUTION/GROUP3_SPRINT_EXECUTION/include/server_functions.h"
#include "/home2/user15/GROUP3_SPRINT_EXECUTION/GROUP3_SPRINT_EXECUTION/include/logger.h"
#define BUFFER_SIZE 1024

extern int client_sockets[MAX_CLIENTS]; // Client socket array
extern pthread_mutex_t mutex; // Mutex for thread safety
 
void *handle_client(void *arg) {
    int client_socket = *(int *)arg;
    free(arg); // Free the allocated memory for client socket pointer
    char buffer[BUFFER_SIZE]={0};
    char username[BUFFER_SIZE] = ""; // Store username after login
    while (1) {
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            printf("Client disconnected: %d\n", client_socket);
            break; // Connection closed
        }
        buffer[bytes_received] = '\0'; // Null-terminate the received message
 
        // Parse the received message
        char command[BUFFER_SIZE], password[BUFFER_SIZE];
        sscanf(buffer, "%[^:]:%[^:]:%s", command, username, password);
 
        if (strcmp(command, "REGISTER") == 0) {
            printf("Registration attempt from %s\n", username);
            handle_authentication(client_socket, username, password, 1); // Register user
        } else if (strcmp(command, "LOGIN") == 0) {
            printf("Login attempt from %s\n", username);
            if (handle_authentication(client_socket, username, password, 0)) { // Login user
                // Add client socket to the list of connected clients
                pthread_mutex_lock(&mutex);
                for (int i = 0; i < MAX_CLIENTS; i++) {
                    if (client_sockets[i] == 0) {
                        client_sockets[i] = client_socket;
                        break;
                    }
                }
                pthread_mutex_unlock(&mutex);
                if (send(client_socket, "Login successful", 16, 0) < 0) {
                    perror("Failed to send login success message");
                }
				printf("%s logged in successfully.\n", username);
            } else {
                if (send(client_socket, "Login failed", 12, 0) < 0) {
                    perror("Failed to send login failed message");
                }
				printf("Login failed for %s.\n", username);
            }
        } else {
            // If user is logged in, broadcast the message
            if (strlen(username) > 0) {
                char full_message[BUFFER_SIZE];
				decryption(buffer);
                snprintf(full_message, sizeof(full_message), "%s: %s", username, buffer); // Include username in the message
                printf("Message from %s: %s\n", username, buffer);
                broadcast_message(full_message, client_socket);
            } else {
            	if (send(client_socket, "You must log in first", 22, 0) < 0) {
                    perror("Failed to send login prompt message");
                }
			}
        }
    }
 
    // Remove client socket from the list
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] == client_socket) {
            client_sockets[i] = 0; // Mark as free
            break;
        }
    }
    pthread_mutex_unlock(&mutex);
 
    close(client_socket);
    printf("Client socket closed: %d\n", client_socket);
    return NULL;
}
