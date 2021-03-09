/* file Name: client.c
	author: Nicholai Gallegos
	date: 3/8/2021
	note: uses code modified from module examples */
#include "client.h"

// keycode for encryption/decryption
static const char KeyCode[28] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

// Set up the address struct
void setupAddressStruct(struct sockaddr_in* address, int portNumber, 
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

// loads files with paths plaintextPath and keyPath into buffers for use with
// client
int loadFiles(char* plainTextPath, char* keyPath, char* plainBuffer, 
		char* keyBuffer) {
	char temp1[72000], temp2[72000];
	char* counter1, * counter2;

	memset(temp1, '\0', sizeof(temp1));
	memset(temp2, '\0', sizeof(temp2));

	FILE* plaintextFile = fopen(plainTextPath, "r");
	FILE* keyFile = fopen(keyPath, "r");

	// check if any errors opening files (file exists or not, etc..)
	if (keyFile == NULL || plaintextFile == NULL) {
		error("FILE");
		return -1;
	}

	else {
		size_t buffSize = 72000;
		ssize_t nread;
		char* newlineLoc;
		/* getline from file, only one call to getline for eachsince files only 
			contain one line */
		nread = getline(&plainBuffer, &buffSize, plaintextFile);
		nread = getline(&keyBuffer, &buffSize, keyFile);
		
		// remove newline characters from buffers
		if ((newlineLoc = strstr(plainBuffer, "\n")) != NULL) {
			*newlineLoc = '\0';
		}
		if ((newlineLoc = strstr(keyBuffer, "\n")) != NULL) {
			*newlineLoc = '\0';
		}	
	}
	// close open file descriptors
	fclose(keyFile);
	fclose(plaintextFile);
	return 1;
}

// check the length of a file for encryption/decryption
bool checkLengths(char* file, char* key) {
	// check if the keylength is atleast as long as the plaintext length
	if (strlen(key) >= strlen(file)) {
		return true;
	}
	else {
		return false;
	}
}

/* checkValidChars:
	Checks if a file (loading into buffer toCheck) contains valid characters */
bool checkValidChars(char* toCheck) {
	bool found;
	// iterate through entire lenght of toCheck
	for (int i = 0; i < strlen(toCheck); ++i) {
		found = false; // assume false initially
		for (int j = 0; j < 27; ++j) {
			// if the current character is in the list, set to true and break 
			// from j for loop
			if (toCheck[i] == KeyCode[j] || toCheck[i] == '\n') {
				found = true;
				break;
			}
		}
		// if the current character is not in the list, return false
		if (found == false) {
			return false;
		}
	}

	return true;
}

/* checkHandshake:
	check if correct program (enc_client) is connecting to the server.  Returns 
	true if it correct, if it is another program it returns false; */
bool checkHandshake(int sock, char* handshakeMessage) {
	int bytesSent, bytesReceived; // to keep track of characters written from send
	// and read from recv

	// send initial handshake message
	bytesSent = send(sock, handshakeMessage, strlen(handshakeMessage) + 1, 0);
	if (bytesSent < 0) {
		error("ERROR: error writing to socket");
	}
	if (bytesSent < strlen(handshakeMessage)) {
		error("CLIENT: Not all handshake data written");
	}

	// clear buffer, prepare to recieve handshake response from server
	memset(handshakeMessage, '\0', sizeof(handshakeMessage));

	// recieve handshake response from server
	bytesReceived = recv(sock, handshakeMessage, sizeof(handshakeMessage) - 1, 0);

	if (bytesReceived < 0) {
		error("ERROR: recv()");
	}

	// see if handshake was valid or not, return TRUE or FALSE respectively
	if (strcmp(handshakeMessage, "OK") == 0) {
		return true;
	}
	else {
		return false;
	}

}

void run_client(char* plaintextPath, char* keyfilePath, int portNumber,
		char* handshakeToken) {
	
	int socketFD, charsWritten, charsRead;
	int handshakeChars;
	struct sockaddr_in serverAddress;
	char buffer[256]; // for sending and recieving data from server

	bool validHandshake;

	char plaintextBuffer[72000]; // holds plaintext
	char keyBuffer[72000]; // holds key up to the size of plaintext
	char cypherText[72000]; // for recieved cypher text from server
	char handshakeMessage[256]; // holds handshake token to send to server

	// clear buffers before use
	memset(plaintextBuffer, '\0', sizeof(plaintextBuffer));
	memset(keyBuffer, '\0', sizeof(keyBuffer));
	memset(handshakeMessage, '\0', sizeof(handshakeMessage));

	// copy the passed in handshake token to the local one
	strcpy(handshakeMessage, handshakeToken);
	
	// load files supplied as command line arguments
	loadFiles(plaintextPath, keyfilePath, plaintextBuffer, keyBuffer);

	// check if file lengths match
	if(!(checkLengths(plaintextBuffer, keyBuffer))) {
		error("ERROR: file lengths don't match");
	}
	
	// make key length make plaintext length
	keyBuffer[strlen(plaintextBuffer)] = '\0';

	// check if the files contain only valid characters	
	if (checkValidChars(plaintextBuffer) == false || 
		checkValidChars(keyBuffer) == false) {
		error("ERROR: Invalid characters in file");
	}
	
	// Create a socket
	socketFD = socket(AF_INET, SOCK_STREAM, 0); 
	if (socketFD < 0){
		error("CLIENT: ERROR opening socket");
	}

	// Set up the server address struct
	setupAddressStruct(&serverAddress, portNumber, "localhost");

	// Connect to server
	if (connect(socketFD, (struct sockaddr*)&serverAddress, 
		sizeof(serverAddress)) < 0) {
		error("CLIENT: ERROR connecting");
	}

	// Clear out the buffer array arrays
	memset(buffer, '\0', sizeof(buffer));

	validHandshake = checkHandshake(socketFD, handshakeMessage);

	if (validHandshake) {
		int bytesToSend, bytesSent = 0;
		char dataToSend[144000], sendBuffer[256];

		memset(dataToSend, '\0', sizeof(dataToSend));

		/* concatenate both "files" into a single buffer before sending.  I did it
		 	this way because at the time it seemed like an  easier solution to 
			split the files (buffers) on the server end by dividing the length of
			the total buffer by 2, and iterating through it and comparing the (i)th
			and (i + (buffer length / 2))th terms on the server side instead of 
			having to so two sets of sends on the client side and 2 sets of 
			recieves on the server side */
		strcat(dataToSend, plaintextBuffer);
		strcat(dataToSend, keyBuffer);

		// get the total number of bytes that need to be sent
		bytesToSend = (int) strlen(plaintextBuffer) + (int) strlen(keyBuffer);

		// send the entire buffer
		sendData(socketFD, dataToSend, &bytesToSend);		
		
		/* This is a way of "encapsulating" the data if you can call it that.
			It's just a token that the server looks for to tell it that all the
			data has been sent without having to send an integer or string of the
			size */
		send(socketFD, "@", 2, 0);

		/* clear memory before recieving response from server, which is the 
			cypher text */
		memset(cypherText, '\0', sizeof(cypherText));

		recvAll(socketFD, cypherText, buffer, "@");
		
		/* The server sends the same delimiter to indicate that all the data has
			been sent. This is to replace the token with a null terminator */
		char* delim_location = strstr(cypherText, "@");
		if (delim_location != NULL) {
			*delim_location = '\0';
		}

		// print the recieved cypher text to the screen
		printf("%s", cypherText);
	}

	else {
		error("ERROR: Handshake failed, client exiting");
	}

	close(socketFD); 
}
