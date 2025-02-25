#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define length 10
#define MAX 100

int main() {
    int feld1[length] = {0};
    int feld2[length] = {0};

    srand(time(NULL));

    printf("Feld1: ");
    for (int i=0; i<length; i++) {
        feld1[i] = rand() % MAX;
        printf("%d, ", feld1[i]);
    }
    printf("\nFeld2: ");
    for (int i=0; i<length; i++) printf("0, ");

    int pos=0;
    for (int i=0; i<length; i++) {
        if (feld1[i] % 5 == 0) {
            feld2[pos] = feld1[i];
            pos++;
            feld1[i] = -1;
        }
    }
    printf("\nFelder nach filterung.\nFeld1: ");
    for (int i=0; i<length; i++) printf("%d, ", feld1[i]);
    printf("\nFeld2: ");
    for (int i=0; i<length; i++) printf("%d, ", feld2[i]);
    printf("\n");

    return 0;
}
