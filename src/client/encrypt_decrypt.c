/***************************************************************************
 *   FILENAME : encrypt_decrypt.c
 *   Owner : Group 3            Date : 15/10/24
 *
 *   DESCRIPTION: This program ensures the privacy and security of multiple clients by encrypting the user password.
 *
 *   REVISION HISTORY:
 *
 *   Name : Pranavya Deepthi Dachepalli             Date : 16/10/24
 *   Reason : Function implementation for encryption and decryption of password
 *
 ***************************************************************************/

#include "/home2/user15/GROUP3_SPRINT_EXECUTION/GROUP3_SPRINT_EXECUTION/include/functions.h" 
#include<stdio.h>

void encryption(char *text) {
	if(text == NULL){
		fprintf(stderr, "Error: NULL pointer passed to encryption function.\n");
		return;
	}

	for (int i = 0; text[i] != '\0'; i++) {
        // Handle uppercase letters
        if (text[i] >= 'A' && text[i] <= 'Z') {
            text[i] = ((text[i] - 'A' + 3) % 26) + 'A';
        }
        // Handle lowercase letters
        else if (text[i] >= 'a' && text[i] <= 'z') {
            text[i] = ((text[i] - 'a' + 3) % 26) + 'a';
        }
        // Handle non-alpha characters by rotating them
        else {
            text[i] = ((text[i] + 3) % 256);
        }
    }
}

// Function to perform Caesar cipher decryption
void decryption(char *text) {
    if(text == NULL){
		fprintf(stderr,"Error: NULL pointer passed to decryption function.\n");
		return;
	}

	for (int i = 0; text[i] != '\0'; i++) {
        // Handle uppercase letters
        if (text[i] >= 'A' && text[i] <= 'Z') {
            text[i] = ((text[i] - 'A' - 3 + 26) % 26) + 'A';
        }
        // Handle lowercase letters
        else if (text[i] >= 'a' && text[i] <= 'z') {
            text[i] = ((text[i] - 'a' - 3 + 26) % 26) + 'a';
        }
        // Handle non-alpha characters by rotating them
        else {
            text[i] = ((text[i] - 3 + 256) % 256);
        }
    }
}
