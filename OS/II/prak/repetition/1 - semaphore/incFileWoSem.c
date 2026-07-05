#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "incFile.h"

int incFileLoop(char *path, int iter, int inc, int bufSize) {
    for (int i=0; i < iter; i++) {
        int ret = incFile(path, bufSize, inc);
        if (ret != 0) i--;
    }
    printf("Increased file counter %d times\n", iter);
}

int main() {
    FILE *init = fopen("./zahl.dat", "w");
    if (init) {
        fprintf(init, "0");
        fclose(init);
    } else {
        printf("File zahl.dat cannot be created/opened.\n");
    }

    int status;
    for (int i=0; i<5; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            incFileLoop("./zahl.dat", 20000, 1, 64);
            exit(0);
        }
    }

    for (int i=0; i<5; i++) {
        pid_t childPid = wait(&status);
        if (WIFEXITED(status)) {
            printf("Kind %d beendet mit Exit-Status %d\n", childPid, WEXITSTATUS(status));
        }
    }
    return 0;
}