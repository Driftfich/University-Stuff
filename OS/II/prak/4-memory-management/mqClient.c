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

int main(int argc, char *argv[]) {
    struct mq_msg *msgbuf = malloc(sizeof(struct mq_msg));
    if (!msgbuf) {
        fprintf(stderr, "Error: Failed to allocate memory for message buffer\n");
        exit(1);
    }
    
    int inputQ = msgget(INKEY, S_IWUSR);
    if (inputQ == -1) {
        fprintf(stderr, "Error: Failed to open input message queue\n");
        exit(1);
    }

    int outputQ = msgget(OUTKEY, S_IRUSR);
    if (outputQ == -1) {
        fprintf(stderr, "Error: Failed to open output message queue\n");
        exit(1);
    }

    int ret;
    while (fgets(msgbuf->text, sizeof(msgbuf->text), stdin) != NULL) {
        msgbuf->type = 42;
        msgbuf->text[strcspn(msgbuf->text, "\n")] = '\0';

        ret = msgsnd(inputQ, msgbuf, strlen(msgbuf->text) + 1, 0);
        if (ret == -1) {
            perror("Error: Failed to send message to input message queue");
            free(msgbuf);
            exit(EXIT_FAILURE);
        }

        printf("Client sent message to input message queue: %s\n", msgbuf->text);

        ret = msgrcv(outputQ, msgbuf, sizeof(msgbuf->text), 0, MSG_NOERROR);
        if (ret == -1) {
            perror("Error: Failed to receive message from output message queue");
            free(msgbuf);
            exit(EXIT_FAILURE);
        }

        printf("Client received message from output message queue: %s\n", msgbuf->text);
    }

    free(msgbuf);
    printf("Client stopped; pid %d\n", getpid());
    exit(EXIT_SUCCESS);
}