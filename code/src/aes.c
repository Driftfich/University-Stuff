#include "aes.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// S-Box und Inverse S-Box

static const uint8_t sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b,
    0xfe, 0xd7, 0xab, 0x76, 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
    0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, 0xb7, 0xfd, 0x93, 0x26,
    0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2,
    0xeb, 0x27, 0xb2, 0x75, 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
    0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, 0x53, 0xd1, 0x00, 0xed,
    0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f,
    0x50, 0x3c, 0x9f, 0xa8, 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
    0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, 0xcd, 0x0c, 0x13, 0xec,
    0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14,
    0xde, 0x5e, 0x0b, 0xdb, 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
    0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, 0xe7, 0xc8, 0x37, 0x6d,
    0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f,
    0x4b, 0xbd, 0x8b, 0x8a, 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
    0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, 0xe1, 0xf8, 0x98, 0x11,
    0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f,
    0xb0, 0x54, 0xbb, 0x16 };

static const uint8_t rsbox[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e,
    0x81, 0xf3, 0xd7, 0xfb, 0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87,
    0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb, 0x54, 0x7b, 0x94, 0x32,
    0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49,
    0x6d, 0x8b, 0xd1, 0x25, 0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16,
    0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92, 0x6c, 0x70, 0x48, 0x50,
    0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05,
    0xb8, 0xb3, 0x45, 0x06, 0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02,
    0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b, 0x3a, 0x91, 0x11, 0x41,
    0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8,
    0x1c, 0x75, 0xdf, 0x6e, 0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89,
    0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b, 0xfc, 0x56, 0x3e, 0x4b,
    0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59,
    0x27, 0x80, 0xec, 0x5f, 0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d,
    0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef, 0xa0, 0xe0, 0x3b, 0x4d,
    0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63,
    0x55, 0x21, 0x0c, 0x7d };

unsigned int numRounds(unsigned int keySize) {
    // Rijndael Algorithm num Rounds depends on Blocksize and Keysize. Because Blocksize in AES is always 128 Bit, the num of Rounds depends just on the keysize
    switch (keySize) {
        case 128:
            return 10;
        case 160:
            return 11;
        case 192:
            return 12;
        case 224:
            return 13;
        case 256:
            return 14;
        default:
            printf("No valid keysize!");
            exit(1);
    }
}

unsigned int numKeyWords(unsigned int keySize) {
    // switch (keySize) {
    //     case 128:
    //         return 4;
    //     case 192:
    //         return 6;
    //     case 256:
    //         return 8;
    //     default:
    //         printf("No valid keysize!");
    //         exit(1);
    // }
    return keySize / 32;
}

u_int8_t getSBoxValue(u_int8_t num) {
    int xpos = num % 16;
    int ypos = num / 16;
    int idx = (16 * ypos) + xpos;
    if (idx < 0 || idx > 256) exit(2);
    return sbox[idx];
}

u_int8_t getSBoxInvert(u_int8_t num) {
    int xpos = num % 16;
    int ypos = num / 16;
    int idx = (16 * ypos) + xpos;
    if (idx < 0 || idx > 256) exit(2);
    return rsbox[idx];
}

u_int8_t rc(u_int8_t num) {
    if (num==0) return 0;
    u_int8_t rcon = 1;
    for (u_int8_t j=1; j<num; j++) {
        if (rcon >= 0x80) {
            rcon = (rcon << 1) ^ 0x1b;
        } else {
            rcon = (rcon << 1);
        }
    }
    return rcon;
}

