#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>

int defended=0;

void handler(int c) {
    defended++;
    return;
}

int nrand(int min, int max) {
    int dist = max - min;
    srand((unsigned int) time(NULL));
    return (rand() % dist) + min;
}

void create_wall() {
    // random signal selektieren welches nicht gehandelt werden soll + SIGKILL + SIGSTOP
    int rand = nrand(1, 31);
    if (rand == 19 || rand == 9 || rand == 17) rand++;

    for (int i=1; i<=31; i++) {
        if (i != 9 && i != 17 && i != 19 && i != rand) {
            if (signal(i, &handler) == SIG_ERR) {
                printf("Creating the wall for Signal %d failed.\n", i);
                exit(1);
            }
        }
    }

    return;
}

int is_alive(pid_t pid) {
    if (kill(pid, 0) == 0) return 1;
    return 0;
}

void kill_process(pid_t pid) {
    int ret;
    for (int i=1; i<=31; i++) {
        if (i != 9 && i != 17 && i != 19) {
            if (kill(pid, i) != 0) {
                printf("Kill signal %d wasnt sent succesfully.\n", i);
                break;
            }
        }

        if (is_alive(pid) == 0) {
            printf("Process with pid %d was killed by signal %d\n", (int) pid, i);
            break;
        }
    }
    return;
}

int main() {
    pid_t res = fork();
    int other_pid = getpid();
    int ret;
    if (res == -1) {
        puts("Creating the child process failed.\n");
        exit(1);
    }
    else if (res == 0) {
        // Sohnprozess
        other_pid = getppid();
        puts("Creating wall for child...\n");
        create_wall();
        printf("Father process has pid %d\n", (int) other_pid);
        sleep(1);
        while (is_alive(other_pid)) {
            kill_process(other_pid);
        }

        printf("Father process got killed.\n");

        exit(0);
    }
    else {
        // Dad Prozess
        other_pid = res;

        puts("Creating wall for father...\n");
        create_wall();
        printf("Son process has pid %d\n", (int) other_pid);
        sleep(1);
        while (is_alive(other_pid)) {
            kill_process(other_pid);
        }

        printf("Child process got killed.\n");

        exit(0);
    }
}
