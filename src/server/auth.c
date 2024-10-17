//#include "encrypt_decrypt.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "/home2/user18/GROUP3_SPRINT_EXECUTION/include/functions.h"

#define USER_FILE "/home2/user18/GROUP3_SPRINT_EXECUTION/data/users.txt"
#define BUFFER_SIZE 1024


void register_user(const char *username, const char *password) {
    FILE *file = fopen(USER_FILE, "a");
    if (!file) {
        perror("Could not open user file");
        return;
    }

    fprintf(file, "%s:%s\n", username, password);
    fclose(file);
}

int authenticate_user(const char *username, const char *password) {
    FILE *file = fopen(USER_FILE, "r");
    if (!file) {
        perror("Could not open user file");
        return 0;
    }

    char line[BUFFER_SIZE];
    while (fgets(line, sizeof(line), file)) {
        char stored_username[BUFFER_SIZE], stored_password[BUFFER_SIZE];
        sscanf(line, "%[^:]:%s", stored_username, stored_password);
//		decryption(stored_username);
//		decryption(stored_password);
        if (strcmp(username, stored_username) == 0 && strcmp(password, stored_password) == 0) {
            fclose(file);
            return 1; // Authentication successful
        }
    }

    fclose(file);
    return 0; // Authentication failed
}

int handle_authentication(int client_socket, const char *username, const char *password, int is_registration) {
    if (is_registration) {
        register_user(username, password);
        send(client_socket, "Registration successful", 23, 0);
        return 1; // Indicate registration success
    } else {
        if (authenticate_user(username, password)) {
//			send(client_socket,"authentication successful",25,0);
            return 1; // Indicate login success
        } else {
            send(client_socket, "Invalid credentials", 19, 0);
            return 0; // Indicate login failure
        }
    }
}
