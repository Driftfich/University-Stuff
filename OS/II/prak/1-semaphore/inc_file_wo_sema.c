#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "inc_file.h"


int loop_inc_file(char *path, int n, int buf_size, int iter) {
    for (int i = 0; i < iter; i++) {
        int ret = inc_file(path, n, buf_size);
        if (ret == 0) continue;
        else if (ret == 1) {
            printf("Error opening file %s: %d\n", path, ret);
        }
        else printf("Error incrementing file %s: %d\n", path, ret);
    }
    printf("Successfully incremented file %s %d times\n", path, iter);
    return 0;
}

int main() {
    FILE *init = fopen("./zahl.dat", "w");
    if (init) {
        fprintf(init, "0");
        fclose(init);
    }
    
    int status;
    // create 5 children processes that increment the file 20000 times each
    for (int i = 0; i < 5; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            loop_inc_file("./zahl.dat", 1, 65, 20000);
            exit(0);
        }
    }
    for (int i = 0; i < 5; i++) {
        pid_t child_pid = wait(&status);
        if (WIFEXITED(status)) {
            printf("Kind %d beendet mit Exit-Status %d\n", child_pid, WEXITSTATUS(status));
        }
    }
    return 0;
}