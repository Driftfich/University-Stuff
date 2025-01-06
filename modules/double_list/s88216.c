#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#define _GNU_SOURCE
#include "list.h"

#define debug 0

#ifdef _WIN32
    char *media_path = "C:\\Users\\fragf\\Documents\\Github\\C-Programming\\modules\\double_list\\media.csv";
    char *html_path = "C:\\Users\\fragf\\Documents\\Github\\C-Programming\\modules\\double_list\\test.html";
#elif __linux__
    char *media_path = "./media.csv";
    char *html_path = "./test.html";
    // char *html_path = "/home/USER/Dokumente/Github/C-Programming/modules/double_list/test.html";
#endif

typedef enum {
    TYPE_CHAR,
    TYPE_INT,
    TYPE_FLOAT
} tColumnType;

int _comp_int(const void *s1, const void *s2) {
    float f1 = *(int*)s1;
    float f2 = *(int*)s2;

    if (f1 < f2) return -1;
    if (f1 > f2) return 1;
    return 0;
}

int _comp_float(const void *s1, const void *s2) {
    float f1 = *(float*)s1;
    float f2 = *(float*)s2;

    if (f1 < f2) return -1;
    if (f1 > f2) return 1;
    return 0;
}

int _comp_col(const void *s1, const void *s2, int offset, tColumnType type) {
    // Korrekte Pointer-Handhabung für qsort
    const void *att1 = *((const void **)&s1 + offset);
    const void *att2 = *((const void **)&s2 + offset);

    // const char *att1 = (const char *)s1 + offset;
    // const char *att2 = (const char *)s2 + offset;

    int res;
    if (type == TYPE_INT) {
        res = _comp_int(att1, att2);
    } else if (type == TYPE_FLOAT) {
        res = _comp_float(att1, att2);
    } else {
        res = strcmp(*(const char**)att1, *(const char**)att2);
    }
    
    if (debug) {
        // Debug-Ausgabe
        if (type == TYPE_INT) {
            printf("Comparing ints: %d, %d\n", 
                *(const int*)att1, *(const int*)att2);
        } else if (type == TYPE_FLOAT) {
            printf("Comparing floats: %f, %f\n", 
                *(const float*)att1, *(const float*)att2);
        } else {
            printf("Comparing strings: %s, %s\n", 
                *(const char**)att1, *(const char**)att2);
        }

        printf("Result: %d\n", res);
    }

    return res;
}

int cmp_name(const void *d1, const void *d2) {
    int offset = offsetof(tMedia, name);
    return _comp_col(d1, d2, offset, TYPE_CHAR);
}

int cmp_author(const void *media, const void *search) {
    int offset = offsetof(tMedia, author);
    return _comp_col(media, search, offset, TYPE_CHAR);
}

int cmp_borrower(const void *s1, const void *s2) {
    if (!s1 || !s2) return 0;
    
    tMedia *m1 = (tMedia*)s1;
    tMedia *m2 = (tMedia*)s2;
    
    // Null-Check für Strings
    if (!m1->borrower || !m2->borrower) return 0;
    
    return strcmp(m1->borrower, m2->borrower);
}

int cmp_date(const void *media, const void *search) {
    // int offset = offsetof(tMedia, borrowed_date);
    // return _comp_col(media, search, offset, TYPE_CHAR);

    if (!media || !search) return 0;

    tMedia *m1 = (tMedia*)media;
    tMedia *m2 = (tMedia*)search;

    // Null-Check für Strings
    if (!m1->borrowed_date || !m2->borrowed_date) return 0;

    return strcmp(m1->borrowed_date, m2->borrowed_date);
}

// int strcasestr(const char *haystack, const char *needle) {
//     char *haystack_lower = strdup(haystack);
//     char *needle_lower = strdup(needle);
//     if (!haystack_lower || !needle_lower) {
//         free(haystack_lower);
//         free(needle_lower);
//         return 0;
//     }

//     for (int i=0; haystack_lower[i]; i++) {
//         haystack_lower[i] = tolower(haystack_lower[i]);
//     }
//     for (int i=0; needle_lower[i]; i++) {
//         needle_lower[i] = tolower(needle_lower[i]);
//     }

//     int ret = strstr(haystack_lower, needle_lower) != NULL;
//     free(haystack_lower);
//     free(needle_lower);
//     return ret;
// }

