#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char name[50]; 
    int data;     
    struct Node* next;
} Node;

typedef struct {
    Node* head;
    int size;
} PriorityQueue;

PriorityQueue* createPriorityQueue();

void insertPriorityQueue(PriorityQueue* pq, const char* name, int data);

void printPriorityQueue(PriorityQueue* pq);

void destroyPriorityQueue(PriorityQueue* pq);

#endif // PRIORITY_QUEUE_H