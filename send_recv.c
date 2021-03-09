/* filename: send_recv.c
	author: Nicholai Gallegos
	date: 3/8/2021
*/
#include "send_recv.h"

// adapted from Beej's guide to network programming, source found here: 
// https://beej.us/guide/bgnet/html/index-wide.html#sendall
// sends all data from a buffer through specified socket sock
void sendData(int sock, char* buffer, int* numBytes) {
   int total = 0;  
   int bytesRemaining = *numBytes;
   int bytesSent;

	// keep sending until no more bytes remaining
   while (total < *numBytes) {
      bytesSent = send(sock, buffer + total, bytesRemaining, 0); 
      if (bytesSent < 0)
         error("ERROR: Socket");
      total += bytesSent;
      bytesRemaining -= bytesSent;
   }   
}

// Error function used for reporting issues
void error(const char *msg) {
  perror(msg);
  exit(1);
} 

// recieves all data from sender until a delimiter is found
void recvAll (int sock, char* storeBuffer, char* sockBuffer, char* delimiter) {
   int charsRead; // amount of bytes read from socket

	// keep recieving data until delimiting string found
   while (strstr(sockBuffer, delimiter) == NULL) {
      memset(sockBuffer, '\0', sizeof(sockBuffer));
      charsRead = recv(sock, sockBuffer, sizeof(sockBuffer) - 1, 0); 
          
      if (charsRead < 0) {
         error("Server: Error reading from socket");
      }   
      //store what the socket recieved into the storage buffer
      strcat(storeBuffer, sockBuffer);
   }   
}

