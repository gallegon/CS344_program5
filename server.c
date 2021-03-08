#include "server.h"

// Set up the address struct for the server socket
void setupAddressStruct(struct sockaddr_in* address, int portNumber){

	// Clear out the address struct
	memset((char*) address, '\0', sizeof(*address)); 

	// The address should be network capable
	address->sin_family = AF_INET;
	// Store the port number
	address->sin_port = htons(portNumber);
	// Allow a client at any address to connect to this server
	address->sin_addr.s_addr = INADDR_ANY;
}

// runs server
// TODO comment fully
void run_server(int portNumber, char* validHandshakeToken, char* mode) {
	int connectionSocket, charsRead;
	char buffer[256], plaintextBuffer[72000], keyBuffer[72000], cypherText[72000];
	struct sockaddr_in serverAddress, clientAddress;
	bool socketBound = true;
	socklen_t sizeOfClientInfo = sizeof(clientAddress);
	
	// Create the socket that will listen for connections
	int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket < 0) {
		error("ERROR opening socket");
	}

	// Set up the address struct for the server socket
	setupAddressStruct(&serverAddress, portNumber);

	// Associate the socket to the port
	if (bind(listenSocket, 
				(struct sockaddr *)&serverAddress, 
				sizeof(serverAddress)) < 0){
		error("ERROR on binding");
		socketBound = false;
	}

	// Start listening for connetions. Allow up to 5 connections to queue up
	listen(listenSocket, 5); 

	// Accept a connection, blocking if one is not available until one connects
	while(1 && socketBound){
		char recieveBuffer[144000];

		// Accept the connection request which creates a connection socket
		connectionSocket = accept(listenSocket, 
				(struct sockaddr *)&clientAddress, 
				&sizeOfClientInfo); 
		if (connectionSocket < 0){
			error("ERROR on accept");
		}

		// printf("SERVER: Connected to client running at host %d port %d\n", ntohs(clientAddress.sin_addr.s_addr), ntohs(clientAddress.sin_port));

		// Get the message from the client and display it
		memset(buffer, '\0', 256);
		// Read the client's message from the socket
		charsRead = recv(connectionSocket, buffer, 255, 0); 
		if (charsRead < 0){
			error("ERROR reading from socket");
		}
		//printf("SERVER: I received this from the client: \"%s\"\n", buffer);

		// Send a Success message back to the client
		if (strcmp(buffer, validHandshakeToken) != 0) {	
			charsRead = send(connectionSocket, "BAD_HANDSHAKE", 14, 0);

			if (charsRead < 0) {
				error("ERROR writing to socket");
			}
		}	
		else {
			memset(recieveBuffer, '\0', sizeof(recieveBuffer));
			charsRead = send(connectionSocket, "OK", 3, 0); 
			if (charsRead < 0){
				error("ERROR writing to socket");
			}

			recvAll(connectionSocket, recieveBuffer, buffer, "@");

			// for testing
			//printf("Server recieved: %s\n", recieveBuffer);	
		}

		// remove delimiting character from recieved data
		char* delim_location = strstr(recieveBuffer, "@");
		*delim_location = '\0';

		// clearout buffers before copying data recieved into them
		memset(plaintextBuffer, '\0', sizeof(plaintextBuffer));
		memset(keyBuffer, '\0', sizeof(keyBuffer));
		memset(cypherText, '\0', sizeof(cypherText));

		int buffLen = (int) strlen(recieveBuffer) / 2;

		for (int i = 0; i < buffLen; ++i) {
			plaintextBuffer[i] = recieveBuffer[i];
			keyBuffer[i] = recieveBuffer[i + buffLen];
		}

		if (strcmp(mode, "encrypt") == 0) 
			encryptPlaintext(plaintextBuffer, keyBuffer, cypherText, buffLen);
		else if (strcmp(mode, "decrypt") == 0)
			decryptKey (plaintextBuffer, keyBuffer, cypherText, buffLen);

		//printf("server cypher text: %s\n", cypherText);

		int bytesToSend = strlen(cypherText);

		sendData(connectionSocket, cypherText, &bytesToSend);
		send(connectionSocket, "@", 2, 0); 
		close(connectionSocket); 
	}
	// Close the listening socket
	close(listenSocket); 
}
