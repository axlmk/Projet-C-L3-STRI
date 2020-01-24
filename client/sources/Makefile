CFLAGS= -W -Wall -pedantic -Wno-unused-value -Wno-sign-compare -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-variable
CC=gcc

main: main.o client.o
			$(CC) -o ./main ./main.o client.o $(CFLAGS); rm *.o

client.o: client.c
					$(CC) -o client.o -c client.c -lm $(CFLAGS)

main.o: main.c ../headers/client.h
				$(CC) -o main.o -c main.c -lm $(CFLAGS)
