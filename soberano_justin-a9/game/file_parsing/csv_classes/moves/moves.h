#ifndef MOVES_H
#define MOVES_H

#include <fstream>
#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include <string>
#include <climits>

class moves {
    private:
        int id;
        std::string identifier;
        int generation_id;
        int type_id;
        int power;
        int pp;
        int accuracy;
        int priority;
        int target_id;
        int damage_class_id;
        int effect_id;
        int effect_chance;
        int contest_type_id;
        int contest_effect_id;
        int super_contest_effect_id;

    public:

        moves() : id(0), identifier(""), generation_id(0), type_id(0), power(0), pp(0), accuracy(0), 
                  priority(0), target_id(0), damage_class_id(0), effect_id(0), effect_chance(0), 
                  contest_type_id(0), contest_effect_id(0), super_contest_effect_id(0) {};

        moves(int id, std::string identifier, 
              int generation_id, int type_id, 
              int power, int pp, int accuracy, 
              int priority, int target_id, 
              int damage_class_id, int effect_id, 
              int effect_chance, int contest_type_id,
              int contest_effect_id, int super_contest_effect_id) : id(id), identifier(identifier), generation_id(generation_id), type_id(type_id), power(power), pp(pp), accuracy(accuracy), 
                                                                    priority(priority), target_id(target_id), damage_class_id(damage_class_id), effect_id(effect_id), effect_chance(effect_chance), 
                                                                    contest_type_id(contest_type_id), contest_effect_id(contest_effect_id), super_contest_effect_id(super_contest_effect_id) {};

        int get_id() const { return id; }
        std::string get_identifier() const { return identifier; }
        int get_generation_id() const { return generation_id; }
        int get_type_id() const { return type_id; }
        int get_power() const { return power; }
        int get_pp() const { return pp; }
        int get_accuracy() const { return accuracy; }
        int get_priority() const { return priority; }
        int get_target_id() const { return target_id; }
        int get_damage_class_id() const { return damage_class_id; }
        int get_effect_id() const { return effect_id; }
        int get_effect_chance() const { return effect_chance; }
        int get_contest_type_id() const { return contest_type_id; }
        int get_contest_effect_id() const { return contest_effect_id; }
        int get_super_contest_effect_id() const { return super_contest_effect_id; }

    friend std::ostream& operator<<(std::ostream &os, const moves m);
};

extern std::vector<moves> moves_database;

void parse_moves(std::string path);
moves parse_moves_line(const std::string &line);
void print_moves();
moves get_moves(int index);
void home_directory_parse_moves();
void custom_directory_parse_moves(std::string path);

#endif