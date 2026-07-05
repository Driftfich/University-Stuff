#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <time.h>

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

struct sembuf p_op = { 0, -1, SEM_UNDO };
struct sembuf v_op = { 0, 1, SEM_UNDO };

int openSharedMemory(char *path) {
    int shfd = shm_open(path, O_CREAT | O_RDWR, 0600);
    if (shfd < 0) {
        printf("Error creating shared memory for path: %s\n", path);
        exit(EXIT_FAILURE);
    }
    return shfd;
}

void *mapSharedMemory(int shfd, size_t size) {
    ftruncate(shfd, size);
    void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shfd, 0);
    if (!ptr) {
        printf("Error mapping shared memory segment for shfd %d", shfd);
        exit(EXIT_FAILURE);
    }
    return ptr;
}

int createSemaphore(int startVal) {
    int semkey = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);

    if (semkey == -1) {
        printf("Error when trying to create semaphore with start value %d.\n", startVal);
        exit(1);
    }

    union semun arg;
    arg.val = startVal;

    if (semctl(semkey, 0, SETVAL, arg) == -1) {
        printf("Error when trying to change semaphore value to %d.\n", startVal);
        exit(1);
    }

    return semkey;
}

int removeSemaphore(int semkey) {
    if (semctl(semkey, 0, IPC_RMID) == -1) {
        printf("Error when trying to remove semaphore %d.\n", semkey);
        exit(1);
    }

    return 0;
}

void P(int semkey) {
    if (semop(semkey, &p_op, 1) == -1) {
        printf("Error running P() operation on semaphore %d.\n", semkey);
        exit(1);
    }

    return;
}

void V(int semkey) {
    if (semop(semkey, &v_op, 1) == -1) {
        printf("Error running V() operation on semaphore %d.\n", semkey);
        exit(1);
    }

    return;
}

void readerP(int readerSemkey, int sharedSemkey, void *rc) {
    P(readerSemkey);

    int rcVal = * (int *)(rc) + 1;
    if (rcVal == 1) P(sharedSemkey);

    *(int*) rc = rcVal;
    V(readerSemkey);
}

void readerV(int readerSemkey, int sharedSemkey, void *rc) {
    P(readerSemkey);

    int rcVal = *(int*)(rc) - 1;
    if (rcVal == 0) V(sharedSemkey);

    *(int*) rc = rcVal;
    V(readerSemkey);
}

int main() {
    int readerSemkey = createSemaphore(1);
    int sharedSemkey = createSemaphore(1);
    int shfd1 = openSharedMemory("/sharedRC1");
    void *rc1 = mapSharedMemory(shfd1, sizeof(int));
    *(int*)rc1 = 0;

    int shfd2 = openSharedMemory("/sharedRC2");
    void *rc2 = mapSharedMemory(shfd2, sizeof(int));
    *(int *)rc2 = 0;

    int status;

    time_t t1 = time(NULL);

    // test writer blocks all readers
    for (int i=0; i<10; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            if (i == 0) {
                P(sharedSemkey);
                *(int*)rc2 += 2;
                sleep(5);
                V(sharedSemkey);
                exit(EXIT_SUCCESS);
            } else {
                sleep(1); // Ensure writer gets first access
                readerP(readerSemkey, sharedSemkey, rc1);
                printf("Reader %d read %d\n", i, *(int *)rc2);
                readerV(readerSemkey, sharedSemkey, rc1);
                exit(0);
            }
        }
    }

    for (int i = 0; i < 10; i++) {
        wait(&status);
    }

    time_t t2 = time(NULL);

    printf("Test 1 - Writer blocks Readers - took %fs\n", (double)(t2 - t1));
    puts("Should take around 5 seconds");

    // test mutliple readers at the same time
    for (int i=0; i<10; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            sleep(1);
            readerP(readerSemkey, sharedSemkey, rc1);
            printf("Reader %d read %d\n", i, *(int *)rc2);
            readerV(readerSemkey, sharedSemkey, rc1);
            exit(0);
        }
    }

    for (int i = 0; i < 10; i++) {
        wait(&status);
    }

    time_t t3 = time(NULL);

    printf("Test 2 - Simultanious readers - took %fs\n", (double)(t3 - t2));
    puts("Should take around 1 second");

    munmap(rc1);
    munmap(rc2);
    removeSemaphore(sharedSemkey);
    removeSemaphore(readerSemkey);

    shm_unlink("/sharedRC1");
    shm_unlink("/sharedRC2");
}