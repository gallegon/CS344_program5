CC=gcc -std=gnu99 -g

all: keygen enc_client enc_server

encrypt_decrypt.o:
	$(CC) -c encrypt_decrypt.c

send_recv.o:
	$(CC) -c send_recv.c

keygen.o:
	$(CC) -c keygen.c

enc_client.o:
	$(CC) -c enc_client.c

enc_server.o:
	$(CC) -c enc_server.c

keygen: keygen.o encrypt_decrypt.o
	$(CC) -o keygen keygen.o encrypt_decrypt.o

enc_client: enc_client.o send_recv.o
	$(CC) -o enc_client enc_client.o send_recv.o

enc_server: enc_server.o encrypt_decrypt.o send_recv.o
	$(CC) -o enc_server enc_server.o encrypt_decrypt.o send_recv.o

clean:
	rm -f *.o keygen enc_client enc_server

