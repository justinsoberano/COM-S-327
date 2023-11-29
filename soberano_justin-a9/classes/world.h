#ifndef WORLD_H
#define WORLD_H

#include "character.h"
#include "../data_structures/priority_queue/priority_queue.h"

class world {
    public:
        char map[21][80];
        int char_map[21][80];
        Character* characters;
        PriorityQueue* smart_queue;
        PriorityQueue* other_queue;
        int north_gate;
        int south_gate;
        int east_gate;
        int west_gate;
};

#endif