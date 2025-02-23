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
    // c equals the first char of input string
    char c=*in++, v1, v2;
    while(c != '\0') {
        if(c == '%') {
            // when c equals percentage, next two char get converted to integer
            v1 = hex_to_int(*in++);
            v2 = hex_to_int(*in++);
            if((v1)<0 || (v2)<0) {
                *out = '\0';
                DEBUG_STR("Error: Failed to decode URL-encoded string.\n");
                return -1;
            }
            // Combine the two original 4 Bit to 8 Bit representation to get original char
            c = (v1<<4)|v2;
        }
        // Set the next output char to c
        *out++ = c;
        c=*in++;
    }
    // Set last output char to terminating zero
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
    // copy post_data to keep input the same for further possible usage
    char *post_copy = strdup(post_data);
    if (!post_copy) {
        free(params);
        error_msg("Memory allocation failed for post data copy.\n");
    }
    // "split" up the post_data by "&", check if the variable is in the current split, scan the value, decode it back
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
            // set start to first row number/id
            char *start = strstr(token, "ids=");
            start += 4;
            // set end to & after last id
            char *end = strstr(start, "&");
            int len;
            if (end) {
                len = end - start;
            }
            else {
                len = strlen(start);
            }
            // copy string with separated numbers to params->ids
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
    // Iterates over given list and uses _row_printer from media.csv library
    tNode *tmp = list->head->nxt;
    int idx = 0;
    while (tmp != list->head) {
        _row_printer(tmp->data, idx);
        tmp = tmp->nxt;
        idx++;
    }

    return;
}

int handle_get_request() {
    char buf[2048];
    FILE *F;
    F = fopen(html_path, "rt");

    if (F == NULL) {
        error_msg("Failed to open html file.\n");
    }
    // Iterates over html file and prints it out
    while (fgets(buf, sizeof(buf), F))
    {   
        printf("%s", buf);
        // if table body is in the current line the media.csv is loaded, sorted and printed out
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

void handle_post_request() {
    // // read in query string
    char *content_length = getenv("CONTENT_LENGTH");
    if (content_length == NULL) {
        error_msg("No content length for the post request found.\n");
    }

    int length = atoi(content_length) + 1;
    char post_data[length];
    
    if (fgets(post_data, length, stdin) == NULL) {
        error_msg("No post data for the post request found.\n");
    }

    // // Fake post data
    // char *post_data = strdup("sort_key=2&search=");
    // int length = strlen(post_data);

    // parse the post_data into separate attributes
    PostParams *params = parse_post_data(post_data);
    if (!params) {
        error_msg("Failed to parse post data.\n");
    }
    
    tList *list = from_file(media_path, DELIMITER, read_media);
    if (!list) {
        error_msg("Failed to create list from file.\n");
    }

    // If the action equals add, new media gets created with given attributes and inserted into the list
    if (strstr(params->action, "add") != NULL) {
        tMedia *new_media = create_media(params->name, params->author, params->borrower, params->date);
        if (new_media) {
            insert_tail(list, new_media);
        }
        else {
            error_msg("Memory allocation failed for new media.\n");
        }
    }

    tList *found = NULL;
    int split = 0;
    // Check if the query string has been provided
    if (strlen(params->query) > 0) {
        // Create media item, where all attributes equal the search query
        tMedia *search_query = create_media(params->query, params->query, params->query, params->query);
        if (!search_query) {
            error_msg("Failed to create search query.\n");
        }
        // search the list with the _search_media method and the search query media item
        found = search(list, _search_media, search_query);
        if (!found) {
            error_msg("Search failed. Found list is NULL.\n");
        }
        // set split to 1/true, to concat the lists later on
        split = 1;

        free_media(search_query);
    }
    else {
        found = list;
    }

    if (found->length > 1) {
        int (*comp[4])(const void*, const void*) = {cmp_name, cmp_author, cmp_borrower, cmp_date};
        int idx = atoi(params->sort_key);
        if (idx < 0 || idx > 3) {
            DEBUG_STR("Warning: Invalid sort key. Defaulting to 0.\n");
            idx = 0;
        }
        found = qsort_list(found, comp[idx]);
    }

    if (!found || found->length == 0) {
        error_msg("Sort failed. List is NULL.\n");
    }

    if (strstr(params->action, "delete_all") != NULL) { 
        // empty found list
        list_destroy(found);
        found = list_create();
    }

    if (strstr(params->action, "delete") != NULL && params->ids) {
        if (delete_ids(found, params->ids) == 0) {
            error_msg("Failed to delete ids.\n");
        }
    }
    if (params->ids) {
        free(params->ids);
    }

    free(params);

    if (found->length > 0) {
        _table_printer(found);
    }
    else {
        puts("<tr><td>No results found.</td></tr>\n");
    }

    // concat the rest and list together to save the whole list
    if (split) {
        found = concat_lists(found, list);
        if (!found) DEBUG_STR("Error: Failed concat list and found together.\n");
    }
    
    // Save the list to file
    if (to_file(found, media_path, DELIMITER, "w", &write_media) == 0) DEBUG_STR("Error: Failed saving found list to file\n");

    // Free the list
    list_destroy(found);

    return;
}
