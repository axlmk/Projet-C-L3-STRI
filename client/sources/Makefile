CFLAGS= -W -Wall -pedantic -Wno-unused-value -Wno-sign-compare -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-variable
CC=gcc

main: main.o client.o pdu.o tools.o accountio.o account_utils.o directory.o directory_utils.o record_utils.o record.o recordio.o
	$(CC) -o ./main ./main.o client.o pdu.o tools.o accountio.o account_utils.o directory.o directory_utils.o record_utils.o record.o recordio.o $(CFLAGS); rm *.o

client.o: client.c
	$(CC) -o client.o -c client.c -lm $(CFLAGS)

pdu.o: ../../server/sources/pdu.c
	$(CC) -o pdu.o -c ../../server/sources/pdu.c -lm $(CFLAGS)

tools.o: ../../server/sources/tools.c
	$(CC) -o tools.o -c ../../server/sources/tools.c -lm $(CFLAGS)

account_utils.o: ../../server/sources/account_utils.c
	$(CC) -o account_utils.o -c ../../server/sources/account_utils.c -lm $(CFLAGS)

accountio.o: ../../server/sources/accountio.c
	$(CC) -o accountio.o -c ../../server/sources/accountio.c -lm $(CFLAGS)

directory.o: ../../server/sources/directory.c
	$(CC) -o directory.o -c ../../server/sources/directory.c -lm $(CFLAGS)

recordio.o: ../../server/sources/recordio.c
	$(CC) -o recordio.o -c ../../server/sources/recordio.c -lm $(CFLAGS)

record.o: ../../server/sources/record.c
	$(CC) -o record.o -c ../../server/sources/record.c -lm $(CFLAGS)

directory_utils.o: ../../server/sources/directory_utils.c
	$(CC) -o directory_utils.o -c ../../server/sources/directory_utils.c -lm $(CFLAGS)

record_utils.o: ../../server/sources/record_utils.c
	$(CC) -o record_utils.o -c ../../server/sources/record_utils.c -lm $(CFLAGS)

main.o: main.c ../headers/client.h ../../server/headers/pdu.h
	$(CC) -o main.o -c main.c -lm $(CFLAGS)
