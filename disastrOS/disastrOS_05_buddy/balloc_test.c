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
    if(a) printf("OK\n");
    else printf("ERROR\n");
    char * b = balloc(32*sizeof(char));
    Bitmap_status(&bitmap, "Second alloc (32 bytes)");
    if(b) printf("OK\n");
    else printf("ERROR\n");
    short * c = balloc(5*sizeof(short));
    Bitmap_status(&bitmap, "Third alloc (10 bytes)");
    if(c) printf("OK\n");
    else printf("ERROR\n");
    long * d = balloc(19 * sizeof(long));
    Bitmap_status(&bitmap, "Fourth alloc (152 bytes)");
    if(!d) printf("OK\n");
    else printf("ERROR\n");

    return 0;
}