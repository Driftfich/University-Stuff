#include <stdio.h>
#include <stdlib.h>

typedef struct tCurrency {
    char country[30];
    char ccode[3];
    char cur[3];
    double buy;
    double sell;
} curr;

curr vWrg[] = {
    #include "waehrung.dat"
    };

