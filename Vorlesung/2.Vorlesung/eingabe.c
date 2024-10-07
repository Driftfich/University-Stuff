#include <stdio.h>
#include <stdlib.h>

int main () {
    int dividend, divisor;
    double DividendDouble;
    char buf[128];

    printf("Eingabe Dividend: ");
    fgets(buf, 128, stdin); // Eingabe Text
    dividend = atoi(buf);
    DividendDouble = atof(buf);

    printf("Eingabe Divisor: ");
    fgets(buf, 128, stdin);
    divisor = atoi(buf);

    printf("Quotient (int) %d/%d: %d\nQuotient (double) %lf/%d: %lf\n",
        dividend, divisor, dividend/divisor,
        DividendDouble, divisor, DividendDouble/divisor);

    return 0;
}