#include <stdio.h>
#include <stdlib.h>

#ifndef MEDIA_H
#define MEDIA_H

typedef struct BorrowedMediaItem {
    char *name;
    char *author;
    char *borrower;
    char *borrowed_date;
} tMedia;

tMedia *create_search_query(char *query);

int cmp_name(const void *media, const void *search);
int cmp_author(const void *media, const void *search);
int cmp_borrower(const void *media, const void *search);
int cmp_date(const void *media, const void *search);

int _search_media(const void *media, const void *search);

int _media_printer(void *item);
void _row_printer(void *item, int row_idx);

void *read_media(FILE *file, char *delimiter);
int write_media(FILE *file, void *item, char *delimiter);

#endif