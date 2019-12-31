CFLAGS= -W -Wall -pedantic
CC=gcc

main: main.o server.o
			$(CC) -o ../build/main ./main.o $(CFLAGS); rm *.o

server.o: server.c
					$(CC) -o server.o -c server.c $(CFLAGS)

main.o: main.c ../headers/server.h
				$(CC) -o main.o -c main.c $(CFLAGS)
