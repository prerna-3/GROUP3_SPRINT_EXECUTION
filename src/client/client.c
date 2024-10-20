/***************************************************************************
*   FILENAME : client.c   
*   Owner : Group 3            Date : 15/10/24
*	
*   DESCRIPTION:The program allows the user to either login or register by interacting with a server.
                After successful authentication, the client can send and receive messages in a chat environment


*   REVISION HISTORY:

*   Name : Pranavya Deepthi Dachepalli             Date : 16/10/24
*   Reason : For establishing client and server connection

*   Name : Priyanka Solanki, Shaista Parveen       Date : 17/10/24
*   Reason : integrating logger file

*****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "/home2/user15/GROUP3_SPRINT_EXECUTION/GROUP3_SPRINT_EXECUTION/include/functions.h"
#include "/home2/user15/GROUP3_SPRINT_EXECUTION/GROUP3_SPRINT_EXECUTION/include/client_functions.h"
#include "/home2/user15/GROUP3_SPRINT_EXECUTION/GROUP3_SPRINT_EXECUTION/include/logger.h"

#define BUFFER_SIZE 1024

void login_or_register(int sockfd) {
    int choice;
    char username[BUFFER_SIZE], password[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];

    printf("1. Login\n2. Register\nChoose an option: ");
    if( scanf("%d", &choice)!=1){
    perror("Invalid input for choice");
	log_message(FATAL, "client.c","Invalid input for login/register choice");
	exit(1);
	}
    getchar(); // Consume newline
	
	
	
	log_message(INFO,"client.c","Client login/register request sent");



    printf("Enter username: ");
    if(fgets(username, sizeof(username), stdin)==NULL){
	perror("Error reading username");
        log_message(FATAL, "client.c", "Error reading username");
        exit(1);
    }
	
    username[strcspn(username, "\n")] ='\0'; // Remove newline character
//	encryption(username);

    printf("Enter password: ");
    if (fgets(password, sizeof(password), stdin) == NULL) {
        perror("Error reading password");
        log_message(FATAL, "client.c", "Error reading password");
        exit(1);
    }
    password[strcspn(password, "\n")] = '\0'; // Remove newline character
	encryption(password);

    if (choice == 1) {
        snprintf(buffer, sizeof(buffer), "LOGIN:%s:%s", username, password);
    } else if (choice == 2) {
        snprintf(buffer, sizeof(buffer), "REGISTER:%s:%s", username, password);
    } else {
        printf("Invalid choice.\n");
	    log_message(WARN, "client.c", "Invalid login/register choice");
        exit(1);
    }

    // Send the request to the server
     send_message(sockfd, buffer);
    // Receive response from the server
    if (recv(sockfd, buffer, sizeof(buffer), 0) < 0) {
        perror("Failed to receive server response");
        log_message(FATAL, "client.c", "Failed to receive server response");
        exit(1);
    }
    printf("Server: %s\n", buffer);
    
    // If login/registration is successful, start the chat
    if (choice == 1 && strcmp(buffer, "Login successful") == 0) {
        printf("You can now start chatting!\n");
    } else if (choice == 2 && strcmp(buffer, "Registration successful") == 0) {
        printf("Registration successful, you can now login!\n");
		close(sockfd);
		exit(0);
    } else {
	    log_message(WARN, "client.c", "Login/Registration failed");
        exit(1);
    }
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");



    	log_message(FATAL,"client.c","Socket creation failed");
		
		
		return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(5669);
	

	log_message(INFO,"client.c","Client socket created");



    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sockfd);
    	

		log_message(FATAL,"client.c","Connection failed");



		return 1;
    }


	log_message(INFO,"client.c","Connected to server");



    // Call the login/register function
    login_or_register(sockfd);

    // Start a thread to receive messages from the server
    pthread_t recv_thread;
    if (pthread_create(&recv_thread, NULL, receive_messages, (void *)&sockfd) != 0) {
        perror("Failed to create thread for receiving messages");
		log_message(FATAL, "client.c", "Failed to create thread for receiving messages");
        close(sockfd);
        return 1;
    }

//    pthread_detach(recv_thread); // Detach the thread to clean up automatically

    // Main loop for sending messages
    char message[BUFFER_SIZE];
    while (1) {
        if (fgets(message, sizeof(message), stdin) == NULL) {
            perror("Error reading message");
            log_message(FATAL, "client.c", "Error reading message from input");
            break;
        }
        message[strcspn(message, "\n")] = 0; // Remove newline character
        if (strcmp(message, "exit") == 0) {
            break; // Exit the chat
        }
		encryption(message);
	
	  send_message(sockfd, message);
    }
 
    pthread_join(recv_thread, NULL);
    close(sockfd);
    

	log_message(INFO,"client.c","Client disconnected");


	
	return 0;
}


