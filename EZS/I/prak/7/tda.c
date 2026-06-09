#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tda.h"

float taskUtil(Task task) {
    return task.te / task.tp;
}

int LaylandTest(int numTasks, float sysUtil) {
    float res = numTasks * (pow(2, 1.0 / numTasks) - 1);
    return sysUtil <= res;
}

void increaseTaskSetSize(TaskSet* taskSet) {
    taskSet->size *= 2;
    taskSet->tasks = (Task*)realloc(taskSet->tasks, sizeof(Task) * taskSet->size);
}

void decreaseTaskSetSize(TaskSet* taskSet) {
    taskSet->size /= 2;
    taskSet->tasks = (Task*)realloc(taskSet->tasks, sizeof(Task) * taskSet->size);
}

// read tasks from stdin
void readTasks(TaskSet* taskSet) {
    while (1) {
        if (taskSet->numTasks >= taskSet->size) {
            increaseTaskSetSize(taskSet);
        }

        Task task;
        int n = scanf(" T%d(%f, %f, %f, %f)", &task.id, &task.phi, &task.tp, &task.te, &task.td);
        task.ok = 0;
        task.tmr = task.te;
        if (n != 5) {
            break;
        }
        taskSet->tasks[taskSet->numTasks++] = task;
    }
}

void printTasks(TaskSet* taskSet) {
    printf("Task\tphi\tp\te\td\tutil\ttmr\tok\n");
    float util = 0;
    for (int i = 0; i < taskSet->numTasks; i++) {
        util = taskUtil(taskSet->tasks[i]);
        printf("T(%03d)\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%01d\n", taskSet->tasks[i].id, taskSet->tasks[i].phi, taskSet->tasks[i].tp, taskSet->tasks[i].te, taskSet->tasks[i].td, util, taskSet->tasks[i].tmr, taskSet->tasks[i].ok);
    }
}

// smaller period first
int compareTasksPeriod(const void* a, const void* b) {
    Task* task1 = (Task*)a;
    Task* task2 = (Task*)b;
    if (task1->tp < task2->tp) return -1;
    if (task1->tp > task2->tp) return 1;
    return 0;
}

void sortTasks(TaskSet* taskSet) {
    qsort(taskSet->tasks, taskSet->numTasks, sizeof(Task), compareTasksPeriod);
}

void calculateTmr(TaskSet* taskSet, int index) {
    float te = taskSet->tasks[index].te;
    float tp = taskSet->tasks[index].tp;
    taskSet->tasks[index].tmr = te;
    float currentTmr = taskSet->tasks[index].tmr;
    float newTmr;
    while (1) {
        newTmr = te;
        for (int i = 0; i < index; i++) {
            // += math.ceil(current tmr / task[i].tp) * task[i].tmr
            newTmr += ceil(currentTmr / taskSet->tasks[i].tp) * taskSet->tasks[i].te;
        }
        if (newTmr <= currentTmr) {
            taskSet->tasks[index].tmr = newTmr;
            taskSet->tasks[index].ok = 1;
            break;
        }
        if (newTmr > tp) {
            taskSet->tasks[index].tmr = newTmr;
            taskSet->tasks[index].ok = 0;
            break;
        }
        currentTmr = newTmr;
    }
    return;
}

void calculateTmrForAllTasks(TaskSet* taskSet) {
    for (int i = 0; i < taskSet->numTasks; i++) {
        calculateTmr(taskSet, i);
    }
}

int main() {
    TaskSet* taskSet = (TaskSet*)malloc(sizeof(TaskSet));
    taskSet->tasks = (Task*)malloc(sizeof(Task) * 100);
    taskSet->numTasks = 0;
    taskSet->size = 100;
    readTasks(taskSet);
    sortTasks(taskSet);
    calculateTmrForAllTasks(taskSet);
    printTasks(taskSet);

    free(taskSet->tasks);
    free(taskSet);
    return 0;
}