#include "priority_queue.h"

PriorityQueue* createPriorityQueue() {
    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    if (!pq) {
        return NULL;
    }
    pq->head = NULL;
    pq->size = 0;
    return pq;
}

void insertPriorityQueue(PriorityQueue* pq, const char* name, int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        return;
    }
    pq->size++;
    strcpy(newNode->name, name);
    newNode->data = data;
    newNode->next = NULL;

    Node** current = &(pq->head);
    while (*current != NULL && (*current)->data >= data) {
        current = &((*current)->next);
    }
    newNode->next = *current;
    *current = newNode;
}

void printPriorityQueue(PriorityQueue* pq) {
    Node* current = pq->head;
    while (current != NULL) {
        printf("Name: %s, Score: %d\n", current->name, current->data);
        current = current->next;
    }
}

void destroyPriorityQueue(PriorityQueue* pq) {
    Node* current = pq->head;
    while (current != NULL) {
        Node* next = current->next;
        free(current);
        current = next;
    }
    pq->size = 0;
    free(pq);
}