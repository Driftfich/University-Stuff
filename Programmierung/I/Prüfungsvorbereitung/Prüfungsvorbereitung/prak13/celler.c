#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include <ctype.h>
#include <string.h>

#define buf_size 256

int main() {
    char buf[buf_size];
    tList *list = CreateList();
    long count=0;
    char *element;
    while(1==1){
        printf("Enter (I)nsert / (O)ut / (Q)uit:\t");
        fgets(buf, sizeof(buf), stdin);
        buf[strlen(buf)-1] = '\0';
        for (int i=0; buf[i]!='\0'; i++) {
            buf[i]=tolower(buf[i]);
        }

        if (strstr("insert", buf) != NULL) {
            printf("Enter Text (max %d):\t", buf_size);
            fgets(buf, sizeof(buf), stdin);

            element=malloc(sizeof(char)*buf_size + 1);
            strcpy(element, buf);
            InsertTail(list, element);
            count++;
        }
        else if (strstr("out", buf) != NULL) {
            element=(char *) GetFirst(list);
            printf("%s", element);
            RemoveItem(list);
            free(element);
            count--;
        }
        else if (strstr("quit", buf) != NULL) {
            while ((element=GetFirst(list)) != NULL) {
                RemoveItem(list);
                free(element);
            }
            DeleteList(list);
            exit(0);
        }
    }
}
