#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <limits.h>

int main(int argc, char *argv[]) {
    char path[PATH_MAX];
    if (argc < 2) {
        printf("Please enter a path name: ");
        if (!fgets(path, sizeof(path), stdin)) {
            printf("Error entering path. Exit");
            exit(1);
        }
        path[strcspn(path, "\n")] = '\0';
    }
    else {
        // path=argv[1];
        strncpy(path, argv[1], sizeof(path));
        path[sizeof(path)-1] = '\0';
    }
    printf("Given path: %s\n", path);
    char buf[PATH_MAX];

    DIR *dir = opendir(path);
    if (!dir) exit(1);

    struct dirent *file;
    long size=0;
    struct stat inf;

    while ((file = readdir(dir)) != NULL) {
        if (file->d_type!=DT_REG) {
            continue;}
        // size+=file->d_reclen;
        snprintf(buf, PATH_MAX, "%s/%s", path, file->d_name);
        if (stat(buf, &inf) == 0) {
            size+=inf.st_size;
        }
        else printf("Error loading stats for file: %s\n", buf);
    }

    closedir(dir);
    printf("Size of all files in current dir: %ld\n bytes", size);
    return 0;
}