/*
* Author: Franz Rehschuh
* Date: 2025-02-25
* Description: Header file for managing media items, including creation, comparison, printing, and I/O operations.
*/
#ifndef MEDIA_H
#define MEDIA_H

typedef struct BorrowedMediaItem {
    char *name;
    char *author;
    char *borrower;
    char *borrowed_date;
} tMedia;

// creation and destruction
void free_media(tMedia *media);
tMedia *create_media(char *name, char*author, char*borrower, char*borrowed_date);

// compare functions for qsort
int cmp_name(const void *media, const void *search);
int cmp_author(const void *media, const void *search);
int cmp_borrower(const void *media, const void *search);
int cmp_date(const void *media, const void *search);

// compare function for search
int _search_media(const void *media, const void *search);

// printer functions
int _media_printer(void *item);
void _row_printer(void *item, int row_idx);

// Methods for file I/O with media instances
void *read_media(FILE *file, char *delimiter);
int write_media(FILE *file, void *item, char *delimiter);

#endif