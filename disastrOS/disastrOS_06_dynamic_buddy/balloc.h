#pragma once
#include "bit_map.h"
#include "block.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

// Default params
#define DEFAULT_MEM_SIZE 160 // [DEBUG] (1024*1024) // 1 MB
#define DEFAULT_LEVELS 4 // [DEBUG] 14
#define DEFAULT_MIN_SIZE (DEFAULT_MEM_SIZE >> (DEFAULT_LEVELS - 1))

// Params
uint64_t MEM_SIZE;
uint8_t LEVELS;
uint32_t MIN_SIZE;

// Buddy references
uint8_t * bitmap_buffer;
BitMap bitmap;
char * memory;

// Buddy initialization
// Set default params
void Buddy_init();

// Buddy resize
// Double old size until is sufficient for 'new_size'
void Buddy_resize(size_t new_size);

// MALLOC
// Buddy memory allocation
void * balloc(size_t bytes);

// FREE
// Buddy memory release
void bfree(void * ptr);
