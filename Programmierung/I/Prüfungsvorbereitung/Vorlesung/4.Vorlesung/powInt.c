#include <stdio.h>
#include <stdlib.h>

char buf[128];
long powInt(int base, int exp)
{
    int i;
    long p=base;
    for (i=2; i<=exp; i++) p*=base;
    return p;
}

int main()
{

    int base, exp;
    long result;

    printf("base\t: "); fgets(buf, 128, stdin); base=atoi(buf);
    printf("exponent\t: "); fgets(buf, 128, stdin); exp=atoi(buf);

    result=powInt(base, exp);
    printf("%d ^ %d = %ld", base, exp, result);
    return 0;
}