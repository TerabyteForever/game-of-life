CC = gcc
CFLAGS = `sdl2-config --cflags --libs`

all: life

life: gol.o
	$(CC) $(CFLAGS) gol.o -o life

gol.o: gol.c
	$(CC) $(CFLAGS) -c gol.c

clean:
	rm -f life gol.o