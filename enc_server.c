#include "server.h"

int main(int argc, char *argv[]){
	#if 0
	int connectionSocket, charsRead;
	char buffer[256], plaintextBuffer[72000], keyBuffer[72000], cypherText[72000];
	struct sockaddr_in serverAddress, clientAddress;
	bool socketBound = true;
	socklen_t sizeOfClientInfo = sizeof(clientAddress);
	#endif
	// Check usage & args
	if (argc < 2) { 
		fprintf(stderr,"USAGE: %s port\n", argv[0]); 
		exit(1);
	} 

	run_server(atoi(argv[1]), "enc_client", "encrypt");
	
	return 0;
}
