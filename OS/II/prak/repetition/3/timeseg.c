#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>


int openSharedMemory(char *path) {
    int shfd = shm_open(path, O_CREAT | O_RDWR, 0600);
    if (shfd < 0) {
        printf("Error creating shared memory for path: %s\n", path);
        exit(EXIT_FAILURE);
    }
    return shfd;
}

void *mapSharedMemory(int shfd, size_t size) {
    ftruncate(shfd, size);
    void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shfd, 0);
    if (!ptr) {
        printf("Error mapping shared memory segment for shfd %d", shfd);
        exit(EXIT_FAILURE);
    }
    return ptr;
}