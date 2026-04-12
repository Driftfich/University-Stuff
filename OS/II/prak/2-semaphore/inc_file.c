#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "inc_file.h"


/**
 * Opens the file from given path, uses a buffer of size buf_size to read the file into and increments the number by n.
 * 
 * @param path The path to the file to increment.
 * @param n The number to increment by.
 * @param buf_size The size of the buffer to read the file into. Must be bigger than the maximum number of digits estimated in the file.
 * @return 0 on success, positive number on error.
 */
int inc_file(char *path, int n, int buf_size) {
    // open the file
    FILE *file = fopen(path, "r+");
    if (!file) {
        return 1;
    }

    // read the file into a buffer
    char buffer[buf_size];
    long num = 0;
    int bytes_read = fread(buffer, 1, buf_size, file);
    if (bytes_read == 0) {
        buffer[0] = '0';
        bytes_read = 1;
    }

    buffer[bytes_read] = '\0';

    // increment the number in the buffer
    num = atol(buffer);
    num += n;

    rewind(file);

    // delete file content
    ftruncate(fileno(file), 0);
    fflush(file);
    rewind(file);

    fprintf(file, "%ld", num);
    fclose(file);
    return 0;
}