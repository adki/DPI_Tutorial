#include <stdio.h>
#include "dpiheader.h"

int myCFunc ( int A, int B ) {
    printf("Hello world from C! %d %d\n", A, B);
    int C = mySVFunc(A+10, B+20);
    return C;
}
