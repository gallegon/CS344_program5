#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include "encrypt_decrypt.h"
#include "send_recv.h"

void setupAddressStruct(struct sockaddr_in* address, int portNumber);
void run_server(int portNumber, char* validHandshakeToken, char* mode);

#endif
