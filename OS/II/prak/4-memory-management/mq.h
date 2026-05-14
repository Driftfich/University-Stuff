#ifndef MQ_H
#define MQ_H

#define INKEY 0xaffedead
#define OUTKEY 0xdeadbeef
#define MAXSIZE 4096

struct mq_msg {
    long type;
    char text[MAXSIZE];
};

#endif