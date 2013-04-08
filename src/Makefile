CC=gcc
SRC=src
INC=../include
CFLAGS=-I$(INC) -Wall -c
OBJS=logger.o wrap.o http.o assist.o server.o 
TARGET=server 
ifeq ($(DEBUG),)
	CFLAGS += -g -DDEBUG 
else
	CFLAGS += -O2
endif

torchilus: $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)
server.o: server.c
	$(CC) $(CFLAGS) server.c
wrap.o: wrap.c
	$(CC) $(CFLAGS) wrap.c
assist.o: assist.c
	$(CC) $(CFLAGS) assist.c
http.o: http.c
	$(CC) $(CFLAGS) http.c
logger.o: logger.c
	$(CC) $(CFLAGS) logger.c

clean: 
	echo "cleaning project"
	rm -f $(OBJS)
	rm -f $(TARGET)
	echo "clean finished"
