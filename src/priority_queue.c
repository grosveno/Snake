#include "priority_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(QueueNode *a, QueueNode *b) {
    if (a == NULL || b == NULL) return;
    QueueNode temp = *a;
    *a = *b;
    *b = temp;
}

// 自定义比较函数，确保大的优先级排在小的前面，优先级相同则比较数据
int compare(QueueNode a, QueueNode b) {
    if (a.priority != b.priority) {
        return a.priority > b.priority; // 如果a的优先级大于b的优先级，返回1
    } else {
        return strcmp(a.data, b.data) > 0; // 优先级相同，比较数据
    }
}

void maxHeapify(PriorityQueue *pq, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < pq->size && compare(pq->items[left], pq->items[largest])) {
        largest = left;
    }

    if (right < pq->size && compare(pq->items[right], pq->items[largest])) {
        largest = right;
    }

    if (largest != i) {
        swap(&pq->items[i], &pq->items[largest]);
        maxHeapify(pq, largest);
    }
}

void buildMaxHeap(PriorityQueue *pq) {
    for (int i = pq->size / 2 - 1; i >= 0; i--) {
        maxHeapify(pq, i);
    }
}

void insert(PriorityQueue *pq, int priority, char *data) {
    if (pq->size >= MAX_SIZE) {
        printf("Priority Queue is full\n");
        return;
    }
    pq->items[pq->size].priority = priority;
    pq->items[pq->size].data = strdup(data); 
    pq->size++;
    int i = pq->size - 1;

    // 上堆化
    while (i != 0 && compare(pq->items[(i - 1) / 2], pq->items[i])) {
        swap(&pq->items[(i - 1) / 2], &pq->items[i]);
        i = (i - 1) / 2;
    }
}

char* extractMax(PriorityQueue *pq) {
    if (pq->size == 0) {
        printf("Priority Queue is empty\n");
        return NULL;
    }
    char *rootData = pq->items[0].data; 
    pq->items[0] = pq->items[--pq->size];
    if (pq->size > 0) {
        maxHeapify(pq, 0);
    }
    free(rootData); // 释放被移除元素的内存
    return rootData; 
}