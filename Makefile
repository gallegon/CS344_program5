CC=gcc -std=gnu99 -g

all: keygen enc_client enc_server dec_client dec_server

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

dec_client.o:
	$(CC) -c dec_client.c

dec_server.o:
	$(CC) -c dec_server.c

server.o:
	$(CC) -c server.c
client.o:
	$(CC) -c client.c

keygen: keygen.o encrypt_decrypt.o
	$(CC) -o keygen keygen.o encrypt_decrypt.o

enc_client: enc_client.o send_recv.o client.o
	$(CC) -o enc_client enc_client.o send_recv.o client.o

enc_server: enc_server.o encrypt_decrypt.o send_recv.o server.o
	$(CC) -o enc_server enc_server.o encrypt_decrypt.o send_recv.o server.o

dec_client: dec_client.o send_recv.o client.o
	$(CC) -o dec_client dec_client.o send_recv.o client.o

dec_server: dec_server.o encrypt_decrypt.o send_recv.o server.o
	$(CC) -o dec_server dec_server.o encrypt_decrypt.o send_recv.o server.o

clean:
	rm -f *.o keygen enc_client enc_server dec_client dec_server

