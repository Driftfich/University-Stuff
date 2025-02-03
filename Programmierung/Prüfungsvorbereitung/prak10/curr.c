#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tCurrency {
    char country[30+1];
    char ccode[3+1];
    char cur[3+1];
    double buy;
    double sell;
} curr;

void disblay_curr(curr *cr) {
    if (!cr) return;
    printf("%-30s, %-6s, %-8s, %12.5lf, %12.5lf\n", cr->country, cr->ccode, cr->cur, cr->buy, cr->sell);
    return;
}

int country_cmp(curr *c1, curr*c2) {
    return strcmp((**((curr **) c1)).country, (**((curr **) c2)).country);
}

int ccode_cmp(curr *c1, curr*c2) {
    return strcmp((**((curr **) c1)).ccode, (**((curr **) c2)).ccode);
}

int cur_cmp(const void *c1, const void *c2) {
    return strcmp((**((curr **) c1)).cur, (**((curr **) c2)).cur);
}

void sort(curr *pWrg[], int size, int (*comp)(curr *c1, curr*c2)) {
    for (int i=0; i<size; i++) {
        for (int j=i+1; j<size; j++) {
            // printf("%s | %s, %d\n", ((curr)(*pWrg[i])).country, ((curr)(*pWrg[j])).country, strcmp(((curr)(*pWrg[i])).country, ((curr)(*pWrg[j])).country));
            if (comp(pWrg[i], pWrg[j]) >= 0) {
                curr *tmp=pWrg[i];
                pWrg[i]=pWrg[j];
                pWrg[j]=tmp;
            }
        }
    }
    return;
}

curr calc_sell_price(curr Wrg) {
    Wrg.sell = 0.8 * Wrg.buy;
    return Wrg;
}

int main() {
    curr vWrg[] = {
        #include "waehrung.dat"
    };

    int size = sizeof(vWrg) / sizeof(curr);
    curr *pWrg[size];
    for (int i=0; i<size; i++) {
        vWrg[i]=calc_sell_price(vWrg[i]);
        pWrg[i] = &vWrg[i];
    }

    // sort(pWrg, size, &cur_cmp);
    qsort(pWrg, size, sizeof(pWrg[0]), &cur_cmp);

    for (int i=0; i<size; i++) {
        disblay_curr(pWrg[i]);
    }

    return 0;
}
