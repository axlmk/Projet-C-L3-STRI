CC=gcc
CFLAGS=-W -Wall -pedantic -ln -g
LDFLAGS=
EXEC=main

all: $(EXEC)

main: main.o server.o pdu.o install.o directory.o authentication.o account.o accountio.o account_utils.o directory_utils.o recordio.o record_utils.o record.o tools.o
	$(CC) -o $@ $^ $(LDFLAGS);rm -f ./*.o

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
