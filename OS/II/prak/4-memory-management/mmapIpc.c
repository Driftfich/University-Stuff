#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/sem.h>

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

struct sembuf p_op = {0, -1, SEM_UNDO};
struct sembuf v_op = {0, 1, SEM_UNDO};

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


int main() {
    size_t size = 1024;
    void *sharedMemPtr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (sharedMemPtr == MAP_FAILED || sharedMemPtr == NULL) {
        printf("Error: Failed to map shared memory\n");
        exit(1);
    }

    printf("Shared memory mapped at %p\n", sharedMemPtr);

    int semkey = create_semaphore(0);
    pid_t pid = fork();
    if (pid == 0) {
        // child process
        printf("Semkey: %d\n", semkey);
        P(semkey);
        printf("Child process read from shared memory: %s\n", (char *)sharedMemPtr);
    } else {
        // parent process
        printf("Semkey: %d\n", semkey);
        strcpy(sharedMemPtr, "Hello, World!");
        printf("Parent process wrote to shared memory: %s\n", (char *)sharedMemPtr);
        V(semkey);
    }

    wait(NULL);
    printf("Parent process waited for child process to finish\n");

    munmap(sharedMemPtr, size);
    remove_semaphore(semkey);

    return 0;
}