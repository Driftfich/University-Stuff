#include <stdio.h>

void printDual(char x) {
    unsigned char mask = 128;
    printf(" ");
    for (int i = 0; i < 8; i++) {
        if (x & mask) printf("1"); else printf("0");
        mask = mask >> 1;
    }
    printf("\n");
}

int main(void) {
    // Beispiel
    int n=1;
    for (int c=1; c<=8; c++) {
        printDual(n);
        n = n << 1;
    }
    return 0;
    // Hinweise:
    // Bitoperationen &, |, >>, << sollen verwendet werden.
    // Lösungen mit Schleifen werden hoeher bewertet als Lösungen
    // mit Zuweisungen, die mehrfach wiederholt werden.
    // Nur die Funktion printDual ist für die Ausgabe erlaubt.
    // In Ihrem eigenen Programmcode ist kein printf erlaubt.
    // Ab hier darf der Programmcode eingefuegt werden.
}
