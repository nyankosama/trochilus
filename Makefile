CC=gcc
SRC=src
INC=include
CFLAGS=-I. -Wall -c
OBJS=wrap.o http.o assist.o server.o 
TARGET=server 
ifeq ($(DEBUG),)
	CFLAGS += -g -DDEBUG 
else
	CFLAGS += -O2
endif

torchilus: $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)
server.o: server.c wrap.h
	$(CC) $(CFLAGS) server.c
wrap.o: wrap.h wrap.c
	$(CC) $(CFLAGS) wrap.c
assist.o: assist.h assist.c
	$(CC) $(CFLAGS) assist.c
http.o: http.h http.c
	$(CC) $(CFLAGS) http.c

clean: 
	echo "cleaning project"
	rm -f $(OBJS)
	rm -f $(TARGET)
	echo "clean finished"
