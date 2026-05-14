#ifndef MQ_H
#define MQ_H

#define INKEY 0xaffedead
#define OUTKEY 0xdeadbeef
#define MAXSIZE 4096

enum direction {
    IN = 0,
    OUT = 1
};

struct mq_msg {
    long type;
    char text[MAXSIZE];
};

void stdoutServerCallback(struct mq_msg *msgbuf, int rcvlength, enum direction direction);
void stdoutClientCallback(struct mq_msg *msgbuf, int rcvlength, enum direction direction);

int serverProcess(void (*callback)(struct mq_msg *msgbuf, int rcvlength, enum direction direction));
int clientProcess(void (*callback)(struct mq_msg *msgbuf, int rcvlength, enum direction direction));

#endif