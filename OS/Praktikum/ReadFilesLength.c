#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main()

{
    DIR *d;
    struct dirent *dir;

    d = opendir(".");

    if (d)
    {
        printf("Files in current directory:\n");

        while ((dir = readdir(d)) != NULL)
        {
            printf("%s\n", dir->d_name);
        }
    }
    else
    {
        perror("Error opening directory");
        return EXIT_FAILURE;
    }

    return 0;
}