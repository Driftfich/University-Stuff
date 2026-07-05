#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "mq.h"

int clientProc(void (*callback)(struct mq_msg *msgbuf, int rcvLength, enum direction direction)) {
    struct mq_msg* msgbuf = malloc(sizeof(struct mq_msg));

    int inpQ = msgget(INKEY, S_IWUSR);
    if (inpQ == -1) {
        puts("Failed to get input queue");
        exit(1);
    }

    int outQ = msgget(OUTKEY, S_IRUSR);
    if (outQ == -1) {
        puts("Failed to get output queue");
        exit(1);
    }

    int ret;
    while(fgets(msgbuf->text, sizeof(msgbuf->text)-1, stdin) != NULL) {
        msgbuf->type = 42;
        msgbuf->text[strcspn(msgbuf->text, "\n")] = '\0';

        ret = msgsnd(inpQ, msgbuf, strlen(msgbuf->text) + 1, 0);
        if (ret == -1) break;

        callback(msgbuf, strlen(msgbuf->text) + 1, OUT);

        ret = msgrcv(outQ, msgbuf, sizeof(msgbuf->text), 0, MSG_NOERROR);
        msgbuf->text[strcspn(msgbuf->text, "\n")] = '\0';
        if (ret == -1) break;

        callback(msgbuf, strlen(msgbuf->text) + 1, IN);
    }

    free(msgbuf);
    msgctl(inpQ, IPC_RMID, NULL);
    msgctl(outQ, IPC_RMID, NULL);
    printf("Client stopped; pid %d\n", getpid());
    exit(EXIT_SUCCESS);
}

void stdoutClientCallback(struct mq_msg *msgbuf, int rcvlength, enum direction direction) {
    if (direction == IN) {
        printf("Client received message from input message queue: %s\n", msgbuf->text);
    } else if (direction == OUT) {
        printf("Client sent message to output message queue: %s\n", msgbuf->text);
    } else {
        printf("Client: Invalid direction");
    }
    return;
}

int main() {
    clientProc(stdoutClientCallback);
}
