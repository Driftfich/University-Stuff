#include <stdio.h>
#include <stdlib.h>

int main()
{
    int ar[3] = {1,2,3};
    int *val = ar;
    
    printf("array[1]: %d\n", ar[1]);
    printf("array[1]: %d\n", *(ar+1));
    printf("array[1]: %d\n", *(val+1));
    printf("array[1]: %d\n", val[1]);

    return 0;
}