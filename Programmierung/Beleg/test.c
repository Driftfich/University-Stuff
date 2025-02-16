#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

int main() {
    // Liste und Strings erstellen
    tList *list = list_create();
    if (!list) return 1;
    
    for (int i=0; i<10; i++) {
        char *str1 = malloc(sizeof(char) * 5);
        // copy i to str1
        snprintf(str1, 5, "%d", i);
        insert_tail(list, str1);
    }
    _move_index(list, 5);
    tList *found = search(list, (int (*)(const void *, const void *)) strcmp, "5");
    if (!found) return 1;
    printf("Found: %d\n", found->length);
    printf("Item: %s\n", (char *) found->head->nxt->data);
    
    list_destroy(list);
    
    return 0;
}
