#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#define _GNU_SOURCE
#include "list.h"

#ifndef debug
    #define debug 0
#endif

#ifdef _WIN32
    char *media_path = "C:\\Users\\fragf\\Documents\\Github\\C-Programming\\modules\\double_list\\media.csv";
    char *html_path = "C:\\Users\\fragf\\Documents\\Github\\C-Programming\\modules\\double_list\\test.html";
#elif __linux__
    char *media_path = "./media.csv";
    char *html_path = "./index.html";
#endif


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


//// Compare Methods for sorting 
int cmp_name(const void *media, const void *search) {
    if (!media || !search) return 0;

    tMedia *m1 = (tMedia*)media;
    tMedia *m2 = (tMedia*)search;

    // Null-Check für Strings
    if (!m1->author || !m2->author) return 0;

    return strcmp(m1->author, m2->author);
}

int cmp_author(const void *media, const void *search) {
    if (!media || !search) return 0;

    tMedia *m1 = (tMedia*)media;
    tMedia *m2 = (tMedia*)search;

    // Null-Check für Strings
    if (!m1->author || !m2->author) return 0;

    return strcmp(m1->author, m2->author);
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
    if (!media || !search) return 0;

    tMedia *m1 = (tMedia*)media;
    tMedia *m2 = (tMedia*)search;

    // Null-Check für Strings
    if (!m1->borrowed_date || !m2->borrowed_date) return 0;

    return strcmp(m1->borrowed_date, m2->borrowed_date);
}

int _search_media(const void*media_item, const void *search_item) {
    tMedia *media = (tMedia*) media_item;
    tMedia *search = (tMedia*) search_item;

    if (strcasestr(media->name, search->name) || strcasestr(media->author, search->author) || strcasestr(media->borrower, search->borrower) || strcasestr(media->borrowed_date, search->borrowed_date)) {
        return 0;
    }

    return 1;
}

// Methods for printing the media list to the html table
void _media_printer(void *item) {
    tMedia *media = (tMedia*) item;
    printf("Media: %s, Author: %s, Borrower: %s , Date: %s\n", media->name, media->author, media->borrower, media->borrowed_date);
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

void _row_printer(void *item, int row_idx) {
    tMedia *media = (tMedia*) item;

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

// Methods for file I/O with media instances
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
            tList *list = from_file(media_path, ";", read_media);
            
            if (!list) {
                _error_row(0);
            }
            else {
                if (list->length > 0) {
                    list = sort(list, cmp_name);
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
    // char *request_method = "GET";
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

        tList *list = from_file(media_path, ";", read_media);
        if (!list) {
            _error_row(0);
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
            _error_row(0);
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
        }
        
        // Save the list to file
        to_file(found, media_path, ";", "w", write_media);

        // Free the list
        list_destroy(found);
        }
    else {
        puts("Content-Type: text/plain\n");
        puts("No valid request method found."); 
    }

    return 0;
}