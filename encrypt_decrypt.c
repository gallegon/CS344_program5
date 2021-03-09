/* file name: encrypt_decrypt.c
	author: Nicholai Gallegos
	date: 3/8/2021 */
#include "encrypt_decrypt.h"


#define MAX_KEY_LENGTH 100000
// ascii value for capital 'A' starts at 65...
#define ASCII_CONST 65
//for dealing with the space character
#define ASCII_SPACE 32
// symbolic place for ascii space in our keygen "code" (27th character)
#define SYMB_SPACE 26

const char KeyCode[28] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

// encrypt a plaintext with a key and store into cypherText
void encryptPlaintext(char* plainText, char* key, char* cypherText, int keyLength) {
   int encryptedChar; // ASCII value for char
   int keyChar;
   int plainChar;

   if (strlen(plainText) != strlen(key)) {
      //cannot encrypt
   }   
   else {
      for (int i = 0; i < keyLength; ++i) {
         /* get position relative to KeyCode rather than ASCII */
			plainChar = plainText[i] - ASCII_CONST;
         keyChar = key[i] - ASCII_CONST;
			
			/* if the value of the character after subtracting ASCII_CONST is less
				than 0, it means that the character in the string is a space 
				character.  After this we give the space character a value of 26 
				(SYMB_SPACE) as it is the 27th character in our keycode */
			if (plainChar < 0)
				plainChar = SYMB_SPACE;
			if (keyChar < 0)
				keyChar = SYMB_SPACE;
         
			/* add the correct character from KeyCode to the cyphertext */
			encryptedChar = (plainChar + keyChar) % 27; 
			cypherText[i] = KeyCode[encryptedChar];	
      }   
   	// add newline and null terminator 
      cypherText[keyLength] = '\n';
      cypherText[keyLength + 1] = '\0';
   }   
}

// decrypt a cyphertext with a key and store into decryptedText
void decryptKey(char* encryptedText, char* key, char* decryptedText, int keyLength) {
   int decryptedChar;
   int encryptedChar;
   int keyChar;

   if (strlen(encryptedText) != strlen(key)) {
      //cannot decrypt
   }
   else {
      for (int i = 0; i < keyLength; ++i) {
         encryptedChar = encryptedText[i] - ASCII_CONST;
     		keyChar = key[i] - ASCII_CONST;

			/* since space is 32 subtracting 65 (ASCII_SPACE) will result in a
			   negative number */
			if (keyChar < 0) {
				keyChar = SYMB_SPACE;
			}
			if (encryptedChar < 0) {
				encryptedChar = SYMB_SPACE;
			}
			
			// subtract values to get decrypted character
			decryptedChar = (encryptedChar - keyChar);

			// add 27 if character is negative
         if (decryptedChar < 0) { 
            decryptedChar += 27;
			}

			// modulo 27 just in case...
         decryptedChar %= 27;

			// set decrypted text to the corresponding keycode value
         decryptedText[i] = KeyCode[decryptedChar];
      }
		// add newline and null terminator
      decryptedText[keyLength] = '\n';
      decryptedText[keyLength + 1] = '\0';

   }
}
