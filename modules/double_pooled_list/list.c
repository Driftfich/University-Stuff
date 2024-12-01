#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include "list.h"

tPool *create_pool(tList *list) {
    tPool *pool = (tPool*) malloc(sizeof(tPool));
    if (!pool) return NULL;

    pool->nodes = (tNode*) malloc(list->pool_amount * sizeof(tNode));

    if (!pool->nodes) {
        free(pool);
        return NULL;
    }

    pool->capacity = list->pool_amount;
    pool->used = 0;

    return pool;
}

tList* list_create(unsigned int pool_size) {
    tList *list = (tList*) malloc(sizeof(tList));
    if (!list) return NULL;

    unsigned int pool_size = pool_size;
    list->pool_size = 1;
    tPool *pool = create_pool(list);
    if (!pool) {
        free(list);
        return NULL;
    }

    list->head = pool;
    list->curpos = 
}






