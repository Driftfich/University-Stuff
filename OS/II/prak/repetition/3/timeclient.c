#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "timeseg.h"
#include <wait.h>

int main() {
    int shfd = openSharedMemory(TIMEKEY);
    void *ptr = mapSharedMemory(shfd, sizeof(char) * 12 + 1);
    while (1) {
        printf("Current content: %s", (char *) ptr);
        sleep(1);
    }
}