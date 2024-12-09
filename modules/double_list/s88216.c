#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "list.h"

typedef enum {
    TYPE_CHAR,
    TYPE_NUMERIC
} tColumnType;

int _comp_num(void *media, void *search, int offset) {
    void *media_offset = media + offset;
    void *search_offset = search + offset;

    if (media_offset < search_offset) {
        return -1;
    } else if (media_offset > search_offset) {
        return 1;
    } else {
        return 0;
    }
}

int _comp_col(void *media, void *search, int offset, tColumnType type) {
    if (type == TYPE_NUMERIC) {
        return _comp_num(media, search, offset);
    } else {
        return strcmp((media + offset), (search + offset));
    }
}

int comp_media(void *media, void *search) {
    int offset = offsetof(tMedia, name);
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

int main () {
    // tList *list = list_create();
    // if (!list) {
    //     printf("Error creating list\n");
    //     return 1;
    // }
    // tMedia *query = (tMedia*) malloc(sizeof(tMedia));
    // for (int i=0; i<10; i++) {
    //     tMedia *media = (tMedia*) malloc(sizeof(tMedia));
    //     // format i into the string
    //     media->name = (char*) malloc((strlen("Media") + 3)* sizeof(char));
    //     media->author = (char*) malloc((strlen("Author") + 3) * sizeof(char));
    //     media->borrower = (char*) malloc((strlen("Borrower") + 3) * sizeof(char));
        
    //     if (!media->name || !media->author || !media->borrower) {
    //         printf("Error allocating memory for media\n");
    //         return 1;
    //     }
    //     snprintf(media->name, 20, "Media %d", i);
    //     snprintf(media->author, 20, "Author %d", i);
    //     snprintf(media->borrower, 20, "Borrower %d", i);

    //     if (i == 5) {
    //         query = media;
    //     }
        
    //     insert_tail(list, media);
    // }

    // tList *found = search(list, comp_media, query);
    // print_list(found, _item_printer);
    // printf("\n\n");
    // print_list(list, _item_printer);

//     char *query = getenv("QUERY_STRING");
//     char *request_method = getenv("REQUEST_METHOD");

//     if (strcmp(request_method, "GET") == 0) {
//         _initial_page_load();
//     } else if (strcmp(request_method, "POST") == 0) {
//         // parse the query string
//         // if the query string is empty, return the initial page
// }
    _initial_page_load();
    return 0;
}