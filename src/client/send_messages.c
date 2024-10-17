// send_messages.c
//#include "encrypt_decrypt.c"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "functions.h"
#include "client_functions.h"
#include "logger.h"
void send_message(int sockfd, const char *message) {
    if (send(sockfd, message, strlen(message), 0) < 0) {
//		encryption(message);
        perror("Send failed");

		log_message(WARN, "send_messages.c", "Failed to send message");


    }else{
		
		
		log_message(DEBUG,"send_messages.c","Message sent: %s", message);

	}
}
