/***************************************************************************
 *   FILENAME : recieve_messages.c
 *   Owner : Group 3            Date : 15/10/24
 *
 *   DESCRIPTION: This code defines the receive_messages function, which runs in a separate
                  thread to continuously receive messages from the server. 
 *
 *   REVISION HISTORY:
 *
 *   Name : Deepali Kumari         Date : 16/10/24
 *   Reason : implemented function for recieving messages from multiple client
 *
 *   Name : Priyanka Solanki, Shaista Parveen       Date : 17/10/24
 *   Reason : integrating logger file
 *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "/home2/user15/GROUP3_SPRINT_EXECUTION/GROUP3_SPRINT_EXECUTION/include/functions.h"
#include "/home2/user15/GROUP3_SPRINT_EXECUTION/GROUP3_SPRINT_EXECUTION/include/logger.h"
void *receive_messages(void *arg) {
    int sockfd = *(int *)arg;
    char buffer[1024];
	
	// Check if the socket file descriptor is valid
    if (sockfd < 0) {
        fprintf(stderr, "Error: Invalid socket file descriptor.\n");
        log_message(FATAL, "receive_messages.c", "Invalid socket file descriptor");
	   return NULL; // Exit the thread if the socket is invalid
    }

    while (1) {
        int bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) {
            printf("Disconnected from server\n");
           log_message(WARN, "receive_messages.c","Connection closed or error");
		   break;
        }
        buffer[bytes_received] = '\0'; // Null-terminate the received message
	    printf("%s\n", buffer);
    }
	close(sockfd); // Close the socket when done
    log_message(INFO, "receive_messages.c", "Socket closed after receiving messages");

    return NULL;
}
