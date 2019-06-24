#pragma once
#include <stdint.h>

// Bitmap structure
typedef struct BitMap {
    uint8_t * buffer;
    int size;
    int num_bits;
} BitMap;

// Get number of bytes needed for 'num_bits' bits
int BitMap_getBytes(int num_bits);

// Initialize bitmap with preallocated 'buffer'
void BitMap_init(BitMap * bit_map, int num_bits, uint8_t * buffer);

// Set 'bit_num'-th bit to 'status'
void BitMap_setBit(BitMap * bit_map, int bit_num, int status);

// Get 'status' of 'bit_num'-th bit
int BitMap_getBit(const BitMap * bit_map, int bit_num);