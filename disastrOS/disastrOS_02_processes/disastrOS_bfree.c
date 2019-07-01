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


// MEMORY FREE
void internal_bfree() {

    // RETRIEVE ARGUMENT FROM PCB OF RUNNING PROCESS
    void * ptr = (void*) running->syscall_args[0];

    // Make sure bitmap is initialized
    assert(bitmap.num_bits);

    // Get bitmap index and block size, then clean it
    int block_size, idx;
    Block_clean(ptr - 2*sizeof(int), &block_size, &idx);
    // Mark as unused 'idx' and every child (subtree)
    bitmap_set_subtree(&bitmap, idx, 0);

    // Check that EVEN BUDDY is UNUSED
    if(!BitMap_getBit(&bitmap, get_buddy_idx(idx))) {
        // IF SO coalesce buddies iterativelly as long as possible
        // by marking parents as used too
        int parent_idx = get_parent_idx(idx);
        while(parent_idx > 0) {
            BitMap_setBit(&bitmap, parent_idx, 0);
            // If buddy is used STOP
            if(BitMap_getBit(&bitmap, get_buddy_idx(parent_idx)))
                break;
            parent_idx = get_parent_idx(parent_idx);
        }
    }
}
