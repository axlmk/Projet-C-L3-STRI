CFLAGS= -W -Wall -pedantic
CC=gcc

main: main.o client.o
			$(CC) -o ./main ./main.o $(CFLAGS); rm *.o

server.o: client.c
					$(CC) -o client.o -c client.c $(CFLAGS)

main.o: main.c ../headers/client.h
				$(CC) -o main.o -c main.c $(CFLAGS)
