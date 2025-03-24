/*
* Author: Franz Rehschuh
* Date: 2025-02-25
* Description: Header file containing methods for decoding url-encoded strings, parsing post data and handling GET and POST requests.
*/

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

// Methods for decoding url-encoded strings
int url_decode_inplace(char* out, const char* in);
char *url_decode(const char* in);

// Methods for parsing post data
char *parse_attribute(char *token, char *needle, char*delim, int offset, int max_len);
PostParams *parse_post_data(char *post_data);

// Method for freeing post params
void free_post_params(PostParams *params);

// Methods for printing
void _error_row(int idx);
void _table_printer(tList *list);

// Methods for handling GET and POST requests
int handle_get_request();
void handle_post_request();

#endif
