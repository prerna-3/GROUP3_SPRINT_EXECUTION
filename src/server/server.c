/***************************************************************************
 *   FILENAME : server.c
 *   Owner : Group 3            Date : 15/10/24
 *
 *   DESCRIPTION: This server.c code sets up a server that listens for client connections. For each client that connects,
                  it creates a new thread to handle communication, logging important events like when clients connect or if errors occur.
 *
 *   REVISION HISTORY:
 *
 *   Name : Deepali Kumari, Prerna Sharma             Date : 16/10/24
 *   Reason : establishing connection that client is requestiong for
 *
 *   Name : Priyanka Solanki, Shaista Parveen       Date : 17/10/24
 *   Reason : integrating logger file and error handling
 *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "/home2/user15/GROUP3_SPRINT_EXECUTION/GROUP3_SPRINT_EXECUTION/include/functions.h"
#include "/home2/user15/GROUP3_SPRINT_EXECUTION/GROUP3_SPRINT_EXECUTION/include/server_functions.h"
#include "/home2/user15/GROUP3_SPRINT_EXECUTION/GROUP3_SPRINT_EXECUTION/include/logger.h"

#define MAX_CLIENTS 100

int client_sockets[MAX_CLIENTS] = {0}; // Initialize all to 0
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        log_message(FATAL, "server.c", "Failed to create server socket");
        return EXIT_FAILURE;
    }
	server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(5669);

	
	
	log_message(INFO,"server.c","Serve socket created");




if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        log_message(FATAL, "server.c", "Bind failed");
        close(server_socket);
        return EXIT_FAILURE;
    }

    if (listen(server_socket, 5) < 0) {
        perror("Listen failed");
        log_message(FATAL, "server.c", "Failed to listen on server socket");
        close(server_socket);
        return EXIT_FAILURE;
    }	

	log_message(INFO,"server.c","Server is listening on port 5669");



    printf("Server is listening on port 5669...\n");

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
        if (client_socket < 0) {
           perror("Accept failed");
           


		   log_message(WARN,"server.c","Client connection failed");



		   continue; // Handle error, try accepting again
        }


		log_message(INFO,"server.c","Client connected");



        // Allocate memory for the client socket
        int *new_sock = malloc(sizeof(int));
         if (new_sock == NULL) {
            perror("Memory allocation failed");
            log_message(FATAL, "server.c", "Failed to allocate memory for client socket");
            close(client_socket);
            continue; // Skip this client if memory allocation fails
        }
         *new_sock = client_socket;

        pthread_t thread;
    
      if (pthread_create(&thread, NULL, handle_client, (void *)new_sock) != 0) {
            perror("Thread creation failed");
            log_message(WARN, "server.c", "Failed to create thread for client");
            free(new_sock); // Free allocated memory if thread creation fails
            close(client_socket); // Close the client socket
            continue; // Skip this client if thread creation fails
        }
	pthread_detach(thread); // Detach the thread so it can clean up after itself
    }

    close(server_socket);
    
	
	log_message(INFO,"server.c","Server shutdown");


	return 0;
}
