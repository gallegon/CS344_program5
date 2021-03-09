#include "server.h"

// run enc_server
int main(int argc, char *argv[]){
	// Check usage & args
	if (argc < 2) { 
		fprintf(stderr,"USAGE: %s port\n", argv[0]); 
		exit(1);
	} 

	// run server in encrypt mode, and only accept "enc_client" handshake
	run_server(atoi(argv[1]), "enc_client", "encrypt");
	
	return 0;
}
