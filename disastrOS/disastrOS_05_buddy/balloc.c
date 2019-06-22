#include "balloc.h"
#ifndef DEBUG
//#define DEBUG
#endif

// Get corresponding level from a request
static inline int get_level(size_t req) {
    int level = 0, curr_size = MEM_SIZE;
    while(curr_size >> 1 >= req) {
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

// Set 'status' on root and all child (subtree) within 'bitmap'
static inline void set_subtree(BitMap * bitmap, int root_idx, int status) {
    if(root_idx >= bitmap->num_bits)
        return;
    BitMap_setBit(bitmap, root_idx, status);
    int first_child_idx = get_child_idx(root_idx);
    set_subtree(bitmap, first_child_idx, status);
    set_subtree(bitmap, first_child_idx + 1, status);
}

// MALLOC
void * balloc(size_t bytes) {
    // Init bitmap if necessary
    if(!bitmap.num_bits)
        BitMap_init(&bitmap, 1 << LEVELS, bitmap_buffer);
    // Assuming that's enough memory at the beginning
    assert(bytes + 2*sizeof(int) <= MEM_SIZE);

    // Go to level found and check whether parent blocks are already split
    // Get proper level for request of 'bytes'
    int level = get_level(bytes + 2*sizeof(int));
    // Get bitmap indexes to check
    int idx_to_check = 1 << level;
    
    #ifdef DEBUG
    fprintf(stderr, "[DEBUG BALLOC] first idx to check: %d\n", idx_to_check);
    #endif

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
    if(idx == -1) return NULL;

    #ifdef DEBUG
    fprintf(stderr, "[DEBUG BALLOC] idx found: %d\n", idx);
    fprintf(stderr, "[DEBUG BALLOC] offset: %d\n", offset);
    #endif
    
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
    // TODO: fix this...

    // Find block size (bytes) and its start pointer
    int block_size = MEM_SIZE / (1 << level);
    int start = block_size * offset;

    #ifdef DEBUG
    fprintf(stderr, "[DEBUG BALLOC] block_size: %d\n", block_size);
    fprintf(stderr, "[DEBUG BALLOC] block start: %d\n", start);
    #endif

    // Mark as used idx and all child (subtree)
    set_subtree(&bitmap, idx, 1);
    // Add preamble and return proper memory pointer
    return Block_init(&memory[start], block_size, MIN_SIZE, idx);
}


// FREE
void bfree(void * ptr) {
    // Make sure bitmap is initialized
    assert(bitmap.num_bits);
    
    // ...
}
