#pragma once
#include <assert.h>
#include <strings.h>

// Block init
// Write preamble (bitmap_index, min_buddy_used -> size: 8 bytes)
// Return start of memory allocated
void * Block_init(char * mem, int mem_size, int min_buddy_size, int bitmap_index);

// Block clean
// Return and clean preamble
void Block_clean(char * mem, int * min_buddy_used, int * bitmap_index);