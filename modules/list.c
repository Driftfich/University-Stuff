#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include "list.h"
#include "list.c"

typedef struct tConnector {
    struct tConnector *nxt;
    struct tConnector *prv;
    void *data;
} tConct;

typedef struct tList {
    tConct *head;
    tConct *curpos;
    int length;
} tList;

tList* list_create() {
    tList *list = (tList*) malloc(sizeof(tList));
    if (!list) return NULL;
    
    tConct *head = (tConct*) malloc(sizeof(tConct));
    if (!head) {
        free(list);
        return NULL;
    }
    
    head->nxt = head;
    head->prv = head;
    head->data = NULL;
    
    list->head = head;
    list->curpos = head;
    list->length = 0;
    return list;
}

int _move_index(tList *list, int index) {
    if (!list || !list->head || index < 0 || index >= list->length) {
        return 0;
    }
    if (index < list->length / 2) {
        list->curpos = list->head;
        for (int i = 0; i < index; i++) {
            list->curpos = list->curpos->nxt;
        }
    } else {
        list->curpos = list->head->prv;
        for (int i = list->length - 1; i > index; i--) {
            list->curpos = list->curpos->prv;
        }
    }
    return 1;
}

int _mov_nxt(tList *list) {
    if (!list || !list->curpos) return 0;
    list->curpos = list->curpos->nxt;
    return 1;
}

int _mov_prv(tList *list) {
    if (!list || !list->curpos) return 0;
    list->curpos = list->curpos->prv;
    return 1;
}

int insert_behind(tList *list, void *data) {
    if (!list || !list->curpos) return 0;
    
    tConct *new = (tConct*) malloc(sizeof(tConct));
    if (!new) return 0;
    
    new->data = data;
    new->nxt = list->curpos->nxt;
    new->prv = list->curpos;
    list->curpos->nxt->prv = new;
    list->curpos->nxt = new;
    list->length++;
    return 1;
}

int insert_tail(tList *list, void *data) {
    list->curpos = list->head->prv;
    return insert_behind(list, data);
}

int insert_head(tList *list, void *data) {
    list->curpos = list->head;
    return insert_behind(list, data);
}

int delete_node(tList *list) {
    if (!list || !list->curpos || list->curpos == list->head) return 0;
    
    tConct *tmp = list->curpos;
    tmp->nxt->prv = tmp->prv;
    tmp->prv->nxt = tmp->nxt;
    list->curpos = tmp->nxt;
    if (list->curpos == list->head) {
        list->curpos = list->head->nxt;
    }
    
    free(tmp);
    list->length--;
    return 1;
}

void list_print(tList *list) {
    if (!list || !list->head) return;
    
    tConct *tmp = list->head->nxt;
    while (tmp != list->head) {
        printf("%p\n", tmp->data);
        tmp = tmp->nxt;
    }
}

void list_destroy(tList *list) {
    if (!list || !list->head) return;
    
    list->curpos = list->head->nxt;
    while (list->length > 0) {
        delete_node(list);
    }
    
    free(list->head);
    free(list);
}


