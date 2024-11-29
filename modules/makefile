CC=gcc
CFLAGS=-Wall -Wextra -g

all: test

test: test.o list.o
    $(CC) $(CFLAGS) -o test test.o list.o

list.o: list.c list.h
    $(CC) $(CFLAGS) -c list.c

test.o: test.c list.h
    $(CC) $(CFLAGS) -c test.c

clean:
    rm -f *.o test