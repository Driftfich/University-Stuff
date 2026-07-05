#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "incFile.h"

int incFile(char *path, int bufSize, int inc) {
    FILE *f = fopen(path, "r+");
    if (!f) {
        return 1;
    }

    char buf[bufSize];
    long num;
    int bytesRead = fread(buf, 1, bufSize - 1, f);
    if (bytesRead == 0) {
        buf[0] = '0';
        bytesRead = 1;
    }

    buf[bytesRead] = '\0';

    num = atol(buf);
    num += inc;

    rewind(f);
    ftruncate(fileno(f), 0);
    fflush(f);
    rewind(f);

    fprintf(f,"%ld", num);
    fclose(f);
    return 0;
}