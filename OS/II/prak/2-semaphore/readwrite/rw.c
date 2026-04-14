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
#include <time.h>

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

    // take start time
    time_t start = time(NULL);

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
            } else {
                sleep(1); // Ensure writer gets first access
                readerP(readerSemkey, sharedSemkey, rc);
                printf("Reader %d read %d\n", i, *(int *)rc2);
                readerV(readerSemkey, sharedSemkey, rc);
                exit(0);
            }
        }
    }
    for (int i = 0; i < 10; i++) {
        wait(NULL);
    }
    // take end time of first test
    time_t endTest1 = time(NULL);
    printf("If writer blocks all readers, time for first test should be around 5 seconds\n");
    printf("Time for first test: %f seconds\n", ((double)(endTest1 - start)));

    // test multiple readers can read at the same time
    for (int i = 0; i < 10; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            readerP(readerSemkey, sharedSemkey, rc);
            sleep(1); // If all readers can read simultaneously, they should finish after a bit more than 1 second, else they will finish after 10 seconds
            printf("Reader %d read %d\n", i, *(int *)rc2);
            readerV(readerSemkey, sharedSemkey, rc);
            exit(0);
        }
    }

    for (int i = 0; i < 10; i++) {
        wait(NULL);
    }
    // take end time of second test
    time_t endTest2 = time(NULL);
    printf("If all readers read simultaneously, time for second test should be around 1 second, else it should be around 10 seconds\n");
    printf("Time for second test: %f seconds\n", ((double)(endTest2 - endTest1)));

    // test multiple readers block writer for a long time <> unfair solution/implementation of the reader/writer problem
    for (int i = 0; i < 10; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            if (i != 0) {
                readerP(readerSemkey, sharedSemkey, rc);
                sleep(10); // readers will finish after 10 seconds
                readerV(readerSemkey, sharedSemkey, rc);
                exit(0);
            } else {
                sleep(1); // let readers first
                P(sharedSemkey);
                printf("Writer needed to wait for %f seconds in third test\n", ((double)(time(NULL) - endTest2 - 1)));
                *(int *)rc2+=1;
                V(sharedSemkey);
                exit(0);
            }
        }
    }
    for (int i = 0; i < 10; i++) {
        wait(NULL);
    }

    // take end time of third test
    time_t endTest3 = time(NULL);
    printf("Multiple readers block writer for a long time (10s in this case) which is a unfair soltuion to the reader/writer problem and therefore doesnt let the writer in after its 1s warmup\n");
    printf("Time for third test: %f seconds\n", ((double)(endTest3 - endTest2)));

    // close shared memory and semaphores
    munmap(rc, sizeof(int));
    munmap(rc2, sizeof(int));
    remove_semaphore(readerSemkey);
    remove_semaphore(sharedSemkey);

    shm_unlink("/sharedRC1");
    shm_unlink("/sharedRC2");

    return 0;
}
