#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <errno.h>
#include <signal.h>

#include "mq.h"

int endFlg = 0;

void sigintHandler(int signum) {
    printf("Server received %s signal\n", strsignal(signum));
    endFlg = 1;
    return;
}

void stdoutServerCallback(struct mq_msg *msgbuf, int rcvlength, enum direction direction) {
    if (direction == IN) {
        printf("Server received message from input message queue: %s\n", msgbuf->text);
    } else {
        printf("Server sent message to output message queue: %s\n", msgbuf->text);
    }
    return;
}

int serverProcess(void (*callback)(struct mq_msg *msgbuf, int rcvlength, enum direction direction)) {
    // allocate memory for message buffer
    struct mq_msg *msgbuf = malloc(sizeof(struct mq_msg));
    if (!msgbuf) {
        fprintf(stderr, "Error: Failed to allocate memory for message buffer\n");
        exit(1);
    }

    if (signal(SIGINT, sigintHandler) == SIG_ERR) {
        perror("Error: Failed to register signal handler");
        exit(EXIT_FAILURE);
    }

    // create two message queues
    int inputQ = msgget(INKEY, IPC_CREAT | S_IRUSR | S_IWUSR | S_IWGRP | S_IWOTH);
    if (inputQ == -1) {
        fprintf(stderr, "Error: Failed to create input message queue\n");
        exit(1);
    }

    int outputQ = msgget(OUTKEY, IPC_CREAT | S_IRUSR | S_IWUSR | S_IWGRP);
    if (outputQ == -1) {
        fprintf(stderr, "Error: Failed to create output message queue\n");
        exit(1);
    }

    printf("Server started; pid %d\n", getpid());

    int rcvlength, ret;
    while (!endFlg) {
        rcvlength = msgrcv(inputQ, msgbuf, sizeof(msgbuf->text), 0, MSG_NOERROR);
        if (rcvlength == -1) {
            if (errno == EINTR) {
                // signal received, continue
                continue;
            } else {
                perror("Error: Failed to receive message from input message queue");
                free(msgbuf);
                exit(EXIT_FAILURE);
            }
        }

        // printf("Server received message from input message queue: %s\n", msgbuf->text);
        callback(msgbuf, rcvlength, IN);

        ret = msgsnd(outputQ, msgbuf, rcvlength, 0);
        if (ret == -1) {
            perror("Error: Failed to send message to output message queue");
            free(msgbuf);
            exit(EXIT_FAILURE);
        }

        callback(msgbuf, rcvlength, OUT);
    }

    ret = msgctl(inputQ, IPC_RMID, NULL);
    if (ret == -1) {
        perror("Error: Failed to remove input message queue");
        free(msgbuf);
        exit(EXIT_FAILURE);
    }

    ret = msgctl(outputQ, IPC_RMID, NULL);
    if (ret == -1) {
        perror("Error: Failed to remove output message queue");
        free(msgbuf);
        exit(EXIT_FAILURE);
    }

    free(msgbuf);
    printf("Server stopped; pid %d\n", getpid());
    exit(EXIT_SUCCESS);
}