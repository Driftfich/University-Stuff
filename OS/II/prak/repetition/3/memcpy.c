#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <src> <dst>", argv[0]);
        exit(1);
    }

    char *srcPath = argv[1];
    char *dstPath = argv[2];

    if (srcPath == NULL || dstPath == NULL || srcPath[0] == '\0' || dstPath[0] == '\0') {
        printf("Empty src Path or empty dst Path\nUsage: %s <src> <dst>", argv[0]);
        exit(1);
    }

    int srcFD = open(srcPath, O_RDONLY);

    if (!srcFD) {
        puts("Cannot open source File in Readonly mode");
        exit(1);
    }

    int dstFD = open(dstPath, O_CREAT | O_RDWR | O_TRUNC, 0644);

    if (!dstFD) {
        puts("Cannot open dst File in read&write mode");
        close(srcFD);
        exit(1);
    }

    struct stat srcInfo;
    if (fstat(srcFD, &srcInfo) == -1) {
        puts("Unable to retrieve stats for src file");
        close(srcFD);
        close(dstFD);
        exit(1);
    }

    if (!S_ISREG(srcInfo.st_mode)) {
        fprintf(stderr, "Error: Source is not a regular file\n");
        close(srcFD);
        close(dstFD);
        exit(EXIT_FAILURE);
    }

    off_t srcSize = srcInfo.st_size;
    
    if (srcSize == 0) {
        return EXIT_SUCCESS;
    }

    if (ftruncate(dstFD, srcSize) == -1) {
        fprintf(stderr, "Error: Setting dst File size to src File size\n");
        close(srcFD);
        close(dstFD);
        exit(EXIT_FAILURE);
    }

    void *srcMap = mmap(NULL, srcSize, PROT_READ, MAP_SHARED, srcFD, 0);
    if (srcMap == MAP_FAILED) {
        perror("Error mapping source file");
        close(srcFD);
        close(dstFD);
        exit(EXIT_FAILURE);
    }

    void *dstMap = mmap(NULL, srcSize, PROT_WRITE, MAP_SHARED, dstFD, 0);
    if (dstMap == MAP_FAILED) {
        perror("Error mapping source file");
        close(srcFD);
        close(dstFD);
        munmap(srcMap, srcSize);
        exit(EXIT_FAILURE);
    }

    memcpy(dstMap, srcMap, srcSize);

    munmap(srcMap, srcSize);
    munmap(dstMap, srcSize);
    close(srcFD);
    close(dstFD);

    return EXIT_SUCCESS;
}