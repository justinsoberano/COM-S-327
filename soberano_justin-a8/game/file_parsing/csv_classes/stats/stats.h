#ifndef STATS_H
#define STATS_H

#include <fstream>
#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include <string>
#include <climits>

class stats {
    private:
        int id;
        int damage_class_id;
        std::string identifier;
        int is_battle_only;
        int game_index;

    public:
        stats() : id(0), damage_class_id(0), identifier(""), is_battle_only(0), game_index(0) {};

        stats(int id, int damage_class_id, std::string identifier, int is_battle_only, int game_index) : 
            id(id), damage_class_id(damage_class_id), identifier(identifier), 
            is_battle_only(is_battle_only), game_index(game_index) {};

        int get_id() const { return id; }
        int get_damage_class_id() const { return damage_class_id; }
        std::string get_identifier() const { return identifier; }
        int get_is_battle_only() const { return is_battle_only; }
        int get_game_index() const { return game_index; }

    friend std::ostream& operator<<(std::ostream &os, const stats &s);

};

extern std::vector<stats> stats_database;

void parse_stats(std::string path);
stats parse_stats_line(const std::string &line);
void print_stats();
stats get_stat(int index);
void home_directory_parse_stats();
void custom_directory_parse_stats(std::string path);

#endif