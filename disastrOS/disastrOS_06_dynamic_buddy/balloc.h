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
#define DEFAULT_MEM_SIZE (1024*1024) // 1 MB
#define DEFAULT_LEVELS 14
#define DEFAULT_MIN_SIZE (DEFAULT_MEM_SIZE >> (DEFAULT_LEVELS - 1))

// Params
uint64_t MEM_SIZE;
uint8_t LEVELS;
uint32_t MIN_SIZE;

// Buddy references
uint8_t * bitmap_buffer;
BitMap bitmap;
char * memory;

// Structures initialization
void Buddy_init();

// Structures resize
void Buddy_resize(size_t size);

// MALLOC
// Buddy memory allocation
void * balloc(size_t bytes);

// FREE
// Buddy memory release
void bfree(void * ptr);
