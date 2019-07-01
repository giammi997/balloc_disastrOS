#pragma once
#include "bit_map.h"
#include "block.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

// #####
// Settings
// #####
#define MAX_BYTES 1U << 29 // 512 MB
#define ZERO_GENERATOR "/dev/zero"

#define DEFAULT_MEM_SIZE 1U << 20 // 1 MB
#define DEFAULT_LEVELS 14
#define DEFAULT_MIN_SIZE (DEFAULT_MEM_SIZE >> (DEFAULT_LEVELS - 1))
// #####

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
