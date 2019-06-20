#include <assert.h>
#include "bit_map.h"

// Get number of bytes needed for 'num_bits' bits
int BitMap_getBytes(int num_bits) {
    return num_bits/8 + ((num_bits % 8)!=0);
}

// Initialize bitmap with preallocated 'buffer'
void BitMap_init(BitMap * bit_map, int num_bits, uint8_t * buffer) {
    bit_map->buffer = buffer;
    bit_map->num_bits = num_bits;
    bit_map->size = BitMap_getBytes(num_bits);
}

// Set 'bit_num'-th bit to 'status'
void BitMap_setBit(BitMap * bit_map, int bit_num, int status) {
    assert(status == 0 || status == 1);
    // Get corresponding byte and bit within it
    int byte = bit_num >> 3;
    assert(byte < bit_map->size);
    int bit_in_byte = bit_num % 8;
    // Set new 'status' to 'bit_num'-th bit
    if(status)
        bit_map->buffer[byte] |= (1 << bit_in_byte);
    else
        bit_map->buffer[byte] &= ~(1 << bit_in_byte);
}

// Get 'status' of 'bit_num'-th bit [either 0 or 1]
int BitMap_getBit(const BitMap * bit_map, int bit_num) {
    // Get corresponding byte and bit within it
    int byte = bit_num >> 3;
    assert(byte < bit_map->size);
    int bit_in_byte = bit_num % 8;
    // Get status from 'bit_num'-th bit
    return (bit_map->buffer[byte] >> bit_in_byte) & 0x01;
}