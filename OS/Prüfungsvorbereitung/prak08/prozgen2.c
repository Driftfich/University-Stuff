#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void proz_gen(int depth, int max_depth) {
    if (depth <= max_depth) {
        pid_t ret = fork();
        depth++;
        if (ret == 0) {
            proz_gen(depth, max_depth);
            exit(0);
        }
        else if (ret == -1) {
            printf("Error creating %d generation\n", depth);
        }
        wait(NULL);
        printf("%d\n", depth);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <generation_depth>\n", argv[0]);
        exit(1);
    }
    unsigned int generation_depth;
    char *conv_error;

    generation_depth = strtoul(argv[1], &conv_error, 10);

    if (*conv_error != '\0') {
        printf("Werent able to parse argument %s to integer\n", argv[1]);
        exit(2);
    }
    printf("Generating %d generations\n", generation_depth);
    proz_gen(0, generation_depth);
    exit(0);
}
