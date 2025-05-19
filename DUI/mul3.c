#include <stdio.h>
#include <stdlib.h>

int mul2(int a) {
    // This function multiplies the input by 2 in aes galois field (GF(2^8))
    // Therefore a left shift with a conditional xor is used and ^=
    if (a > 0x7f) {
        a = (a << 1) ^ 0x1b;
    } else {
        a = a << 1;
    }

    return a;
}

int mul3(int a) {
    // This function multiplies the input by 3 in aes galois field (GF(2^8))
    // Therefore a left shift with a conditional xor is used and ^=
    if (a > 0x7f) {
        a = (a << 1) ^ 0x1b ^ a;
    } else {
        a = a << 1 ^ a;
    }

    return a;
}

int main() {
    // int a = 0x49; // Example input
    // int result3x = mul3(a);
    // printf("Result(3x): %x\n", result3x); // Output in hexadecimal format
    // int result2x = mul2(a);
    // printf("Result(2x): %x\n", result2x); // Output in hexadecimal format
    // xor the two results
    printf("%x\n", 0x6f ^ 0xEC ^ 0x80 ^ 0x92);
    
    return 0;


}