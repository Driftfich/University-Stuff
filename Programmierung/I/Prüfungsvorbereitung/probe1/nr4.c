#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LENGTH 20
#define MAX 100

int main() {
    int feld1[LENGTH] = {0};
    srand(time(NULL));
    printf("Das Feld:\n");
    for (int i=0; i<LENGTH; i++) {feld1[i]=rand() % MAX; printf("%d\t", feld1[i]);}
    printf("\nAnzahl der Elemente:\t%d\n", LENGTH);
    printf("Ausgabe von Mitte nach Links und von Rechts zur Mitte:\n");
    int lg=0;
    for (int i=0; i<LENGTH/2; i++) {
        printf("%d\t+\t%d\t=%d\n", feld1[i], feld1[LENGTH-i-1], feld1[i] + feld1[LENGTH-i-1]);
        if (feld1[i] > 12) {
            lg++;
            feld1[i]=-10;
        }
    }

    printf("Anzahl der Zahlen groesser 12 in linker Haelfte des Feldes:\t%d\n", lg);
    printf("Anzahl der Zahlen nicht groesser 12 in linker Haelfte des Feldes:\t%d\n", LENGTH/2-lg);

    printf("Das Feld:\n");
    for (int i=0; i<LENGTH; i++) printf("%d\t", feld1[i]);
    printf("\nAnzahl Elemente:\t%d\n", LENGTH);

    return 0;
}
