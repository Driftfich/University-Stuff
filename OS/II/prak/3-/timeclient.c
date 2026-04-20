#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "timeseg.h"

int main() {
    int shfd = openSharedMemory("/sharedTimer");
    void *timerStr = mapSharedMemory(shfd, sizeof(char) * 12 + 1);
    while(1) {
        printf("Client read out timerStr: %s\n", (char *)timerStr);
        sleep(1);
    }
    return 0;
}