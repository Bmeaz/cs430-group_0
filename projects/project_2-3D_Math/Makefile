CC=gcc
CFLAGS=-Wall
RM=rm -f

all: v3test

v3test: v3test.o
	$(CC) $(CFLAGS) -o v3test  v3test.c v3math.c -lm
clean:
	$(RM) v3test *~ *.o
