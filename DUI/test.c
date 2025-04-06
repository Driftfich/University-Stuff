#include <stdio.h>
#include <stdlib.h>
#include "ceasar.h"

int main() {
    char test[] = "Hallo, ich bin ceasar verschlueselt!";
    printf("Original: %s\n", test);
    encrypt(test, 3);
    printf("Encrypted: %s\n", test);
    decrypt(test, 3);
    printf("Decrypted: %s\n", test);
    return 0;
}