#include <stdio.h>
#include <ctype.h>

void main()
{
    int i,j;
    for (i=0; i<4; i++)
    {
        printf("| dec hex Char ");
    }
    printf("|\n");

    for (i=0; i<128/4; i++)
    {
        printf("\n| ");
        for (j=0; j<i + (384/4) + 1; j+=128/4)
        {
            int combined=i+j;
            printf("%3d %2X ", combined, combined);
            if (isgraph(combined))
            {
                printf("  %c   | ", combined);
            }
            else
            {
                printf("  .   | ");
            }
        }
    }

    printf("\n");
}