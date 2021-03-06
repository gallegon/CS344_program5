CC=gcc -std=gnu99 -g

all: keygen.o enc_client.o enc_server.o
	$(CC) -o keygen keygen.o 
	$(CC) -o enc_client enc_client.o 
	$(CC) -o enc_server enc_server.o

keygen.o:
	$(CC) -c keygen.c

enc_client.o:
	$(CC) -c enc_client.c

enc_server.o:
	$(CC) -c enc_server.c


keygen: keygen.o
	$(CC) -o keygen keygen.o

enc_client: enc_client.o
	$(CC) -o enc_client enc_client.o

enc_server: enc_server.o
	$(CC) -o enc_server enc_server.c

clean:
	rm -f *.o keygen enc_client enc_server

