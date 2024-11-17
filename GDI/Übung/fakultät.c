#include <stdio.h>
#include <stdlib.h>

int f(int n)
{
    if (n==0)
        return 1;
    else
        return n*f(n-1);
}

int main()
{
    int i;
    for (i=0; i<=3; i++)
        printf("f(%d)=%d\n", i, f(i));
}