#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
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

int main() {
    tList *list = from_file(media_path, ";", read_media);

    if (!list) {
        return 1;
    }

    int (*comp[4])(const void*, const void*) = {comp_name, comp_author, comp_borrower, comp_date};
    int idx = atoi("1");
    if (idx < 0 || idx > 3) {
        idx = 0;
    }
    list = sort(list, comp_name);
    _table_printer(list);

    // Save the list to file
    to_file(list, media_path, ";", "w", write_media);

    // Free the list
    list_destroy(list);
}
