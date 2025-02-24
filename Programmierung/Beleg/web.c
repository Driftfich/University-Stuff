#include <stdio.h>
#include <stdlib.h>

#include "web.h"
#include "list.h"
#include "media.h"
#include "logger.h"
#include "utility.h"
#include "config.h"

// method for parsing URL-encoded string into original string
int url_decode_inplace(char* out, const char* in)
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

char *url_decode(const char*in) {
    char *out = malloc(strlen(in) + 1);
    if (!out) {
        DEBUG_STR("Error: Memory allocation failed for URL-decoding.\n");
        return NULL;
    }
    if (url_decode_inplace(out, in) != 0) {
        free(out);
        return NULL;
    }
    return out;
}

void free_post_params(PostParams *params) {
    // free all attributes of the PostParams struct which are all dynamically allocated
    if (!params) return;
    if (params->query) free(params->query);
    if (params->name) free(params->name);
    if (params->author) free(params->author);
    if (params->borrower) free(params->borrower);
    if (params->date) free(params->date);
    if (params->ids) free(params->ids);
    free(params);
    return;
}

char *parse_attribute(char *token, char *needle, char*delim, int offset, int max_len) {
    if (!token || !needle || !delim) {
        DEBUG_STR("Error: Invalid input for parse_attribute.\n");
        return NULL;
    }
    // search for the start of the attribute in the token
    char *start = strstr(token, needle);
    if (!start) {
        DEBUG_STR("Warning: Needle not found in token.\n");
        return NULL;
    }
    // move the start pointer to the end of the needle, therefore before the attribute value
    if (strlen(start) < offset) {
        DEBUG_STR("Warning: Offset is greater than length of attribute.\n");
        return NULL;
    }
    start += offset;
    // search for the end of the attribute in the token and calculate the length
    char *end = strstr(start, delim);
    int len;
    if (end) {
        len = end - start;
    }
    else {
        len = strlen(start);
    }
    // check if the length exceeds the maximum length
    if (len > max_len && max_len > 0) {
        DEBUG_STR("Warning: Length of attribute exceeds maximum length.\n");
        len = max_len;
    }
    // allocate memory for the output string
    char *out = malloc(len + 1);
    if (!out) {
        DEBUG_STR("Error: Memory allocation failed for attribute.\n");
        return NULL;
    }
    // copy the attribute value to the output string
    strncpy(out, start, len);
    out[len] = '\0';

    return out;
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

    params->query = NULL;
    params->name = NULL;
    params->author = NULL;
    params->borrower = NULL;
    params->date = NULL;
    params->ids = NULL;
    params->action[0] = '\0';
    params->sort_key[0] = '\0';

    char *tmp;
    // copy post_data to keep input the same for further possible usage
    char *post_copy = strdup(post_data);
    if (!post_copy) {
        free(params);
        error_msg("Memory allocation failed for post data copy.\n");
    }
    // "split" up the post_data copy by "&", check if the variable is in the current split, parse the value and save it
    char *token = strtok(post_copy, "&");
    while (token != NULL) {
        if (strstr(token, "search=")) {
            params->query = parse_attribute(token, "search=", "&", 7, MAX_INPUT_LENGTH);
        }
        else if (strstr(token, "sort_key=")) {
            if (sscanf(token, "sort_key=%29s", params->sort_key) != 1) {
                free_post_params(params);
                free(post_copy);
                error_msg("Error: Failed to parse sort key.\n");
            }
        }
        else if (strstr(token, "name=")) {
            params->name = parse_attribute(token, "name=", "&", 5, MAX_INPUT_LENGTH);
        }
        else if (strstr(token, "author=")) {
            params->author = parse_attribute(token, "author=", "&", 7, MAX_INPUT_LENGTH);
        }
        else if (strstr(token, "borrower=")) {
            params->borrower = parse_attribute(token, "borrower=", "&", 9, MAX_INPUT_LENGTH);
        }
        else if (strstr(token, "date=")) {
            params->date = parse_attribute(token, "date=", "&", 5, 50);
        }
        else if (strstr(token, "action=")) {
            if (sscanf(token, "action=%29s", params->action) != 1) {
                free_post_params(params);
                free(post_copy);
                error_msg("Error: Failed to parse action.\n");
            }
        }
        else if (strstr(token, "ids=")) {
            params->ids = parse_attribute(token, "ids=", "&", 4, -1);

        }
        token = strtok(NULL, "&");
    }

    free(post_copy);
    return params;
}

void _error_row(int idx) {
    // Prints out a row with 4 error cells for debugging purposes
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
    // Iterates over given list and uses _row_printer from media.c library
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
    DEBUG_STR("Info: Handling post request.\n");
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
    // char *post_data = strdup("action=delete_all");
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
    if (params->query && strlen(params->query) > 0) {
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
        
        // remove the search query media item from memory
        free_media(search_query);
    }
    else {
        // If no query string is given, the whole list is shown
        found = list;
    }
    // sort the visible list
    if (found->length > 1) {
        // sort the list with the given sort key
        int (*comp[4])(const void*, const void*) = {cmp_name, cmp_author, cmp_borrower, cmp_date};
        int idx = atoi(params->sort_key);
        if (idx < 0 || idx > 3) {
            DEBUG_STR("Warning: Invalid sort key. Defaulting to 0.\n");
            idx = 0;
        }
        found = qsort_list(found, comp[idx]);
    }

    if (!found) {
        error_msg("Sort failed. List is NULL.\n");
    }

    if (strstr(params->action, "delete_all") != NULL) {
        // empty found list by recreating it
        list_destroy(found);
        found = list_create();
    }

    if (strstr(params->action, "delete") != NULL && params->ids) {
        // delete specific nodes from the found list
        if (delete_ids(found, params->ids) == 0) {
            error_msg("Failed to delete ids.\n");
        }
    }
    // clean up the post params
    free_post_params(params);

    // if the found list has elements, print them out. Otherwise, print out that no results were found
    if (found->length > 0) {
        _table_printer(found);
    }
    else {
        puts("<tr><td>No results found.</td></tr>\n");
    }

    // concat the visible and rest list together when they got split up before. Else not, because then found and list refer to the same list
    if (split) {
        found = concat_lists(found, list);
        if (!found) DEBUG_STR("Error: Failed concat list and found together.\n");
    }
    
    // Save the complete list to file
    if (to_file(found, media_path, DELIMITER, "w", &write_media) == 0) DEBUG_STR("Error: Failed saving found list to file\n");

    // Free the list
    list_destroy(found);

    return;
}
