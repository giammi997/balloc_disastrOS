#include "buddy.h"
#include <stdio.h>

#define N 64
char mem[N];

int main(int argc, char** argv) {

    // Test Buddy_init without bitmap
    void * start = Buddy_init(mem, N, N/4, 3 /* dummy */);
    printf("Initial:\n");
    int * p = (int*) mem;
    for(int i=0; i < N / sizeof(int); i++) {
        printf("%d-th: %d\n",i,*p++);
    }

    // Test Buddy_clean
    int min_buddy_used, bitmax_index;
    Buddy_clean(mem, &min_buddy_used, &bitmax_index);
    printf("\nAfter cleaning:\n");
    p = (int*) mem;
    for(int i=0; i < N / sizeof(int); i++) {
        printf("%d-th: %d\n",i,*p++);
    }

    // Must return min_buddy_used = 4, bitmax_index = 3
    printf("\nmin_buddy_used: %d, bitmax_index: %d\n",min_buddy_used,bitmax_index);

    return 0;
}