#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#define _GNU_SOURCE

#include "media.h"
#include "logger.h"
#include "utility.h"
#include "web.h"

void free_media(tMedia *media) {
    // free all attributes of the media struct which are dynamically allocated
    if (!media) return;
    if (media->name) free(media->name);
    if (media->author) free(media->author);
    if (media->borrower) free(media->borrower);
    if (media->borrowed_date) free(media->borrowed_date);
    free(media);
    return;
}

tMedia *create_media(char *name, char*author, char*borrower, char*borrowed_date) {
    // Create a new media item with the given attributes + Checks if the attributes arent NULL
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

    return strcmp(m1->name, m2->name);
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

// Printing media item to table row with the given row index for identification
void _row_printer(void *item, int row_idx) {
    tMedia *media = (tMedia*) item;

    // url-decoding just happens here, because the data should be in the encoded format in the file, to make it work with standard delimiters
    char *name = url_decode(media->name);
    char *author = url_decode(media->author);
    char *borrower = url_decode(media->borrower);
    char *borrowed_date = url_decode(media->borrowed_date);

    if (!name) {
        DEBUG_STR("Error: Failed to decode media name.\n");
        name = "";
    }
    if (!author) {
        DEBUG_STR("Error: Failed to decode media author.\n");
        author = "";
    }
    if (!borrower) {
        DEBUG_STR("Error: Failed to decode media borrower.\n");
        borrower = "";
    }
    if (!borrowed_date) {
        DEBUG_STR("Error: Failed to decode media borrowed date.\n");
        borrowed_date = "";
    }
    puts("<tr class=clickable-row\n");
    puts("onclick=\"this.classList.toggle('row-selected')\"\n");
    printf("data-id=\"%d\">\n", row_idx);
    printf("<td>%s</td>\n", name);
    printf("<td>%s</td>\n", author);
    printf("<td>%s</td>\n", borrower);
    printf("<td>%s</td>\n", borrowed_date);
    puts("</tr>\n");

    if (name) free(name);
    if (author) free(author);
    if (borrower) free(borrower);
    if (borrowed_date) free(borrowed_date);

    return;
}

// Methods for file I/O with media instances
void *read_media(FILE *file, char *delimiter) {
    char *name = NULL, *author = NULL, *borrower = NULL, *borrowed_date = NULL;
    char *line = NULL;
    size_t len = 0;
    size_t read;
    
    // read complete line from file
    read = getline(&line, &len, file);
    if (read == -1) {
        DEBUG_STR("Error: Failed to read line from file.\n");
        free(line);
        return NULL;
    }
    if (!line) {
        DEBUG_STR("Error: Failed to read line from file.\n");
        return NULL;
    }

    // split the line into the attributes using strsep to handle empty fields
    char *tmp_line = line;
    name = strsep(&tmp_line, delimiter);
    author = strsep(&tmp_line, delimiter);
    borrower = strsep(&tmp_line, delimiter);
    borrowed_date = strsep(&tmp_line, "\n");

    // As author and date are optional, check if they are NULL and set them to empty string
    if (!author) author = "";
    if (!borrowed_date) borrowed_date = "";

    // create a new media item with the given attributes
    tMedia *media = create_media(name, author, borrower, borrowed_date);
    if (!media) {
        DEBUG_STR("Error: Failed to create media item from file.\n");
        free(line);
        return NULL;
    }

    free(line);
    return media;
}

int write_media(FILE *file, void *item, char *delimiter) {
    tMedia *media = (tMedia*) item;
    if (!media) {
        DEBUG_STR("Error: Failed to write media item to file. Media item is NULL.\n");
        return -1;
    }
    // Check if any of the media attributes are NULL
    if (!media->name || !media->author || !media->borrower || !media->borrowed_date) {
        DEBUG_STR("Error: One or more media attributes are NULL. Cannot write to file.\n");
        return -1;
    }
    return fprintf(file, "%s%s%s%s%s%s%s\n", media->name, delimiter, media->author, delimiter, media->borrower, delimiter, media->borrowed_date);
}
