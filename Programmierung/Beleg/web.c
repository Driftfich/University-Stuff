#include <stdio.h>
#include <stdlib.h>

#include "web.h"
#include "list.h"
#include "media.h"
#include "logger.h"
#include "utility.h"
#include "config.h"

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

PostParams *parse_post_data(char *post_data) {
    if (!post_data) {
        DEBUG_STR("Warning: No post data given.\n");
        return NULL;
    }

    PostParams *params = malloc(sizeof(PostParams));
    if (!params) {
        error_msg("Memory allocation failed for post params.\n");
    }

    char tmp[200] = {0};
    char *post_copy = strdup(post_data);
    if (!post_copy) {
        free(params);
        error_msg("Memory allocation failed for post data copy.\n");
    }

    char *token = strtok(post_copy, "&");
    while (token != NULL) {
        if (strstr(token, "search")) {
            sscanf(token, "search=%199s", tmp);
            url_decode(params->query, tmp);
            memset(tmp, 0, sizeof(tmp));
        }
        else if (strstr(token, "sort_key")) {
            sscanf(token, "sort_key=%29s", params->sort_key);
        }
        else if (strstr(token, "name")) {
            sscanf(token, "name=%199s", tmp);
            url_decode(params->name, tmp);
            memset(tmp, 0, sizeof(tmp));
        }
        else if (strstr(token, "author")) {
            sscanf(token, "author=%199s", tmp);
            url_decode(params->author, tmp);
            memset(tmp, 0, sizeof(tmp));
        }
        else if (strstr(token, "borrower")) {
            sscanf(token, "borrower=%199s", tmp);
            url_decode(params->borrower, tmp);
            memset(tmp, 0, sizeof(tmp));
        }
        else if (strstr(token, "date")) {
            sscanf(token, "date=%199s", tmp);
            url_decode(params->date, tmp);
            memset(tmp, 0, sizeof(tmp));
        }
        else if (strstr(token, "action=")) {
            sscanf(token, "action=%29s", params->action);
        }
        else if (strstr(token, "ids=")) {
            char *start = strstr(token, "ids=");
            start += 4;
            char *end = strstr(start, "&");
            int len;
            if (end) {
                len = end - start;
            }
            else {
                len = strlen(start);
            }

            params->ids = malloc(len + 1);
            if (params->ids) {
                strncpy(params->ids, start, len);
                params->ids[len] = '\0';
            }

        }
        token = strtok(NULL, "&");
    }

    free(post_copy);
    return params;
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

int initial_page_load() {
    char buf[2048];
    FILE *F;
    F = fopen(html_path, "rt");

    if (F == NULL) {
        error_msg("Failed to open html file.\n");
    }
    while (fgets(buf, sizeof(buf), F))
    {   
        printf("%s", buf);
        if (strstr(buf, "<tbody id=\"table-body\">") != NULL) {
            tList *list = from_file(media_path, DELIMITER, read_media);
            
            if (!list) {
                error_msg("Failed to create base list from media file.\n");
            }
            else {
                if (list->length > 0) {
                    list = qsort_list(list, &cmp_name);

                    if (!list) {
                        error_msg("Failed to sort base list.\n");
                    }

                    _table_printer(list);
                }
                else puts("<tr><td>No results found.</td></tr>\n");
            }
            
        }
    }
    fclose(F);
    return 0;
}
