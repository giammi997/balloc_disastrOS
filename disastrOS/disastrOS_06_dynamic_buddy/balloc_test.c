#include "balloc.h"
#include <stdio.h>

// print bitmap status
static void bitmap_status(BitMap * bm, char * msg) {
    fprintf(stderr, "%s: ", msg);
    for(int i = 0; i < bm->num_bits; i++)
        fprintf(stderr, "%d ", BitMap_getBit(bm, i));
    fprintf(stderr, "\n");
}

int main(int argc, char** argv) {
    // Init default Buddy
    Buddy_init();
    // Test balloc
    char * a = balloc(12 * sizeof(char));
    //bitmap_status(&bitmap, "first alloc");
    char * b = balloc(72 * sizeof(char));
    //bitmap_status(&bitmap, "second alloc");
    // Test memory copy
    a[0] = 9;
    Buddy_resize(MEM_SIZE + 1);
    //  ^
    //  |
    // BUG
    printf("a:%d memory:%d\n", a, memory+8);
    printf("a->%d memory->%d\n", *a, memory[8]);
    return 0;
}