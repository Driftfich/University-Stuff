#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#define _GNU_SOURCE

#include "media.h"
#include "logger.h"
#include "utility.h"

tMedia *create_search_query(char *query) {
    if (!query) {
        DEBUG_STR("Warning: No query given.\n");
        return NULL;
    }

    tMedia *search_query = malloc(sizeof(tMedia));
    if (!search_query) {
        error_msg("Memory allocation failed for search query.\n");
    }
    
    search_query->name = strdup(query);
    search_query->author = strdup(query);
    search_query->borrower = strdup(query);
    search_query->borrowed_date = strdup(query);

    if (!search_query->name || !search_query->author || !search_query->borrower || !search_query->borrowed_date) {
        if (search_query->name) free(search_query->name);
        if (search_query->author) free(search_query->author);
        if (search_query->borrower) free(search_query->borrower);
        if (search_query->borrowed_date) free(search_query->borrowed_date);
        free(search_query);
        error_msg("Memory allocation failed for search query attributes.\n");
    }
    
    return search_query;
}

void free_media(tMedia *media) {
    if (!media) return;
    if (media->name) free(media->name);
    if (media->author) free(media->author);
    if (media->borrower) free(media->borrower);
    if (media->borrowed_date) free(media->borrowed_date);
    free(media);
    return;
}

tMedia *create_media(char *name, char*author, char*borrower, char*borrowed_date) {
    if (!name || !author || !borrower || !borrowed_date) {
        DEBUG_STR("Warning: At least one attribute was not given.\n");
        return NULL;
    }

    tMedia *new_media = malloc(sizeof(tMedia));
    if (!new_media) {
        error_msg("Memory allocation failed for new media item.\n");
    }
    
    new_media->name = strdup(name);
    new_media->author = strdup(author);
    new_media->borrower = strdup(borrower);
    new_media->borrowed_date = strdup(borrowed_date);

    if (!new_media->name || !new_media->author || !new_media->borrower || !new_media->borrowed_date) {
        free_media(new_media);
        error_msg("Memory allocation failed for at least one attribute of the new media item.\n");
    }
    
    return new_media;
}

//// Compare Methods for sorting 
int cmp_name(const void *media, const void *search) {
    if (!media || !search) return 0;

    tMedia *m1 =  *((tMedia**)media);
    tMedia *m2 = *((tMedia**)search);

    // Null-Check für Strings
    if (!m1->name || !m2->name) {
        DEBUG_STR("Error: Failed to compare media names. Name is NULL.\n");
        return 0;
    }

    return strcmp(m1->author, m2->author);
}

int cmp_author(const void *media, const void *search) {
    if (!media || !search) return 0;

    tMedia *m1 =  *((tMedia**)media);
    tMedia *m2 = *((tMedia**)search);

    // Null-Check für Strings
    if (!m1->author || !m2->author) {
        DEBUG_STR("Error: Failed to compare media author. Author is NULL.\n");
        return 0;
    }

    return strcmp(m1->author, m2->author);
}

int cmp_borrower(const void *media, const void *search) {
    if (!media || !search) return 0;
    
    tMedia *m1 =  *((tMedia**)media);
    tMedia *m2 = *((tMedia**)search);
    
    // Null-Check für Strings
    if (!m1->borrower || !m2->borrower) {
        DEBUG_STR("Error: Failed to compare media borrower. Borrower is NULL.\n");
        return 0;
    }
    
    return strcmp(m1->borrower, m2->borrower);
}

int cmp_date(const void *media, const void *search) {
    if (!media || !search) return 0;

    tMedia *m1 =  *((tMedia**)media);
    tMedia *m2 = *((tMedia**)search);

    // Null-Check für Strings
    if (!m1->borrowed_date || !m2->borrowed_date) {
        DEBUG_STR("Error: Failed to compare media borrowed_date. Borrowed date is NULL.\n");
        return 0;
    }

    return strcmp(m1->borrowed_date, m2->borrowed_date);
}

// Compare Method for Searching purpose
int _search_media(const void*media_item, const void *search_item) {
    tMedia *media = (tMedia*) media_item;
    tMedia *search = (tMedia*) search_item;

    if (strcasestr(media->name, search->name) || strcasestr(media->author, search->author) || strcasestr(media->borrower, search->borrower) || strcasestr(media->borrowed_date, search->borrowed_date)) {
        return 0;
    }

    return 1;
}

//// Methods for printing the media list to the html table
// Regular Printing Method for the cmd
int _media_printer(void *item) {
    tMedia *media = (tMedia*) item;
    return printf("Media: %s, Author: %s, Borrower: %s , Date: %s\n", media->name, media->author, media->borrower, media->borrowed_date);
}

// Printing media item to table row
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

// Methods for file I/O with media instances
void *read_media(FILE *file, char *delimiter) {
    char name[200], author[200], borrower[200], borrowed_date[200];
    char format[256] = {0};
    int ret;
    ret = snprintf(format, sizeof(format),
         "%%[^%s]%s"     // Name bis Delimiter
         "%%[^%s]%s"     // Author bis Delimiter
         "%%[^%s]%s"     // Borrower bis Delimiter
         "%%s\n",    // Date bis Zeilenende
         delimiter, delimiter,
         delimiter, delimiter,
         delimiter, delimiter);
    if (ret == -1) {
        DEBUG_STR("Error: Failed to create format string for reading media from file.\n");
        return NULL;
    }
    ret = fscanf(file, format,
                 name, author, borrower, &borrowed_date);
    if (ret != 4) {
        DEBUG_STR("Error: Failed to read media from file.\n");
        return NULL;
    }

    tMedia *media = malloc(sizeof(tMedia));
    if (!media) {
        DEBUG_STR("Error: Failed to allocate memory for media.\n");
        printf("Error allocating memory for media\n");
        return NULL;
    }

    // Strings duplizieren
    media->name = strdup(name);
    media->author = strdup(author);
    media->borrower = strdup(borrower);
    media->borrowed_date = strdup(borrowed_date);

    if (!media->name || !media->author || !media->borrower || !media->borrowed_date) {
        DEBUG_STR("Error: Failed to duplicate media strings.\n");
        if (media->name) free(media->name);
        if (media->author) free(media->author);
        if (media->borrower) free(media->borrower);
        if (media->borrowed_date) free(media->borrowed_date);
        free(media);
        return NULL;
    }

    return (void *) media;
}

int write_media(FILE *file, void *item, char *delimiter) {
    tMedia *media = (tMedia*) item;
    return fprintf(file, "%s%s%s%s%s%s%s\n", media->name, delimiter, media->author, delimiter, media->borrower, delimiter, media->borrowed_date);
}
