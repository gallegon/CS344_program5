#include "client.h"

int main(int argc, char *argv[]) {
	// Check usage & args
	if (argc < 4) { 
		fprintf(stderr,"USAGE: %s plaintext key port\n", argv[0]); 
		exit(0); 
	} 

	run_client(argv[1], argv[2], atoi(argv[3]), "enc_client");
	
	return 0;
}
