#ifndef TIMESEG_H
#define TIMESEG_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int openSharedMemory(char *path);
void *mapSharedMemory(int shfd, size_t size);
#endif