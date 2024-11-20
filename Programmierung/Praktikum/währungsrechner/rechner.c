#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char country_name [30];
    char country_code [3];
    char currency_code [3];
    double buy_price;
    double sell_price;
} currency;

void print_currency(currency c)
{
    printf("Land: %-30s\t,", c.country_name);
    printf("Landescode: %-3s\t,", c.country_code);
    printf("Waehrungscode: %-3s\t,", c.currency_code);
    printf("Ankaufspreis: %.5f\t,", c.buy_price);
    printf("Verkaufspreis: %.5f\n", c.sell_price);
}

void sort_currency(currency *c[], int length)
// sort by country name
{
    int i, j;
    currency *temp;
    for (i=0; i<length-1; i++)
    {
        for (j=i+1; j<length; j++)
        {
            if (strcmp(c[i]->country_name, c[j]->country_name) > 0)
            {
                temp = c[i];
                c[i] = c[j];
                c[j] = temp;
            }
        }
    }
}

void set_sell_price(currency *c)
{
    c->sell_price = c->buy_price * 0.92;
}

int main()
{
    currency vWrg[] = {
        #include "waehrung.dat"
    };
    int i;
    int size = sizeof(vWrg)/sizeof(currency);
    currency *vWrg_ptr[size];
    for (i = 0; i < size; i++) {
        vWrg_ptr[i] = &vWrg[i];
    }
    sort_currency(vWrg_ptr, size);
    for (i=0; i<size; i++)
    {
        set_sell_price(vWrg_ptr[i]);
        print_currency(*vWrg_ptr[i]);
    }
}

