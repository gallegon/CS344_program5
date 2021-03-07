#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>  // ssize_t
#include <sys/socket.h> // send(),recv()
#include <netdb.h>      // gethostbyname()
#include "send_recv.h"

static const char KeyCode[28] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
/**
* Client code
* 1. Create a socket and connect to the server specified in the command arugments.
* 2. Prompt the user for input and send that input as a message to the server.
* 3. Print the message received from the server and exit the program.
*/

#if 0
// Error function used for reporting issues
void error(const char *msg) { 
  perror(msg); 
  exit(0); 
} 
#endif

// Set up the address struct
void setupAddressStruct(struct sockaddr_in* address, 
                        int portNumber, 
                        char* hostname){
 
  // Clear out the address struct
  memset((char*) address, '\0', sizeof(*address)); 

  // The address should be network capable
  address->sin_family = AF_INET;
  // Store the port number
  address->sin_port = htons(portNumber);

  // Get the DNS entry for this host name
  struct hostent* hostInfo = gethostbyname(hostname); 
  if (hostInfo == NULL) { 
    fprintf(stderr, "CLIENT: ERROR, no such host\n"); 
    exit(0); 
  }
  // Copy the first IP address from the DNS entry to sin_addr.s_addr
  memcpy((char*) &address->sin_addr.s_addr, 
        hostInfo->h_addr_list[0],
        hostInfo->h_length);
}

int loadFiles(char* plainTextPath, char* keyPath, char* plainBuffer, 
	char* keyBuffer) {
	char temp1[72000], temp2[72000];
	char* counter1, * counter2;

	memset(temp1, '\0', sizeof(temp1));
	memset(temp2, '\0', sizeof(temp2));

	FILE* keyFile = fopen(plainTextPath, "r");
	FILE* plaintextFile = fopen(keyPath, "r");
	
	if (keyFile == NULL || plaintextFile == NULL) {
		error("FILE");
		return -1;
	}
	else {
		
		do {
			counter1 = fgets(temp1, sizeof(plainBuffer) - 1, plaintextFile);
			strcat(plainBuffer, temp1);
		} while (counter1 != NULL);
		do {
			counter2 = fgets(temp2, sizeof(keyBuffer) - 1, keyFile);
			strcat(keyBuffer, temp2);
		} while (counter2 != NULL);
		// remove newline characters
		plainBuffer[strcspn(plainBuffer, "\n")] = '\0';
		keyBuffer[strcspn(keyBuffer, "\n")] = '\0';
	}
	fclose(keyFile);
	fclose(plaintextFile);
	return 1;
}

// check the length of a file for encryption/decryption
bool checkLengths(char* first, char* second) {
	if (strlen(first) == strlen(second)) {
		return true;
	}
	else {
		return false;
	}
}
// TODO fix this function, it rejects evertything
bool checkValidChars(char* toCheck) {
	bool found;
	for (int i = 0; i < strlen(toCheck); ++i) {
		found = false;
		for (int j = 0; j < 27; ++j) {
			if (toCheck[i] == KeyCode[j] || toCheck[i] == '\n') {
				found = true;
			}
			if (found == false) {
				return false;
			}
		}
	}

	return true;
}

// check if correct program (enc_client) is connecting to the server.  Returns 
// true if it correct, if it is another program it returns false;
bool checkHandshake(int sock, char* handshakeMessage) {
	int bytesSent, bytesReceived; // to keep track of characters written from send
																// and read from recv
	
	bytesSent = send(sock, handshakeMessage, strlen(handshakeMessage) + 1, 0);
	if (bytesSent < 0) {
		error("ERROR: error writing to socket");
	}
	if (bytesSent < strlen(handshakeMessage)) {
		error("CLIENT: Not all handshake data written");
	}
	
	// clear buffer, prepare to recieve handshake response from server
	memset(handshakeMessage, '\0', sizeof(handshakeMessage));

	bytesReceived = recv(sock, handshakeMessage, sizeof(handshakeMessage) - 1, 0);
	
	if (bytesReceived < 0) {
		error("ERROR: recv()");
	}

	//printf("Handshake: %s\n", handshakeMessage);
	// see if handshake was valid or not, return TRUE or FALSE respectively
	if (strcmp(handshakeMessage, "OK") == 0) {
		return true;
	}
	else {
		return false;
	}
	
}

