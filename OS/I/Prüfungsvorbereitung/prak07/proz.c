#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t ret;
    for (int c=0; c<3; c++) {
        ret = fork();
        if (ret == -1) {
            printf("fork() failed"); exit(1);}
        if (ret == 0) {
            printf("pid of the son nr.%d: %d\n", c, getpid());
            exit(0);}
    }
    exit(0);
}