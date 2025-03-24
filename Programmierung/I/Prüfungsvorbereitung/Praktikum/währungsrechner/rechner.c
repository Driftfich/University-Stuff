#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char country_name [30+1];
    char country_code [3+1];
    char currency_code [3+1];
    double buy_price;
    double sell_price;
} currency;

void print_currency(currency c)
{
    printf("%-30s\t|", c.country_name);
    printf("%-6s%-6s|", "", c.country_code);
    printf("%-7s%-8s|", "", c.currency_code);
    printf(" %12.5f |", c.buy_price);
    printf("%14.5f |\n", c.sell_price);
}

void sort(currency *c[], int length, void (*f)(currency *c[], int i, int j))
// sort by country name
{
    int i, j;
    for (i=0; i<length-1; i++)
    {
        for (j=i+1; j<length; j++)
        {
            (*f)(c, i,j);
        }
    }
}

void _switch_vars(currency *c[], int i, int j)
{
    currency *temp;
    temp = c[i];
    c[i] = c[j];
    c[j] = temp;
}

void comp_country_name(currency *c[], int i, int j)
// sort by country name
{
    if (strcmp(c[i]->country_name, c[j]->country_name) > 0)
    {
        _switch_vars(c, i, j);
    }
}

void comp_currency_code(currency *c[], int i, int j)
// sort by country name
{
    if (strcmp(c[i]->country_code, c[j]->country_code) > 0)
    {
        _switch_vars(c, i, j);
    }
}

void _set_sell_price(currency *c)
{
    c->sell_price = c->buy_price * 0.92;
}

int main()
{
    currency vWrg[] = {
        #include "waehrung.dat"
    };
    int head_length, head_length_cp, i;
    int size = sizeof(vWrg)/sizeof(currency);
    currency *vWrg_ptr[size];
    for (i = 0; i < size; i++) {
        vWrg_ptr[i] = &vWrg[i];
    }
    sort(vWrg_ptr, size, comp_country_name);
    head_length = printf("%-14sLand%-14s| Landescode | Waehrungscode | Ankaufspreis | Verkaufspreis |\n", "", "") - 1;
    head_length_cp = head_length;
    while(head_length_cp--) printf("-");
    printf("\n");
    for (i=0; i<size; i++)
    {
        _set_sell_price(vWrg_ptr[i]);
        print_currency(*vWrg_ptr[i]);
    }
    head_length_cp = head_length;
    while(head_length--) printf("-");
    printf("\n");
}

