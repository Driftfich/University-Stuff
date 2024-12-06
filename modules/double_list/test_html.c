#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main()
{
    char buf[2048];
    FILE *F;
    F = fopen("/var/www/html/test.html", "rt");
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