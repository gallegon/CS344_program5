#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>  // ssize_t
#include <sys/socket.h> // send(),recv()
#include <netdb.h>      // gethostbyname()
#include "send_recv.h"

void run_client(char* plaintextPath, char* keyfilePath, int portNumber,
      char* handshakeToken);
bool checkHandshake(int sock, char* handshakeMessage);
bool checkValidChars(char* toCheck);
bool checkLengths(char* first, char* second);
int loadFiles(char* plainTextPath, char* keyPath, char* plainBuffer,
      char* keyBuffer);
void setupAddressStruct(struct sockaddr_in* address, int portNumber, 
      char* hostname);


#endif
