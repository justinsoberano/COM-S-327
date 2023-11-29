#ifndef POKEMON_TYPES_H
#define POKEMON_TYPES_H

#include <fstream>
#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include <string>
#include <climits>

class pokemon_types {
    private:
        int pokemon_id;
        int type_id;
        int slot;

    public:
        pokemon_types() : pokemon_id(0), type_id(0), slot(0) {};

        pokemon_types(int pokemon_id, int type_id, int slot) : 
            pokemon_id(pokemon_id), type_id(type_id), slot(slot) {};

        int get_pokemon_id() const { return pokemon_id; }
        int get_type_id() const { return type_id; }
        int get_slot() const { return slot; }

    friend std::ostream& operator<<(std::ostream &os, const pokemon_types &p);

};

extern std::vector<pokemon_types> pokemon_types_database;

void parse_pokemon_types(std::string path);
pokemon_types parse_pokemon_types_line(const std::string &line);
void print_pokemon_types();
pokemon_types get_pokemon_type(int index);
void home_directory_parse_pokemon_types();
void custom_directory_parse_pokemon_types(std::string path);

#endif