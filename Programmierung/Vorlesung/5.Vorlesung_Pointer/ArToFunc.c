#include <stdio.h>
#include <stdlib.h>

void ArToConsole(int* ar, int ar_length)
{
    printf("%d\n", sizeof ar);
    int i;
    for (i=0; i<ar_length; i++)
    {
        printf("ar[%d]=%d\n", i, *(ar + i));
    }
}

int main()
{
    int ar[3] = {1,2,3};
    printf("%d\n", sizeof ar);
    ArToConsole(ar, sizeof ar / sizeof(int));
    return 0;
}