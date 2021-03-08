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
	
	for (int i = 0; i < keyLength; ++i) {
		randChar = rand() % 27;
		
		buffer[i] = KeyCode[randChar];
	}

	buffer[keyLength] = '\n';
	buffer[keyLength + 1] = '\0';
}

#if 0
// TODO figure encrypt/decryption of SPACE character
void encryptPlainText(char* plainText, char* key, char* cypherText, int keyLength) {
	int encryptedChar; // ASCII value for char
	int keyChar;
	int plainChar;

	if (strlen(plainText) != strlen(key)) {
		//cannot encrypt
	}
	else {
		for (int i = 0; i < keyLength; ++i) {
			plainChar = plainText[i];
			keyChar = key[i];
			
			/* 
				These are to keep track of the modulo math, as the ASCII space
				value doesn't align with the values of the upper case letters
			*/
			
			if (plainChar == ASCII_SPACE || keyChar == ASCII_SPACE) {
				if (plainChar == ASCII_SPACE && keyChar != ASCII_SPACE) {
					plainChar = SYMB_SPACE;
					keyChar -= ASCII_CONST;
				}
				else if (keyChar == SYMB_SPACE && plainChar != ASCII_SPACE) {
					keyChar = SYMB_SPACE;
					plainChar -= ASCII_CONST;
				}
				else {
					keyChar = SYMB_SPACE;
					plainChar = SYMB_SPACE;
				}
			}
			else {
				plainChar -= ASCII_CONST;
				keyChar -= ASCII_CONST;
			}

			encryptedChar = (plainChar + keyChar) % 27;

			cypherText[i] = KeyCode[encryptedChar];

		}
		
		cypherText[keyLength] = '\n';
		cypherText[keyLength + 1] = '\0';
	}
}
void decryptKey(char* encryptedText, char* key, char* decryptedText, int keyLength) {
	int decryptedChar;
	int encryptedChar;
	int keyChar;

	if (strlen(encryptedText) != strlen(key)) {
		//cannot decrypt
	}
	else {
		for (int i = 0; i < keyLength; ++i) {
			encryptedChar = encryptedText[i];
			keyChar = key[i];

			if (encryptedChar == ASCII_SPACE || keyChar == ASCII_SPACE) {
				if (encryptedChar == ASCII_SPACE && keyChar != ASCII_SPACE) {
					encryptedChar = SYMB_SPACE;
					keyChar -= ASCII_CONST;
				}
				else if (keyChar == ASCII_SPACE && encryptedChar != ASCII_SPACE) {
					keyChar = SYMB_SPACE;
					encryptedChar -= ASCII_CONST;	
				}
				else {
					keyChar= SYMB_SPACE;
					encryptedChar = SYMB_SPACE;
				}
			}
			else {
				encryptedChar -= ASCII_CONST;
				keyChar -= ASCII_CONST;
			}

			decryptedChar = (encryptedChar - keyChar);
			
			if (decryptedChar < 0) {
				decryptedChar += 27;
			}
			//decryptedChar %= 27;

			decryptedText[i] = KeyCode[decryptedChar];
			
		}
		decryptedText[keyLength] = '\n';
		decryptedText[keyLength + 1] = '\0';

	}
}
#endif

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
	
	generateKey(key, keyLength);
	//strcat(key, "\n");
	printf("%s", key);
	#if 0
	printf("%s", "\nbeginning test\n--------------------\n");
	generateKey(key, 8);
	printf("HELLO ME\n");
	printf("%s", key);
	encryptPlaintext("HELLO ME\n", key, cyph, 8);
	printf("%s", cyph);
	decryptKey(cyph, key, decryp, 8);
	printf("%s", decryp);
	#endif
	return 0;
}
