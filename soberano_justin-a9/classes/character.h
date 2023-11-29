#ifndef CHARACTER_H
#define CHARACTER_H

#include "point.h"
#include "../character/pokemon/pokemon_functions.h"
#include <vector>

class Character {
    public:
        int type;
        Point position;
        int distance;
        bool west;
        int direction;
        std::vector<pokedex> trainer_pokemon;

        Character() : type(0), position(Point(0, 0)), distance(0), west(false), direction(0), trainer_pokemon(std::vector<pokedex>()) {};
};

#endif