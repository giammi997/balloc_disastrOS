// *** GIANMARCO ***

#include "bit_map.h"
#include "block.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "disastrOS.h"
#include "disastrOS_syscalls.h"

// Get level corresponding to request 'req'
static inline int get_level(size_t req) {
    int level = 0, curr_size = MEM_SIZE;
    while((curr_size >> 1) >= req) {
        level++;
        curr_size = curr_size >> 1;
    }
    return level;
}
// Get parent idx from child idx
static inline int get_parent_idx(int child_idx) {
    return child_idx >> 1;
}
// Get first child idx from parent (second = first + 1)
static inline int get_child_idx(int parent_idx) {
    return parent_idx << 1;
}
// Get buddy idx
static inline int get_buddy_idx(int idx) {
    return (idx & 0x01) ? idx - 1 : idx + 1;
}
// Set 'status' on root and every child (subtree) within 'bitmap'
static inline void bitmap_set_subtree(BitMap * bitmap, int root_idx, int status) {
    if(root_idx >= bitmap->num_bits)
        return;
    BitMap_setBit(bitmap, root_idx, status);
    int first_child_idx = get_child_idx(root_idx);
    bitmap_set_subtree(bitmap, first_child_idx, status);
    bitmap_set_subtree(bitmap, first_child_idx + 1, status);
}
// Get level from index
static inline int get_level_of(int idx) {
    assert(idx > 0);
    return (int) floor(log2(idx));
}
// Build new bitmap from the old one
static inline void build_bitmap(BitMap * new, BitMap * old) {
    // Set the new root bit
    int k = 1;
    BitMap_setBit(new, k++, 1);

    // Loop for each old level
    int level_iter = 0;
    for(int i = 1; i < old->num_bits; i++) {
        // New (different) part
        if(get_level_of(i) > level_iter) {
            // Add 2^level new entries as zeros
            for(int j = 0; j < (1 << level_iter); j++)
                BitMap_setBit(new, k++, 0);
            level_iter++;
        }
        // Old (common) part
        // Simply copy old bits into the new one
        BitMap_setBit(new, k++, BitMap_getBit(old, i));
    }
}

// Buddy initialization
void Buddy_init() {
    assert(!bitmap.num_bits);

    // Set default parameters
    MEM_SIZE = DEFAULT_MEM_SIZE;
    LEVELS = DEFAULT_LEVELS;
    MIN_SIZE = DEFAULT_MIN_SIZE;

    // Find bitmap buffer size
    // Allocate it in heap
    size_t buffer_size = 1 << LEVELS;
    int fd = open(ZERO_GENERATOR, O_RDWR);
    bitmap_buffer = (uint8_t*) mmap(0, buffer_size, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);

    // Initialize bitmap
    BitMap_init(&bitmap, buffer_size, bitmap_buffer);
    
    // Reserve a contiguous region of 'MAX_BYTES_ALLOCATABLE' for future allocations
    memory = (char*) mmap(0, MAX_BYTES_ALLOCATABLE, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);
    close(fd);
}

// Buddy resize
void Buddy_resize(size_t new_min_size) {
    assert(bitmap.num_bits);
    assert(new_min_size > MEM_SIZE);

    // Get old values
    uint8_t * old_buffer = bitmap_buffer;
    int old_buffer_size = bitmap.size;
    int old_num_bits = bitmap.num_bits;

    // Find sufficient times to double
    int n_to_double = 1;
    while(new_min_size > (MEM_SIZE << n_to_double++));
    MEM_SIZE = MEM_SIZE << --n_to_double;
    LEVELS = (uint8_t) log2(MEM_SIZE / MIN_SIZE) + 1;

    // Allocate new bitmap buffer
    int buffer_size = 1 << LEVELS;
    int fd = open(ZERO_GENERATOR, O_RDWR);
    bitmap_buffer = mmap(0, buffer_size, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);

    // Build new bitmap buffer by starting from the old one
    BitMap_init(&bitmap, buffer_size, bitmap_buffer);
    BitMap old;
    BitMap_init(&old, old_num_bits, old_buffer);
    build_bitmap(&bitmap, &old);

    // Unmap old bitmap buffer
    int ret = munmap((void*) old_buffer, old_buffer_size);
    assert(!ret);
    close(fd);
}


// MEMORY ALLOCATION
void internal_balloc() {

    // RETRIEVE ARGUMENT FROM PCB OF RUNNING PROCESS
    size_t bytes = running->syscall_args[0];

    // Initialize Buddy if necessary
    if(!bitmap.num_bits)
        Buddy_init();

    // Go to level found and check whether parent blocks are already split
    // Get proper level for request of 'bytes'
    int level = get_level(bytes + 2*sizeof(int) /* preamble */);
    // Get bitmap indexes to check
    int idx_to_check = 1 << level;

    // Find first free block within 'level'
    int idx = -1;
    // Find offset from first block within 'level'
    int offset = 0;
    for(int i = idx_to_check; idx_to_check < (i << 1); idx_to_check++, offset++) {
        if(!BitMap_getBit(&bitmap, idx_to_check)) {
            idx = idx_to_check;
            break;
        }
    }
    // Not enough memory available
    if(idx == -1) {
        assert((MEM_SIZE + bytes) < MAX_BYTES_ALLOCATABLE);
        Buddy_resize(MEM_SIZE + bytes);
        internal_balloc();
    }
    
    // Check whether PARENT IS ALREADY SPLIT
    if(!BitMap_getBit(&bitmap ,get_parent_idx(idx))) {
        // Simply return block found IF SO, ELSE SPLIT PARENT blocks
        int iter_idx = get_parent_idx(idx);
        while(iter_idx > 0) {
            // Mark parent block as used
            BitMap_setBit(&bitmap, iter_idx, 1);
            iter_idx = get_parent_idx(iter_idx);
        }
    }
    // Find block size (bytes) and its start pointer
    int block_size = MEM_SIZE / (1 << level);
    int start = block_size * offset;

    // Mark as used 'idx' and every child (subtree)
    bitmap_set_subtree(&bitmap, idx, 1);
    // Add preamble and return proper memory pointer

    // PUT RESULT IN PCB ARGS VECTOR TO AVOID PTR SIZE PROBLEM
    running->syscall_args[1] = (long int) Block_init(&memory[start], block_size, idx);
}

