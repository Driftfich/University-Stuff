#ifndef TIMESEG_H
#define TIMESEG_H

#define TIMEKEY "/timenote"

int openSharedMemory(char *path);
void *mapSharedMemory(int shfd, size_t size);

#endif