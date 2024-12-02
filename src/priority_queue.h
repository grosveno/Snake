#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

// 定义优先队列节点结构
typedef struct {
    int priority;  // 整数优先级
    char *data;    // 字符串数据
} QueueNode;

// 定义优先队列结构
typedef struct {
    QueueNode items[MAX_SIZE];
    int size;
} PriorityQueue;

// 函数原型声明
void insert(PriorityQueue *pq, int priority, char *data);
char* extractMax(PriorityQueue *pq);
void buildMaxHeap(PriorityQueue *pq);
void maxHeapify(PriorityQueue *pq, int i);
void swap(QueueNode *a, QueueNode *b);

#endif // PRIORITY_QUEUE_H