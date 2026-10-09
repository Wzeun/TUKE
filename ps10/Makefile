CC=gcc
CFLAGS=-std=c11 -Wall -Werror -Wconversion

all: game

game: main.o k.o hof.o ui.o
	$(CC) $(CFLAGS) -o game main.o k.o hof.o ui.o

k.o: k.c
	$(CC) $(CFLAGS) -c k.c

hof.o: hof.c
	$(CC) $(CFLAGS) -c hof.c

ui.o: ui.c
	$(CC) $(CFLAGS) -c ui.c

clean:
	rm -f *.o game