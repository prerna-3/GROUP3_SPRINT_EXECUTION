/***************************************************************************
 *   FILENAME : broadcast.c
 *   Owner : Group 3            Date : 15/10/24
 *
 *   DESCRIPTION:  this program used to broadcast a message recieve from 1 client to all the clients
 *
 *   REVISION HISTORY:
 *
 *   Name : Priyanka Solanki, Shaista Parveen       Date : 17/10/24
 *   Reason : implemented function and did error handling
 *
 ***************************************************************************/


#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include "/home2/user15/GROUP3_SPRINT_EXECUTION/GROUP3_SPRINT_EXECUTION/include/functions.h"
#include "/home2/user15/GROUP3_SPRINT_EXECUTION/GROUP3_SPRINT_EXECUTION/include/server_functions.h"
#include "/home2/user15/GROUP3_SPRINT_EXECUTION/GROUP3_SPRINT_EXECUTION/include/logger.h"
#define MAX_CLIENTS 100

extern int client_sockets[MAX_CLIENTS];
extern pthread_mutex_t mutex;

void broadcast_message(const char *message, int sender_id) {
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] != 0 && client_sockets[i] != sender_id) {

       if (send(client_sockets[i], message, strlen(message), 0) < 0) {
                perror("Send failed");
                log_message(WARN, "broadcast.c", "Failed to send message to client socket %d", client_sockets[i]);
            }
        }
    }
    pthread_mutex_unlock(&mutex);
}
