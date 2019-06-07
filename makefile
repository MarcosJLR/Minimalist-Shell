CC = gcc
CFLAGS = -Wall -std=gnu99
OBJ = myls mygrep mychmod

tesh: tesh.c $(OBJ)
	$(CC) -o $@ $< $(CFLAGS)

myls: ls.c
	$(CC) -o $@ $< $(CFLAGS)

mygrep: grep.c
	$(CC) -o $@ $< $(CFLAGS)

mychmod: chmod.c
	$(CC) -o $@ $< $(CFLAGS)

.PHONY: clean

clean: 
	rm myls mygrep mychmod tesh