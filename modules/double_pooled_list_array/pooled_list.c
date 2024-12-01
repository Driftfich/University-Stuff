#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "pooled_list.h"


tPool *_create_pool(tList *list, unsigned int pool_size) {
    tPool *pool = (tPool*) malloc(sizeof(tPool));
    if (!pool) return NULL;
    
    // Speicher für das Array von tNode-Zeigern reservieren
    pool->ar = (tNode**) malloc(sizeof(tNode*) * pool_size);
    if (!pool->ar) {
        free(pool);
        return NULL;
    }

    // Array-Elemente initialisieren
    for (unsigned int i = 0; i < pool_size; i++) {
        pool->ar[i] = NULL;
    }

    pool->nxt = NULL;
    pool->prv = NULL;
    pool->capacity = pool_size;
    pool->used = 0;
    return pool;
}

tList *create_list(unsigned int pool_size) {
    tList *list = (tList*) malloc(sizeof(tList));
    if (!list) return NULL;
    
    tPool *head = _create_pool(list, pool_size);
    if (!head) {
        free(list);
        return NULL;
    }
    // tNode nan_data = {0, 1};
    // tNode head_data[1] = {nan_data};
    head->nxt = head;
    head->prv = head;
    head->capacity = 0;
    head->used = 0;

    list->head = head;
    list->curpos = head;
    list->curnode = NULL;
    list->curnodeidx = 0;
    list->length = 0;
    list->pool_size = pool_size;
    return list;
}


// ABSTRACT CURRENT POSITION POINTER METHODS
// abstract forward move of the position pointer in the list
// int _mov_nxt_idx(tList *list) {
//     if (!list || !list->curpos) return 0;
//     list->curpos = list->curpos->nxt;
//     return 1;
// }

// // abstract backward move of the position pointer in the list
// int _mov_prv_idx(tList *list) {
//     if (!list || !list->curpos) return 0;
//     list->curpos = list->curpos->prv;
//     return 1;
// }

// moves the current pool position pointer to the desired int global index
int _move_pool_idx(tList *list, int index) {
    if (!list || !list->head) {
        return 0;
    }
    if (index < 0 || index >= list->length) {
        return 0;
    }
    int forward = (index < list->length / 2);
    list->curpos = list->head->nxt;
    int sum = list->curpos->capacity;
    while (sum < index)
    {
        sum += list->curpos->capacity;
        if (forward) {
            list->curpos = list->curpos->nxt;
        } else {
            list->curpos = list->curpos->prv;
        }
    }
    list->cursum = sum;
    list->curnode = list->curpos->ar[0];
    list->curnodeidx = 0;
    return 1;
}

// returns an int index of the current pool
int _get_pool_idx(tList *list) {
    if (!list || !list->head) {
        return -1;
    }
    tPool *tmp = list->head->nxt;
    int idx = 0;
    while (tmp != list->curpos) {
        idx += tmp->used;
        tmp = tmp->nxt;
    }
    return idx;
}

// moves the current node position pointer to the desired int index
int _move_node_idx(tList *list, int node_idx) {
    if (!list || !list->head) {
        return 0;
    }
    if (node_idx < 0 || node_idx >= list->curpos->capacity) {
        return 0;
    }
    list->curnode = list->curpos->ar[0];
    list->curnodeidx = 0;
    while (node_idx > 0) {
        list->curnode = list->curpos->ar[list->curnodeidx];
        if (list->curnode->nan == 0) {
            node_idx--;
        }
        list->curnodeidx++;
    }
    list->curnodeidx = node_idx;
    return 1;
}

// returns an int index of the current node
// int _get_node_idx(tList *list) {
//     if (!list || !list->head) {
//         return -1;
//     }
//     tNode *tmp = list->curpos->data;
//     int idx = 0;
//     while (tmp != list->curnode) {
//         idx++;
//         tmp++;
//     }
//     return idx;
// }

// move the pool and node position pointers based on a global index
int _move_idx(tList *list, int index) {
    if (!list || !list->head) {
        return 0;
    }
    if (index < 0 || index >= list->length) {
        return 0;
    }
    _move_pool_idx(list, index);
    _move_node_idx(list, list->cursum - index);
    return 1;
}

