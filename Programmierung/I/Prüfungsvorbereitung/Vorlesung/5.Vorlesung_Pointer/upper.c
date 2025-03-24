#include <stdio.h>
#include <stdlib.h>

void toUpper(char *pchar)
{
    *pchar = *pchar & (~0x20);
}

int main()
{
    char c = 'A';
    toUpper(&c);
    printf("%c\n", c);
    return 0;
}