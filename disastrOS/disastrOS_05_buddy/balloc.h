#pragma once
#include "bit_map.h"
#include "buddy.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

// Params
#define MEM_SIZE 160 // (1024 * 1024) [DEBUG]
#define LEVELS 4 // 12 [DEBUG]
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
