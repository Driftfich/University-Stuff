#include <stdio.h>
#include "list.h"
#include <stdlib.h>
#include <string.h>

char buf[128];
int main(){
    tList* list = CreateList();
    printf("Enter (I)nsert / (O)ut / (Q)uit:");
    fgets(buf, 128 , stdin);
    while(buf[0] != 'Q' && buf[0] != 'q'){
        if(buf[0] == 'I' | buf[0] == 'i'){
            printf("Insert String: ");
            fgets(buf, 128 , stdin);
            char* data = malloc(strlen(buf));
            strcpy(data, buf);
            InsertHead(list, data);
        }
        if(buf[0] == 'O'|buf[0] == 'o'){
            char* data = GetFirst(list);
            printf("First String: %s", data);
            RemoveItem(list);
            free(data);
        }
        printf("Enter (I)nsert / (O)ut / (Q)uit:");
        fgets(buf, 128 , stdin);
    }
    DeleteList(list);
return 0;
}
