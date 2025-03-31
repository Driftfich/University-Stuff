#include <stdio.h>
#include <stdlib.h>
#include "ceasar.h"

int encrypt(char *in, int dist) {
    if (!in) return -1;

    for (int i=0; in[i] != '\0'; i++) {
        char c = in[i];

        if (c >= 'a' && c <= 'z') {
            c = (c - 'a' + dist) % 26 + 'a';
        }
        else if (c >= 'A' && c <= 'Z') {
            c = (c - 'A' + dist) % 26 + 'A';
        }
        in[i] = c;
    }

    return 0;
}

int decrypt(char *in, int dist) {
    if (!in) return -1;

    for (int i=0; in[i] != '\0'; i++) {
        char c = in[i];

        if (c >= 'a' && c <= 'z') {
            c = (c - 'a' - dist + 26) % 26 + 'a';
        }
        else if (c >= 'A' && c <= 'Z') {
            c = (c - 'A' - dist + 26) % 26 + 'A';
        }
        in[i] = c;
    }

    return 0;
}