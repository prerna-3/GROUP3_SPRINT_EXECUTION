#ifndef SERVER_FUNCTIONS_H
#define SERVER_FUNCTIONS_H

#include "logger.h"
void broadcast_message(const char *message, int sender_id);
void *handle_client(void *arg);
int handle_authentication(int client_socket, const char *username, const char *password, int is_registration);
void encryption(char *text);
void decryption(char *text);
void log_message(LogLevel level, const char *filename, const char *fmt, ...);

#endif

