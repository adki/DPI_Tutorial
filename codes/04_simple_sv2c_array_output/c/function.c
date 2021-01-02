#include "svdpi.h"

int myFunc(svOpenArrayHandle v)
{
    int i;
    int low = svLow(v, 1);
    int high = svHigh(v, 1);
    for(i=low; i<=high; i++) {
        *((int*)svGetArrElemPtr1(v, i)) = 100+i;
    }
    return i;
}
