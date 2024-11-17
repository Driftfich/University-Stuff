#include <stdio.h>
#include <stdlib.h>

int main()
{
    int i=12;
    int *p = &i;
    printf("i: %d, &i %p, p -> %d", i, p, *p);
    return 0;
}