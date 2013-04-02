CC=gcc
SRC=src
INC=include
CFLAGS=-I. -Wall -c
OBJS=wrap.o server.o client.o 
TARGET=server client

torchilus: server.o client.o wrap.o
	$(CC) wrap.o server.o -o server
	$(CC) client.o wrap.o -o client
server.o: server.c wrap.h
	$(CC) $(CFLAGS) server.c
client.o: client.c wrap.h
	$(CC) $(CFLAGS) client.c
wrap.o: wrap.h wrap.c
	$(CC) $(CFLAGS) wrap.c

clean: 
	@echo "cleaning project"
	-rm *.o server client
	@echo "clean finished"
