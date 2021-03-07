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

void recvAll (int sock, char*, char*, char*);
void sendData(int sock, char*, int*);
void error(const char*);

#endif
