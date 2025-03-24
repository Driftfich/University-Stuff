#include <stdio.h>
#include <stdlib.h>

int main()
{
    int i=12;
    int j=15;
    int *p = &i;
    printf("i: %d, &i %p, p -> %d\n", i, p, *p);
    p = &j;
    printf("i: %d, &i %p, p -> %d\n", j, p, *p);
    return 0;
}