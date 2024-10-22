/*****.**********************************************************************
*   FILENAME : auth.c
*   Owner : Group 3            Date : 15/10/24
*
*   DESCRIPTION : This code handles user registration and authentication by storing usernames and passwords in a text file,
*                 then verifying login attempts by comparing input credentials with stored data. It supports both registration and authentication,
*                 sending appropriate success or failure messages to the client.
*
*   REVISION HISTORY:
*
*   Name :  Pranavya Deepthi Dachepalli            Date : 16/10/24
*   Reason : Function to authenticate clients
*
*   Name : Shaista Parveen, Priyanka Solanki       Date : 18/10/24
*   Reason : error handling
***************************************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "/home2/user15/GROUP3_SPRINT_EXECUTION/GROUP3_SPRINT_EXECUTION/include/functions.h"
 
#define USER_FILE "/home2/user15/GROUP3_SPRINT_EXECUTION/GROUP3_SPRINT_EXECUTION/data/users.txt"
#define BUFFER_SIZE 1024
 extern ClientInfo client_sockets[MAX_CLIENTS]; 
int register_user(const char *username, const char *password) {
    // Check if the user already exists
    FILE *file = fopen(USER_FILE, "r");
    if (!file) {
        perror("Could not open user file for reading");
        return -1; // Error opening file
    }
 
    char line[BUFFER_SIZE];
    while (fgets(line, sizeof(line), file)) {
        char stored_username[BUFFER_SIZE];
        sscanf(line, "%[^:]:", stored_username);
        if (strcmp(username, stored_username) == 0) {
            fclose(file);
            return 0; // User already exists
        }
    }
    fclose(file);
 
    // User not found, proceed to register
    file = fopen(USER_FILE, "a");  // Correctly assign the FILE pointer
    if (!file) {
        perror("Could not open user file for writing");
        return -1; // Error opening file
    }
 
    fprintf(file, "%s:%s\n", username, password);
    fclose(file);
    return 1; // Registration successful
}

int is_username_connected(const char *username) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i].socket != 0 && strcmp(client_sockets[i].username, username) == 0) {
            return 1; // Username is already connected
        }
    }
    return 0; // Username is not connected
}
int authenticate_user(const char *username, const char *password) {
 
 // Check if the user is already connected
    if (is_username_connected(username)) {
        return -1; // User is already logged in
    }

   FILE *file = fopen(USER_FILE, "r");
    if (!file) {
        perror("Could not open user file");
        return -2;
    }
 
    char line[BUFFER_SIZE];
    while (fgets(line, sizeof(line), file)) {
        char stored_username[BUFFER_SIZE], stored_password[BUFFER_SIZE];
        sscanf(line, "%[^:]:%s", stored_username, stored_password);
  /*      if (strcmp(username, stored_username) == 0 && strcmp(password, stored_password) == 0) {
            fclose(file);
            return 1; // Authentication successful
        }
	}
		else{
		fclose(file);
		return -3;
    }*/
  
  if (strcmp(username, stored_username) == 0) {
            fclose(file);
            // Check for password match
            if (strcmp(password, stored_password) == 0) {
                return 1; // Authentication successful
            } else {
                fclose(file); // Close the file before returning
                return -3; // Invalid credentials
            }
        }
    }
    fclose(file);
    return 0; // Authentication failed
}
 
int handle_authentication(int client_socket, const char *username, const char *password, int is_registration) {
    if (is_registration) {
        int result = register_user(username, password);
        if (result == 1) {
            if (send(client_socket, "Registration successful", 23, 0) < 0) {
                perror("Failed to send registration success message");
            }
            return 1; // Indicate registration success
        } else if (result == 0) {
            printf("User already registered: %s\n", username);
            if (send(client_socket, "User already registered", 23, 0) < 0) {
                perror("Failed to send user already registered message");
            }
            return 0;
        }
        return 0;
    } else {
        if (authenticate_user(username, password)==1) {
            // send(client_socket, "Authentication successful", 25, 0);
            return 1; // Indicate login success

        }
		else if (authenticate_user(username, password)==-1){
		  return -1;
		  }
		else {
            if (send(client_socket, "Invalid credentials", 19, 0) < 0) {
                perror("Failed to send invalid credentials message");
            }
            return -3; // Indicate login failure
        }
    }
}
