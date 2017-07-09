CC=clang
# CC=gcc
CFLAGS=`pkg-config --libs opencv --cflags opencv` -levent -lm

all	: server client

server 	:	server.c
	$(CC) server.c libs/stcam.c libs/udpsock.c libs/b64.c -o server $(CFLAGS)

client 	:	client.c
	$(CC) client.c libs/stcam.c libs/udpsock.c libs/b64.c -o client $(CFLAGS)

clean	:
	rm -rf server
	rm -rf client