int _search_media(const void*media_item, const void *search_item) {
    tMedia *media = (tMedia*) media_item;
    tMedia *search = (tMedia*) search_item;

    if (strcasestr(media->name, search->name) || strcasestr(media->author, search->author) || strcasestr(media->borrower, search->borrower) || strcasestr(media->borrowed_date, search->borrowed_date)) {
        return 0;
    }
    // if (strstr(media->name, search->name) || strstr(media->author, search->author) || strstr(media->borrower, search->borrower) || strstr(media->borrowed_date, search->borrowed_date)) {
    //     return 0;
    // }

    return 1;
}

void _media_printer(void *item) {
    tMedia *media = (tMedia*) item;
    printf("Media: %s, Author: %s, Borrower: %s , Date: %s\n", media->name, media->author, media->borrower, media->borrowed_date);
    return;
}

void _row_printer(void *item, int row_idx) {
    tMedia *media = (tMedia*) item;

    // printf("Media: %s, Author: %s, Borrower: %s\n", media->name, media->author, media->borrower);
    puts("<tr class=clickable-row\n");
    puts("onclick=\"this.classList.toggle('row-selected')\"\n");
    printf("data-id=\"%d\">\n", row_idx);
    printf("<td>%s</td>\n", media->name);
    printf("<td>%s</td>\n", media->author);
    printf("<td>%s</td>\n", media->borrower);
    printf("<td>%s</td>\n", media->borrowed_date);
    puts("</tr>\n");
    return;
}

