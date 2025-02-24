#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stddef.h>

#include "list.h"
#include "logger.h"
#include "utility.h"

tList* list_create() {
    // allocate memory for the list and its header
    tList *list = (tList*) malloc(sizeof(tList));
    if (!list) {
        DEBUG_STR("Error: Failed to allocate memory for the list body => Failed to create List.\n");
        return NULL;
    }
    
    tNode *head = (tNode*) malloc(sizeof(tNode));
    if (!head) {
        free(list);
        DEBUG_STR("Error: Failed to allocate memory for the header => Failed to create List.\n");
        return NULL;
    }
    // set the header node attributes
    head->nxt = head;
    head->prv = head;
    head->data = NULL;
    
    // set the list attributes
    list->head = head;
    list->curpos = head;
    list->curidx = 0;
    list->length = 0;
    return list;
}

// checks that important pointers for the list are not NULL
int _check_null(tList *list){
    if (!list) {
        printf("Error: List is NULL.\n");
        DEBUG_STR("Error: List is NULL.\n");
        return 0;
    } else if (!list->head) {
        printf("Error: List head is NULL.\n");
        DEBUG_STR("Error: List head is NULL.\n");
        return 0;
    } else if (!list->curpos) {
        printf("Error: No current position set in the list");
        DEBUG_STR("Error: No current position set in the list\n");
        return 0;
    }
    else return 1;
}


// ABSTRACT CURRENT POSITION POINTER METHODS
// resets the curpos and curidx to the top of the list
int _curidx_top(tList *list) {
    if (!list || !list->head) {
        DEBUG_STR("Error: Failed to set the curpos/curidx to top of list. List or List header equals NULL.\n");
        return 0;
    }
    list->curpos = list->head->nxt;
    list->curidx = 1;
    return 1;
}

// resets the curpos and curidx to the end of the list
int _curidx_end(tList *list) {
    if (!list || !list->head) {
        DEBUG_STR("Error: Failed to set the curpos/curidx to end of list. List or List header equals NULL.\n");
        return 0;
    }
    list->curpos = list->head->prv;
    list->curidx = list->length;
    return 1;
}

// abstract forward move of the position pointer in the list
int _mov_nxt_idx(tList *list) {
    if (!list || !list->head) {
        DEBUG_STR("Error: Failed to move curpos to nxt index. List or List header equals NULL.\n");
        return 0;
    }
    list->curpos = list->curpos->nxt;
    list->curidx++;
    if (list->curpos == list->head) list->curidx = 0;
    return 1;
}

// abstract backwdatad move of the position pointer in the list
int _mov_prv_idx(tList *list) {
    if (!list || !list->head) {
        DEBUG_STR("Error: Failed to move the curpos of the list backward. List or List header equals NULL.\n");
        return 0;
    }
    list->curpos = list->curpos->prv;
    list->curidx--;
    if (list->curpos == list->head) list->curidx = 0;
    return 1;
}

// get current selected data index
int _get_curidx(tList *list) {
    if (!list || !list->head) {
        DEBUG_STR("Error: Failed to get current list->curidx. List or List header equals NULL.\n");
        return 0;
    }
    return list->curidx;
}

// move the position pointer to a specific index
int _move_index(tList *list, int index) {
    if (_check_null(list) == 0){
        return 0;
    }
    // Check that index is not out of bounds
    else if (index < 0) {
        printf("Error: Index is negative.\n");
        DEBUG_STR("Error: Index is negative.\n");
        return 0;
    } else if (index >= list->length) {
        printf("Error: Index is out of bounds.\n");
        DEBUG_STR("Error: Index is out of bounds.\n");
        return 0;
    }
    // Calculate the distance between the current selected node and the target node
    int dist = index - list->curidx + 1;
    // move the position pointer to the target node based on the distance
    if (dist >= 0 && dist < list->length / 2) {
        for (int i=0; i<dist; i++) {
            _mov_nxt_idx(list);
        }
    }
    else if (dist < 0 && -dist < list->length / 2) {
        for (int i=dist; i<0; i++) {
            _mov_prv_idx(list);
        }
    }
    else if (index < list->length / 2) {
        list->curpos = list->head->nxt;
        list->curidx = 1;
        for (int i = 0; i < index; i++) {
            _mov_nxt_idx(list);
        }
    } else {
        list->curpos = list->head->prv;
        list->curidx = list->length;
        for (int i = list->length - 1; i > index; i--) {
            _mov_prv_idx(list);
        }
    }
    return 1;
}


// INSERTION METHODS
// insert new data element behind the current position pointer
int _insert_behind(tList *list, void *data) {
    if (!list || !list->curpos) return 0;
    
    // allocate memory for the new node
    tNode *new = (tNode*) malloc(sizeof(tNode));
    if (!new) {
        DEBUG_STR("Error: Failed to insert behind the list.\n");
        return 0;
    }
    // set the new node attributes
    new->data = data;
    new->nxt = list->curpos->nxt;
    new->prv = list->curpos;

    new->nxt->prv = new;
    list->curpos->nxt = new;

    // increase the list length
    list->length++;
    return 1;
}

