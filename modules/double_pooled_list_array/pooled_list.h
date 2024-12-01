#ifndef POOLED_LIST_H
#define POOLED_LIST_H

#define GROWTH_FACTOR 2

// typedef union mixed_type {
//     int i;
//     float f;
//     char c;
//     char *s;
// } mx_type;

typedef struct tNode {
    int *data;
    unsigned int nan;
} tNode;

typedef struct tPool {
    tNode **ar;
    struct tPool *nxt;
    struct tPool *prv;
    unsigned int capacity;
    unsigned int used;
} tPool;

typedef struct tList {
    tPool *head;
    tPool *curpos;
    tNode *curnode;
    unsigned int curnodeidx;
    unsigned int length;
    unsigned int pool_size;
    unsigned int cursum;
} tList;
// add values to tList to describe the distribution of pool sizes to move the index faster to the desired position

tList* create_list(unsigned int pool_size);
int insert_tail(tList *list, int *data);
int insert_head(tList *list, int *data);
int _insert_behind(tList *list, int *data);
int _move_node_idx(tList *list, int node_idx);
int _get_pool_idx(tList *list);
int _move_pool_idx(tList *list, int index);
int _move_idx(tList *list, int index);
#endif