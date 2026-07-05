#ifndef MQ_H
#define MQ_H

#define INKEY 0xaffedead
#define OUTKEY 0xbeafdead
#define MAXQUEUESIZE 4096

enum direction {
    IN = 0,
    OUT = 1
};

struct mq_msg {
    long type;
    char text[MAXQUEUESIZE];
};

#endif