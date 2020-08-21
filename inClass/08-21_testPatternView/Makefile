CC=gcc
CFLAGS=-Wall
RM=rm -f

all: test_pattern_view

test_pattern: test_pattern_view.c
	$(CC) $(CFLAGS) test_pattern_view.c -o test_pattern_view

clean:
	$(RM) test_pattern_view *~ image.data

