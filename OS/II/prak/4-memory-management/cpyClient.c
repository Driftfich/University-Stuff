#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "mq.h"

int main() {
    clientProcess(stdoutClientCallback);
    return 0;
}