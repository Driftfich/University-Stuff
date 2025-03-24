#include <stdio.h>
#include <stdlib.h>

int main() {
    char buf[3];
    while (1==1) {
        printf("Gebe ein Zeichen ein: ");
        fgets(buf, sizeof(buf), stdin);
        if (buf[0] == 'q' && buf[1] == 'q') break;
        char c = buf[0];
        if ('A' <= c && c <= 'Z') printf("Die Eingabe ist ein Großbuchstabe\n");
        else if ('a' <= c && c <= 'z') printf("Die Eingabe ist ein Kleinbuchstabe\n");
        else if ('0' <= c && c <= '9') printf("Die Eingabe ist eine Zahl\n");
        else  printf("Die Eingabe ist ein Sonderzeichen\n");
    }
    return 0;
}