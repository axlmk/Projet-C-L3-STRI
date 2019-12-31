CFLAGS= -W -Wall -pedantic
CC=gcc

main: main.o
			$(CC) -o ../build/main ./main.o; rm *.o

main.o: main.c
				$(CC) -o main.o -c main.c $(CFLAGS)
