#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "list.h"

typedef struct BorrowedMediaItem {
    char *name;
    char *author;
    char *borrower;
} tMedia;

int _comp_cols(void *media, void *search, int offsets[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        int offset = offsets[i];
        if (media + offset == search + offset) {
            return 1;
        }
    }
    return 0;
}

int comp_media(void *media, void *search) {
    int offsets[] = {offsetof(tMedia, name)};
    return _comp_cols(media, search, offsets, sizeof(offsets) / sizeof(offsets[0]));
}

int comp_borrower(void *media, void *search) {
    int offsets[] = {offsetof(tMedia, borrower)};
    return _comp_cols(media, search, offsets, sizeof(offsets) / sizeof(offsets[0]));
}

void _item_printer(void *data) {
    tMedia *media = (tMedia*) data;
    printf("Media: %s, Author: %s, Borrower: %s\n", media->name, media->author, media->borrower);
    return;
}

int main () {
    tList *list = list_create();
    if (!list) {
        printf("Error creating list\n");
        return 1;
    }
    tMedia *query = (tMedia*) malloc(sizeof(tMedia));
    for (int i=0; i<10; i++) {
        tMedia *media = (tMedia*) malloc(sizeof(tMedia));
        // format i into the string
        media->name = (char*) malloc((strlen("Media") + 3)* sizeof(char));
        media->author = (char*) malloc((strlen("Author") + 3) * sizeof(char));
        media->borrower = (char*) malloc((strlen("Borrower") + 3) * sizeof(char));
        
        if (!media->name || !media->author || !media->borrower) {
            printf("Error allocating memory for media\n");
            return 1;
        }
        snprintf(media->name, 20, "Media %d", i);
        snprintf(media->author, 20, "Author %d", i);
        snprintf(media->borrower, 20, "Borrower %d", i);

        if (i == 5) {
            query = media;
        }
        
        insert_tail(list, media);
    }

    tList *found = search(list, comp_media, query);
    print_list(found, _item_printer);
    printf("\n\n");
    print_list(list, _item_printer);
}