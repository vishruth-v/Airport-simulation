airport.out: server.o client.o
	gcc -o airport server.c client.c -lm

server.o: server.c airport.h
	gcc -c server.c -lm

client.o: client.c airport.h
	gcc -c client.c -lm