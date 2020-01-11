CFLAGS= -W -Wall -pedantic
CC=gcc

main: main.o client.o
			$(CC) -o ./main ./main.o ./client.o -lm $(CFLAGS); rm *.o

server.o: client.c
					$(CC) -o client.o -c client.c -lm $(CFLAGS)

main.o: main.c ../headers/client.h
				$(CC) -o main.o -c main.c -lm $(CFLAGS)
