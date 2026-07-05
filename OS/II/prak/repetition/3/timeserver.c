#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>
#include <string.h>
#include "timeseg.h"

int main() {
    int shfd = openSharedMemory(TIMEKEY);
    void *ptr = mapSharedMemory(shfd, sizeof(char) * 12 + 1);

    int status;
    pid_t pid = fork();
    if (pid == 0) {
        while (1) {
            printf("Child Time readout: %s", (char *) ptr);
            sleep(1);
        }
    } else {
        time_t t;
        char *tim;
        while (1) {
            t = time(NULL);
            tim = ctime(&t);
            strcpy((char *) ptr, tim);
            sleep(0.5);
        }

        wait(&status);
    }

}