#include "priority_queue.h"
#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void maxHeapify(PriorityQueue *pq, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < pq->size && pq->items[left] > pq->items[largest]) {
        largest = left;
    }

    if (right < pq->size && pq->items[right] > pq->items[largest]) {
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

void insert(PriorityQueue *pq, int value) {
    if (pq->size >= MAX_SIZE) {
        printf("Priority Queue is full\n");
        return;
    }
    pq->items[pq->size] = value;
    pq->size++;
    int i = pq->size - 1;
    while (i != 0 && pq->items[(i - 1) / 2] < pq->items[i]) {
        swap(&pq->items[(i - 1) / 2], &pq->items[i]);
        i = (i - 1) / 2;
    }
}

int extractMax(PriorityQueue *pq) {
    if (pq->size == 0) {
        printf("Priority Queue is empty\n");
        return -1;
    }
    int root = pq->items[0];
    pq->items[0] = pq->items[--pq->size];
    maxHeapify(pq, 0);
    return root;
}
