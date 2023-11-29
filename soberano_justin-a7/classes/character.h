#ifndef CHARACTER_H
#define CHARACTER_H

#include "point.h"

class Character {
    public:
        int type;
        Point position;
        int distance;
        bool west;
        int direction;
};

#endif