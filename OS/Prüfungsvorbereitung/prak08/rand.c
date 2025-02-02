#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main() {
    srand((unsigned int)time(NULL));
    printf("Returning with %ld\n", random());
    exit(random());
}