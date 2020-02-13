CC=gcc
CFLAGS=-W -Wall -pedantic -ln
LDFLAGS=
EXEC=main

all: $(EXEC)

main: main.o server.o record.o pdu.o install.o directory.o authentication.o accountio.o
	$(CC) -o $@ $^ $(LDFLAGS);rm -f ./*.o

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
