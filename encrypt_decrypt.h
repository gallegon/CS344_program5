/* filename: encrypt_decrypt.h
	author: Nicholai Gallegos
	date: 3/08/2021
*/
#ifndef ENCRYPT_DECRYPT
#define ENCRYPT_DECRYPT

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

// function prototypes for encryption/decryption
void encryptPlaintext(char*, char*, char*, int);
void decryptKey(char*, char*, char*, int);

#endif