void keyExpansion(u_int8_t* key, u_int8_t* roundKeys, unsigned int keySize) {
    unsigned int Nk = numKeyWords(keySize);
    unsigned int Nr = numRounds(keySize); // Number of rounds
    unsigned int Nw = (Nr + 1) * 4; // Number of 32-bit words in the expanded key
    
    // Copy the original key into the first Nk words of the expanded key
    for (int i = 0; i < Nk; i++) {
        roundKeys[i * 4] = key[i * 4];
        roundKeys[i * 4 + 1] = key[i * 4 + 1];
        roundKeys[i * 4 + 2] = key[i * 4 + 2];
        roundKeys[i * 4 + 3] = key[i * 4 + 3];
    }

    // Expand the key
    u_int8_t temp[4];
    for (int i = Nk; i < Nw; i++) {
        // copy the previous word
        for (int j = 0; j < 4; j++) {
            temp[j] = roundKeys[(i - 1) * 4 + j];
        }

        if (i % Nk == 0) {
            // Rotate the word
            u_int8_t t = temp[0];
            temp[0] = temp[1];
            temp[1] = temp[2];
            temp[2] = temp[3];
            temp[3] = t;
            // Apply S-Box
            for (int j = 0; j < 4; j++) {
                temp[j] = getSBoxValue(temp[j]);
            }
            // XOR with the round constant
            u_int8_t rcon = rc(i / Nk);
            temp[0] ^= rcon;
        }
        else if (Nk > 6 && i % Nk == 4) {
            // Apply S-Box
            for (int j = 0; j < 4; j++) {
                temp[j] = getSBoxValue(temp[j]);
            }
        }

        // copy the new word into the expanded key and xor with the previous word
        for (int j = 0; j < 4; j++) {
            roundKeys[i * 4 + j] = roundKeys[(i - Nk) * 4 + j] ^ temp[j];
        }
    }
}

void getRoundKey(u_int8_t* roundKeys, u_int8_t* roundKey, u_int8_t round) {
    for (int i=0; i<16; i++) {
        roundKey[i] = roundKeys[round * 16 + i];
    }
}

void addRoundKey(u_int8_t* state, u_int8_t* roundKey) {
    for (int i=0; i<16; i++) {
        state[i] ^= roundKey[i];
    }
}

void subBytes(u_int8_t* state) {
    for (int i=0; i<16; i++) {
        state[i] = getSBoxValue(state[i]);
    }
}

void shiftRows(u_int8_t* state) {
    // Shift the columns upwards by the column index
    u_int8_t temp[16];

    // // transpose the state matrix
    memcpy(temp, state, 16);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i * 4 + j] = temp[j * 4 + i];
            // printf("%x ", state[i * 4 + j]);
        }
        // printf("\n");
    }

    memcpy(temp, state, 16);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i * 4 + j] = temp[i * 4 + (j + i) % 4];
            // printf("%x ", state[i * 4 + j]);
        }
        // printf("\n");
    }

    // // transpose the state matrix back
    memcpy(temp, state, 16);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[j * 4 + i] = temp[i * 4 + j];
            // printf("%x ", state[j * 4 + i]);
        }
        // printf("\n");
    }
}

void multiply2(u_int8_t* state) {
    for (int i = 0; i < 16; i++) {
        if (state[i] > 0x7f) {
            state[i] = (state[i] << 1) ^ 0x1b;
        } else {
            state[i] = (state[i] << 1);
        }
    }
}

void multiply3(u_int8_t* state) {
    for (int i = 0; i < 16; i++) {
        if (state[i] > 0x7f) {
            state[i] = ((state[i] << 1) ^ 0x1b) ^ state[i];
        } else {
            state[i] = (state[i] << 1) ^ state[i];
        }
    }
}

void mixColumns(u_int8_t* state) {
    u_int8_t orig[16];
    u_int8_t state2[16];
    u_int8_t state3[16];

    memcpy(orig, state, 16);
    memcpy(state2, state, 16);
    memcpy(state3, state, 16);

    multiply2(state2);
    multiply3(state3);

    for (int i = 0; i < 4; i++) {
        state[i * 4] = state2[i * 4] ^ state3[i * 4 + 1] ^ orig[i * 4 + 2] ^ orig[i * 4 + 3];
        state[i * 4 + 1] = orig[i * 4] ^ state2[i * 4 + 1] ^ state3[i * 4 + 2] ^ orig[i * 4 + 3];
        state[i * 4 + 2] = orig[i * 4] ^ orig[i * 4 + 1] ^ state2[i * 4 + 2] ^ state3[i * 4 + 3];
        state[i * 4 + 3] = state3[i * 4] ^ orig[i * 4 + 1] ^ orig[i * 4 + 2] ^ state2[i * 4 + 3];
    }
}

void multiply9(u_int8_t* state) {
    u_int8_t x8[16];

    memcpy(x8, state, 16);

    multiply2(x8);
    multiply2(x8);
    multiply2(x8);

    for (int i = 0; i < 16; i++) {
        state[i] = x8[i] ^ state[i];
    }
}

