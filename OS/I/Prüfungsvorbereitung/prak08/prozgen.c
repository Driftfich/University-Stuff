#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#ifndef MAX_SONS
    #define MAX_SONS 10000
#endif

int main(int argc, char *argv[]) {
    unsigned int count;
    char *conv_error;
    if (argc < 2) {
        printf("Specify the count of processes with a -n <count> flag or just as a number <count>\n"); 
        exit(1);
    }

    count = strtoul(argv[1], &conv_error, 10);

    if (*conv_error != '\0') {
        printf("Couldnt convert argument %s to integer. Exit.\n", argv[1]);
        exit(2);
    }

    if (MAX_SONS < count) {
        printf("The specified %d processes get reduced to the %d MAX_SONS (set with -MAX_SONS) processes.\n", count, MAX_SONS);
    }
    printf("%d processes get created.\n", count);

    pid_t rets[count];
    for (int i=0; i<count; i++) {
        rets[i] = fork();
        if (rets[i] == 0) {
            sleep(10);
            exit(0);
        }
        else if (rets[i] == -1) {
            printf("Creating the %d-th children failed.", i);
            exit(3);
        }
    }

    for (int c=0; c<count; wait(NULL), c++);
    return 0;
}