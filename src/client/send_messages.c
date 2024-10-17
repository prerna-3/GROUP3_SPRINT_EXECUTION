// send_message.c
//#include "encrypt_decrypt.c"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "/home2/user18/GROUP3_SPRINT_EXECUTION/include/functions.h"


void send_message(int sockfd, const char *message) {
    if (send(sockfd, message, strlen(message), 0) < 0) {
//		encryption(message);
        perror("Send failed");
    }
}