int _insert_behind(tList *list, int *data) {
    if (!list || !list->curpos) return 0;

    tNode *new_item = (tNode*) malloc(sizeof(tNode));   
    if (!new_item) return 0;
    new_item->data = data;
    new_item->nan = 0;
    // the current node in the array equals the last idx in the array => create a new pool
    if (list->curnodeidx == list->curpos->capacity - 1) {
        tPool *new_pool = _create_pool(list, list->pool_size);
        if (!new_pool) {
            free(new_item);
            return 0;
        }
        new_pool->nxt = list->curpos->nxt;
        new_pool->prv = list->curpos;

        new_pool->nxt->prv = new_pool;
        new_pool->prv->nxt = new_pool;
        new_pool->capacity = list->pool_size;
        new_pool->used = 1; // one element gets added to the new pool
        list->curpos = new_pool;
        list->cursum += list->curpos->capacity;
        list->curnode = list->curpos->ar[0];
        list->curnodeidx = 0;
        list->curnode->nan = 0;
        list->curnode = new_item;
    }
    else {
        // move the curnode pointer to the next position in the array
        if (list->curnodeidx + 1 >= list->curpos->capacity) {
            return 0;
        }
        
        list->curnode = list->curpos->ar[list->curnodeidx + 1];
        list->curnodeidx++;
        if (list->curnode->nan == 1) {
            list->curnode = new_item;
            list->curnode->nan = 0;
        }
        else {
            // create a new pool, check which side of the array is closer to start/end, copy the side + new element into the new pool and update the pointers and sizes
            // check size
            int forward = (list->curnodeidx < list->curpos->capacity / 2);
            int new_ar_size = forward ? list->curnodeidx : list->curpos->capacity - list->curnodeidx;
            new_ar_size++;
            // tNode *new_data[new_ar_size];
            tPool *new_pool = _create_pool(list, list->pool_size);
            if (!new_pool) {
                free(new_item);
                return 0;
            }

            if (forward)
            {
                // add the new pool between the previous and the current pool
                list->curpos->prv->nxt = new_pool;
                new_pool->nxt = list->curpos;
                new_pool->prv = list->curpos->prv;
                list->curpos->prv = new_pool;

                // copy ar from start to curnodeidx + data to the new pool using a loop
                for (int i = 0; i < list->curnodeidx; i++) {
                    new_pool->ar[i] = list->curpos->ar[i];
                    if (new_pool->ar[i]->nan == 0) {
                        new_pool->used++;
                        list->curpos->used--;
                    }
                }
                new_pool->ar[new_ar_size - 1] = new_item;
                list->curnodeidx = new_ar_size - 1;

                // update the arr of the current pool
                tNode *tmp[list->curpos->capacity - new_ar_size];
                memcpy(tmp, list->curpos->ar + list->curnodeidx + 1, (list->curpos->capacity - list->curnodeidx - 1) * sizeof(tNode*));
                list->curpos->ar = tmp;
            }
            else
            {
                // add the new pool between the current and the next pool
                list->curpos->nxt->prv = new_pool;
                new_pool->prv = list->curpos;
                new_pool->nxt = list->curpos->nxt;
                list->curpos->nxt = new_pool;

                // copy the data from the current pool to the new pool from curnodeidx to the end using memcpy
                for (int i = 0; i < list->curpos->capacity - list->curnodeidx; i++) {
                    new_pool->ar[i] = list->curpos->ar[list->curnodeidx + i];
                    if (new_pool->ar[i]->nan == 0) {
                        new_pool->used++;
                        list->curpos->used--;
                    }
                }
                
                new_pool->ar[0] = new_item;
                list->curnodeidx = 0;

                // update the arr of the current pool
                tNode *tmp[list->curpos->capacity - new_ar_size];
                memcpy(tmp, list->curpos->ar, ((list->curnodeidx) - 1) * sizeof(tNode*));
                list->curpos->ar = tmp;
            }
            // reduce the size of the current pool
            list->curpos->capacity = list->curpos->capacity - new_ar_size + 1;
            // set curpos, curnode to the new pool
            list->curpos = new_pool;
            list->curnode = list->curpos->ar[0];
        }
    }
    list->length++;
    return 1;
}

int insert_tail(tList *list, int *data) {
    list->curpos = list->head->prv;
    tNode *tmp = list->curpos->ar[list->curpos->capacity -1];
    int i=1;
    while (tmp->nan == 1) {
        i++;
        tmp = list->curpos->ar[list->curpos->capacity - i];
    }
    list->curnodeidx = list->curpos->capacity - i;
    return _insert_behind(list, data);
}

int insert_head(tList *list, int *data) {
    list->curpos = list->head;
    return _insert_behind(list, data);
}

