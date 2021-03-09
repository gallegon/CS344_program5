/* filename: send_recv.h
	author: Nicholai Gallegos
	date: 03/08/2021
*/
#ifndef SEND_RECV
#define SEND_RECV

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>

// function prototypes for send_recv
void recvAll (int sock, char*, char*, char*);
void sendData(int sock, char*, int*);
// I put error in here because it was convienient, no other reason than that
void error(const char*);

#endif
