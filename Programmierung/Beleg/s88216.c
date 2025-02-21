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
#include "utility.h"
#include "config.h"

FILE *ProtFile;

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