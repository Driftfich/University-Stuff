#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

typedef struct tFileStats {
    int binSize;
    int binCount;
    int* fileBins;
} FileStats;

FileStats* initFileStats(int binSize, int initalBinCount) {
    FileStats* stats = malloc(sizeof(FileStats));
    if (stats == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    stats->binSize = binSize;
    stats->binCount = initalBinCount;
    stats->fileBins = malloc(initalBinCount * sizeof(int));
    for (int i = 0; i < initalBinCount; i++) {
        stats->fileBins[i] = 0;
    }
    return stats;
}

void freeFileStats(FileStats* stats) {
    free(stats->fileBins);
    free(stats);
}

void increaseFileStats(FileStats* stats, int minBinCount) {
    // round up the binCount to the next base of 2
    int oldBinCount = stats->binCount;
    int newBinCount = oldBinCount;
    while (newBinCount < minBinCount) {
        newBinCount *= 2;
    }
    stats->fileBins = realloc(stats->fileBins, newBinCount * sizeof(int));
    if (stats->fileBins == NULL) {
        perror("realloc failed");
        exit(EXIT_FAILURE);
    }
    stats->binCount = newBinCount;
    for (int i = oldBinCount; i < newBinCount; i++) {
        stats->fileBins[i] = 0;
    }
}

void registerFileInStats(FileStats* stats, char* filePath) {
    // get the size of the file
    struct stat st;
    if (stat(filePath, &st) == 0) {
        int size = st.st_size;
        // find the bin for the size
        int bin = size / stats->binSize;

        if (bin >= stats->binCount) {
            increaseFileStats(stats, bin + 1);
        }

        stats->fileBins[bin]++;
    } else {
        perror("stat failed");
    }
}

void createFileStats(FileStats* stats, char* basePath, int maxDepth) {
    DIR* dir = opendir(basePath);
    if (dir == NULL) {
        perror("opendir failed");
        exit(EXIT_FAILURE);
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        char* fullPath = malloc(strlen(basePath) + strlen(entry->d_name) + 2);
        if (fullPath == NULL) {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }
        snprintf(fullPath, strlen(basePath) + strlen(entry->d_name) + 2, "%s/%s", basePath, entry->d_name);

        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
                free(fullPath);
            }
            if (maxDepth == -1 || maxDepth > 0) {
                int nextDepth = maxDepth == -1 ? -1 : maxDepth - 1;
                createFileStats(stats, fullPath, nextDepth);
            }

        } else if (entry->d_type == DT_REG) {
            registerFileInStats(stats, fullPath);
        }
        free(fullPath);
    }

    closedir(dir);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <base path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char* basePath = argv[1];
    int maxDepth = -1;

    FileStats* stats = initFileStats(1024, 1024);
    createFileStats(stats, basePath, maxDepth);

    for (int i = 0; i < stats->binCount; i++) {
        if (stats->fileBins[i] > 0) {
            printf("Bin %d (%d - %d): %d files\n", i, i * stats->binSize, (i + 1) * stats->binSize - 1, stats->fileBins[i]);
        }
    }

    freeFileStats(stats);
    return EXIT_SUCCESS;
}