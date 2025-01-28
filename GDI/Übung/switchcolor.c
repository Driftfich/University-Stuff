#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int sort(char *ar[], int length) {
    int bidx=0, gidx=length-1, mid=0;

    while(mid <=gidx) {
        if (strcmp(ar[mid],"black") == 0) {
            ar[mid] = ar[bidx];
            ar[bidx] = "black";
            bidx++;
        }
        else if (strcmp(ar[mid],"gold") == 0) {
            ar[mid] = ar[gidx];
            ar[gidx] = "gold";
            gidx--;
        }
        else if (strcmp(ar[mid], "red") == 0) {
            mid++;
        }
    }

    return 0;
}

int main() {
    char *arr[] = {"gold", "black", "gold", "red", "gold", "black", "red", "gold"};
    int length = 8;
    sort(arr, length);
    for (int i=0; i<length; i++) {
        printf("%s, ", arr[i]);
    }
    printf("\n");
    return 0;
}