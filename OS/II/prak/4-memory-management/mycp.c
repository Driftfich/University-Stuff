#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <source> <destination>\n", argv[0]);
        exit(1);
    }

    char *srcPath = argv[1];
    char *dstPath = argv[2];

    if (srcPath == NULL || dstPath == NULL) {
        printf("Error: Source or destination path is NULL\n");
        exit(1);
    }

    if (srcPath[0] == '\0' || dstPath[0] == '\0') {
        printf("Error: Source or destination path is empty\n");
        exit(1);
    }
    
    int srcFd = open(srcPath, O_RDONLY);
    if (srcFd == -1) {
        printf("Error: Failed to open source file %s\n", srcPath);
        exit(1);
    }

    struct stat srcInfo;
    if (fstat(srcFd, &srcInfo) == -1) {
        perror("Error getting source file info");
        close(srcFd);
        exit(EXIT_FAILURE);
    }

    if (!S_ISREG(srcInfo.st_mode)) {
        fprintf(stderr, "Error: Source is not a regular file\n");
        close(srcFd);
        exit(EXIT_FAILURE);
    }

    off_t srcSize = srcInfo.st_size;

    if (srcSize == 0) {
        // Handle empty file
        int dstFd = open(dstPath, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (dstFd == -1) {
            perror("Error creating destination file");
            close(srcFd);
            exit(EXIT_FAILURE);
        }
        close(srcFd);
        close(dstFd);
        return EXIT_SUCCESS;
    }

    int dstFd = open(dstPath, O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (dstFd == -1) {
        perror("Error creating destination file");
        close(srcFd);
        exit(EXIT_FAILURE);
    }

    if (ftruncate(dstFd, srcSize) == -1) {
        perror("Error setting destination file size");
        close(srcFd);
        close(dstFd);
        exit(EXIT_FAILURE);
    }

    // map source file to memory
    void *srcMap = mmap(NULL, srcSize, PROT_READ, MAP_SHARED, srcFd, 0);
    if (srcMap == MAP_FAILED) {
        perror("Error mapping source file");
        close(srcFd);
        close(dstFd);
        exit(EXIT_FAILURE);
    }

    void *dstMap = mmap(NULL, srcSize, PROT_WRITE, MAP_SHARED, dstFd, 0);
    if (dstMap == MAP_FAILED) {
        perror("Error mapping destination file");
        munmap(srcMap, srcSize);
        close(srcFd);
        close(dstFd);
        exit(EXIT_FAILURE);
    }

    // copy data from source to destination
    memcpy(dstMap, srcMap, srcSize);

    // unmap source file from memory
    if (munmap(srcMap, srcSize) == -1) {
        printf("Failed to unmap source file from memory\n");
        exit(1);
    }

    // unmap destination file from memory
    if (munmap(dstMap, srcSize) == -1) {
        printf("Failed to unmap destination file from memory\n");
        exit(1);
    }

    close(srcFd);
    close(dstFd);

    return EXIT_SUCCESS;
}