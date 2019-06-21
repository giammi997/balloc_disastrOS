CC=gcc
OPTS=--std=gnu99 -Wall

HEADERS=bit_map.h buddy.h
BINS=bit_map_test buddy_test

.phony: all clean

all: $(BINS)

%.o: %.c $(HEADERS)
	$(CC) $(OPTS) -c $<

bit_map_test: bit_map_test.o bit_map.o
	$(CC) $(OPTS) $^ -o $@

buddy_test: buddy_test.o buddy.o
	$(CC) $(OPTS) $^ -o $@

clean:
	rm -rf *.o *~ $(BINS)