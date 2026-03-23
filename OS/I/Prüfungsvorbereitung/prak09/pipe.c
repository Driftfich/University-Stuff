#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define LINESIZE 256
#define PATH "./pipe.c"

int main() {
    char buf[LINESIZE];
    pid_t res = fork();
    int ret;
    int fd[2];
    ret = pipe(fd);

    if (ret != 0) {
        puts("Pipe creation failed.\n");
        exit(1);
    }

    if (res == -1) {
        puts("Creating the child process failed.\n");
        exit(2);
    }
    else if (res == 0) {
        // Sohn => close write pipe, read pipe in a loop, close read pipe, exit
        ret = close(fd[1]);

        if (ret != 0) {
            puts("Child: Closing the write pipe failed.\n");
            exit(3);
        }
        sleep(1);
        ssize_t bytesRead = 0;
        while ((bytesRead = read(fd[0], buf, LINESIZE - 1)) != 0){
            // buf[bytesRead] = '\0';
            // printf("Child: %s\n", buf);
            write(1, buf, bytesRead);
        }

        if (bytesRead <= 0) {
            perror("Child: Error reading from pipe");
            exit(EXIT_FAILURE);
        }

        ret = close(fd[0]);

        if (ret != 0) {
            puts("Child: Closing the read pipe failed.\n");
            exit(5);
        }

        exit(0);
    }
    else {
        // Vater => close read pipe, write pipe in a loop, close write pipe, exit
        ret = close(fd[0]);

        if (ret != 0) {
            puts("Father: Closing the read pipe failed.\n");
            exit(4);
        }

        FILE *f = fopen(PATH, "R");

        while(fgets(buf, LINESIZE, f)) {
            ssize_t len = strlen(buf);
            if (write(fd[1], buf, len) != len) {
                perror("Father: Write to pipe failed\n");
                exit(EXIT_FAILURE);
            }
        }

        ret = close(fd[1]);

        if (ret != 0) {
            puts("Father: Closing the write pipe failed.\n");
            exit(6);
        }

        int wpid = wait(NULL);

        if (wpid != 0) {
            puts("Father: Waiting for Child failed.\n");
            exit(7);
        }

        exit(0);
    }

    exit(0);
}
