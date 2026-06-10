#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char* filename = argv[1];
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    write(fd, "Hello, World!", 13);

    // seek 1024 bytes past end of file
    lseek(fd, 1024, SEEK_END);
    write(fd, "Hello, World!", 13);
    close(fd);

    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    char buffer[4096];
    ssize_t n = read(fd, buffer, sizeof(buffer) - 1);
    if (n == -1) {
        perror("read failed");
        close(fd);
        exit(EXIT_FAILURE);
    }
    buffer[n] = '\0';
    
    printf("Read %zd bytes\n", n);
    for (ssize_t i = 0; i < n; i++) {
        if (buffer[i] >= 32 && buffer[i] <= 126)
            putchar(buffer[i]);
        else
            printf("[%02x]", (unsigned char)buffer[i]);
    }
    putchar('\n');

    close(fd);

    return EXIT_SUCCESS;
}
