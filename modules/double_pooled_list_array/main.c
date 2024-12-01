#include <stdio.h> 
#include <stdlib.h>
#include <malloc.h>

#include "pooled_list.h"

int main()
{
    int i=0;
    tList *list = create_list(5);
    for (i; i<5; i++) {
        printf("Inserting %d\n", i);
        int *data = malloc(sizeof(int));
        *data = i;
        insert_tail(list, data);
    }
    printf("List([");
    for (i; i<5; i++) {
        // _move_idx(list, i);
        printf("%d, ", list->curpos->ar[i]->data);
        printf("%u, ", list->curnodeidx);
        printf("%d, ", list->curnode->data);
    }
    printf("])");
}