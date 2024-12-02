#include "priority_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(QueueNode *a, QueueNode *b) {
    QueueNode temp = *a;
    *a = *b;
    *b = temp;
}

void maxHeapify(PriorityQueue *pq, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < pq->size && pq->items[left].priority > pq->items[largest].priority) {
        largest = left;
    }

    if (right < pq->size && pq->items[right].priority > pq->items[largest].priority) {
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
    pq->items[pq->size].data = strdup(data); // 复制字符串
    pq->size++;
    int i = pq->size - 1;
    while (i != 0 && pq->items[(i - 1) / 2].priority < pq->items[i].priority) {
        swap(&pq->items[(i - 1) / 2], &pq->items[i]);
        i = (i - 1) / 2;
    }
}

char* extractMax(PriorityQueue *pq) {
    if (pq->size == 0) {
        printf("Priority Queue is empty\n");
        return NULL;
    }
    char *rootData = pq->items[0].data; // 保存根节点的数据
    pq->items[0] = pq->items[--pq->size];
    maxHeapify(pq, 0);
    return rootData; // 返回根节点的数据
}