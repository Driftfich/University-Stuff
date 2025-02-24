#include "list.h"

#ifndef WEB_H
#define WEB_H

// attributes set by the user are dynamic, rest (action, sort_key) for internal use are static
typedef struct {
    char *query;
    char *name;
    char *author;
    char *borrower;
    char *date;
    char action[30];
    char sort_key[30];
    char* ids;
} PostParams;

int url_decode_inplace(char* out, const char* in);
char *url_decode(const char* in);
char *parse_attribute(char *token, char *needle, char*delim, int offset, int max_len);
void free_post_params(PostParams *params);
PostParams *parse_post_data(char *post_data);
void _error_row(int idx);
void _table_printer(tList *list);
int handle_get_request();
void handle_post_request();

#endif
