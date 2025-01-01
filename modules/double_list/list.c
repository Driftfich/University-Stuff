#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
// #include <sqlite3.h>

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
// abstract forwdatad move of the position pointer in the list
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


// GET METHODS
// get data at current position pointer
int* _get(tList *list) {
    return list->curpos->data;
}

// get data at specifc idx
int* get(tList *list, int idx) {
    _move_index(list, idx);
    return _get(list);
}

// int get_batch(tList *list, int stdatat, int end) {

// }


// DELETION METHODS
// delete the current selected (current position pointer) node (+data)
int delete_node(tList *list) {
    if (!list || !list->curpos || list->curpos == list->head) return 0;
    
    tNode *tmp = list->curpos;
    tmp->nxt->prv = tmp->prv;
    tmp->prv->nxt = tmp->nxt;
    list->curpos = tmp->nxt;
    if (list->curpos == list->head) {
        list->curpos = list->head->nxt;
    }
    
    free(tmp->data);
    free(tmp);
    list->length--;
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

void** to_datarray(tList *list) {
    if (!list || !list->head) {
        fprintf(stderr, "Error: List is NULL\n");
        return NULL;
    }
    
    void **array = malloc(list->length * sizeof(list->head->nxt->data));
    // void *array = malloc(list->length * sizeof(list->head->nxt->data));
    if (!array) {
        fprintf(stderr, "Error allocating memory for array\n");
        return NULL;
    }

    tNode *tmp = list->head->nxt;
    for (int i = 0; i < list->length; i++) {
        array[i] = tmp->data;
        tmp = tmp->nxt;
    }

    return array;
}

tList *from_datarray(void **dataray, int size) {
    tList *list = list_create();
    if (!list || !list->head) {
        fprintf(stderr, "Error creating list\n");
        return NULL;
    }
    
    for (int i = 0; i < size; i++) {
        insert_tail(list, dataray[i]);
    }

    return list;
}

int to_file(tList *list, char *filename, char *delimiter, char *mode, void (*item_saver)(FILE *file, void *item, char *delimiter)) {
    if (!list || !list->head) return 0;

    FILE *file = fopen(filename, mode);
    if (!file) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return 0;
    }

    tNode *tmp = list->head->nxt;
    while (tmp != list->head) {
        void *item = tmp->data;
        item_saver(file, item, delimiter);
        tmp = tmp->nxt;
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

    // char name[200], author[200], borrower[200];
    // unsigned long borrowed_date;
    // int ret = 0;

    // Erste Zuweisung vor der Schleife
    // char format[256] = {0};
    // snprintf(format, 256, "%%[^%s]%s%%[^%s]%s%%[^%s]%s%%lu\n", delimiter, delimiter, delimiter, delimiter, delimiter, delimiter);
    // printf("Format: %s\n", format);
    // ret = fscanf(file, format,
    //              name, author, borrower, &borrowed_date);
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

tList *search(tList *list, int (*cmp)(const void*, const void*), const void *data) {
    // input list: tList to search in, compare function: int (*cmp) (void*, void*), data: const void* to compare with
    // output: tList with found elements
    if (!list || !list->head || !cmp) return NULL;
    
    tList *found = list_create();
    if (!found) return NULL;

    tNode *ftmp = list->head->nxt;
    tNode *ptmp = list->head->prv;
    for (int i=0; i<(list->length + 1)/2; i++) {
        if (cmp(&(ftmp->data), &data) == 0) {
            insert_tail(found, ftmp->data);
        }

        if ((ftmp != ptmp) && (cmp(&(ptmp->data), &data) == 0)) {
            insert_tail(found, ptmp->data);
        }
        ftmp = ftmp->nxt;
        ptmp = ptmp->prv;
    }

    return found;
}

// tList *sort(tList *list, int (*cmp) (const void*, const void*)) {
//     // input list: tList to sort, compare function: int (*cmp) (void*, void*)
//     // output: tList with sorted elements
//     if (!list || !list->head || !cmp) return NULL;
//     int length = list->length;
//     // create a array 
//     void **array = to_datarray(list);
//     list_destroy(list);
//     if (!array) return NULL;

//     // sort the array using qsort
//     qsort(array, length, sizeof(void *), cmp);

//     // create a new list from the sorted array
//     tList *new_list = from_datarray(array, length);

//     // free the array
//     free(array);

//     return new_list;
// }

tList *sort(tList *list, int (*cmp)(const void *c1, const void*c2)) {
    // bubble sort
    if (!list || !list->head || !cmp) return NULL;

    tNode *tmp = list->head->nxt;
    tNode *tmp2 = list->head->nxt;

    for (int i = 0; i < list->length; i++) {
        for (int j = 0; j < list->length - i - 1; j++) {
            if (cmp(tmp->data, tmp2->data) > 0) {
                void *tmp_data = tmp->data;
                tmp->data = tmp2->data;
                tmp2->data = tmp_data;
            }
            tmp2 = tmp2->nxt;
        }
        tmp = tmp->nxt;
        tmp2 = list->head->nxt;
    }    
}







