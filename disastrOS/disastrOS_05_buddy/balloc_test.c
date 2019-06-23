#include "balloc.h"
#include <stdio.h>

#define SIZE1 2048
#define SIZE2 2048

int main(int argc, char** argv) {

    int * a = balloc(SIZE1 * sizeof(int));
    for(int i = 0; i < SIZE1; i++)
        a[i] = i+1;
    int * b = balloc(SIZE2 * sizeof(int));
    for(int i = 0; i < SIZE2; i++)
        b[i] = SIZE2 - i;
    for(int i = 0; i < SIZE1; i++)
        printf("%d ",a[i]);
    printf("\n\n\n");
    for(int i = 0; i < SIZE2; i++)
        printf("%d ",b[i]);
    printf("\n");
    bfree(a);
    bfree(b);

    return 0;
}