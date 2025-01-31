#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <limits.h>  // For PATH_MAX

int main() {
    char *path = ".";
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("opendir failed");
        return EXIT_FAILURE;
    }

    struct dirent *file;
    long size = 0;
    char full_path[PATH_MAX];
    struct stat inf;

    while ((file = readdir(dir)) != NULL) {
        if (file->d_type != DT_REG) {
            continue;  // Skip non-regular files
        }

        // Construct the full path
        snprintf(full_path, PATH_MAX, "%s/%s", path, file->d_name);

        // Get file stats
        if (stat(full_path, &inf) == 0) {
            size += inf.st_size;
        } else {
            perror("stat failed");
        }
    }

    closedir(dir);

    printf("Size of all files in current dir: %ld bytes\n", size);
    return EXIT_SUCCESS;
}