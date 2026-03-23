#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/ipc.h>

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

int main() {
    // create semaphore
    int semkey = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    if (semkey == -1) {
        printf("Error creating semaphore: %d\n", semkey);
        exit(1);
    }

    union semun arg;
    arg.val = 0;

    // set semaphore to 0
    if (semctl(semkey, 0, SETVAL, arg) == -1) {
        printf("Error setting semaphore to 0: %d\n", semkey);
        exit(1);
    }

    // create operation structs
    struct sembuf p_op = {0, -1, SEM_UNDO};
    struct sembuf v_op = {0, 1, SEM_UNDO};

    // create child process
    int status;
    pid_t pid = fork();
    if (pid == -1) {
        printf("Error creating child process: %d\n", pid);
        exit(1);
    }

    else if (pid == 0) {
        // child process
        // try to run V operation on semaphore
        printf("Child process sleeping for 4 seconds\n");
        sleep(4);
        printf("Child process %d running V operation on semaphore\n", getpid());
        if (semop(semkey, &v_op, 1) == -1) {
            printf("Error running V operation on semaphore: %d\n", semkey);
        }
        exit(0);
    } else {
        // parent process
        // try to run the second P operation on semaphore
        printf("Parent process running P operation on semaphore\n");
        if (semop(semkey, &p_op, 1) == -1) {
            printf("Error running second P operation on semaphore: %d\n", semkey);
        }

        printf("Parent process unblocked\n");

        semctl(semkey, 0, IPC_RMID);
        printf("Semaphore removed\n");
        exit(0);
    }

    return 0;
}