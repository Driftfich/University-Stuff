#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
    unsigned short s;
    int i;
    char buf[80];

    if(argc < 3){
    fprintf(stderr, "Usage: %s <string> <length-of-string>\n", argv[0]);
    return -1;
    }

    i = atoi(argv[2]);
    s = i;

    if(s >= 80){
    printf("Don't you dare!\n");
    return -1;
    }

    printf("s = %d\n", s);

    memcpy(buf, argv[1], i);
    buf[i] = '\0';
    printf("%s\n", buf);

    return 0;
}