void multiply11(u_int8_t* state) {
    u_int8_t x9[16];
    u_int8_t x2[16];

    memcpy(x9, state, 16);
    memcpy(x2, state, 16);

    multiply2(x2);

    multiply9(x9);

    for (int i = 0; i < 16; i++) {
        state[i] = x9[i] ^ x2[i];
    }
}

void multiply13(u_int8_t* state) {
    u_int8_t s_x2[16];
    u_int8_t s_x4[16];
    u_int8_t s_x8[16];

    memcpy(s_x2, state, 16);
    multiply2(s_x2);

    memcpy(s_x4, s_x2, 16);
    multiply2(s_x4);

    memcpy(s_x8, s_x4, 16);
    multiply2(s_x8);

    for (int i = 0; i < 16; i++) {
        state[i] = s_x8[i] ^ s_x4[i] ^ state[i];
    }
}

void multiply14(u_int8_t* state) {
    u_int8_t s_x2[16];
    u_int8_t s_x4[16];
    u_int8_t s_x8[16];

    memcpy(s_x2, state, 16); 
    multiply2(s_x2);

    memcpy(s_x4, s_x2, 16);
    multiply2(s_x4); 

    memcpy(s_x8, s_x4, 16);
    multiply2(s_x8);

    for (int i = 0; i < 16; i++) {
        state[i] = s_x8[i] ^ s_x4[i] ^ s_x2[i];
    }
}

void invMixColumns(u_int8_t* state) {
    u_int8_t state9[16];
    u_int8_t state11[16];
    u_int8_t state13[16];
    u_int8_t state14[16];

    memcpy(state9, state, 16);
    memcpy(state11, state, 16);
    memcpy(state13, state, 16);
    memcpy(state14, state, 16);

    multiply9(state9);

    multiply11(state11);

    multiply13(state13);

    multiply14(state14);

    for (int i = 0; i < 4; i++) {
        state[i * 4] = state14[i * 4] ^ state11[i * 4 + 1] ^ state13[i * 4 + 2] ^ state9[i * 4 + 3];
        state[i * 4 + 1] = state9[i * 4] ^ state14[i * 4 + 1] ^ state11[i * 4 + 2] ^ state13[i * 4 + 3];
        state[i * 4 + 2] = state13[i * 4] ^ state9[i * 4 + 1] ^ state14[i * 4 + 2] ^ state11[i * 4 + 3];
        state[i * 4 + 3] = state11[i * 4] ^ state13[i * 4 + 1] ^ state9[i * 4 + 2] ^ state14[i * 4 + 3];
    }

}

void printBlock(u_int8_t* block) {
}

void encrypt(u_int8_t* block, u_int8_t* roundKeys, unsigned int rounds) {
    // Initial AddRoundKey
    addRoundKey(block, roundKeys);

    // Main rounds
    for (int i=0; i<rounds; i++) {
        subBytes(block);
        shiftRows(block);
        if (i < rounds - 1) {
            mixColumns(block);
        }
        addRoundKey(block, roundKeys + (i + 1) * 16);
    }
}

void invSubBytes(u_int8_t* state) {
    for (int i=0; i<16; i++) {
        state[i] = getSBoxInvert(state[i]);
    }
}

void invShiftRows(u_int8_t *state) {
    u_int8_t temp[16];

    // // transpose the state matrix
    memcpy(temp, state, 16);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i * 4 + j] = temp[j * 4 + i];
            // printf("%x ", state[i * 4 + j]);
        }
        // printf("\n");
    }

    memcpy(temp, state, 16);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i * 4 + j] = temp[i * 4 + (j - i + 4) % 4];
            // printf("%x ", state[i * 4 + j]);
        }
        // printf("\n");
    }

    // // transpose the state matrix back
    memcpy(temp, state, 16);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[j * 4 + i] = temp[i * 4 + j];
            // printf("%x ", state[j * 4 + i]);
        }
        // printf("\n");
    }
}

void decrypt(u_int8_t* block, u_int8_t* roundKeys, unsigned int rounds) {
}

void ecb_encrypt(u_int8_t *content, u_int8_t *key, unsigned int keySize, size_t length){
}

void ecb_decrypt(u_int8_t *content, u_int8_t *key, unsigned int keySize, size_t length){
}

void cbc_encrypt(u_int8_t *content, u_int8_t *key, unsigned int keySize, u_int8_t *iv, size_t length) {
}

void cbc_decrypt(u_int8_t *content, u_int8_t *key, unsigned int keySize, u_int8_t *iv, size_t length) {
}
