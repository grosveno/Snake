#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// 定义节点结构体
typedef struct Node {
    char name[50]; // 假设名字不超过49个字符
    int data;     // 分数
    struct Node* next;
} Node;

// 定义优先队列结构体
typedef struct {
    Node* head;
    int size;
} PriorityQueue;

// 创建一个新的优先队列
PriorityQueue* createPriorityQueue();

// 向优先队列中插入一个新节点
void insertPriorityQueue(PriorityQueue* pq, const char* name, int data);

// 打印优先队列中的所有节点
void printPriorityQueue(PriorityQueue* pq);

// 销毁优先队列，释放内存
void destroyPriorityQueue(PriorityQueue* pq);

#endif // PRIORITY_QUEUE_H