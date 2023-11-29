#ifndef QUEUE_H
#define QUEUE_H

#include "../random/random.h"

class tile {
    public:
        int x;
        int y;
        int type;
};

void push(int x, int y, int type);
tile pop();
void init_queue(int max_size);

extern tile q[512];
extern int count;

#endif