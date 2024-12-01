#ifndef LIST_H
#define LIST_H

typedef struct tConnector {
    struct tConnector *nxt;
    struct tConnector *prv;
    void *data;
} tNode;

typedef struct tPool {
    tNode *nodes;
    unsigned int capacity;
    unsigned int used;
} tPool;

typedef struct tList {
    tPool *head;
    tNode *curpos;
    unsigned int length;
    unsigned int pool_size;
    unsigned int pool_amount;
} tList;



// List creation and destruction
tList* list_create();
void list_destroy(tList *list);

// Internal position management
int _check_null(tList *list);
int _mov_nxt_idx(tList *list);
int _mov_prv_idx(tList *list);
int _get_idx(tList *list);
int _move_index(tList *list, int index);
int* _get(tList *list);

// Insertion operations
int _insert_behind(tList *list, void *data);
int insert_tail(tList *list, void *data);
int insert_head(tList *list, void *data);

// Getter operations
int* get(tList *list, int idx);

// Deletion operations
int delete_node(tList *list);

// Utility functions
void print_list(tList *list);

#endif // LIST_H
