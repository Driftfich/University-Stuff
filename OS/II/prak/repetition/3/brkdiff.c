#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

int main() {
    void *initBrk = sbrk(0);
    size_t step = 1024 * 1024;
    size_t totalAllocated = 0;
    
    while (1) {
        if (sbrk(step) == (void *) -1) {
            if (step <= 1) break;
            step /= 2;
        } else {
            totalAllocated += step;
            step *= 2;
        }
    }

    printf("Maximaler Zuwachs in MB: %.2f", (double) totalAllocated / (1024 * 1024 ));
}