// insert data at the bottom of the list
int insert_tail(tList *list, void *data) {
    if (_curidx_end(list) == 0) {
        DEBUG_STR("Error: Failed to inset tail.\n");
        return 0;
    }
    return _insert_behind(list, data);
}

// insert data at the head of the list
int insert_head(tList *list, void *data) {
    list->curpos = list->head;
    list->curidx = 0;
    return _insert_behind(list, data);
}

// insert data at a specific index
int insert(tList *list, void *data, int idx) {
    // move the position pointer before the target index and insert the data behind
    if (_move_index(list, idx-1) == 0) {
        DEBUG_STR("Error: Failed to inset new data.\n");
        return 0;
    }
    return _insert_behind(list, data);
}


// DELETION METHODS
// disconnect the current selected node from the list/chain
tNode *_disconnect_node(tList *list) {
    tNode *tmp = list->curpos;
    tmp->nxt->prv = tmp->prv;
    tmp->prv->nxt = tmp->nxt;
    
    return tmp;
}

// delete the current selected (current position pointer) node (+data) and reset the current position pointer to the top of the list
int delete_node(tList *list) {
    if (!list || !list->curpos || list->curpos == list->head) {
        DEBUG_STR("Error: Failed to delete list node. list, list header equals NULL or the current position equals the list head.\n");
        return 0;
    }
    tNode *tmp = _disconnect_node(list);
    
    free(tmp->data);
    free(tmp);
    list->length--;

    if (_curidx_top(list) == 0) {
        DEBUG_STR("Warning: Failed to reset the current position to the top of the list.\n");
        return 0;
    }
    return 1;
}

// Delete nodes with the given ids separated by ~
int delete_ids(tList *list, char *ids) {
    if (!list) {
        error_msg("No list given for deletion.\n");
    }
    if (!ids) {
        DEBUG_STR("Warning: No ids given for deletion. Therefore no items got deleted.\n");
        return 0;
    }

    char *tok = strtok(ids, "~");
    int count = 0;
    while (tok != NULL) {
        int id = atoi(tok);
        // Because deletion begins with smallest id, following ids have to be decremented by the already deleted id count
        id -= count;
        count++;

        if (_move_index(list, id) == 0) {
            error_msg("Failed to move index to the given id.\n");
        }
        if (delete_node(list) == 0) {
            error_msg("Failed to delete node.\n");
        }
        tok = strtok(NULL, "~");
    }

    return 1;
}

// remove the whole list, its nodes with the corresponding data from the memory
void list_destroy(tList *list) {
    if (!list || !list->head) {
        DEBUG_STR("Error: Failed to destroy list. list or list header is NULL.\n");
        return;
    }
    
    _mov_nxt_idx(list);
    while (list->length > 0) {
        delete_node(list);
    }
    
    free(list->head);
    free(list);
    return;
}


// Utility METHODS
// print out the whole list
void print_list(tList *list, void (*printer) (void *data)) {
    if (!list || !list->head) {
        DEBUG_STR("Error: Failed to print out the list. List or List header equals NULL.\n");
        return;
    }
    
    tNode *tmp = list->head->nxt;
    printf("List([\n");
    while (tmp != list->head) {
        printer(tmp->data);
        tmp = tmp->nxt;
        printf(", ");
    }
    printf("])\n");
}

// Create a pointer array from the list
void **to_arr(tList *list) {
    // if list->length <= 0, a empty array is returned
    if (!list || !list->head) {
        DEBUG_STR("Error: Failed to convert the list to array. List or List header equals NULL.\n");
        return NULL;
    }
    // allocate memory for the pointer array
    void **arr = malloc(sizeof(void *) * list->length);
    if (!arr) {
        DEBUG_STR("Error: Failed to convert the list to array. Failed to allocate the memory for the pointer array.\n");
        return NULL;
    }
    // Iterate over the list and copy the data pointers to the array
    tNode *current = list->head->nxt;
    for (int i=0; i<list->length && current != list->head; i++) {
        arr[i] = current->data;
        current = current->nxt;
    }

    return arr;
}

// create a list from a pointer array
tList *from_arr(void **arr, int arr_length) {
    if (!arr) {
        DEBUG_STR("Error: Failed to create list from array. Input array is NULL.\n");
        return NULL;
    }
    // create a new list
    tList *list = list_create();
    if (!list) {
        DEBUG_STR("Error: Failed to create list from array. Failed to create list.\n");
        return NULL;
    }
    // iterate over the array and insert each element into the list
    for (int i=0; i<arr_length; i++) {
        if (!insert_tail(list, arr[i])) {
            DEBUG_STR("Error: Failed to create list from array. Failed to insert element into the list.\n");
            return NULL;
        }
    }

    return list;
}

