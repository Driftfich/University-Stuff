#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

#include "mq.h"

void cpyServerCallback(struct mq_msg *msgbuf, int rcvlength, enum direction direction) {
    if (direction == IN) {
        // fork child process
        pid_t pid = fork();
        if (pid == 0) {
            // child process copies given file into given output file
            system(msgbuf->text);
            printf("Server copied file using command: %s\n", msgbuf->text);
        } else {
            // parent process
            wait(NULL);
        }
    }
    else if (direction == OUT) {
        printf("Server sent message to output message queue: %s\n", msgbuf->text);
    }
    return;
}

int main() {
    serverProcess(cpyServerCallback);
    return 0;
}