#include "balloc.h"
#include <stdio.h>

// print bitmap status
static void bitmap_status(BitMap * bm, char * msg) {
    fprintf(stderr, "%s: ", msg);
    for(int i = 0; i < bm->num_bits; i++)
        fprintf(stderr, "%d ", BitMap_getBit(bm, i));
    fprintf(stderr, "\n");
}
// print memory status
static void mem_status(char * mem, char * msg) {
    fprintf(stderr, "%s: ", msg);
    for(int i = 0; i < MEM_SIZE; i++)
        fprintf(stderr, "%d ", mem[i]);
    fprintf(stderr, "\n");
}

int main(int argc, char** argv) {
    
    // Test balloc
    size_t size_a = 12;
    char * a = balloc(size_a * sizeof(char));
    for(int i = 0; i < size_a; i++)
        a[i] = (char) (i+1);

    // Test balloc
    size_t size_b = 72;
    char * b = balloc(size_b * sizeof(char));
    for(int i = 0; i < size_b; i++)
        b[i] = (char) (size_b - i);

    // Print bitmap status
    bitmap_status(&bitmap, "bitmap before resize");

    // Test balloc
    size_t size_c = 72;
    char * c = balloc(size_c * sizeof(char));
    for(int i = 0; i < size_c; i++)
        c[i] = (char) (i+1);

    // Print bitmap status
    bitmap_status(&bitmap, "bitmap after resize");
    // Print memory status
    mem_status(memory, "mem");

    return 0;
}