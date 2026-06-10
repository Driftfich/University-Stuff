#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/xattr.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char* filename = argv[1];
    // get all xattrs
    ssize_t size = llistxattr(filename, NULL, 0);
    if (size == -1) {
        perror("llistxattr failed");
        exit(EXIT_FAILURE);
    }

    char* list = malloc(size);
    if (list == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    
    if (llistxattr(filename, list, size) == -1) {
        perror("llistxattr failed");
        free(list);
        exit(EXIT_FAILURE);
    }

    printf("Xattrs: %s\n", list);

    for (ssize_t i = 0; i < size; i++) {
        // get the xattr value
        ssize_t valueSize = getxattr(filename, list + i, NULL, 0);
        if (valueSize == -1) {
            perror("getxattr failed");
            exit(EXIT_FAILURE);
        }
        char* value = malloc(valueSize);
        if (value == NULL) {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }
        if (getxattr(filename, list + i, value, valueSize) == -1) {
            perror("getxattr failed");
            free(value);
            exit(EXIT_FAILURE);
        }
        printf("Xattr %s: %s\n", list + i, value);
        free(value);
    }
    free(list);
    return EXIT_SUCCESS;
}