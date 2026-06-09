#ifndef TDA_H
#define TDA_H

typedef struct tTask {
    int id;
    float phi;
    float tp;
    float te;
    float td;
    float tmr;
    int ok;
} Task;

typedef struct tTaskSet {
    Task* tasks;
    int numTasks;
    int size;
} TaskSet;


#endif