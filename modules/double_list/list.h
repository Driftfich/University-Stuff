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
    char *borrowed_date;
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
tNode *_disconnect_node(tList *list);
int delete_node(tList *list);

// Utility functions
void** to_datarray(tList *list);
tList *from_datarray(void **dataray, int size);

void print_list(tList *list, void (*printer) (void *data));

tList* search(tList *list, int (*comp)(const void*c1, const void*c2), const void *data);
tList* sort(tList *list, int (*comp)(const void *c1, const void*c2)); //
tList* concat_lists(tList *list1, tList *list2);

// FILE I/O
int to_file(tList *list, char *filename, char *delimiter, char *mode, void (*item_saver)(FILE *file, void *item, char *delimiter));
tList *from_file(char *filename, char *delimiter, void *(*item_loader)(FILE *file, char *delimiter));

#endif // LIST_H
