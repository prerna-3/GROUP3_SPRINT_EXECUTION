// receive_message.c
//#include "encrypt_decrypt.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "/home2/user18/GROUP3_SPRINT_EXECUTION/include/functions.h"

void *receive_messages(void *arg) {
    int sockfd = *(int *)arg;
    char buffer[1024];

    while (1) {
        int bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) {
            printf("Disconnected from server\n");
            break;
        }
        buffer[bytes_received] = '\0'; // Null-terminate the received message
  //      decryption(buffer);
	    printf("%s\n", buffer);
    }
    return NULL;
}
