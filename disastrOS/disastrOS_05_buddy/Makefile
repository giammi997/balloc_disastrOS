CC=gcc
OPTS=--std=gnu99 -Wall

HEADERS=bit_map.h block.h balloc_test.h
BINS=bit_map_test block_test balloc_test

.phony: all clean

all: $(BINS)

%.o: %.c $(HEADERS)
	$(CC) $(OPTS) -c $<

bit_map_test: bit_map_test.o bit_map.o
	$(CC) $(OPTS) $^ -o $@

block_test: block_test.o block.o
	$(CC) $(OPTS) $^ -o $@

balloc_test: balloc_test.o balloc.o bit_map.o block.o
	$(CC) $(OPTS) $^ -o $@

clean:
	rm -rf *.o *.gch *~ $(BINS)