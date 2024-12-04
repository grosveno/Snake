#include "priority_queue.h"

// 创建一个新的优先队列
PriorityQueue* createPriorityQueue() {
    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    if (!pq) {
        return NULL;
    }
    pq->head = NULL;
    pq->size = 0;
    return pq;
}

// 向优先队列中插入一个新节点
void insertPriorityQueue(PriorityQueue* pq, const char* name, int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        return;
    }
    pq->size++;
    strcpy(newNode->name, name);
    newNode->data = data;
    newNode->next = NULL;

    // 插入到正确的位置以保持队列的有序性
    Node** current = &(pq->head);
    while (*current != NULL && (*current)->data >= data) {
        current = &((*current)->next);
    }
    newNode->next = *current;
    *current = newNode;
}

// 打印优先队列中的所有节点
void printPriorityQueue(PriorityQueue* pq) {
    Node* current = pq->head;
    while (current != NULL) {
        printf("Name: %s, Score: %d\n", current->name, current->data);
        current = current->next;
    }
}

// 销毁优先队列，释放内存
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