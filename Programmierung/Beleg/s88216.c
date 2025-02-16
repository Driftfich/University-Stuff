#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
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

int initial_page_load() {
    char buf[2048];
    FILE *F;
    F = fopen(html_path, "rt");

    if (F == NULL) {
        puts("<html><head><title><p>Dateifehler<p></title></body></html>");
        return 1;
    }
    while (fgets(buf, sizeof(buf), F))
    {   
        if (strstr(buf, "<tbody id=\"table-body\">") != NULL) {
            printf("%s", buf);
            tList *list = from_file(media_path, DELIMITER, read_media);
            
            if (!list) {
                DEBUG_STR("Error: Failed to create base list.\n");
            }
            else {
                if (list->length > 0) {
                    list = qsort_list(list, &cmp_name);

                    if (!list) {
                        DEBUG_STR("Error: Failed to sort base list.\n");
                        return 1;
                    }

                    _table_printer(list);
                }
                else puts("<tr><td>No results found.</td></tr>\n");
            }
            
        }
        else printf("%s", buf);
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
        DEBUG_STR("Warning: No request method found.\n");
        puts("No request method found.");
        return 1;
    }

    if (strcmp(request_method, "GET") == 0) {
        initial_page_load();
    }
    else if (strcmp(request_method, "POST") == 0) {

        // // read in query string
        char *content_length = getenv("CONTENT_LENGTH");
        if (content_length == NULL) {
            puts("Error: No content length for the post request found.");
            DEBUG_STR("Error: No content length for the post request found.\n");
            return 1;
        }

        int length = atoi(content_length) + 1;
        char post_data[length];
        
        if (fgets(post_data, length, stdin) == NULL) {
            puts("No post data found.");
            DEBUG_STR("Error: No post data for the post request found.\n");
            return 1;
        }

        // // Fake post data
        // char *post_data = strdup("sort_key=2&search=");
        // int length = strlen(post_data);

        
        char tmp[200] = {0};
        char query[200] = {0}, name[200] = {0}, author[200] = {0};
        char borrower[200] = {0}, date[200] = {0}, action[30] = {0};
        char sort_key[30] = {0};
        char *ids = NULL;

        // Token-Parsing Schleife
        char *token = strtok(post_data, "&");
        while (token != NULL) {
            if (strstr(token, "search")) {
                sscanf(token, "search=%199s", tmp);
                // url decode the query
                url_decode(query, tmp);
                memset(tmp, 0, sizeof(tmp));
            }
            else if (strstr(token, "sort_key")) {
                sscanf(token, "sort_key=%29s", sort_key);
            }
            else if (strstr(token, "name")) {
                sscanf(token, "name=%199s", tmp);
                url_decode(name, tmp);
                memset(tmp, 0, sizeof(tmp));
            }
            else if (strstr(token, "author")) {
                sscanf(token, "author=%199s", tmp);
                url_decode(author, tmp);
                memset(tmp, 0, sizeof(tmp));
            }
            else if (strstr(token, "borrower")) {
                sscanf(token, "borrower=%199s", tmp);
                url_decode(borrower, tmp);
                memset(tmp, 0, sizeof(tmp));
            }
            else if (strstr(token, "date")) {
                sscanf(token, "date=%199s", tmp);
                url_decode(date, tmp);
                memset(tmp, 0, sizeof(tmp));
            }
            else if (strstr(token, "action=")) {
                sscanf(token, "action=%29s", action);
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

                ids = malloc(len + 1);
                if (ids) {
                    strncpy(ids, start, len);
                    ids[len] = '\0';
                }

            }
            token = strtok(NULL, "&");
        }
        
        tList *list = from_file(media_path, DELIMITER, read_media);
        if (!list) {
            DEBUG_STR("Error: Failed to create list from file.\n");
            return 1;
        }

        if (strstr(action, "add") != NULL) {
            tMedia *media = malloc(sizeof(tMedia));
            if (media) {
                media->name = strdup(name);
                media->author = strdup(author);
                media->borrower = strdup(borrower);
                media->borrowed_date = strdup(date);

                insert_tail(list, media);
                }
        }

        tList *found = NULL;
        tMedia *search_query = malloc(sizeof(tMedia));

        if (!search_query) {
            puts("Search query is NULL.");
            DEBUG_STR("Error: Failed to allocate memory for the search_query item.\n");
            return 1;
        }

        search_query->name = strdup(query);
        search_query->author = strdup(query);
        search_query->borrower = strdup(query);
        search_query->borrowed_date = strdup(query);

        if (strlen(query) > 0) {
            found = search(list, _search_media, search_query);
            if (!found) {
                puts("Search failed. Found list is NULL.");
                DEBUG_STR("Error: Search failed. Found list is NULL.\n");
                return 1;
            }
        }
        else {
            found = list;
        }

        free(search_query->name);
        free(search_query->author);
        free(search_query->borrower);
        free(search_query->borrowed_date);
        free(search_query);

        if (found->length > 0) {
            int (*comp[4])(const void*, const void*) = {cmp_name, cmp_author, cmp_borrower, cmp_date};
            int idx = atoi(sort_key);
            if (idx < 0 || idx > 3) {
                idx = 0;
            }
            found = qsort_list(found, comp[idx]);
        }

        if (!found) {
            puts("Sort failed. List is NULL.");
            DEBUG_STR("Error: Sort failed. List is NULL.\n");
            return 1;
        }

        if (strstr(action, "delete_all") != NULL) { 
            // empty found list
            list_destroy(found);
            found = list_create();
        }

        if (strstr(action, "delete") != NULL && ids) {
            char *tok = strtok(ids, "~");
            int count = 0;
            while (tok != NULL) {
                int id = atoi(tok);
                id -= count;
                count++;

                _move_index(found, id);
                delete_node(found);
                tok = strtok(NULL, "~");
            }
        }
        if (ids) {
            free(ids);
        }

        if (found->length > 0) {
            _table_printer(found);
        }
        else {
            puts("<tr><td>No results found.</td></tr>\n");
        }

        // concat the rest and list together to save the whole list
        if (strlen(query) > 0) {
            found = concat_lists(found, list);
            if (!found) DEBUG_STR("Error: Failed concat list and found together.\n");
        }
        
        // Save the list to file
        if (to_file(found, media_path, DELIMITER, "w", &write_media) == 0) DEBUG_STR("Error: Failed saving found list to file\n");

        // Free the list
        list_destroy(found);
        }
    else {
        puts("Content-Type: text/plain\n");
        puts("No valid request method found."); 
        DEBUG_STR("Error: No valid request method found.\n");
    }

    return 0;
}