#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define linesize 128

int main(){
    char buf[linesize + 1];
    char *word;
    const char tok[] = " ";
    int lengths[linesize] = {0};
    int length;

    while (fgets(buf, linesize, stdin) != NULL){
        if (buf[0] == 'q' && buf[1] == 'q') {
            break;
        }

        word = strtok(buf, tok);
        while (word != NULL) {
            length = strlen(word)-1;
            if (length >= 0 && length < linesize) lengths[length] += 1;
            word = strtok(NULL, tok);
        }}

    for (int i=0; i<linesize; i++) {
        if (lengths[i] > 0) {
            printf("%d:\t%d\n", i+1, lengths[i]);
        }
    }
    return 0;
}
