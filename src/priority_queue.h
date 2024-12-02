#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

typedef struct {
    int priority;  
    char *data;    
} QueueNode;

typedef struct {
    QueueNode items[MAX_SIZE];
    int size;
} PriorityQueue;

void insert(PriorityQueue *pq, int priority, char *data);
char* extractMax(PriorityQueue *pq);
void buildMaxHeap(PriorityQueue *pq);
void maxHeapify(PriorityQueue *pq, int i);
void swap(QueueNode *a, QueueNode *b);

#endif // PRIORITY_QUEUE_H