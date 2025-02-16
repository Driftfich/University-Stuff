#include <stdio.h>
#include <stdlib.h>

#include "web.h"
#include "list.h"
#include "media.h"
#include "logger.h"

int hex_to_int(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }
    return -1;
}

// method for parsing URL-encoded string into original string
int url_decode(char* out, const char* in)
{
    if (!in) return -1;
    char c=*in++, v1, v2;
    while(c != '\0') {
        if(c == '%') {
            v1 = hex_to_int(*in++);
            v2 = hex_to_int(*in++);
            if((v1)<0 || (v2)<0) {
                *out = '\0';
                DEBUG_STR("Error: Failed to decode URL-encoded string.\n");
                return -1;
            }
            c = (v1<<4)|v2;
        }
        *out++ = c;
        c=*in++;
    }
    *out = '\0';
    return 0;
}


void _error_row(int idx) {
    puts("<tr class=clickable-row\n");
    puts("onclick=\"this.classList.toggle('row-selected')\"\n");
    printf("data-id=\"%d\">\n", idx);
    for (int i=0; i<4; i++) printf("<td>Error</td>\n");
    puts("</tr>\n");
    return;
}

void _table_printer(tList *list) {
    if (!list || list->length == 0) {
        puts("");
        return;
    }
    tNode *tmp = list->head->nxt;
    int idx = 0;
    while (tmp != list->head) {
        _row_printer(tmp->data, idx);
        tmp = tmp->nxt;
        idx++;
    }

    return;
}
