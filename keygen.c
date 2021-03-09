#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "encrypt_decrypt.h"

#define MAX_KEY_LENGTH 100000
// ascii value for capital 'A' starts at 65...
#define ASCII_CONST 65
//for dealing with the space character
#define ASCII_SPACE 32
// symbolic place for ascii space in our keygen "code" (27th character)
#define SYMB_SPACE 26

static const char KeyCode[28] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

/* Name: generateKey
 	Description: generates a key of size keyLength into the array of characters 
 	buffer
 */
void generateKey(char* buffer, int keyLength) {
	// represents the ASCII value of possible upper case letters
	int randChar;
	
	// this assumes that buffer is the correct size
	memset(buffer, '\0', MAX_KEY_LENGTH);
	
	// set a random char from keycode to the respective position in the buffer
	for (int i = 0; i < keyLength; ++i) {
		randChar = rand() % 27;	
		buffer[i] = KeyCode[randChar];
	}

	// insert newline character and null terminator
	buffer[keyLength] = '\n';
	buffer[keyLength + 1] = '\0';
}

// run keygen
int main(int argc, char* argv[]) {
	char key[MAX_KEY_LENGTH];
	char cyph[MAX_KEY_LENGTH];
	char decryp[MAX_KEY_LENGTH];

	// seed rand() 
	srand(time(NULL));

	if (argc < 2) {
		printf("ERROR: Usage: ./%s keylength\n", argv[0]);
		exit(0);
	}
	
	int keyLength = atoi(argv[1]);
	
	// generate key then print to stdout
	generateKey(key, keyLength);
	printf("%s", key);
	
	return 0;
}
