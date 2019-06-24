#pragma once
#include "bit_map.h"
#include "block.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

// Params
#define MEM_SIZE (1024 * 1024)
#define LEVELS 14
#define MIN_SIZE (MEM_SIZE >> (LEVELS - 1))

// Memory allocated in .bss
uint8_t bitmap_buffer[1 << LEVELS];
BitMap bitmap;
char memory[MEM_SIZE];

// MALLOC
// Buddy memory allocation
void * balloc(size_t bytes);

// FREE
// Buddy memory release
void bfree(void * ptr);
