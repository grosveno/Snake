#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

// 优先队列的结构定义
typedef struct {
    int items[MAX_SIZE];
    int size;
} PriorityQueue;

// 函数原型声明
void insert(PriorityQueue *pq, int value);
int extractMax(PriorityQueue *pq);
void buildMaxHeap(PriorityQueue *pq);
void maxHeapify(PriorityQueue *pq, int i);
void swap(int *a, int *b);

#endif // PRIORITY_QUEUE_H