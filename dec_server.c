#include "server.h"

int main(int argc, char* argv[]) {	
	if (argc < 2) { 
		fprintf(stderr,"USAGE: %s port\n", argv[0]); 
		exit(1);
	} 

	run_server(atoi(argv[1]), "dec_client", "decrypt");
	return 0;
}
