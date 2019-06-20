#include "bit_map.h"
#include <stdio.h>

#define N 16
uint8_t buffer[N] = {0};
BitMap bitmap = {0};

int main(int argc, char** argv) {

    // Init
    BitMap* bm = &bitmap;
    BitMap_init(bm,N,buffer);

    // Test BitMap_getBit
    printf("Initial:\n");
    for(int i=0; i<N; i++)
        printf("%d-th bit: %d\n",i, BitMap_getBit(bm,i));

    // Test BitMap_setBit
    BitMap_setBit(bm,10,1);
    BitMap_setBit(bm,0,1);
    BitMap_setBit(bm,11,1);
    BitMap_setBit(bm,15,1);
    BitMap_setBit(bm,11,0);

    // Test BitMap_getBit
    printf("\nThereafter:\n");
    for(int i=0; i<N; i++)
        printf("%d-th bit: %d\n",i, BitMap_getBit(bm,i));

    return 0;
}