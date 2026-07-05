#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <fcntl.h>

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

struct sembuf p_op = { 0, -1, SEM_UNDO };
struct sembuf v_op = { 0, 1, SEM_UNDO };

int createSemaphore(int startval) {
    int semkey = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);
    if (semkey == -1) {
        puts("Failed to create semaphore");
        exit(1);
    }

    union semun arg;
    arg.val = startval;

    if (semctl(semkey, 0, SETVAL, arg) == -1 ) {
        puts("Failed to set semaphore value");
        exit(1);
    }


    return semkey;
}

void removeSemaphore(int semkey) {
    if (semctl(semkey, 0, IPC_RMID) == -1) {
        puts("Failed to remove semaphore");
        exit(1);
    }
}

void P(int semkey) {
    if (semop(semkey, &p_op, 1) == -1) {
        puts("Failed to run P op on semaphore");
        exit(1);
    }
}

void V(int semkey) {
    if (semop(semkey, &v_op, 1) == -1) {
        puts("Failed to run V op on semaphore");
        exit(1);
    }
}

int main() {
    int mapSize = 1024;
    void *ptr = mmap(NULL, mapSize, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (!ptr) {
        puts("Failed to create anonym shared memory page");
    }

    int semkey = createSemaphore(0);
    pid_t pid = fork();
    if (pid == 0) {
        P(semkey);
        printf("Message from parent proc: %s\n", (char *) ptr);
        exit(0);
    } else {
        strcpy((char *) ptr, "Some Message to the child process");
        V(semkey);
        wait(NULL);

        munmap(ptr, mapSize);
        removeSemaphore(semkey);
        return EXIT_SUCCESS;
    }
}