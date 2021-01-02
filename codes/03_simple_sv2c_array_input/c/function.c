#include "svdpi.h"
#include <stdio.h>

int myFunc(const svOpenArrayHandle v)
{
    int i;
    int low = svLow(v, 1);
    int high = svHigh(v, 1);
    for(i=low; i<=high; i++) {
        printf("[%d]=%d ", i, *((int*)svGetArrElemPtr1(v, i)));
    }
    printf("\n");
    return i;
}
