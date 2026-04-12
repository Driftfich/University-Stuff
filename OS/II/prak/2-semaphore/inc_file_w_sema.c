#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include "inc_file.h"

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

struct sembuf p_op = {0, -1, SEM_UNDO};
struct sembuf v_op = {0, 1, SEM_UNDO};

int loop_inc_file_sema(char *path, int n, int buf_size, int iter, int semkey, int abortFlg) {
    for (; iter > 0; iter--) {

        // decrement semaphore
        if (semop(semkey, &p_op, 1) == -1) {
            printf("Error running P operation on semaphore: %d\n", semkey);
            exit(1);
        }

        if (abortFlg) {
            abort();
        }

        // increment file
        int ret = inc_file(path, n, buf_size);

        // increment semaphore
        if (semop(semkey, &v_op, 1) == -1) {
            printf("Error running V operation on semaphore: %d\n", semkey);
            exit(1);
        }

        if (ret == 0) continue;
        else if (ret == 1) {
            printf("Error opening file %s: %d\n", path, ret);
        }
        else printf("Error incrementing file %s: %d\n", path, ret);
        iter++;
    }
    // printf("Successfully incremented file %s %d times\n", path, iter);
    return 0;
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

int main() {
    FILE *init = fopen("./zahl.dat", "w");
    if (init) {
        fprintf(init, "0");
        fclose(init);
    }

    int semkey = create_semaphore(1);

    int status;
    // create 5 children processes that increment the file 20000 times each
    for (int i = 0; i < 5; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            int abort = (i == 2);
            // int abort = 0;
            loop_inc_file_sema("./zahl.dat", 1, 65, 20000, semkey, abort);
            exit(0);
        }
    }
    for (int i = 0; i < 5; i++) {
        pid_t child_pid = wait(&status);
        if (WIFEXITED(status)) {
            printf("Kind %d beendet mit Exit-Status %d\n", child_pid, WEXITSTATUS(status));
        }
    }

    remove_semaphore(semkey);
    return 0;
}
