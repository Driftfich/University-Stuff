#include <stdio.h>
#include "FakultLib.h"

// Funktionsdefinition Einstiegsfunktion main
// Funktionskopf
void main()
// Funktionskörper
{
    int i;
    long f;
    static int c=5;
    c=10;

    for(i=0; i<10; i+=5)
    {
        f=fakult(i);
        printf("Fakultaet von %d: %ld\n", i, f);
    }

    return;
}