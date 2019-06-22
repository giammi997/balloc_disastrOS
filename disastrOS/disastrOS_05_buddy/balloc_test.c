#include "balloc.h"
#include <stdio.h>

// Print bitmap status
void Bitmap_status(BitMap * bitmap, char * msg) {
    printf("%s: ", msg);
    for(int i = 0; i < bitmap->num_bits; i++)
        printf("%d ", BitMap_getBit(bitmap, i));
    printf("\n");
}

int main(int argc, char** argv) {

    // Test balloc
    int * a = balloc(5*sizeof(int)); // 20 bytes -> level 2
    Bitmap_status(&bitmap, "First alloc (20 bytes)");
    char * b = balloc(32*sizeof(char)); // 32 bytes -> level 2
    Bitmap_status(&bitmap, "Second alloc (32 bytes)");
    short * c = balloc(5*sizeof(short)); // 10 bytes -> level 3
    Bitmap_status(&bitmap, "Third alloc (10 bytes)");
    long * d = balloc(19 * sizeof(long)); // Not enough memory
    Bitmap_status(&bitmap, "Fourth alloc (152 bytes)");

    // Test bfree
    bfree(a);
    Bitmap_status(&bitmap, "First release (first alloc: 20 bytes)");
    bfree(c);
    Bitmap_status(&bitmap, "Second release (third alloc: 10 bytes)");

    return 0;
}