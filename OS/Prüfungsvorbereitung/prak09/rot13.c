#include <stdio.h>
#include <stdlib.h>
// #include <unistd.h>
// #include <sys/types.h>
#include <string.h>
#include <ctype.h>

#define LINESIZE 256

void invert(char *buf) {
    int len = strlen(buf) - 1;
    int mid = len / 2;
    char tmp;
    for (int i=0; i<mid; i++) {
        char tmp = buf[i];
        buf[i] = buf[len-i];
        buf[len-i] = tmp;
    }

    return;
}

void rot13(char *buf) {
    char x;
    for (int c=0; c< strlen(buf); c++) {
        x = buf[c];
        if (isalpha(x)) {
            if (x >= 'A' && x<= 'M' || x >= 'a' && x <= 'm') {
                x+=13;
            }
            else {
                x-=13;
            }
        }
        buf[c] = x;
    }

    return;
}

int main() {
    char line[LINESIZE];
    int ret;
    while (fgets(line, LINESIZE, stdin)) {
        // rot13(line);
        invert(line);
        printf("%s", line);
    }
    exit(0);
}
