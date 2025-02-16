#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#ifndef WEB_H
#define WEB_H

int hex_to_int(char c);
int url_decode(char *out, const char*in);
void _error_row(int idx);
void _table_printer(tList *list);

#endif
