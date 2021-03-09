/* filename: dec_server.c
	author: Nicholai Gallegos
	date: 3/8/2021 */
#include "server.h"

// run dec_server
int main(int argc, char* argv[]) {	
	if (argc < 2) { 
		fprintf(stderr,"USAGE: %s port\n", argv[0]); 
		exit(1);
	} 

	// run server in decrypt mode and only accept handshake token "dec_client"
	run_server(atoi(argv[1]), "dec_client", "decrypt");
	return 0;
}
