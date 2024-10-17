// server.c
//#include "encrypt_decrypt.c"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "functions.h"
#include "server_functions.h"
#include "logger.h"

#define MAX_CLIENTS 100

int client_sockets[MAX_CLIENTS] = {0}; // Initialize all to 0
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(5669);

	
	
	log_message(INFO,"server.c","Serve socket created");




    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_socket, 5);
	

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
        *new_sock = client_socket;

        pthread_t thread;
        pthread_create(&thread, NULL, handle_client, (void *)new_sock);
        pthread_detach(thread); // Detach the thread so it can clean up after itself
    }

    close(server_socket);
    
	
	log_message(INFO,"server.c","Server shutdown");


	return 0;
}
