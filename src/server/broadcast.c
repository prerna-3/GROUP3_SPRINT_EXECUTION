// broadcast.c

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
//#include "encrypt_decrypt.c"
#include "functions.h"
#include "server_functions.h"
#include "logger.h"
#define MAX_CLIENTS 100

extern int client_sockets[MAX_CLIENTS];
extern pthread_mutex_t mutex;

void broadcast_message(const char *message, int sender_id) {
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] != 0 && client_sockets[i] != sender_id) {
//   			decryption(message);
   			send(client_sockets[i], message, strlen(message), 0);

      //			log_info("Messages sent to client socket %d", client_sockets[i]);
        }
    }
    pthread_mutex_unlock(&mutex);
}
