#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include "list.h"

#define debug 0
char *media_path = "C:\\Users\\fragf\\Documents\\Github\\C-Programming\\modules\\double_list\\media.csv";

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
    const void *ds1 = *(const void **)s1;
    const void *ds2 = *(const void **)s2;

    // Zugriff auf Felder mit Offset
    const void *att1 = (const void *)ds1 + offset;
    const void *att2 = (const void *)ds2 + offset;

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
    }

    int res;
    if (type == TYPE_INT) {
        res = _comp_int(att1, att2);
    } else if (type == TYPE_FLOAT) {
        res = _comp_float(att1, att2);
    } else {
        res = strcmp(*(const char**)att1, *(const char**)att2);
    }
    
    if (debug) {
        // print result
        printf("Result: %d\n", res);
    }

    return res;
}

int cmp_name(const void *media, const void *search) {
    int offset = offsetof(tMedia, name);
    return _comp_col(media, search, offset, TYPE_CHAR);
}

int cmp_author(const void *media, const void *search) {
    int offset = offsetof(tMedia, author);
    return _comp_col(media, search, offset, TYPE_CHAR);
}

int cmp_borrower(const void *media, const void *search) {
    int offset = offsetof(tMedia, borrower);
    return _comp_col(media, search, offset, TYPE_CHAR);
}

int cmp_date(const void *media, const void *search) {
    int offset = offsetof(tMedia, borrowed_date);
    // media = (tMedia*) media;
    // search = (tMedia*) search;
    return _comp_col(media, search, offset, TYPE_CHAR);
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

int strcasestr(const char *haystack, const char *needle) {
    char *haystack_lower = strdup(haystack);
    char *needle_lower = strdup(needle);
    if (!haystack_lower || !needle_lower) {
        free(haystack_lower);
        free(needle_lower);
        return 0;
    }

    for (int i=0; haystack_lower[i]; i++) {
        haystack_lower[i] = tolower(haystack_lower[i]);
    }
    for (int i=0; needle_lower[i]; i++) {
        needle_lower[i] = tolower(needle_lower[i]);
    }

    int ret = strstr(haystack_lower, needle_lower) != NULL;
    free(haystack_lower);
    free(needle_lower);
    return ret;
}

int _search_media(const void*media_item, const void *search_item) {
    tMedia *media = (tMedia*) media_item;
    tMedia *search = (tMedia*) search_item;

    if (strcasestr(media->name, search->name) || strcasestr(media->author, search->author) || strcasestr(media->borrower, search->borrower) || strcasestr(media->borrowed_date, search->borrowed_date)) {
        return 0;
    }

    return 1;
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

int main() {
    tList *list = from_file(media_path, ";", read_media);
    tList *found = search(list, _search_media, "0");

    _table_printer(found);

    list = concat_lists(list, found);

    _table_printer(list);
}
