#ifndef CLIENT_FUNCTIONS_H
#define CLIENT_FUNCTIONS_H

#include "logger.h"
void send_message(int sockfd, const char *message);
void *receive_messages(void *arg);
void encryption(char *text);
void decryption(char *text);
void log_message(LogLevel level, const char *filename, const char *fmt, ...);

#endif

