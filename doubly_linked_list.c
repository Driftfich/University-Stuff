
#include <stdio.h>
#include <stdlib.h>
#include "doubly_linked_list.h"

List* createList() {
    List* list = (List*)malloc(sizeof(List));
    list->header = (Node*)malloc(sizeof(Node));
    list->header->next = list->header;
    list->header->prev = list->header;
    list->size = 0;
    return list;
}

void insertAfter(List* list, Node* node, int ar) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->ar = ar;
    
    if (node == NULL) node = list->header;
    
    newNode->next = node->next;
    newNode->prev = node;
    node->next->prev = newNode;
    node->next = newNode;
    
    list->size++;
}

void deleteNode(List* list, Node* node) {
    if (node == list->header) return;
    
    node->prev->next = node->next;
    node->next->prev = node->prev;
    free(node);
    list->size--;
}

Node* findNode(List* list, int ar) {
    Node* current = list->header->next;
    while (current != list->header) {
        if (current->ar == ar) return current;
        current = current->next;
    }
    return NULL;
}

void printList(List* list) {
    Node* current = list->header->next;
    printf("Liste: ");
    while (current != list->header) {
        printf("%d ", current->ar);
        current = current->next;
    }
    printf("\n");
}

void destroyList(List* list) {
    Node* current = list->header->next;
    Node* next;
    
    while (current != list->header) {
        next = current->next;
        free(current);
        current = next;
    }
    
    free(list->header);
    free(list);
}