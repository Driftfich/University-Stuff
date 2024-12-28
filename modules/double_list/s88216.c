#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include "list.h"

#define debug 0

#ifdef _WIN32
    char *path = "C:\\Users\\Franz\\Documents\\Github\\C-Programming\\modules\\double_list\\media.csv";
#elif __linux__
    char *path = "/var/www/html/media.csv";
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
    return _comp_col(media, search, offset, TYPE_INT);
}

void _media_printer(void *item) {
    tMedia *media = (tMedia*) item;
    printf("Media: %s, Author: %s, Borrower: %s\n", media->name, media->author, media->borrower);
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
    printf("<td>%lu</td>\n", media->borrowed_date);
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
    char name[200], author[200], borrower[200];
    unsigned long borrowed_date;
    char format[256] = {0};
    int ret;
    snprintf(format, 256, "%%[^%s]%s%%[^%s]%s%%[^%s]%s%%lu\n", delimiter, delimiter, delimiter, delimiter, delimiter, delimiter);
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
    media->borrowed_date = borrowed_date;

    if (!media->name || !media->author || !media->borrower) {
        free(media);
        return NULL;
    }

    return (void *) media;
}

void write_media(FILE *file, void *item, char *delimiter) {
    tMedia *media = (tMedia*) item;
    fprintf(file, "%s%s%s%s%s%s%lu\n", media->name, delimiter, media->author, delimiter, media->borrower, delimiter, media->borrowed_date);
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
    #ifdef _WIN32
        {F = fopen("C:\\Users\\Franz\\Documents\\Github\\C-Programming\\modules\\double_list\\test.html", "rt");}
    #elif __linux__
        {F = fopen("/var/www/html/test.html", "rt");}
    #endif

    // Create the file if it does not exist
    FILE *file = fopen(path, "w");
    fclose(file);

    puts("Content-Type: text/html\r\n\r\n");
    if (F == NULL) {
        puts("<html><head><title><p>Dateifehler<p></title></body></html>");
        return 1;
    }
    while (fgets(buf, sizeof(buf), F))
    {   
        if (strstr(buf, "<tbody id=\"table-body\">") != NULL) {
            // Load the list from file
            tList *list = from_file(path, ";", read_media);
            
            if (!list) {
                _error_row(0);
                return 1;
            }

            tMedia *custom = malloc(sizeof(tMedia));
            if (custom) {
                custom->name = strdup("Custom");
                custom->author = strdup("Author");
                custom->borrower = strdup("Borrower");
                custom->borrowed_date = 0;

                insert_tail(list, custom);

                free(custom);
            }

            // Print the list as a table
            _table_printer(list);
        }
        printf("%s", buf);
    }
    fclose(F);
    return 0;
}

int main (int argc, char *argv[], char*env[]) {
    // char **pEnv = env;

    char *request_method = getenv("REQUEST_METHOD");
    if (request_method == NULL) {
        puts("No request method found.");
        return 1;
    }

    if (strcmp(request_method, "GET") == 0) {
        initial_page_load();
    }
    else if (strcmp(request_method, "POST") == 0) {
        printf("Content-Type: text/html\r\n\r\n");
        _error_row(0);
        _error_row(1);
        _error_row(2);
        // read in query string
        // char *content_length = getenv("CONTENT_LENGTH");
        // if (content_length == NULL) {
        //     puts("No content length found.");
        //     return 1;
        // }

        // int length = atoi(content_length);
        // char post_data[length];

        // if (fgets(post_data, length, stdin) == NULL) {
        //     puts("No post data found.");
        //     return 1;
        // }

        // // lookup for action using strstr
        // if (strstr(post_data, "action=add") != NULL) {
        //     // the table gets updated using htmx and just the backend list gets updated
        //     // printf("Content-Type: text/html\r\n\r\n");
        
        //     // char name[200], author[200], borrower[200], date[200];
        //     // unsigned long borrowed_date;
        //     // char format[256] = {0};

        //     // sscanf(post_data, "name=%s&author=%s&borrower=%s&date=%s", name, author, borrower, date);

        //     // tMedia *media = malloc(sizeof(tMedia));
        //     // if (!media) {
        //     //     _error_row();
        //     //     return 1;
        //     // }

        //     // // Strings duplizieren
        //     // media->name = strdup(name);
        //     // media->author = strdup(author);
        //     // media->borrower = strdup(borrower);
        //     // media->borrowed_date = 0;
        //     // // media->borrowed_date = borrowed_date; // convert the string to a long int date representation first

        //     // if (!media->name || !media->author || !media->borrower) {
        //     //     free(media);
        //     //     return 1;
        //     // }

        //     // // Print the new row at the end of the table
        //     printf("Content-Type: text/html\r\n\r\n");
        //     // _row_printer(media, list->length - 1);
        //     // tMedia *custom = malloc(sizeof(tMedia));
        //     // // tList *list = from_file(path, ";", read_media);
        //     // tList *list = list_create();

        //     // if (list) {
        //     //     if (custom) {
        //     //         custom->name = strdup("Custom");
        //     //         custom->author = strdup("Author");
        //     //         custom->borrower = strdup("Borrower");
        //     //         custom->borrowed_date = 0;

        //     //         insert_tail(list, custom);

        //     //         free(custom);
        //     //     }

        //     //     // Print the list as a table
        //     //     _table_printer(list);

        //     //     // Save the list to file
        //     //     // to_file(list, path, ";", "w", write_media);

        //     //     // Free the list
        //     //     list_destroy(list);
        //     // Test-Ausgabe
        //     // }
        //     _error_row(0);
        //     _error_row(1);
        // }
        // else if (strstr(post_data, "action=delete") != NULL) {
        //     // the table gets updated using htmx and just the backend list gets updated
        //     puts("Content-Type: text/html\r\n\r\n");
        //     printf("%s", post_data);
        // }
        // else {
        //     // // Handles changed query string and sorting
        //     char search_term[200] = "";
        //     char sort_by[20] = "";
        //     int sort_key = 0;
            
        //     // Select the search term and sort key with different order
        //     char *token = strtok(post_data, "&");
        //     while (token != NULL) {
        //         if (strstr(token, "search") != NULL) {
        //             sscanf(token, "search=%s", search_term);
        //         }
        //         else if (strstr(token, "sort") != NULL) {
        //             sscanf(token, "sort=%s", sort_by);
        //         }
        //     }

        //     // Load the list from file
        //     tList *list = from_file(path, ";", read_media);

        //     // Search for the search term
        //     list = search(list, comp_name, search_term);

        //     // Sort the list by the sort key
        //     // sort_key = atoi(sort_by);
        //     if (sort_by == "Name") {
        //         sort(list, comp_name);
        //     }
        //     else if (sort_by == "Author") {
        //         sort(list, comp_author);
        //     }
        //     else if (sort_by == "Borrower") {
        //         sort(list, comp_borrower);
        //     }
        //     else if (sort_by == "Borrowed Date") {
        //         sort(list, comp_date);
        //     }

        //     // Print the list as a table
        //     _table_printer(list);
        // }
        }
    else {
        puts("Content-Type: text/plain\n");
        puts("No valid request method found.");   
    }

    return 0;
}