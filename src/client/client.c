#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
//#include "encrypt_decrypt.c"
#include "/home2/user18/GROUP3_SPRINT_EXECUTION/include/functions.h"
#define BUFFER_SIZE 1024

void login_or_register(int sockfd) {
    int choice;
    char username[BUFFER_SIZE], password[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];

    printf("1. Login\n2. Register\nChoose an option: ");
    scanf("%d", &choice);
    getchar(); // Consume newline

    printf("Enter username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] ='\0'; // Remove newline character
//	encryption(username);

    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0'; // Remove newline character
	encryption(password);

    if (choice == 1) {
        snprintf(buffer, sizeof(buffer), "LOGIN:%s:%s", username, password);
    } else if (choice == 2) {
        snprintf(buffer, sizeof(buffer), "REGISTER:%s:%s", username, password);
    } else {
        printf("Invalid choice.\n");
        exit(1);
    }

    // Send the request to the server
    send_message(sockfd, buffer);

    // Receive response from the server
    recv(sockfd, buffer, sizeof(buffer), 0);
    printf("Server: %s\n", buffer);
//	encryption(buffer);
    
    // If login/registration is successful, start the chat
    if (choice == 1 && strcmp(buffer, "Login successful") == 0) {
        printf("You can now start chatting!\n");
    } else if (choice == 2 && strcmp(buffer, "Registration successful") == 0) {
        printf("You can now start chatting!\n");
    } else {
        exit(1);
    }
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(5669);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        return 1;
    }

    // Call the login/register function
    login_or_register(sockfd);

    // Start a thread to receive messages from the server
    pthread_t recv_thread;
    if (pthread_create(&recv_thread, NULL, receive_messages, (void *)&sockfd) != 0) {
        perror("Failed to create thread for receiving messages");
        close(sockfd);
        return 1;
    }

    pthread_detach(recv_thread); // Detach the thread to clean up automatically

    // Main loop for sending messages
    char message[BUFFER_SIZE];
    while (1) {
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = 0; // Remove newline character
        if (strcmp(message, "exit") == 0) {
            break; // Exit the chat
        }
		encryption(message);
        send_message(sockfd, message); // Use the existing send_message function
    }

    close(sockfd);
    return 0;
}