#if 0
// adapted from Beej's guide to network programming, source found here: 
// https://beej.us/guide/bgnet/html/index-wide.html#sendall
void sendData(int sock, char* buffer, int* numBytes) {
	int total = 0; 
	int bytesRemaining = *numBytes;
	int bytesSent;

	while (total < *numBytes) {
		bytesSent = send(sock, buffer + total, bytesRemaining, 0);
		if (bytesSent < 0)
			error("ERROR: Socket");
		total += bytesSent;
		bytesRemaining -= bytesSent;
	}	
}
#endif

int main(int argc, char *argv[]) {
  int socketFD, portNumber, charsWritten, charsRead;
  int handshakeChars;
	struct sockaddr_in serverAddress;
  char buffer[256];
  char plaintextFile[256]; // holds filename for plaintext
  char keyFile[256];

	bool validHandshake;
	
	char plaintextBuffer[72000];
	char keyBuffer[72000];
	char cypherText[72000];
	char handshakeMessage[256]; // holds initial message to 

	memset(plaintextBuffer, '\0', sizeof(plaintextBuffer));
	memset(keyBuffer, '\0', sizeof(keyBuffer));
  
  // Check usage & args
 	if (argc < 4) { 
    fprintf(stderr,"USAGE: %s plaintext key port\n", argv[0]); 
    exit(0); 
  } 

	// load files supplied as command line arguments
	loadFiles(argv[1], argv[2], plaintextBuffer, keyBuffer);
	
	// check if file lengths match
	if(!(checkLengths(plaintextBuffer, keyBuffer))) {
		printf("%s\n", "File lengths do not match");
		exit(0);
	}

/*	
	if (checkValidChars(plaintextBuffer) == false || 
		checkValidChars(keyBuffer) == false) {
		printf("%s\n", "Invalid characters in file");
		exit(0);
	}
*/

	// Create a socket
  socketFD = socket(AF_INET, SOCK_STREAM, 0); 
  if (socketFD < 0){
    error("CLIENT: ERROR opening socket");
  }

   // Set up the server address struct
  setupAddressStruct(&serverAddress, atoi(argv[3]), "localhost");

  // Connect to server
  if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0){
    error("CLIENT: ERROR connecting");
  }
  
  // Clear out the buffer array arrays
  memset(buffer, '\0', sizeof(buffer));
  memset(handshakeMessage, '\0', sizeof(handshakeMessage));
  memset(plaintextFile, '\0', sizeof(plaintextFile));

	//set handshake message
	strcpy(handshakeMessage, "enc_client");
	validHandshake = checkHandshake(socketFD, handshakeMessage);
	
	if (validHandshake) {
		//printf("%s", "CLIENT: preparing to write data\n");
		int bytesToSend, bytesSent = 0;
		char dataToSend[144000], sendBuffer[256];

		memset(dataToSend, '\0', sizeof(dataToSend));

		// concatenate both "files" into a single buffer
		strcat(dataToSend, plaintextBuffer);
		strcat(dataToSend, keyBuffer);

		bytesToSend = (int) strlen(plaintextBuffer) + (int) strlen(keyBuffer);

		// possibly consolidate the send call into the sendData function
		sendData(socketFD, dataToSend, &bytesToSend);		
		send(socketFD, "@", 2, 0);
		
		memset(cypherText, '\0', sizeof(cypherText));

		recvAll(socketFD, cypherText, buffer, "@");
		char* delim_location = strstr(cypherText, "@");

		if (delim_location != NULL) {
			*delim_location = '\0';
		}
		printf("%s\n", cypherText);
	}

	else {
		error("ERROR: Handshake failed, client exiting");
	}
  
	close(socketFD); 
  return 0;
}
