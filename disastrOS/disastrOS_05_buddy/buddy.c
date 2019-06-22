#include "buddy.h"
#include <stdio.h>
#ifndef DEBUG
#define DEBUG
#endif

// Block init
void * Block_init(char * mem, int block_size, int min_block_size, int bitmap_index) {

    #ifdef DEBUG
    fprintf(stderr, "[DEBUG BUDDY] mem ptr: %ld\n", (long)mem);
    #endif

    assert(!(block_size % min_block_size));
    assert(bitmap_index > 0);
    // Add proper preamble
    int * m = (int*) mem;
    *m++ = bitmap_index;
    *m++ = block_size / min_block_size;
    return (void*) m;
}

// Block clean
void Block_clean(char * mem, int * min_block_used, int * bitmap_index) {
    // Return and clean preamble
    int * m = (int*) mem;
    // Side-effect on 'min_block_used' and 'bitmap_index'
    *bitmap_index = *m++;
    *min_block_used = *m++;
    bzero((void*) mem, 2*sizeof(int));
}