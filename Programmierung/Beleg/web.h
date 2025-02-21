#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#ifndef WEB_H
#define WEB_H

typedef struct {
    char query[200];
    char name[200];
    char author[200];
    char borrower[200];
    char date[200];
    char action[30];
    char sort_key[30];
    char* ids;
} PostParams;

int url_decode(char *out, const char*in);
PostParams *parse_post_data(char *post_data);
void _error_row(int idx);
void _table_printer(tList *list);
int initial_page_load();

#endif
