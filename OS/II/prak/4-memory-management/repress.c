#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main() {
    // allocate large block of heap memory in write mode
    long pageSize = sysconf(_SC_PAGESIZE);
    long size = 1024 * 1024 * 1024; // 1GB
    size *= 9; // 10GB
    long pages = (size + pageSize -1) / pageSize;
    int childCount = 100, refCount = 100; // 100 children and 100 references from each child

    // void *ptr = malloc(size);
    // if (ptr == NULL) {
    //     printf("Error: Failed to allocate memory\n");
    //     exit(1);
    // }

    void *ptr;
    if (posix_memalign(&ptr, pageSize, size) != 0) {
        perror("Error: Failed to allocate aligned memory");
        exit(1);
    }

    // father init memory
    printf("Father init memory\n");
    memset(ptr, 'f', size);

    // create child processes
    for (int i = 0; i < childCount; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // child process
            printf("Child process %d allocated memory\n", getpid());
            for (int j = 0; j < refCount; j++) {
                memset(ptr, 'c', (int)(size)); //  / refCount
                sleep(0.1);
            }
            exit(0);
        }
    }

    // father process waits for all children to finish
    for (int i = 0; i < childCount; i++) {
        wait(NULL);
    }

    // father process reads out resident set size using mincore()
    unsigned char *vec = malloc(pages);
    if (mincore(ptr, size, vec) == -1) {
        printf("Error: Failed to read out resident set size\n");
        exit(1);
    }
    
    long residentPages = 0;
    for (int i = 0; i < pages; i++) {
        if (vec[i] & 1) {
            residentPages++;
        }
    }
    printf("Total pages: %ld\n", pages);
    printf("Resident pages: %ld\n", residentPages);
    printf("Resident set size (MB): %ld\n", residentPages * pageSize / 1024 / 1024);
    printf("Non Resident page size (MB): %ld\n", (pages - residentPages) * pageSize / 1024 / 1024);

    free(vec);
    free(ptr);
    printf("Memory freed\n");

    return 0;
}