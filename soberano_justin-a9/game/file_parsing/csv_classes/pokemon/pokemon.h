#ifndef POKEMON_H
#define POKEMON_H

#include <fstream>
#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include <string>
#include <climits>

class pokemon {
    private:
        int id;
        std::string identifier;
        int species_id;
        int height;
        int width;
        int base_experience;
        int order;
        int is_default;

    public:
        pokemon() : id(0), identifier(""), species_id(0), height(0), width(0), base_experience(0), order(0), is_default(0) {};

        pokemon(int id, std::string identifier, int species_id, 
                int height, int width, int base_experience, 
                int order, int is_default) : id(id), identifier(identifier), species_id(species_id), 
                                             height(height), width(width), base_experience(base_experience), 
                                             order(order), is_default(is_default) {};

        int get_id() const { return id; }
        std::string get_identifier() const { return identifier; }
        int get_species_id() const { return species_id; }
        int get_height() const { return height; }
        int get_width() const { return width; }
        int get_base_experience() const { return base_experience; }
        int get_order() const { return order; }
        int get_is_default() const { return is_default; }

    friend std::ostream &operator<<(std::ostream &os, const pokemon &p);

};

extern std::vector<pokemon> pokemon_database;

void parse_pokemon(std::string path);
pokemon parse_pokemon_line(const std::string &line);
void print_pokemon();
pokemon get_pokemon(int index);
void home_directory_parse_pokemon();
void custom_directory_parse_pokemon(std::string path);

#endif