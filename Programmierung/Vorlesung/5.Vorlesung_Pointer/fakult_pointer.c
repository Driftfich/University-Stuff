#include <stdio.h>
#include <stdlib.h>

long fakult (int *num);

int main()
{
    int i;
    int j;
    long fakult_res;

    for (i=0; i<10; i++)
    {
        j=i;
        fakult_res = fakult(&j);
        printf("Fakultaet von %d: %d\n", i, fakult_res);
    }
}

long fakult(int *num)
{
    long res=1;
    while(*num > 1) res*=(*num)--;
    return res;
}