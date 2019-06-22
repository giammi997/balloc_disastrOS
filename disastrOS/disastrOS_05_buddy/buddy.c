#include "buddy.h"
#include <stdio.h>

// Block init
void * Block_init(char * mem, int block_size, int bitmap_idx) {
    assert(block_size > 0 && bitmap_idx > 0);
    // Add proper preamble
    int * m = (int*) mem;
    *m++ = bitmap_idx;
    *m++ = block_size;
    return (void*) m;
}

// Block clean
void Block_clean(char * mem, int * block_size, int * bitmap_idx) {
    // Return and clean preamble
    int * m = (int*) mem;
    // Side-effect on 'block_size' and 'bitmap_idx'
    *bitmap_idx = *m++;
    *block_size = *m++;
    bzero((void*) mem, (*block_size)*sizeof(char));
}