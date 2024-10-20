/***************************************************************************
 *   FILENAME : send_messages.c
 *   Owner : Group 3            Date : 15/10/24
 *
 *   DESCRIPTION: This code defines the function send_message, which is responsible for sending a message 
                  from the client to the server over a socket connection.  
 *
 *   REVISION HISTORY:
 *
 *   Name : Prerna Sharma      Date : 16/10/24
 *   Reason : For sending message over socket
 *
 *   Name : Priyanka Solanki, Shaista Parveen       Date : 17/10/24
 *   Reason : integrating logger file
 *
 ***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "functions.h"
#include "client_functions.h"
#include "logger.h"
void send_message(int sockfd, const char *message) {
	 // Check if the message is NULL
	 if (message == NULL) {
        fprintf(stderr, "Error: NULL message passed to send_message.\n");
        log_message(WARN, "send_messages.c", "NULL message passed");
        return;
    }
	 
	 // Check if the socket is valid
    if (sockfd < 0) {
        fprintf(stderr, "Error: Invalid socket descriptor.\n");
        log_message(WARN, "send_messages.c", "Invalid socket descriptor");
        return;
    }

	 if (send(sockfd, message, strlen(message), 0) < 0) {
//		encryption(message);
        perror("Send failed");

		log_message(WARN, "send_messages.c", "Failed to send message");


    }else{
		
		
		log_message(DEBUG,"send_messages.c","Message sent: %s", message);

	}
}
