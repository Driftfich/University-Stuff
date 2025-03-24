#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mat.h"

tArt *read_art(FILE *file){
    tArt *art=malloc(sizeof(tArt));
    if (!art) return NULL;

    int ret = fread(art, sizeof(tArt), 1, file);
    if (ret != 1) {
        free(art);
        return NULL;
    }

    return art;
}

void disblay_art(tArt *art) {
    printf("%-25s\t%-12s\t%ld\n", art->vBez, art->vNr, art->Lbst);
    return;
}

typedef int (*cmp)(const void *, const void *);

int cmp_bez(const void *c1, const void *c2) {
    tArt *a1=*((tArt **) c1);
    tArt *a2=*((tArt **) c2);

    if (!a1 || !a2) return 0;

    return strcmp(a1->vBez, a2->vBez);
}

int cmp_nr(const void *c1, const void *c2) {
    tArt *a1=*((tArt **) c1);
    tArt *a2=*((tArt **) c2);

    if (!a1 || !a2) return 0;

    return strcmp(a1->vNr, a2->vNr);
}

int main(int argc, char *argv[]) {
    FILE *file = fopen("mat.dat.intel64", "r");
    fseek(file, 0, SEEK_END);
    long len=ftell(file);
    fseek(file, 0, SEEK_SET);

    tArt *pArts[len];
    tArt *tmp;
    int count=0;
    for (int i=0; i<len; i++)  {
        tmp = read_art(file);
        if (!tmp) continue;
        pArts[i] = tmp;
        count++;
    }

    cmp fns[2] = {&cmp_bez, &cmp_nr};
    int fn_sel=0;
    if (argc >= 2) {
        if (strstr(argv[1], "nr") != NULL) {
            fn_sel=1;
        }}

    qsort(pArts, count, sizeof(tArt *), fns[fn_sel]);

    for (int i=0; i<count; i++) {
        disblay_art(pArts[i]);
    }

    return 0;
}

