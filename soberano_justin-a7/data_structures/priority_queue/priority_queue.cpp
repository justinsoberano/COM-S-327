#include "priority_queue.h"

PriorityQueue *create_priority_queue() {
    PriorityQueue *queue = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    queue->head = NULL;
    return queue;
}

void insert(PriorityQueue* queue, Character character) {
    PriorityQueueNode* new_node = (PriorityQueueNode*)malloc(sizeof(PriorityQueueNode));
    new_node->character = character;
    new_node->next = NULL;

    if (!queue->head || character.distance < queue->head->character.distance) {
        new_node->next = queue->head;
        queue->head = new_node;
    } else {
        PriorityQueueNode* temp = queue->head;
        while (temp->next && character.distance >= temp->next->character.distance) {
            temp = temp->next;
        }
        new_node->next = temp->next;
        temp->next = new_node;
    }
}

Character dequeue(PriorityQueue* queue) {
    if (!queue->head) {
        Character empty = {0, {0, 0}, 0};
        return empty;
    }

    PriorityQueueNode* temp = queue->head;
    Character character = temp->character;
    queue->head = temp->next;
    free(temp);
    return character;
}

void free_priority_queue(PriorityQueue* queue) {
    free(queue);
}