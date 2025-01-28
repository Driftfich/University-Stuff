#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main ()
{
    double sinus, x, summand;
    int count=1;
    char buf[128];

    fgets(buf, sizeof buf, stdin);
    x = atof(buf);
    printf("Input x: %f\n", x);
    summand=x;
    sinus=x;
    do {
        summand = -1 * (summand * (x * x) / (count*2 * (count*2 +1)));
        // if (count % 2 == 0)
        // {
        //     sinus += summand;
        // }
        // else
        // {
        //     sinus -= summand;
        // }
        sinus+=summand;
        count++;
    } while(summand > 0.00005);
    printf("sin(%lf)=%lf\n\n", x, sinus);
}