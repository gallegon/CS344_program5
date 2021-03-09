/* filename: dec_client.c
	author: Nicholai Gallegos
	date: 3/8/2021 */
#include "client.h"

// run dec_client
int main(int argc, char* argv[]) {
	
	// Check usage & args
	if (argc < 4) { 
		fprintf(stderr,"USAGE: %s cyphertext key port\n", argv[0]); 
		exit(0); 
	}

	// run client with handshake token "dec_client"
	run_client(argv[1], argv[2], atoi(argv[3]), "dec_client");
	return 0;
}
