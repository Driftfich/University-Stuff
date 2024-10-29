#include <stdio.h>
#include "FakultLib.h"

long fakult(int i)
{
    long f=1;
    int j;
    while (i>1) f*=i--;
    return f;
}