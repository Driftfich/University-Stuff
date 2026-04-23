#include <stdio.h>
#include <stdlib.h>

void swap(int* first, int* second)
{
    *first^=*second; *second^=*first; *first^=*second;
}

int main()
{
    int i=12, j=99;
    printf("i: %d, j: %d\n", i, j);
    swap(&i,&j);
    printf("i: %d, j: %d\n", i, j);
    return 0;
}