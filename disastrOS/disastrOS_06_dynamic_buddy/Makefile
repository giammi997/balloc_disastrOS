CC=gcc
OPTS=--std=gnu99 -Wall

HEADERS=bit_map.h block.h balloc.h
BINS=balloc_test

.phony: all clean

all: $(BINS)

%.o: %.c $(HEADERS)
	$(CC) $(OPTS) -c $<

balloc_test: balloc_test.o balloc.o bit_map.o block.o
	$(CC) $(OPTS) $^ -o $@ -lm

clean:
	rm -rf *.o *.gch *~ $(BINS)