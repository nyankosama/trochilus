torchilus:server client print
server: wrap.o wrap.h server.o 
	gcc wrap.o server.o -o server
server.o: server.c wrap.h
	gcc -c server.c
client: client.o wrap.o 
	gcc client.o wrap.o -o client
wrap.o: wrap.h wrap.c
	gcc -c wrap.c
print: test.c
	gcc test.c -o print
clean: 
	@echo "cleaning project"
	-rm *.o server print client
	@echo "clean finished"
