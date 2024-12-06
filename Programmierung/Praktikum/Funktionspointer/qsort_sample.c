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

typedef int (*comp)(currency *c1, currency *c2);

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

int _comp_cols(const void *c1, const void *c2)
// sort by country name
{
    printf("c1: %s, c2: %s\n", (char*)((currency*)c1)->country_name, (char*)((currency*)c2)->country_name);
    return strcmp((char*)(((currency*)c1)->currency_code), (char*)(((currency*)c2)->currency_code));
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
    // sort(vWrg_ptr, size, comp_country_name);
    // make a partial application of _comp_cols with offset = 0 using function pointer

    // comp *comp_cols = _comp_cols;
    qsort(vWrg_ptr, size, sizeof(currency *), _comp_cols);
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

