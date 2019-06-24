#include "balloc.h"
#include <stdio.h>

int main() {

    printf("%d\n",bitmap.num_bits);
    balloc(3);
    printf("%d\n",bitmap.num_bits);
    return 0;
}