// write the list to a file
int to_file(tList *list, char *filename, char *delimiter, char *mode, int (*item_saver)(FILE *file, void *item, char *delimiter)) {
    if (!list || !list->head) {
        DEBUG_STR("Error: Failed to write list to file. List or List header equals NULL.\n");
        return 0;
    }
    // open the file
    FILE *file = fopen(filename, mode);
    if (!file) {
        printf("Error opening file %s\n", filename);
        DEBUG_STR("Error: Failed to open file for writing.\n");
        return 0;
    }
    // if the list is empty, write an empty line
    if (list->length == 0) {
        fputs("", file);
    }
    else {
        // iterate over the list and write each element to the file with the given item_saver method
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

// create a list from a file
tList *from_file(char *filename, char *delimiter, void *(*item_loader)(FILE *file, char *delimiter)) {
    // create a new list
    tList *list = list_create();
    if (!list) {
        DEBUG_STR("Error: Failed to create list from file. Failed to create list.\n");
        return NULL;
    }
    // open the file
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening file %s\n", filename);
        DEBUG_STR("Error: Failed to open file for reading.\n");
        return NULL;
    }
    // iterate over the file and load each element into the list with the given item_loader method
    while (!feof(file)) {
        void *item = item_loader(file, delimiter);
        if (!item) {
            fprintf(stderr, "Error loading item from file\n");
            DEBUG_STR("Error: Failed to load item from file.\n");
            continue;
        }
        insert_tail(list, item);
    }

    fclose(file);
    return list;
}

// search for data in the list with the given compare method. Input list becomes rest list and found list is returned
tList *search(tList *list, int (*cmp)(const void*c1, const void*c2), const void *data) {
    if (!list || !list->head || !cmp) return NULL;
    
    tList *found = list_create();
    if (!found) {
        DEBUG_STR("Error: Failed to create found list.\n");
        return NULL;
    }
    // set the current position pointer to the top of the list
    if (_curidx_top(list) == 0) return NULL;
    // Iterate over the list. If the comparison is successful, disconnect the node from old list and insert it into the found list
    while (list->curpos != list->head) {
        if (!list->curpos->data) {
            printf("Error: Data is NULL\n");
            DEBUG_STR("Error: Data is NULL.\n");
            continue;
        }
        if (cmp(list->curpos->data, data) == 0) {
            tNode *tmp = _disconnect_node(list);
            list->curpos = tmp->nxt;
            // list->curidx hasnt to be increased, because the next element idx is the same as the current, when the current is disconnected
            list->length--;
            insert_tail(found, tmp->data);
            free(tmp);
        }
        else {
            _mov_nxt_idx(list);
        }
    }
    // reset the current position pointer to the top of the list
    if (_curidx_top(list) == 0) {
        DEBUG_STR("Warning: Failed to reset the current position to the top of the list.\n");
        return NULL;
    }
    return found;
}

// bubblesort the list
tList *sort(tList *list, int (*cmp)(const void *c1, const void*c2)) { // 
    if (!list || !list->head || !cmp) {
        DEBUG_STR("Error: Failed to sort list. List, list header or comp(are) method equals NULL.\n");
        return list;
    }
    if (list->length < 2) return list;

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

// qsort the list, by converting it to an array, use the qsort function and convert it back to a list
tList* qsort_list(tList *list, int (*cmp) (const void *c1, const void *c2)) {
    if (!list || !list->head || !cmp) {
        DEBUG_STR("Error: Failed to sort list. List, list header or comp(are) method equals NULL.\n");
        return list;
    }
    if (list->length < 2) return list;

    void **arr = to_arr(list);
    if (!arr) {
        DEBUG_STR("Error: Failed to sort list. Failed to convert list to array.\n");
        puts("Sort failed. Array is NULL.");
        return NULL;
    }

    qsort(arr, list->length, sizeof(arr[0]), cmp);
    tNode *current = list->head->nxt;
    for (int i=0; i<list->length; i++) {
        current->data = arr[i]; // (tMedia *) arr[i];
        current = current->nxt;
    }
    free(arr);
    
    return list;
}

// concat two lists together and free the second list
// Need to check before if the two input lists refer to the same list
tList* concat_lists(tList *list1, tList *list2) {
    if (!list1 || !list2) {
        DEBUG_STR("Error: List1 or List2 is NULL.\n");
        return NULL;
    }
    if (!list1->head || !list2->head) {
        DEBUG_STR("Error: List1 header or List2 header is NULL.\n");
        return NULL;
    }

    if (list1->length == 0) {
        list_destroy(list1);
        return list2;
    }

    if (list2->length == 0) {
        list_destroy(list2);
        return list1;
    }
    
    // Connect the last element of list1 to the first element of list2
    list1->head->prv->nxt = list2->head->nxt;
    list2->head->nxt->prv = list1->head->prv;
    
    // Connect the last element of list2 to the head of list1
    list2->head->prv->nxt = list1->head;
    list1->head->prv = list2->head->prv;
    
    // Update length
    list1->length += list2->length;

    list1->curpos = list1->head->nxt;
    list1->curidx = 1;
    
    // Free the memory of list2 head
    free(list2->head);
    free(list2);
    
    return list1;
}