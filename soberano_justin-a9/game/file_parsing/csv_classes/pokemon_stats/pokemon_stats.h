#ifndef POKEMON_STATS_H
#define POKEMON_STATS_H

#include <fstream>
#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include <string>
#include <climits>

class pokemon_stats {
    private:
        int pokemon_id;
        int stat_id;
        int base_stat;
        int effort;

    public:
        pokemon_stats() : pokemon_id(0), stat_id(0), base_stat(0), effort(0) {};

        pokemon_stats(int pokemon_id, int stat_id, int base_stat, int effort) : 
            pokemon_id(pokemon_id), stat_id(stat_id), base_stat(base_stat), effort(effort) {};

        int get_pokemon_id() const { return pokemon_id; }
        int get_stat_id() const { return stat_id; }
        int get_base_stat() const { return base_stat; }
        int get_effort() const { return effort; }

    friend std::ostream& operator<<(std::ostream &os, const pokemon_stats &p);

};

extern std::vector<pokemon_stats> pokemon_stats_database;

void parse_pokemon_stats(std::string path);
pokemon_stats parse_pokemon_stats_line(const std::string &line);
void print_pokemon_stats();
pokemon_stats get_pokemon_stat(int index);
void home_directory_parse_pokemon_stats();
void custom_directory_parse_pokemon_stats(std::string path);

#endif