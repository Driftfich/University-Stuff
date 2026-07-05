#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <ctype.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <wait.h>

#include "mq.h"

#define DST "./cpyDir/"

int endFlg = 0;

void sigintHandler(int signum) {
    endFlg = 1;
    printf("Server received %s signal\n", strsignal(signum));
    return;
}

int serverProc(void (*callback)(struct mq_msg * msgbuf, int rcvLength, enum direction direction)) {
    struct mq_msg *msgbuf = malloc(sizeof(struct mq_msg));
    if (!msgbuf) {
        puts("Failed to allocate msgbuf memory");
        exit(1);
    }

    if (signal(SIGINT, sigintHandler) == SIG_ERR) {
        puts("Failed to override the signal handler");
        exit(1);
    }

    int inpQ = msgget(INKEY, IPC_CREAT | S_IRUSR | S_IWUSR | S_IWGRP | S_IWOTH);
    if (inpQ == -1) {
        puts("Failed to create input Queue");
        exit(1);
    }

    int outQ = msgget(OUTKEY, IPC_CREAT | S_IRUSR | S_IWUSR | S_IWGRP | S_IWOTH);
    if (outQ == -1) {
        puts("Failed to create output Queue");
        msgctl(inpQ, IPC_RMID, NULL);
        exit(1);
    }

    int rcvLength;
    int ret;
    while (!endFlg) {
        rcvLength = msgrcv(inpQ, msgbuf, sizeof(msgbuf->text), 0, MSG_NOERROR);
        if (rcvLength == -1) {
            if (errno == EINTR) {
                continue;
            } else {
                puts("Server: Failed to receive message from input Queue");
                break;
            }
        }

        callback(msgbuf, rcvLength, IN);
        ret = msgsnd(outQ, msgbuf, rcvLength, 0);

        if (ret == -1) {
            puts("Server: Failed to send message");
            break;
        }

        callback(msgbuf, MAXQUEUESIZE, OUT);
    }

    ret = msgctl(inpQ, IPC_RMID, NULL);
    if (ret == -1) {
        perror("Error: Failed to remove input message queue");
        free(msgbuf);
        exit(EXIT_FAILURE);
    }

    ret = msgctl(outQ, IPC_RMID, NULL);
    if (ret == -1) {
        perror("Error: Failed to remove output message queue");
        free(msgbuf);
        exit(EXIT_FAILURE);
    }

    free(msgbuf);
    printf("Server stopped; pid %d\n", getpid());
    exit(EXIT_SUCCESS);
}

void stdServerCallback(struct mq_msg* msgbuf, int rcvLength, enum direction direction) {
    if (direction == IN) {
        printf("Server received message from input message queue: %s\n", msgbuf->text);
    } else if (direction == OUT) {
        printf("Server sent message to output message queue: %s\n", msgbuf->text);
    } else {
        puts("Invalid callback direction");
    }
}

void cpyServerCallback(struct mq_msg* msgbuf, int rcvLength, enum direction direction) {
    if (direction == IN) {
        msgbuf->text[strcspn(msgbuf->text, "\r\n")] = '\0';
        const char *srcPath = msgbuf->text;
        const char *fName = strrchr(srcPath, '/');
        fName = fName ? fName + 1 : srcPath;

        pid_t pid = fork();
        if (pid == 0) {
            char dstPath[MAXQUEUESIZE + 100];
            char cmd[MAXQUEUESIZE*2 + 128];

            snprintf(dstPath, sizeof(dstPath), "%s%s", DST, fName);
            snprintf(cmd, sizeof(cmd), "%s %s %s", "cp", srcPath, dstPath);

            printf("Copying %s -> %s\nUsing cmd: %s\n", srcPath, dstPath, cmd);
            system(cmd);
            exit(0);
        } else {
            wait(NULL);
        }
    } else if (direction == OUT) {
        printf("Copied: %s\n", msgbuf->text);
    } else {
        printf("Invalid direction\n");
    }
}

int main() {
    serverProc(cpyServerCallback);
}
