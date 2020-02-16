CFLAGS= -W -Wall -pedantic -Wno-unused-value -Wno-sign-compare -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-variable
CC=gcc

main: main.o client.o pdu.o tools.o
	$(CC) -o ./main ./main.o client.o pdu.o tools.o $(CFLAGS); rm *.o

client.o: client.c
	$(CC) -o client.o -c client.c -lm $(CFLAGS)

pdu.o: ../../server/sources/pdu.c
	$(CC) -o pdu.o -c ../../server/sources/pdu.c -lm $(CFLAGS)

tools.o: ../../server/sources/tools.c
	$(CC) -o tools.o -c ../../server/sources/tools.c -lm $(CFLAGS)

main.o: main.c ../headers/client.h ../../server/headers/pdu.h
	$(CC) -o main.o -c main.c -lm $(CFLAGS)
