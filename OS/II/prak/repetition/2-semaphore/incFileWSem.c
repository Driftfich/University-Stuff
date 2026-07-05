#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include "incFile.h"

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

struct sembuf p_op = {0, -1 };
struct sembuf v_op = {0, 1 };

int createSemaphore(int startVal) {
    int semkey = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);

    if (semkey == -1 ) {
        printf("Error creating Semaphore with start value %d.\n", startVal);
        exit(1);
    }

    union semun arg;
    arg.val = startVal;

    if (semctl(semkey, 0, SETVAL, arg) == -1) {
        printf("Error setting the semaphore to %d\n", startVal);
        exit(1);
    }

    return semkey;
}

int removeSemaphore(int semkey) {
    if (semctl(semkey, 0, IPC_RMID) == -1) {
        puts("unable to delete Semaphore");
        exit(1);
    }

    return 0;
}

int incFileSemaLoop(char *path, int iter, int inc, int bufSize, int semkey) {
    for (int i=0; i < iter; i++) {
        if (semop(semkey, &p_op, 1) == -1) {
            puts("Error runing the P Operation on the semaphore");
            continue;
        }

        if (i == 100) {
            puts("Child: Exited while having semaphore without Semundo");
            exit(1);
        }

        int ret = incFile(path, bufSize, inc);
        if (ret != 0) i--;

        if (semop(semkey, &v_op, 1) == -1) {
            puts("Failed to run the V Operation on the semaphore. Exiting...");
            exit(1);
        }
    }
    printf("Increased file counter %d times\n", iter);
}

int main() {
    char *path = "./zahlWSema.dat";
    int bufSize = 64;
    FILE *init = fopen(path, "w");
    if (!init) {
        printf("Failed to create the file %s", path);
        exit(1);
    } else {
        fprintf(init, "0");
        fclose(init);
    }

    int semkey = createSemaphore(1);

    int status;
    for (int i=0; i<5; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            incFileSemaLoop(path, 20000, 1, bufSize, semkey);
            exit(0);
        }
    }

    for (int i=0; i<5; i++) {
        pid_t childPid = wait(&status);
        if (WIFEXITED(status)) {
            printf("Kind %d beendet mit Exit-Status %d\n", childPid, WEXITSTATUS(status));
        }
    }

    init = fopen(path, "r");

    if (!init) {
        printf("Cannot open and therefore not readout the current count from %s.\n", path);
        exit(1);
    }
    char buf[bufSize];
    int bytesRead = fread(buf, 1, bufSize - 1, init);
    if (bytesRead == 0) {
        printf("Read out Zero Bytes from the file %s", path);
        fclose(init);
        exit(1);
    }

    buf[bufSize] = '\0';

    printf("The output is: %s\n", buf);
    fclose(init);
    removeSemaphore(semkey);
    return 0;
}