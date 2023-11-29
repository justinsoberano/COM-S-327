#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "../../classes/character.h"
#include <stdio.h>
#include <stdlib.h>

class PriorityQueueNode {
    public:
        Character character;
        PriorityQueueNode *next;
};

class PriorityQueue {
    public:
        PriorityQueueNode* head;
};

PriorityQueue *create_priority_queue();
void insert (PriorityQueue *queue, Character character);
Character dequeue(PriorityQueue *queue);
void free_priority_queue(PriorityQueue *queue);

#endif