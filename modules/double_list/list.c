#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <sqlite3.h>

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
    printf("List([");
    while (tmp != list->head) {
        printer(tmp->data);
        tmp = tmp->nxt;
    }
    printf("])");
}

void to_dataray(tList *list, void **dataray) {
    if (!list || !list->head) return;
    
    tNode *tmp = list->head->nxt;
    int i = 0;
    while (tmp != list->head) {
        dataray[i] = tmp->data;
        tmp = tmp->nxt;
        i++;
    }
}

void from_dataray(tList *list, void **dataray, int size) {
    if (!list || !list->head) return;
    
    for (int i = 0; i < size; i++) {
        insert_tail(list, dataray[i]);
    }
}

void _create_db_table(sqlite3 *db, char *table) {
    char *sql = "CREATE TABLE IF NOT EXISTS ? ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT"
                "name TEXT NOT NULL"
                "author TEXT NOT NULL"
                "borrower TEXT"
                "borrowed_date INTEGER"
                ");";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return;
    }
    sqlite3_bind_text(stmt, 1, table, -1, SQLITE_STATIC);
    sqlite3_step(stmt);
    
    sqlite3_finalize(stmt);
}

void to_db(tList *list, sqlite3 *db, char *table) {
    if (!list || !list->head) return;

    _create_db_table(db, table);
    
    char *sql="INSERT INTO ? (name, author, borrower, borrowed_date) VALUES (?, ?, ?, ?);";

    // Statement vorbereiten
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return;
    }

    tNode *tmp = list->head->nxt;
    while (tmp != list->head) {
        tMedia *media = (tMedia*) tmp->data;

        // bind values into insert statement
        sqlite3_bind_text(stmt, 1, table, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, media->borrower, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, media->author, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, media->borrower, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 5, media->bowrrowed_date);

        // execute statement
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        }

        // reset statement
        sqlite3_reset(stmt);

        tmp = tmp->nxt;
    }

    sqlite3_finalize(stmt);
}

tList *from_db(sqlite3 *db, char *table) {
    tList *list = list_create();
    if (!list) return NULL;

    _create_db_table(db, table);

    char *sql = "SELECT * FROM ?;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    sqlite3_bind_text(stmt, 1, table, -1, SQLITE_STATIC);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        tMedia *media = (tMedia*) malloc(sizeof(tMedia));
        if (!media) {
            fprintf(stderr, "Error allocating memory for media\n");
            return NULL;
        }

        media->name = (char*) sqlite3_column_text(stmt, 1);
        media->author = (char*) sqlite3_column_text(stmt, 2);
        media->borrower = (char*) sqlite3_column_text(stmt, 3);
        media->bowrrowed_date = sqlite3_column_int(stmt, 4);

        insert_tail(list, media);
    }

    sqlite3_finalize(stmt);
    return list;
}

tList *_list_tables(sqlite3 *db) {
    tList *list = list_create();
    if (!list) return NULL;

    char *sql = "SELECT name FROM sqlite_master WHERE type='table';";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        char *table = (char*) sqlite3_column_text(stmt, 0);
        insert_tail(list, table);
    }

    sqlite3_finalize(stmt);
    return list;
}

tList *search(tList *list, int (*cmp) (void*, void*), void *data) {
    // input list: tList to search in, compare function: int (*cmp) (void*, void*), data: void* to compare with
    // output: tList with found elements
    if (!list || !list->head || !cmp) return NULL;
    
    tList *found = list_create();
    if (!found) return NULL;

    tNode *ftmp = list->head->nxt;
    tNode *ptmp = list->head->prv;
    for (int i=0; i<(list->length + 1)/2; i++) {
        if (cmp(ftmp->data, data) == 0) {
            insert_tail(found, ftmp->data);
        }

        if ((ftmp != ptmp) && (cmp(ptmp->data, data) == 0)) {
            insert_tail(found, ptmp->data);
        }
        ftmp = ftmp->nxt;
        ptmp = ptmp->prv;
    }

    return found;
}

tList *sort(tList *list, int (*cmp) (void*, void*)) {
    // input list: tList to sort, compare function: int (*cmp) (void*, void*)
    // output: tList with sorted elements
    if (!list || !list->head || !cmp) return NULL;
    
    
}







