#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include "list.h"

#define debug 0

#ifdef _WIN32
    char *media_path = "C:\\Users\\Franz\\Documents\\Github\\C-Programming\\modules\\double_list\\media.csv";
    char *html_path = "C:\\Users\\Franz\\Documents\\Github\\C-Programming\\modules\\double_list\\test.html";
#elif __linux__
    char *media_path = "/var/www/html/media.csv";
    char *html_path = "/var/www/html/test.html";
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
    const void *ds1 = *(const void **)s1;
    const void *ds2 = *(const void **)s2;

    // Zugriff auf Felder mit Offset
    const void *att1 = (const void *)ds1 + offset;
    const void *att2 = (const void *)ds2 + offset;

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

int comp_name(const void *media, const void *search) {
    int offset = offsetof(tMedia, name);
    // media = (tMedia*) media;
    // search = (tMedia*) search;
    return _comp_col(media, search, offset, TYPE_CHAR);
}

int comp_author(const void *media, const void *search) {
    int offset = offsetof(tMedia, author);
    // media = (tMedia*) media;
    // search = (tMedia*) search;
    return _comp_col(media, search, offset, TYPE_CHAR);
}

int comp_borrower(const void *media, const void *search) {
    int offset = offsetof(tMedia, borrower);
    return _comp_col(media, search, offset, TYPE_CHAR);
}

int comp_date(const void *media, const void *search) {
    int offset = offsetof(tMedia, borrowed_date);
    // media = (tMedia*) media;
    // search = (tMedia*) search;
    return _comp_col(media, search, offset, TYPE_CHAR);
}

// void sort_by(tList *list, char sort_key) {
//     if (strstr(sort_key, "name") != NULL) {
//         sort(list, comp_name);
//     }
//     else if (strstr(sort_key, "author") != NULL) {
//         sort(list, comp_author);
//     }
//     else if (strstr(sort_key, "borrower") != NULL) {
//         sort(list, comp_borrower);
//     }
//     else if (strstr(sort_key, "borrowed_at") != NULL) {
//         sort(list, comp_date);
//     }

//     return;
// }

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
    tNode *tmp = list->head->nxt;
    int idx = 0;
    while (tmp != list->head) {
        _row_printer(tmp->data, idx);
        tmp = tmp->nxt;
        idx++;
    }
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

    puts("Content-Type: text/html\r\n\r\n");
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
    // Create the media.csv file if it doesnt exist
    FILE *file = fopen(media_path, "r");
    if (!file) {
        file = fopen(media_path, "w");
    }
    fclose(file);

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
        puts("Content-Type: text/html\r\n\r\n");

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
        // char *post_data = strdup("search=fsdg&sort_key=1");

        // print out the post data for debugging
        if (debug) {
            tMedia *custom = malloc(sizeof(tMedia));
            custom->author = strdup("");
            custom->borrower = strdup("");
            custom->borrowed_date = strdup("");
            custom->name = strdup(post_data);
            _row_printer(custom, -1);
            free(custom->author);
            free(custom->borrower);
            free(custom->borrowed_date);
            free(custom->name);
            free(custom);
        }

        char query[200] = {0}, name[200] = {0}, author[200] = {0};
        char borrower[200] = {0}, date[200] = {0}, action[10] = {0};
        char sort_key[30] = {0};
        
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
                sscanf(token, "action=%9s", action);
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
        // else if (strstr(action, "delete") != NULL) {
        //     // the table gets updated using htmx and just the backend list gets updated
        //     // pass
        // }


        // Save the list to file
        to_file(list, media_path, ";", "w", write_media);

        list = sort(list, comp_name);

        _table_printer(list);

        // Free the list
        list_destroy(list);
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