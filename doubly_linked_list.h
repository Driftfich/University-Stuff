
#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

typedef struct Node {
    int data;
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct List {
    Node* header;
    int size;
} List;

// Funktionsprototypen
List* createList();
void insertAfter(List* list, Node* node, int data);
void deleteNode(List* list, Node* node);
Node* findNode(List* list, int data);
void printList(List* list);
void destroyList(List* list);

#endif