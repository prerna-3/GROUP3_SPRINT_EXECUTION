//#include "encrypt_decrypt.c"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "/home2/user18/GROUP3_SPRINT_EXECUTION/include/functions.h"

#define BUFFER_SIZE 1024

extern int client_sockets[MAX_CLIENTS]; // Client socket array
extern pthread_mutex_t mutex; // Mutex for thread safety
 
void *handle_client(void *arg) {
    int client_socket = *(int *)arg;
    free(arg); // Free the allocated memory for client socket pointer
    char buffer[BUFFER_SIZE];
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
                send(client_socket, "Login successful", 16, 0);
                printf("%s logged in successfully.\n", username);
            } else {
                send(client_socket, "Login failed", 12, 0);
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
                send(client_socket, "You must log in first", 22, 0);
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
