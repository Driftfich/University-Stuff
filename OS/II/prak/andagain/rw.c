#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/wait.h>

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

struct sembuf p_op = { 0, -1, SEM_UNDO };
struct sembuf v_op = { 0, 1, SEM_UNDO };

int createSemaphore(int startVal) {
    int semkey = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);

    if (semkey == -1) {
        puts("Semget failed");
        exit(EXIT_FAILURE);
    }

    union semun arg;
    arg.val = startVal;

    if (semctl(semkey, 0, SETVAL, arg) == -1) {
        puts("Semctl failed");
        exit(EXIT_FAILURE);
    }

    return semkey;
}

void rmSemaphore(int semkey) {
    if (semctl(semkey, 0, IPC_RMID) == -1) {
        puts("Failed to rm Semaphore");
        exit(EXIT_FAILURE);
    }

    return;
}

void P(int semkey) {
    if (semop(semkey, &p_op, 1) == -1) {
        puts("P Operation on Semaphore failed");
        exit(EXIT_FAILURE);
    }

    return;
}

void V(int semkey) {
    if (semop(semkey, &v_op, 1) == -1) {
        puts("V Operation on Semaphore failed");
        exit(EXIT_FAILURE);
    }

    return;
}

void readerP(int readerSemkey, int sharedSemkey, int *rc) {
    P(readerSemkey);

    int rcVal = *(int *) rc;
    rcVal++;
    *(int *) rc = rcVal;
    if (rcVal == 1) P(sharedSemkey);

    V(readerSemkey);
}

void readerV(int readerSemkey, int sharedSemkey, int *rc) {
    P(readerSemkey);

    int rcVal = *(int *)rc;
    rcVal--;
    *(int *) rc = rcVal;
    if (rcVal == 0) V(sharedSemkey);

    V(readerSemkey);
}

int openSharedMemory(char *path) {
    int shfd = shm_open(path, O_CREAT | O_RDWR, 0600);
    if (shfd < 0) {
        puts("Failed to open shared memory segment");
        exit(EXIT_FAILURE);
    }

    return shfd;
}

void *mapSharedMemory(int shfd, int size) {
    ftruncate(shfd, size);
    void *ptr = mmap(shfd, size, PROT_WRITE | PROT_READ, MAP_SHARED, shfd, 0);
    if (ptr == MAP_FAILED) {
        puts("Failed to Map shared Memory"); 
        exit(EXIT_FAILURE);
    }

    return ptr;
}

int main() {
    int readerSemkey = createSemaphore(1);
    int sharedSemkey = createSemaphore(1);
    int shfd1 = openSharedMemory("rc1");
    int shfd2 = openSharedMemory("text");
    void *rc1 = mapSharedMemory(shfd1, 64);
    void *text = mapSharedMemory(shfd2, 64);

    pid_t pid;
    for (int i=0; i<10; i++) {
        pid = fork();
        if (pid == 0) {
            if (i == 0) {
                P(sharedSemkey);

                strncpy(text, "Some text from writer proc", 63);
                printf("Writer wrote a msg into text\n");
                sleep(5);

                V(sharedSemkey);
            } else {
                // wait for the writer proc
                sleep(1);
                puts("Reader ready");
                readerP(readerSemkey, sharedSemkey, rc1);

                printf("Read msg: %s\n", (char *) text);

                readerV(readerSemkey, sharedSemkey, rc1);
            }

            exit(EXIT_SUCCESS);
        }
    }

    wait(NULL);
}