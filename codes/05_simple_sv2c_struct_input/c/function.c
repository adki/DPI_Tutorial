#include <stdio.h>
#include "svdpi.h"

typedef struct pkt_t {
    char   A;
    int    B;
    float  C;
    double D;
} pkt_t;

int myFunc(pkt_t *v)
{
    printf("%s() A=%c B=%d C=%f D=%f\n", __func__, v->A, v->B, v->C, v->D);
    return 0;
}
