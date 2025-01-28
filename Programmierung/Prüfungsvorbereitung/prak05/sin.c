#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    char buf[128];
    double x, y, summand;
    fgets(buf, sizeof(buf), stdin);
    x=atof(buf);
    y=x;
    summand = x;
    int count=1;
    do {
        summand = -1 * summand * x * x / (2*count *(2*count + 1));
        y+=summand;
        count ++;
    } while (summand>0.00009);
    printf("Sinus(%f)=%f", x,y);
    printf("Sinus from math.h: %f", sin(x));
}