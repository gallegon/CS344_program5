#include "client.h"

int main(int argc, char* argv[]) {
	
	// Check usage & args
	if (argc < 4) { 
		fprintf(stderr,"USAGE: %s cyphertext key port\n", argv[0]); 
		exit(0); 
	}

	run_client(argv[2], argv[1], atoi(argv[3]), "dec_client");
	return 0;
}
