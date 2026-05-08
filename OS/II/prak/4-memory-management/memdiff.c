#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>

int main() {
    void *inital_break = sbrk(0);
    size_t step = 1024 * 1024;
    size_t total_allocated = 0;

    printf("Start break: %p\n", inital_break);

    while (1) {
        // return value -1 as pointer if allocation failed
        if (sbrk(step) == (void *) -1) {
            if (step <= 1) {
                break;
            } 
            step /= 2;
        } else {
            total_allocated += step;
            step *= 2;
        }
    }

    printf("Maximaler Zuwachs: %zu Bytes (~%.2f MB)\n", total_allocated, (double) total_allocated / (1024.0 * 1024.0));
    printf("End break: %p\n", sbrk(0));
}