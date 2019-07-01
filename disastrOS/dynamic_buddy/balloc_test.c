#include <stdio.h>
#include "balloc.h"

#define size_a 100000
#define size_b 2000000
#define size_c 17500
#define size_d 350050

int main(int argc, char** argv) {

    int * a = balloc(size_a * sizeof(int));
    for(int i = 0; i < size_a; i++)
        a[i] = i+1;
    
    float * b = balloc(size_b * sizeof(float));
    for(int i = 0; i < size_b; i++)
        b[i] = (0.25 * i);

    char * c = balloc(size_c * sizeof(c));
    for(int i = 0; i < size_c; i++)
        c[i] = i+5;
    
    printf("Print floats: ");
    for(int i = 0; i < size_b; i++)
        printf("%f ", b[i]);
    printf("\n");

    bfree(b);

    printf("Print chars: ");
    for(int i = 0; i < size_c; i++)
        printf("%d ", c[i]);
    printf("\n");

    double * d = balloc(size_d * sizeof(double));
    for(int i = 0; i < size_d; i++)
        d[i] = (1.33 * i);

    bfree(c);
    bfree(a);

    printf("Print doubles: ");
    for(int i = 0; i < size_d; i++)
        printf("%f ", d[i]);
    printf("\n");

    bfree(d);

    return 0;
}