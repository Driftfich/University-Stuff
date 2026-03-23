#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

void siguser_handler(int c) {
    puts("SIGUSR1 received.");

    if (signal(SIGUSR1, SIG_DFL) == SIG_ERR) {
        puts("Adding the default handler wasnt succesful.\n");
        exit(2);
    }
    return;
}

int main() {
    int kret, sret;
    if (signal(SIGUSR1, &siguser_handler) == SIG_ERR) {
        puts("Adding the custom SIGUSR1 handler wasnt succesful.\n");
        exit(1);
    }
    kret = kill(getpid(), SIGUSR1);
    if (kret != 0) {
        puts("Sending the SIGUSR1 Signal wasnt succesful.\n");
    }
    kret = kill(getpid(), SIGUSR1);
    if (kret != 0) {
        puts("Sending the SIGUSR1 Signal wasnt succesful.\n");
    }

    return 0;
}
