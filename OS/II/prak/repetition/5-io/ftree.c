#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "ftree.h"

FileStats *initStats(int binSize, int startAmount) {
    FileStats *stat = malloc(sizeof(FileStats));
    if (!stat) {
        puts("Failed to malloc Filestats ds");
        exit(EXIT_FAILURE);
    }

    stat->bins = malloc(sizeof(int) * startAmount);
    stat->binSize = binSize;
    stat->binAmount = startAmount;

    for (int i=0; i<startAmount; i++) stat->bins[i] = 0;

    return stat;
}

void clearStats(FileStats *stat) {
    free(stat->bins);
    free(stat);
}

void incStatsBinAmount(FileStats *stat, int minBinAmount) {
    int newBinAmount = stat->binAmount;
    while (newBinAmount < minBinAmount) newBinAmount *= 2;

    stat->bins = realloc(stat->bins, newBinAmount * sizeof(int));
    if (!stat->bins) {
        puts("Failed to realloc Filestat bins during size icrement");
        exit(EXIT_FAILURE);
    }

    for (int i=stat->binAmount; i<newBinAmount; i++) {
        stat->bins[i] = 0;
    }
    stat->binAmount = newBinAmount;

    return;
}

void registerInFileStat(FileStats *fstat, char* fullPath) {
    struct stat st;
    if (stat(fullPath, &st) == 0) {
        int size = st.st_size;

        int bin = size / fstat->binSize;

        if (bin > fstat->binAmount) {
            incStatsBinAmount(fstat, bin+1);
        }

        fstat->bins[bin]++;
    } else {
        puts("Failed to increase file Stat");
    }
}

void createFileStat(FileStats *stat, char *basePath, int maxDepth) {

    DIR *dir = opendir(basePath);
    if (!dir) {
        perror("Failed to open dir");
        exit(EXIT_FAILURE);
    }

    struct dirent* entry;
    while((entry = readdir(dir)) != NULL) {
        int fullPathLength = strlen(basePath) + strlen(entry->d_name) + 2;
        char *fullPath = malloc(fullPathLength);
        if (!fullPath) {
            puts("Failed to allocate fullPath String");
            exit(EXIT_FAILURE);
        }

        snprintf(fullPath, fullPathLength, "%s/%s", basePath, entry->d_name);

        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                free(fullPath);
                continue;
            }
            if (maxDepth == -1 || maxDepth > 0) {
                int nextDepth = maxDepth == -1 ? maxDepth : maxDepth - 1;
                createFileStat(stat, fullPath, nextDepth);
            } else {
                free(fullPath);
                printf("Max Depth reached: %d\n", maxDepth);
                return;
            }
        } else if (entry->d_type == DT_REG) {
            registerInFileStat(stat, fullPath);
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

    FileStats* stats = initStats(1024, 1024);
    createFileStat(stats, basePath, maxDepth);

    for (int i = 0; i < stats->binAmount; i++) {
        if (stats->bins[i] > 0) {
            printf("Bin %d (%d - %d): %d files\n", i, i * stats->binSize, (i + 1) * stats->binSize - 1, stats->bins[i]);
        }
    }

    clearStats(stats);
    return EXIT_SUCCESS;
}

