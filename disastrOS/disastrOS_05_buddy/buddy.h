#pragma once

// Buddy init
// Write preamble (bitmap_index, min_buddy_used -> size: 8 bytes)
// Return start of memory allocated
void * Buddy_init(char * mem, int mem_size, int min_buddy_size, int bitmap_index);

// Buddy clean
// Return and clean preamble
void Buddy_clean(char * mem, int * min_buddy_used, int * bitmap_index);