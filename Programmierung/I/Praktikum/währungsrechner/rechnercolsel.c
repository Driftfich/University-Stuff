#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

typedef struct
{
    char country_name [30+1];
    char country_code [3+1];
    char currency_code [3+1];
    double buy_price;
    double sell_price;
} currency;

// Deklaration der Funktion _comp_cols
void _comp_cols(currency *c[], int i, int j, int offset);

void print_currency(currency c)
{
    printf("%-30s\t|", c.country_name);
    printf("%-6s%-6s|", "", c.country_code);
    printf("%-7s%-8s|", "", c.currency_code);
    printf(" %12.5f |", c.buy_price);
    printf("%14.5f |\n", c.sell_price);
}

void sort(currency *c[], int length, int offset)
// sort by country name
{
    int i, j;
    
    for (i=0; i<length-1; i++)
    {
        
        for (j=i+1; j<length; j++)
        {
            _comp_cols(c, i,j, offset);
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

void _comp_cols(currency *c[], int i, int j, int offset)
// sort by country name
{
    if (strcmp((char *)((char *)c[i] + offset), (char *)((char *)c[j] + offset)) > 0)
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
    // currency values import
    currency vWrg[] = {
        #include "waehrung.dat"
    };
    // define vars
    int head_length, head_length_cp, i;
    int size = sizeof(vWrg)/sizeof(currency);
    // creating pointer array on the currency array for sorting purposes
    currency *vWrg_ptr[size];
    for (i = 0; i < size; i++) {
        vWrg_ptr[i] = &vWrg[i];
        // set sell price
        _set_sell_price(vWrg_ptr[i]);
    }
    // sort by the given attribute in offset
    sort(vWrg_ptr, size, offsetof(currency, currency_code));
    // table header
    head_length = printf("%-14sLand%-14s| Landescode | Waehrungscode | Ankaufspreis | Verkaufspreis |\n", "", "") - 1;
    head_length_cp = head_length;
    while(head_length_cp--) printf("-");
    printf("\n");
    // print the sorted array
    for (i=0; i<size; i++)
    {
        // print the currency
        print_currency(*vWrg_ptr[i]);
    }
    // table footer
    head_length_cp = head_length;
    while(head_length--) printf("-");
    printf("\n");
}

