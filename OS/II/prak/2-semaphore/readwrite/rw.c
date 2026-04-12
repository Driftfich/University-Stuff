#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

struct sembuf p_op = {0, -1, SEM_UNDO};
struct sembuf v_op = {0, 1, SEM_UNDO};

int openSharedMemory(char *path) {
    int shfd = shm_open(path, O_CREAT | O_RDWR, 0600);
    if (shfd < 0) exit(EXIT_FAILURE);
    return shfd;
}

void *mapSharedMemory(int shfd, size_t size) {
    ftruncate(shfd, size);
    void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shfd, 0);
    if (ptr == MAP_FAILED) exit(EXIT_FAILURE);
    return ptr;
}

int create_semaphore(int startval) {
    int semkey = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);
    if (semkey == -1) {
        printf("Error creating semaphore: %d\n", semkey);
        exit(1);
    }

    union semun arg;
    arg.val = startval;
    if (semctl(semkey, 0, SETVAL, arg) == -1) {
        printf("Error setting semaphore to 0: %d\n", semkey);
        exit(1);
    }

    return semkey;
}

int remove_semaphore(int semkey) {
    if (semctl(semkey, 0, IPC_RMID) == -1) {
        printf("Error removing semaphore: %d\n", semkey);
        exit(1);
    }
    return 0;
}

void P(int semkey) {
    if (semop(semkey, &p_op, 1) == -1) {
        printf("Error running P operation on semaphore: %d\n", semkey);
        exit(1);
    }
}

void V(int semkey) {
    if (semop(semkey, &v_op, 1) == -1) {
        printf("Error running V operation on semaphore: %d\n", semkey);
        exit(1);
    }
}

void readerP(int readerSemkey, int sharedSemkey, void *rc) {
    // use reader semaphore to protect shared access on rc
    P(readerSemkey);
    // read out rc and increase
    int rcVal = (*(int *)rc) + 1;
    // update rc value
    *(int *)rc = rcVal;
    // if rcVal 1, close shared Semaphore
    if (rcVal == 1) P(sharedSemkey);
    V(readerSemkey);
}

void readerV(int readerSemkey, int sharedSemkey, void *rc) {
    // use reader semaphore to protect shared access on rc
    P(readerSemkey);
    // read out rc and decrease
    int rcVal = (*(int *)rc) - 1;
    // update rc value
    *(int *)rc = rcVal;
    // if rcVal 0, close shared Semaphore
    if (rcVal == 0) V(sharedSemkey);
    V(readerSemkey);
}

int main() {
    int readerSemkey = create_semaphore(1);
    int sharedSemkey = create_semaphore(1);
    int shfd = openSharedMemory("/sharedRC1");
    void *rc = mapSharedMemory(shfd, sizeof(int));
    *(int *)rc = 0;

    int shfd2 = openSharedMemory("/sharedRC2");
    void *rc2 = mapSharedMemory(shfd2, sizeof(int));
    *(int *)rc2 = 0;

    int status;

    // test writer blocks all readers
    for (int i = 0; i < 10; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            if (i == 0) {
                P(sharedSemkey);
                *(int *)rc2+=1;
                sleep(5); // let readers wait for writer to finish
                V(sharedSemkey);
                exit(0);
            } else if (i == 1) {
                sleep(1); // Ensure writer gets first access
                readerP(readerSemkey, sharedSemkey, rc);
                printf("Reader %d read %d\n", i, *(int *)rc2);
                readerV(readerSemkey, sharedSemkey, rc);
                exit(0);
            }
        } else {
            wait(&status);
            // close shared memory and semaphores
            munmap(rc, sizeof(int));
            munmap(rc2, sizeof(int));
            remove_semaphore(readerSemkey);
            remove_semaphore(sharedSemkey);
        }
    }
    return 0;
}
