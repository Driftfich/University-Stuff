#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <cmd>\n", argv[0]);
        exit(1);
    }
    printf("Starting %s ...\n", argv[1]);
    execvp(argv[1], &argv[1]);
    perror("execl");
    exit(0);
}