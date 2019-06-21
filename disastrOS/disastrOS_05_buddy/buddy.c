#include "buddy.h"
#include <assert.h>
#include <strings.h>

// Buddy init
void * Buddy_init(char * mem, int mem_size, int min_buddy_size, int bitmap_index) {
    assert(!(mem_size % min_buddy_size));
    // Fill Buddy up with proper preamble
    int * m = (int*) mem;
    *m++ = bitmap_index;
    *m++ = mem_size / min_buddy_size;
    return (void*) m;
}

// Buddy clean
void Buddy_clean(char * mem, int * min_buddy_used, int * bitmap_index) {
    // Return and clean preamble
    int * m = (int*) mem;
    *bitmap_index = *m++;
    *min_buddy_used = *m++;
    bzero((void*) mem, 2*sizeof(int));
}