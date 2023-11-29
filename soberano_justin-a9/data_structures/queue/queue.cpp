#include "queue.h"

int count = 0;
tile q[512];

void push(int x, int y, int type) {
    if (count == 256) return;
    tile t = {x, y, type};
    q[count] = t;
    count++;
}
tile pop() {
    if(count == 0) {
        tile empty = {0, 0, 0};
        return empty;
    }
    tile t = q[0];
    for (int i = 0; i < count - 1; i++) q[i] = q[i + 1]; 
    count--;
    return t;
}
void init_queue(int max_size) {
    int size = 0;
    while (size < max_size) {
        int type = rand_int(1, 3);
        int x = rand_int(5, 75);
        int y = rand_int(2, 19);
        push(x, y, type);
        size++;
    }
}