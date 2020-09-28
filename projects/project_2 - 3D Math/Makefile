CC=gcc
CFLAGS=-Wall
RM=rm -f

all: v3math v3test

v3math: v3math.c
	$(CC) $(CFLAGS) v3math.c -o v3math -lm

v3test: v3test.c
	$(CC) $(CFLAGS) v3test.c -o v3test -lm

clean:
	$(RM) v3math *~ v3test *~
