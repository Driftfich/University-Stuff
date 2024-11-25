#include <stdio.h>
#include <stdlib.h>

char vBuf[128];

int main()
{
    signed char sc;
    unsigned char uc;
    signed short ss;
    unsigned short us;
    unsigned int ui;
    signed int si;
    unsigned long ul;
    signed long sl;
    float f;
    double d;

    // fgets(vBuf, 128, stdin); sc=atoi(vBuf);
    // fgets(vBuf, 128, stdin); uc=atoi(vBuf);
    // fgets(vBuf, 128, stdin); ss=atoi(vBuf);
    // fgets(vBuf, 128, stdin); us=atoi(vBuf);
    // fgets(vBuf, 128, stdin); ui=atoi(vBuf);
    // fgets(vBuf, 128, stdin); si=atoi(vBuf);
    // fgets(vBuf, 128, stdin); ul=atol(vBuf);
    // fgets(vBuf, 128, stdin); sl=atol(vBuf);
    // fgets(vBuf, 128, stdin); f=atof(vBuf);
    // fgets(vBuf, 128, stdin); d=atof(vBuf);

    scanf("%d", &sc);
    scanf("%u", &uc);
    scanf("%hd", &ss);
    scanf("%hu", &us);
    scanf("%u", &ui);
    scanf("%d", &si);
    scanf("%ld", &sl);
    scanf("%lu", &ul);
    scanf("%f", &f);
    scanf("%d", &d);

    printf("signed char %p %d %X\n", &sc, sc,sc);
    printf("unsigned char %p %d %X\n", &uc , uc,uc);
    printf("signed short %p %d %X\n", &ss , ss,ss);
    printf("unsigned short %p %d %X\n", &us , us,us);
    printf("signed int %p %d %X\n", &ui , ui,ui);
    printf("unsigned int %p %d %X\n", &si , si,si);
    printf("signed long %p %d %X\n", &ul , ul,ul);
    printf("unsigned long %p %d %X\n", &sl , sl,sl);
    printf("float %p %d %X\n", &f , f,f);
    printf("double %p %d %X\n", &d , d,d);
}