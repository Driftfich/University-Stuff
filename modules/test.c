#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int main() { 
    int i=0;
    int *data = malloc(sizeof(int));
    tList *list = list_create(); 
    // for (i; i<11; i++) {
    //     *data = i;
    //     insert_tail(list, data);
    // }
    while (list->length > 0) {
        printf("%d", list->curpos);
        delete_node(list);
    }
}
