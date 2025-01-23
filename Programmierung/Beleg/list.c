#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stddef.h>

#include "list.h"

tList* list_create() {
    tList *list = (tList*) malloc(sizeof(tList));
    if (!list) return NULL;
    
    tNode *head = (tNode*) malloc(sizeof(tNode));
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

int _check_null(tList *list){
    if (!list) {
        printf("Error: List is NULL.\n");
        return 0;
    } else if (!list->head) {
        printf("Error: List head is NULL.\n");
        return 0;
    } else if (!list->curpos) {
        printf("Error: No current position set in the list");
        return 0;
    }
    else return 1;
}


// ABSTRACT CURRENT POSITION POINTER METHODS
// abstract forward move of the position pointer in the list
int _mov_nxt_idx(tList *list) {
    if (!list || !list->curpos) return 0;
    list->curpos = list->curpos->nxt;
    return 1;
}

// abstract backwdatad move of the position pointer in the list
int _mov_prv_idx(tList *list) {
    if (!list || !list->curpos) return 0;
    list->curpos = list->curpos->prv;
    return 1;
}

// get current selected data index
int _get_idx(tList *list) {
    if (!list || !list->head) return 0;
    tNode *tmp_sel = list->head->nxt;
    int idx=0;
    while ((tmp_sel != list->head) && (tmp_sel != list->curpos)) {
        idx++;
    }
    return idx;
}

// move the position pointer to a specific index
int _move_index(tList *list, int index) {
    if (_check_null(list) == 0){
        return 0;
    }
    else if (index < 0) {
        printf("Error: Index is negative.\n");
        return 0;
    } else if (index >= list->length) {
        printf("Error: Index is out of bounds.\n");
        return 0;
    }
    if (index < list->length / 2) {
        list->curpos = list->head->nxt;
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


// INSERTION METHODS
// insert new data element behind the current position pointer
int _insert_behind(tList *list, void *data) {
    if (!list || !list->curpos) return 0;
    
    tNode *new = (tNode*) malloc(sizeof(tNode));
    if (!new) return 0;
    
    new->data = data;
    new->nxt = list->curpos->nxt;
    new->prv = list->curpos;

    new->nxt->prv = new;
    list->curpos->nxt = new;

    list->length++;
    return 1;
}

// insert data at the bottom of the list
int insert_tail(tList *list, void *data) {
    list->curpos = list->head->prv;
    return _insert_behind(list, data);
}

// insert data at the head of the list
int insert_head(tList *list, void *data) {
    list->curpos = list->head;
    return _insert_behind(list, data);
}

int insert(tList *list, void *data, int idx) {
    if (_move_index(list, idx-1) == 0) {
        return 0;
    }
    return _insert_behind(list, data);
}


// DELETION METHODS
// delete the current selected (current position pointer) node (+data)
tNode *_disconnect_node(tList *list) {
    tNode *tmp = list->curpos;
    tmp->nxt->prv = tmp->prv;
    tmp->prv->nxt = tmp->nxt;

    return tmp;
}

int delete_node(tList *list) {
    if (!list || !list->curpos || list->curpos == list->head) return 0;
    
    tNode *tmp = _disconnect_node(list);
    
    free(tmp->data);
    free(tmp);
    list->length--;

    list->curpos = list->head->nxt;
    return 1;
}

// remove the whole list, its nodes with the corresponding data from the memory
void list_destroy(tList *list) {
    if (!list || !list->head) return;
    
    list->curpos = list->head->nxt;
    while (list->length > 0) {
        delete_node(list);
    }
    
    free(list->head);
    free(list);
}


// Utility METHODS
// print out the whole list
void print_list(tList *list, void (*printer) (void *data)) {
    if (!list || !list->head) return;
    
    tNode *tmp = list->head->nxt;
    printf("List([\n");
    while (tmp != list->head) {
        printer(tmp->data);
        tmp = tmp->nxt;
    }
    printf("])\n");
}

int to_file(tList *list, char *filename, char *delimiter, char *mode, void (*item_saver)(FILE *file, void *item, char *delimiter)) {
    if (!list || !list->head) return 0;

    FILE *file = fopen(filename, mode);
    if (!file) {
        printf("Error opening file %s\n", filename);
        return 0;
    }

    if (list->length == 0) {
        fprintf(file, "");
    }
    else {
        tNode *tmp = list->head->nxt;
        while (tmp != list->head) {
            void *item = tmp->data;
            item_saver(file, item, delimiter);
            tmp = tmp->nxt;
        }
    }

    fclose(file);
    return 1;
}

tList *from_file(char *filename, char *delimiter, void *(*item_loader)(FILE *file, char *delimiter)) {
    tList *list = list_create();
    if (!list) return NULL;

    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return NULL;
    }

    while (!feof(file)) {
        void *item = item_loader(file, delimiter);
        if (!item) {
            fprintf(stderr, "Error loading item from file\n");
            continue;
        }
        insert_tail(list, item);
    }

    fclose(file);
    return list;
}

tList *search(tList *list, int (*cmp)(const void*c1, const void*c2), const void *data) {
    if (!list || !list->head || !cmp) return NULL;
    
    // Input list becomes rest list and found list is returned
    tList *found = list_create();
    if (!found) return NULL;

    list->curpos = list->head->nxt;
    while (list->curpos != list->head) {
        if (!list->curpos->data) {
            printf("Error: Data is NULL\n");
            continue;
        }
        if (cmp(list->curpos->data, data) == 0) {
            tNode *tmp = _disconnect_node(list);
            list->curpos = tmp->nxt;
            list->length--;
            insert_tail(found, tmp->data);
            free(tmp);
        }
        else {
            list->curpos = list->curpos->nxt;
        }
    }
    list->curpos = list->head->nxt;
    
    return found;
}

tList *sort(tList *list, int (*cmp)(const void *c1, const void*c2)) { // 
    if (!list || !list->head || !cmp || list->length < 2) return list;

    tNode *t1 = list->head->nxt;
    tNode *t2 = list->head->nxt;

    for (int i=0; i < list->length -1; i++) {
        for (int j=0; j < list->length - 1 - i; j++) {
            if (cmp(t2->data, t2->nxt->data) > 0) {
                void *tmp_data = t2->data;
                t2->data = t2->nxt->data;
                t2->nxt->data = tmp_data;
            }
            t2 = t2->nxt;
        }
        t1 = t1->nxt;
        t2 = list->head->nxt;
    }

    return list;
}

tList* concat_lists(tList *list1, tList *list2) {
    if (!list1 || !list2) return NULL;
    if (!list1->head || !list2->head) return NULL;

    if (list1->length == 0) {
        list_destroy(list1);
        return list2;
    }

    if (list2->length == 0) {
        list_destroy(list2);
        return list1;
    }
    
    // Verbinde letztes Element von list1 mit erstem von list2
    list1->head->prv->nxt = list2->head->nxt;
    list2->head->nxt->prv = list1->head->prv;
    
    // Verbinde letztes Element von list2 mit head von list1
    list2->head->prv->nxt = list1->head;
    list1->head->prv = list2->head->prv;
    
    // Aktualisiere Länge
    list1->length += list2->length;
    
    // Speicher von list2 head freigeben
    free(list2->head);
    free(list2);
    
    return list1;
}







