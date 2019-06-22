#include "balloc.h"
#include <stdio.h>

int main(int argc, char** argv) {

    // Emulate a parent already split situation
    BitMap_init(&bitmap, 1 << LEVELS, bitmap_buffer);
    BitMap_setBit(&bitmap, 1, 1);
    BitMap_setBit(&bitmap, 2, 1);
    BitMap_setBit(&bitmap, 4, 1);
    printf("Initial:\n");
    for(int i = 0; i < bitmap.num_bits; i++)
        printf("%d ",BitMap_getBit(&bitmap, i));
    printf("\n");

    // Test balloc
    int * ptr = balloc(20);

    // Test bit setting
    printf("Thereafter:\n");
    for(int i = 0; i < bitmap.num_bits; i++)
        printf("%d ",BitMap_getBit(&bitmap, i));
    printf("\n");

    // Try to write in memory received
    ptr[0] = 5;
    ptr[1] = 6;
    ptr[2] = 3;
    ptr[3] = 0;
    ptr[4] = 2;
    printf("\nInitial:\n");
    for(int i = 0; i < 5; i++)
        printf("%d ",ptr[i]);
    printf("\n");

    // Check that memory location is right
    int* mem = (int*) memory;
    for(int i = 0; i < MEM_SIZE/sizeof(int); i++) {
        if(i==10) printf("must be the start --> ");
        printf("%d ",mem[i]);
    }
    printf("\n");

    // Check that values written are the same
    int* real = &mem[10];
    fprintf(stderr, "[DEBUG MAIN] real ptr: %ld\n", (long)real);
    for(int i = 0; i < 7; i++)
        fprintf(stderr, "[DEBUG MAIN] %d-th value: %d\n",i,real[i]);
    
    return 0;
}