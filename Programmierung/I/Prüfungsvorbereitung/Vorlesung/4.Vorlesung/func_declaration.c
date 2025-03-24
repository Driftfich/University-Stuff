#include <stdio.h>
#include <stdlib.h>

long fakult(int x);

// Funktionsdefinition Einstiegsfunktion main
// Funktionskopf
void main()
// Funktionskörper
{
    int i;
    long f;

    for(i=0; i<10; i+=5)
    {
        f=fakult(i);
        printf("Fakultaet von %d: %ld\n", i, f);
    }

    return;
}

// Funktionsdefinition fakult
// Funktionskopf
long fakult (int x)
// Funktionskörper
{
    long f=1;
    int j;
    for (j=1; j<=x; j++) (f*=j);
    return f;
}