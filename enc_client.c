#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>  // ssize_t
#include <sys/socket.h> // send(),recv()
#include <netdb.h>      // gethostbyname()

static const char KeyCode[28] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
/**
* Client code
* 1. Create a socket and connect to the server specified in the command arugments.
* 2. Prompt the user for input and send that input as a message to the server.
* 3. Print the message received from the server and exit the program.
*/

// Error function used for reporting issues
void error(const char *msg) { 
  perror(msg); 
  exit(0); 
} 

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

bool checkValidChars(char* toCheck) {
	for (int i = 0; i < strlen(toCheck); ++i) {
		for (int j = 0; j < 27; ++j) {
			if (toCheck[i] != KeyCode[j]) {
				return false;
			}
		}
	}

	return true;
}

int main(int argc, char *argv[]) {
  int socketFD, portNumber, charsWritten, charsRead;
  int handshakeChars;
	struct sockaddr_in serverAddress;
  char buffer[256];
  char plaintextFile[256]; // holds filename for plaintext
  char keyFile[256];

	char plaintextBuffer[72000];
	char keyBuffer[72000];
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
	
	printf("%s%s", plaintextBuffer, keyBuffer);
	fflush(stdout);
	if(!(checkLengths(plaintextBuffer, keyBuffer))) {
		printf("%s\n", "File lengths do not match");
		exit(0);
	}

	if (!(checkValidChars(plaintextBuffer)) || !(checkValidChars(keyBuffer))) {
		printf("%s\n", "Invalid characters in file");
		exit(0);
	}
	
	//printf("%s%s", plaintextBuffer, keyBuffer);
	fflush(stdout);
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
  // Get input message from user
  /*
	printf("CLIENT: Enter text to send to the server, and then hit enter: ");
  */
  // Clear out the buffer array arrays
  memset(buffer, '\0', sizeof(buffer));
  memset(handshakeMessage, '\0', sizeof(handshakeMessage));
  memset(plaintextFile, '\0', sizeof(plaintextFile));

	//set handshake message
	strcpy(handshakeMessage, "lol");
  
	/*
	// Get input from the user, trunc to buffer - 1 chars, leaving \0
  fgets(buffer, sizeof(buffer) - 1, stdin);
  // Remove the trailing \n that fgets adds
  buffer[strcspn(buffer, "\n")] = '\0'; 
	*/

  // Send message to server
  // Write to the server
  /*
	charsWritten = send(socketFD, buffer, strlen(buffer), 0); 
  if (charsWritten < 0){
    error("CLIENT: ERROR writing to socket");
  }
  if (charsWritten < strlen(buffer)){
    printf("CLIENT: WARNING: Not all data written to socket!\n");
  }
	*/

	// send handshake message to server
	handshakeChars = send(socketFD, handshakeMessage, strlen(handshakeMessage), 0);
	
	if (handshakeChars < 0) {
		error("CLIENT: ERROR writing to socket");
	}
	if (handshakeChars < strlen(handshakeMessage)) {
		error("CLIENT: Not all handshake data written, potentially unpredictable behavior!");
	}
	
	// clear out handshakeMessage to recieve reply from server
	memset(handshakeMessage, '\0', sizeof(handshakeMessage));

	handshakeChars = recv(socketFD, handshakeMessage, sizeof(handshakeMessage) - 1, 0);
	if (handshakeChars < 0) {
		error("CLIENT: ERROR reading from socket");
	}
	printf("CLIENT: Handshake: %s\n", handshakeMessage);
	
	if (strcmp(handshakeMessage, "OK") == 0) {
		printf("%s", "CLIENT: preparing to write data\n");
		int bytesToSend;


	}
	else {
		printf("%s\n", "Handshake failed, client exiting");
	}
	/*
  // Clear out the buffer again for reuse
  memset(buffer, '\0', sizeof(buffer));
  // Read data from the socket, leaving \0 at end
  charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0); 
  if (charsRead < 0){
    error("CLIENT: ERROR reading from socket");
  }
  printf("CLIENT: I received this from the server: \"%s\"\n", buffer);
	*/
  // Close the socket
  close(socketFD); 
  return 0;
}
