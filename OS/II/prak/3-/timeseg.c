#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

int openSharedMemory(char *path) {
    int shfd = shm_open(path, O_CREAT | O_RDWR, 0600);
    if (shfd < 0) exit(EXIT_FAILURE);
    return shfd;
}

void *mapSharedMemory(int shfd, size_t size) {
    ftruncate(shfd, size);
    void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shfd, 0);
    if (ptr == MAP_FAILED) exit(EXIT_FAILURE);
    return ptr;
}
