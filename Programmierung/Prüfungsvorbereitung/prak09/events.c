#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    char *name;
    int day;
    int mon;
    int year;
    char* note;
} tPers;

tPers *create_entry() {
    char buf[256];
    char *conv_err;
    unsigned int day, mon, year;
    tPers *new=malloc(sizeof(tPers));

    if (!new) return NULL;

    printf("Please enter a event name: ");
    fgets(buf, sizeof(buf),stdin);
    buf[strlen(buf)-1] = '\0';
    new->name = malloc(strlen(buf)+1);
    if (!new->name) {
        free(new);
        return NULL;
    }
    strcpy(new->name, buf);

    printf("Day: ");
    fgets(buf, sizeof(buf),stdin);
    buf[strlen(buf)-1] = '\0';
    day = strtoul(buf, &conv_err, 10);
    if (*conv_err != '\0') {
        printf("Werent able to parse %s to an integer. Error: %s\n", buf, conv_err);
        return NULL;
    }
    new->day = (int) day;

    printf("Month: ");
    fgets(buf, sizeof(buf),stdin);
    buf[strlen(buf)-1] = '\0';
    mon = strtoul(buf, &conv_err, 10);
    if (*conv_err != '\0') {
        printf("Werent able to parse %s to an integer\n", buf);
        return NULL;
    }
    new->mon = mon;

    printf("Year: ");
    fgets(buf, sizeof(buf),stdin);
    buf[strlen(buf)-1] = '\0';
    year = strtoul(buf, &conv_err, 10);
    if (*conv_err != '\0') {
        printf("Werent able to parse %s to an integer\n", buf);
        return NULL;
    }
    new->year = year;

    printf("Note: ");
    fgets(buf, sizeof(buf), stdin);
    buf[strlen(buf)-1] = '\0';
    new->note = malloc(strlen(buf)+1);
    if (!new->note) {
        free(new->name);
        free(new);
        return NULL;
    }
    strcpy(new->note, buf);

    return new;
}

void disblay_event(tPers *event) {
    for (int i=0; i<strlen(event->name)+7 || i<strlen(event->note)+7;i++) printf("=");
    puts("\n");
    printf("Name: %s\nDate: %d/%d/%d\nNote: %s\n", event->name, event->day, event->mon, event->year, event->note);
    return;
}

int main() {
    char buf[10];
    tPers *new, *pdata, *ptmp;
    int proceed = 1, count=1;
    while (proceed) {
        puts("Do you want to add a new event?[y|n]: ");
        fgets(buf, sizeof(buf), stdin);
        if (strstr(buf, "y") && !strstr(buf, "n")) {
            new=create_entry();
            if (!new) {
                puts("Error creating the new event. Please try again.\n");
                continue;
            }
            ptmp = realloc(pdata, count*sizeof(tPers));
            if (!ptmp) {
                puts("Error while adding new event. Please try again.\n");
                continue;
            }
            pdata = ptmp;
            pdata[count-1] = *new;
            count++;
        }
        else break;
    }

    for (int i=1; i<count; i++) {
        disblay_event(pdata + i-1);
        free(pdata[i].note);
        free(pdata[i].name);
    }
    free(ptmp);
}
