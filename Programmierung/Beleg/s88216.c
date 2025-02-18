#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include <stdarg.h>
#define _GNU_SOURCE
#include "list.h"
#include "media.h"
#include "web.h"
#include "logger.h"

#ifndef debug
    #define debug 0
#endif

#ifdef _WIN32
    char *media_path = ".\\media.csv";
    char *html_path = ".\\index.html";
#elif __linux__
    char *media_path = "./media.csv";
    char *html_path = "./index.html";
#endif

#define DELIMITER ";"

FILE *ProtFile;

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

char *format_string(char *format,...) {
    char *result = NULL;
    va_list args;
    va_start(args, format);
    vasprintf(&result, format, args);
    va_end(args);
    return result;
}

void error_msg(char *msg) {
    if (msg) {
        char *err_msg = format_string("Error: %s", msg);
        DEBUG_STR(err_msg);
        puts(err_msg);
        free(err_msg);
    }
    exit(1);
}

tMedia *create_search_query(char *query) {
    if (!query) {
        DEBUG_STR("Warning: No query given.\n");
        return NULL;
    }

    tMedia *search_query = malloc(sizeof(tMedia));
    if (!search_query) {
        error_msg("Memory allocation failed for search query.\n");
    }
    
    search_query->name = strdup(query);
    search_query->author = strdup(query);
    search_query->borrower = strdup(query);
    search_query->borrowed_date = strdup(query);

    if (!search_query->name || !search_query->author || !search_query->borrower || !search_query->borrowed_date) {
        if (search_query->name) free(search_query->name);
        if (search_query->author) free(search_query->author);
        if (search_query->borrower) free(search_query->borrower);
        if (search_query->borrowed_date) free(search_query->borrowed_date);
        free(search_query);
        error_msg("Memory allocation failed for search query attributes.\n");
    }
    
    return search_query;
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

int delete_ids(tList *list, char *ids) {
    if (!list) {
        error_msg("No list given for deletion.\n");
    }
    if (!ids) {
        DEBUG_STR("Warning: No ids given for deletion. Therefore no items got deleted.\n");
        return 0;
    }

    char *tok = strtok(ids, "~");
    int count = 0;
    while (tok != NULL) {
        int id = atoi(tok);
        // Because deletion begins with smallest id, following ids have to be decremented by the already deleted id count
        id -= count;
        count++;

        if (_move_index(list, id) == 0) {
            error_msg("Failed to move index to the given id.\n");
        }
        if (delete_node(list) == 0) {
            error_msg("Failed to delete node.\n");
        }
        tok = strtok(NULL, "~");
    }

    return 1;
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

int main (int argc, char *argv[], char*env[]) {
    puts("Content-Type: text/html\r\n\r\n");

    // Create the media.csv file if it doesnt exist
    FILE *file = fopen(media_path, "r");
    if (!file) {
        file = fopen(media_path, "w+");
    }
    if (file) fclose(file);

    char *request_method = getenv("REQUEST_METHOD");
    // char *request_method = strdup("POST");

    if (request_method == NULL) {
        error_msg("No request method found.");
    }

    if (strcmp(request_method, "GET") == 0) {
        initial_page_load();
    }
    else if (strcmp(request_method, "POST") == 0) {

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

        
        PostParams *params = parse_post_data(post_data);
        if (!params) {
            error_msg("Failed to parse post data.\n");
        }
        
        tList *list = from_file(media_path, DELIMITER, read_media);
        if (!list) {
            error_msg("Failed to create list from file.\n");
        }

        if (strstr(params->action, "add") != NULL) {
            tMedia *media = malloc(sizeof(tMedia));
            if (media) {
                media->name = strdup(params->name);
                media->author = strdup(params->author);
                media->borrower = strdup(params->borrower);
                media->borrowed_date = strdup(params->date);

                if (!media->name || !media->author || !media->borrower || !media->borrowed_date) {
                    if (media->name) free(media->name);
                    if (media->author) free(media->author);
                    if (media->borrower) free(media->borrower);
                    if (media->borrowed_date) free(media->borrowed_date);
                    free(media);
                    error_msg("Memory allocation failed for new media attributes.\n");
                }

                insert_tail(list, media);
            }
            else {
                error_msg("Memory allocation failed for new media.\n");
            }
        }

        tList *found = NULL;
        int split = 0;
        if (strlen(params->query) > 0) {
            tMedia *search_query = create_search_query(params->query);
            if (!search_query) {
                error_msg("Failed to create search query.\n");
            }

            found = search(list, _search_media, search_query);
            if (!found) {
                error_msg("Search failed. Found list is NULL.\n");
            }
            split = 1;

            free(search_query->name);
            free(search_query->author);
            free(search_query->borrower);
            free(search_query->borrowed_date);
            free(search_query);
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
        }
    else {
        puts("No valid request method found."); 
        DEBUG_STR("Error: No valid request method found.\n");
    }

    return 0;
}