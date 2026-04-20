#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "timeseg.h"

// void cleanup(int status, void *arg) {
//     char *path = (char *)arg;
//     int shfd = (int)arg;
//     void *timerStr = (void *)arg + sizeof(char *) + sizeof(int);
//     int size = (int)arg + sizeof(char *) + sizeof(int);
//     munmap(timerStr, size);
//     close(shfd);
//     shm_unlink(path);
// }

int main() {
    int shfd = openSharedMemory("/sharedTimer");
    void *timerStr = mapSharedMemory(shfd, sizeof(char) * 12 + 1);
    time_t tim = time(NULL);

    // on_exit(cleanup, "/sharedTimer", shfd, timerStr, sizeof(char) * 12 + 1);

    pid_t pid = fork();
    if (pid == 0) {
        // child process
        // read out timerStr
        sleep(0.01);
        while(1) {
            printf("Child process read out timerStr: %s\n", (char *)timerStr);
            sleep(1);
        }
    } else {
        // parent process
        while(1) {
            // get current time
            time_t tim = time(NULL);
            // get hour
            int hour = (tim / 3600 + 2) % 24;
            // get minute
            int minute = tim / 60 % 60;
            // get seconds
            int second = tim % 60 % 60;
            // write time into timerStr
            sprintf((char *)timerStr, "%d:%d.%d.", hour, minute, second);
            sleep(1);
        }
    }
}
