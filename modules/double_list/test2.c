#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

// read bib.html file

int main() {
    puts("Content-Type: text/html\r\n\r\n");
    FILE *file = fopen("./bib.html", "r");
    if (!file) {
        printf("Error opening file from path: %s", "/home/USER/Dokumente/Github/C-Programming/modules/double_list/bib.html");
        return 1;
    }


    char buf[2048];
    while (fgets(buf, sizeof(buf), file)) {
        printf("%s", buf);
    }

    fclose(file);
    return 0;
}