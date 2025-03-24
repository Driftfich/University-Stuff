#include <stdio.h>

int main ()
{
    int i=-99;

    printf("Wert der Variable i als integer: %d\n", i);
    printf("Wert der Variable i als Hexadezimal: %04x \n", i);

    // test with following string interpretations: %i, %x, %X, %6d, %06d, %4x, %04x, %08x
    printf("%i, %x, %X, %6d, %06d, %4x, %04x, %08x", i,i,i,i,i,i,i,i);

    
    return 0;
}