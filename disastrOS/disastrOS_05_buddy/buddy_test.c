#include "buddy.h"
#include <stdio.h>

#define N 64
char mem[N];

int main(int argc, char** argv) {

    // Test Block_init without bitmap
    void * start = Block_init(mem, N, N/4, 3 /* dummy */);
    printf("Initial:\n");
    int * p = (int*) mem;
    for(int i=0; i < N / sizeof(int); i++)
        printf("%d-th: %d\n",i,*p++);

    // Test Buddy_clean
    int min_block_used, bitmax_index;
    Block_clean(mem, &min_block_used, &bitmax_index);
    printf("\nAfter cleaning:\n");
    p = (int*) mem;
    for(int i=0; i < N / sizeof(int); i++) {
        printf("%d-th: %d\n",i,*p++);
    }

    // Must return min_block_used = 4, bitmax_index = 3
    printf("\nmin_block_used: %d, bitmax_index: %d\n",min_block_used,bitmax_index);

    return 0;
}