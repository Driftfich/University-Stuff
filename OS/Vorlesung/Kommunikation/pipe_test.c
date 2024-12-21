#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    char message[] = "Hello World my child!";

    int p[2];
    if (pipe(p) == -1) {
        perror("pipe");
        exit(1);
    }

    if (fork() == 0) {
        close(p[0]);
        write(p[1], message, sizeof(message));
        close(p[1]);
    }
    else {
        close(p[1]);
        char buffer[100];
        read(p[0], buffer, sizeof(buffer));
        printf("Received: %s\n", buffer);
        close(p[0]);
    }

    return 0;
}