#include "balloc.h"
#include <string.h>

#ifndef DEBUG
//#define DEBUG
#endif

// Get corresponding level from a request
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


// Allocate memory and set references
static inline void alloc_and_set(uint64_t mem_size, uint8_t levels) {
    // Find size 's' to allocate
    size_t s = (1 << levels) + mem_size;
    // Allocate 's' bytes in heap
    int zero_fd = open("/dev/zero", O_RDWR);
    char * mem_start = mmap(0, s, PROT_READ | PROT_WRITE, MAP_PRIVATE, zero_fd, 0);
    close(zero_fd);
    // Set Buddy references
    bitmap_buffer = (uint8_t*) mem_start;
    memory = mem_start + (1 << levels);
    // Init bitmap
    BitMap_init(&bitmap, (1 << levels), bitmap_buffer);
}

// Buddy initialization
void Buddy_init() {
    assert(!bitmap.num_bits);
    // Set default params
    MEM_SIZE = DEFAULT_MEM_SIZE;
    LEVELS = DEFAULT_LEVELS;
    MIN_SIZE = DEFAULT_MIN_SIZE;
    // Allocate and set
    alloc_and_set(MEM_SIZE, LEVELS);
}

// Buddy resize
void Buddy_resize(size_t new_size) {
    assert(bitmap.num_bits);
    assert(new_size > MEM_SIZE);
    // Get old values
    BitMap old_bm = {0};
    BitMap_init(&old_bm, bitmap.num_bits, bitmap_buffer);
    char * old_mem = memory;
    uint8_t old_levels = LEVELS;
    uint64_t old_mem_size = MEM_SIZE;
    void * old_start = (void*) bitmap_buffer;
    size_t old_size = (1 << LEVELS) + MEM_SIZE;
    // Find sufficient number to double
    int n_to_double = 1;
    while(new_size > (MEM_SIZE << n_to_double++));
    MEM_SIZE = MEM_SIZE << --n_to_double;
    LEVELS = (uint8_t) log2(MEM_SIZE / MIN_SIZE) + 1;
    // Allocate and set
    alloc_and_set(MEM_SIZE, LEVELS);
    // Build new bitmap
    memcpy((void*) bitmap_buffer, (const void*) old_bm.buffer, (size_t) old_bm.size);
    for(int i = (1 << (old_levels - 1)); i < old_bm.num_bits; i++)
        if(BitMap_getBit(&bitmap, i))
            bitmap_set_subtree(&bitmap, i, 1);
    // Copy old memory into the new one
    memcpy((void*) memory, (const void*) old_mem, old_mem_size);
    // TODO: fix memory move...
    // TODO: implement old mem unmap...
}


// MALLOC
void * balloc(size_t bytes) {

    // Init Buddy if necessary
    if(!bitmap.num_bits)
        Buddy_init();

    // Go to level found and check whether parent blocks are already split
    // Get proper level for request of 'bytes'
    int level = get_level(bytes + 2*sizeof(int) /* preamble */);
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
    // TODO: call buddy resize...

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
    // Find block size (bytes) and its start pointer
    int block_size = MEM_SIZE / (1 << level);
    int start = block_size * offset;

    #ifdef DEBUG
    fprintf(stderr, "[DEBUG BALLOC] block_size: %d\n", block_size);
    fprintf(stderr, "[DEBUG BALLOC] block start: %d\n", start);
    #endif

    // Mark as used 'idx' and every child (subtree)
    bitmap_set_subtree(&bitmap, idx, 1);
    // Add preamble and return proper memory pointer
    return Block_init(&memory[start], block_size, idx);
}


// FREE
void bfree(void * ptr) {
    // Make sure bitmap is initialized
    assert(bitmap.num_bits);
    // Get bitmap index and block size then clean it
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
