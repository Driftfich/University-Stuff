#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "list.h"

#define debug 0

typedef enum {
    TYPE_CHAR,
    TYPE_INT,
    TYPE_FLOAT
} tColumnType;

int _comp_int(const void *s1, const void *s2) {
    // Dynamische Größenbestimmung basierend auf tMedia Struktur
    size_t field_size = sizeof(((tMedia*)0)->bowrrowed_date);
    
    // Binärer Vergleich der Speicherbereiche
    int result = memcmp(s1, s2, sizeof(long));
    
    // Standardisierte Rückgabewerte für Vergleich
    if (result < 0) return -1;
    if (result > 0) return 1;
    return 0;
}

int _comp_float(const void *s1, const void *s2) {
    // Dynamische Größenbestimmung basierend auf tMedia Struktur
    size_t field_size = sizeof(((tMedia*)0)->bowrrowed_date);
    
    // Binärer Vergleich der Speicherbereiche
    int result = memcmp(s1, s2, sizeof(double));
    
    // Standardisierte Rückgabewerte für Vergleich
    if (result < 0) return -1;
    if (result > 0) return 1;
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

int comp_media(const void *media, const void *search) {
    int offset = offsetof(tMedia, name);
    // media = (tMedia*) media;
    // search = (tMedia*) search;
    return _comp_col(media, search, offset, TYPE_CHAR);
}

int comp_borrower(void *media, void *search) {
    int offset = offsetof(tMedia, borrower);
    return _comp_col(media, search, offset, TYPE_CHAR);
}

void _item_printer(void *data) {
    tMedia *media = (tMedia*) data;
    printf("Media: %s, Author: %s, Borrower: %s\n", media->name, media->author, media->borrower);
    return;
}

int _initial_page_load() {
    char buf[2048];
    FILE *F;
    #ifdef _WIN32
        {F = fopen("C:\\Users\\fragf\\Documents\\Github\\C-Programming\\modules\\double_list\\test.html", "rt");}
    #elif __linux__
        {F = fopen("/var/www/html/test.html", "rt");}
    #endif

    printf("Content-Type: text/html\r\n\r\n");
    if (F == NULL) {
        puts("<html><head><title><p>Dateifehler<p></title></body></html>");
        return 1;
    }
    while (fgets(buf, sizeof(buf), F))
    {
        printf("%s", buf);
    }
    fclose(F);
    return 0;
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
    media->bowrrowed_date = borrowed_date;

    if (!media->name || !media->author || !media->borrower) {
        free(media);
        return NULL;
    }

    return (void *) media;
}

void write_media(FILE *file, void *item, char *delimiter) {
    tMedia *media = (tMedia*) item;
    fprintf(file, "%s%s%s%s%s%s%lu\n", media->name, delimiter, media->author, delimiter, media->borrower, delimiter, media->bowrrowed_date);
    return;
}

int main () {
    // char *delim = ";";

    // tList *list = from_file("test.txt", delim, read_media);
    // list = sort(list, comp_media);
    // print_list(list, _item_printer);

    printf("%d\n", strcmp("Media 1", "Media 15"));
    printf("%d\n", strcmp("Media 2", "Media 15"));

    return 0;
}