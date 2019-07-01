// *** GIANMARCO ***

#pragma once
#include <assert.h>
#include <strings.h>

// Block initialization
// Add preamble (bitmap_index, block_size (size: 8 bytes))
// Return start of memory allocated
void * Block_init(char * mem, int block_size, int bitmap_index);

// Block clean
// Clean block and preamble
void Block_clean(char * mem, int * block_size, int * bitmap_index);