void _table_printer(tList *list) {
    if (!list) {
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

void *read_media(FILE *file, char *delimiter) {
    char name[200], author[200], borrower[200], borrowed_date[200];
    char format[256] = {0};
    int ret;
    snprintf(format, sizeof(format),
         "%%[^%s]%s"     // Name bis Delimiter
         "%%[^%s]%s"     // Author bis Delimiter
         "%%[^%s]%s"     // Borrower bis Delimiter
         "%%s\n",    // Date bis Zeilenende
         delimiter, delimiter,
         delimiter, delimiter,
         delimiter, delimiter);
    ret = fscanf(file, format,
                 name, author, borrower, &borrowed_date);
    if (ret != 4) {
        return NULL;
    }

    tMedia *media = malloc(sizeof(tMedia));
    if (!media) {
        printf("Error allocating memory for media\n");
        return NULL;
    }

    // Strings duplizieren
    media->name = strdup(name);
    media->author = strdup(author);
    media->borrower = strdup(borrower);
    media->borrowed_date = strdup(borrowed_date);

    if (!media->name || !media->author || !media->borrower || !media->borrowed_date) {
        free(media->name);
        free(media->author);
        free(media->borrower);
        free(media);
        return NULL;
    }

    return (void *) media;
}

void write_media(FILE *file, void *item, char *delimiter) {
    tMedia *media = (tMedia*) item;
    fprintf(file, "%s%s%s%s%s%s%s\n", media->name, delimiter, media->author, delimiter, media->borrower, delimiter, media->borrowed_date);
    return;
}

void write_int(FILE *file, void *item, char *delimiter) {
    int *num = (int*) item;
    fprintf(file, "%d%s", *num, delimiter);
    return;
}

void _error_row(int idx) {
    puts("<tr class=clickable-row\n");
    puts("onclick=\"this.classList.toggle('row-selected')\"\n");
    printf("data-id=\"%d\">\n", idx);
    for (int i=0; i<4; i++) printf("<td>Error</td>\n");
    puts("</tr>\n");
    return;
}

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
            // Load the list from file
            tList *list = from_file(media_path, ";", read_media);
            
            if (!list) {
                _error_row(0);
            }
            else {
                list = sort(list, cmp_name);
                // Print the list as a table
                _table_printer(list);
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
    // char *request_method = "POST";
    if (request_method == NULL) {
        puts("No request method found.");
        return 1;
    }

    if (strcmp(request_method, "GET") == 0) {
        initial_page_load();
    }
    else if (strcmp(request_method, "POST") == 0) {

        // read in query string
        char *content_length = getenv("CONTENT_LENGTH");
        if (content_length == NULL) {
            puts("No content length found.");
            return 1;
        }

        int length = atoi(content_length) + 1;
        char post_data[length];

        if (fgets(post_data, length, stdin) == NULL) {
            puts("No post data found.");
            return 1;
        }
        // char *post_data = strdup("sort_key=2&search=100");
        // int length = strlen(post_data);
        // print out the post data for debugging
        

        char query[200] = {0}, name[200] = {0}, author[200] = {0};
        char borrower[200] = {0}, date[200] = {0}, action[30] = {0};
        char sort_key[30] = {0};
        char *ids = NULL;

        // tMedia *custom = malloc(sizeof(tMedia));
        // if (custom) {
        //     custom->name = strdup(post_data);
        //     custom->author = strdup("");
        //     custom->borrower = strdup("");
        //     custom->borrowed_date = strdup("");
        //     _row_printer(custom, 0);

        //     free(custom->name);
        //     free(custom->author);
        //     free(custom->borrower);
        //     free(custom->borrowed_date);
        //     free(custom);
        // }

        // Token-Parsing Schleife
        char *token = strtok(post_data, "&");
        while (token != NULL) {
            if (strstr(token, "search")) {
                sscanf(token, "search=%199s", query);
            }
            else if (strstr(token, "sort_key")) {
                sscanf(token, "sort_key=%29s", sort_key);
            }
            else if (strstr(token, "name")) {
                sscanf(token, "name=%199s", name);
            }
            else if (strstr(token, "author")) {
                sscanf(token, "author=%199s", author);
            }
            else if (strstr(token, "borrower")) {
                sscanf(token, "borrower=%199s", borrower);
            }
            else if (strstr(token, "date")) {
                sscanf(token, "date=%199s", date);
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

        tList *list = from_file(media_path, ";", read_media);
        if (!list) {
            _error_row(0);
            return 1;
        }

        // tMedia *custom2 = malloc(sizeof(tMedia));
        //     if (custom2) {
        //         custom2->name = strdup(action);
        //         custom2->author = strdup("");
        //         custom2->borrower = strdup("");
        //         custom2->borrowed_date = strdup("");
        //         _row_printer(custom2, 0);
        //     }

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
        if (strstr(action, "delete_all") != NULL) {
            // _row_printer(custom2, 0);
            // _row_printer(custom2, 0);

            // free(custom2->name);
            // free(custom2->author);
            // free(custom2->borrower);
            // free(custom2->borrowed_date);
            // free(custom2);
            
            // remove list
            list_destroy(list);
            
            // write empty string to file
            FILE *file = fopen(media_path, "w");
            fprintf(file, "");
            fclose(file);

            puts("");

            return 0;
        }

        tList *found = NULL;
        tMedia *search_query = malloc(sizeof(tMedia));

        if (!search_query) {
            puts("Search query is NULL.");
            _error_row(0);
            return 1;
        }

        // printf("Query: %s\n", query);
        // printf("Sort key: %s\n", sort_key);

        search_query->name = strdup(query);
        search_query->author = strdup(query);
        search_query->borrower = strdup(query);
        search_query->borrowed_date = strdup(query);
        if (strlen(query) > 0) {
            found = search(list, _search_media, search_query);
            if (!found) {
                puts("Search failed. Found list is NULL.");
                _error_row(0);
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
            found = sort(found, comp[idx]);
        }

        if (!found) {
            puts("Sort failed. List is NULL.");
            _error_row(0);
            return 1;
        }

        if (strstr(action, "delete") != NULL && ids) {
            char *tok = strtok(ids, "~");
            int count = 0;
            while (tok != NULL) {
                int id = atoi(tok);
                id -= count;
                count++;
                // printf("Deleting id: %d\n", id);
                _move_index(found, id);
                delete_node(found);
                tok = strtok(NULL, "~");
            }
        }
        if (ids) {
            free(ids);
        }

        // puts("Found list:");
        if (found->length > 0) {
            _table_printer(found);
        }
        else {
            puts("No results found.");
        }
        // puts("Rest list:");
        // _table_printer(list);

        // concat the rest and list together to save the whole list
        if (strlen(query) > 0) {
            list = concat_lists(list, found);
        }

        // puts("Complete list:");
        // _table_printer(list);
        
        // // Save the list to file
        to_file(list, media_path, ";", "w", write_media);

        // Free the list
        list_destroy(list);
        // list_destroy(found);
        // list_destroy(new_list);
        // list_destroy(found);
        }
    else {
        puts("Content-Type: text/plain\n");
        puts("No valid request method found."); 
    }

    return 0;
}

// int main() {
//     tList *list = from_file(media_path, ";", read_media);
//     list = sort(list, comp_name);
//     free(list);
// }