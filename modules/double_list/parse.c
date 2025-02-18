#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "list.h"

#define debug 0

#ifdef _WIN32
    char *media_path = "C:\\Users\\fragf\\Documents\\Github\\C-Programming\\modules\\double_list\\media.csv";
    char *html_path = "C:\\Users\\fragf\\Documents\\Github\\C-Programming\\modules\\double_list\\test.html";
#elif __linux__
    char *media_path = "./media.csv";
    char *html_path = "./index.html";
#endif

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

void _error_row(int idx) {
    puts("<tr class=clickable-row\n");
    puts("onclick=\"this.classList.toggle('row-selected')\"\n");
    printf("data-id=\"%d\">\n", idx);
    for (int i=0; i<4; i++) printf("<td>Error</td>\n");
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
        if (media->name) free(media->name);
        if (media->author) free(media->author);
        if (media->borrower) free(media->borrower);
        if (media->borrowed_date) free(media->borrowed_date);
        free(media);
        return NULL;
    }

    return (void *) media;
}

typedef enum {
    TYPE_CHAR,
    TYPE_INT,
    TYPE_FLOAT
} tColumnType;

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
            // v1=tbl[(unsigned char)*in++];
            // v2=tbl[(unsigned char)*in++];
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

int main() {
    // char *encoded = "Hello World%21123";
    // char decoded[256];
    // url_decode(encoded, encoded);
    // printf("Decoded: %s\n", encoded);
    initial_page_load();
    return 0;
}