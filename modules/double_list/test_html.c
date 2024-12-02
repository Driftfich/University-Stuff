#include <stdio.h>
#include <ctype.h>

int main() {
    FILE *F;
    F = fopen("test.html", "rt");
    if (F == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    char c;
    while ((c = fgetc(F)) != EOF) {
        if (isprint(c)) {
            putchar(c);
        } else {
            printf("0x%02X ", c);
        }
    }
}