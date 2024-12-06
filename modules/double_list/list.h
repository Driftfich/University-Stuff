#ifndef LIST_H
#define LIST_H

typedef struct tConnector {
    struct tConnector *nxt;
    struct tConnector *prv;
    void *data;
} tNode;

typedef struct tList {
    tNode *head;
    tNode *curpos;
    unsigned int length;
} tList;

typedef struct BorrowedMediaItem {
    char *name;
    char *author;
    char *borrower;
    unsigned long int bowrrowed_date;
} tMedia;

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
void print_list(tList *list, void (*printer) (void *data));

tList* search(tList *list, int (*comp)(void*, void*), void *data);

#endif // LIST_H
