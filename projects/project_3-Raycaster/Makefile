CC=gcc
CFLAGS=-Wall
RM=rm -f

all: raycast

raycast: raycast.o
	$(CC) $(CFLAGS) -o raycast raycast.c ppmrw.c -lm	
clean:
	$(RM) raycast *~ *.o
