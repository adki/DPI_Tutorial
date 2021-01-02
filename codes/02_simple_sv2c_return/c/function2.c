#include "svdpi.h"

int myCFunc2(int A, int *B)
{
    *B = A/2;
    return A*2;
}
