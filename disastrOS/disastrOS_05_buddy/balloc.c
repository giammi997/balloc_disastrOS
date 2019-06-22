#include "balloc.h"
#ifndef DEBUG
#define DEBUG
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

// Get buddy idx
static inline int get_buddy_idx(int idx) {
    return (idx & 0x01) ? idx - 1 : idx + 1;
}

// MALLOC
void * balloc(size_t bytes) {
    // Assuming that's enough memory at the beginning
    assert(bytes + 2*sizeof(int) <= MEM_SIZE);

    // Go to level found and check whether
    // parent blocks are already split

    // Get proper level for request of 'bytes'
    int level = get_level(bytes + 2*sizeof(int));
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
    if(idx == -1) return NULL;

    #ifdef DEBUG
    fprintf(stderr, "[DEBUG BALLOC] index: %d\n", idx);
    fprintf(stderr, "[DEBUG BALLOC] offset: %d\n", offset);
    #endif
    
    // Check whether PARENT IS ALREADY SPLIT
    if(BitMap_getBit(&bitmap ,get_parent_idx(idx))) {
        // Return block found IF SO and
        // Find block size and start of it (bytes)
        int block_size = MEM_SIZE / (1 << level);
        int start = block_size * offset;

        #ifdef DEBUG
        fprintf(stderr, "[DEBUG BALLOC] block_size: %d\n", block_size);
        fprintf(stderr, "[DEBUG BALLOC] mem start: %d\n", start);
        #endif

        // Set 'idx'-th bit 
        BitMap_setBit(&bitmap, idx, 1);
        // Create preamble and return proper ptr
        return Block_init(&memory[start], block_size, MIN_SIZE, idx);
    }
    // Else split parent blocks
    // ...

    return 0;
}


// FREE
void bfree(void * ptr) {
    assert(bitmap.num_bits);
    
    // ...
}
