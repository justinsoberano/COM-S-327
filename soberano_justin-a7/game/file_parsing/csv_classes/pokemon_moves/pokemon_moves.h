#ifndef POKEMON_MOVES_H 
#define POKEMON_MOVES_H

#include <fstream>
#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include <string>
#include <climits>

class pokemon_moves {
    private:
        int pokemon_id;
        int version_group_id;
        int move_id;
        int pokemon_move_method_id;
        int level;
        int order;

    public:
        pokemon_moves() : pokemon_id(0), version_group_id(0), move_id(0), pokemon_move_method_id(0), level(0), order(0) {};

        pokemon_moves(int pokemon_id, int version_group_id, int move_id, int pokemon_move_method_id, int level, int order) :
                    pokemon_id(pokemon_id), version_group_id(version_group_id), move_id(move_id), pokemon_move_method_id(pokemon_move_method_id),
                    level(level), order(order) {};

        int get_pokemon_id() const { return pokemon_id; }
        int get_version_group_id() const { return version_group_id; }
        int get_move_id() const { return move_id; }
        int get_pokemon_move_method_id() const { return pokemon_move_method_id; }
        int get_level() const { return level; }
        int get_order() const { return order; }
    
    friend std::ostream& operator<<(std::ostream &os, const pokemon_moves &p);

};

extern std::vector<pokemon_moves> pokemon_moves_database;

void parse_pokemon_moves(std::string path);
pokemon_moves parse_pokemon_moves_line(const std::string &line);
void print_pokemon_moves();
pokemon_moves get_pokemon_moves(int index);
void home_directory_parse_pokemon_moves();
void custom_directory_parse_pokemon_moves(std::string path);

#endif