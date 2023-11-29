#ifndef POKEMON_FUNCTIONS_H
#define POKEMON_FUNCTIONS_H

#include "../../game/file_parsing/csv_classes/import_csv_classes.h"
#include <vector>

class encountered_pokemon {
    private:
        int level;
        pokemon p;
        int number_of_moves;
        std::vector<moves> p_moves;
        int hp;
        int health;
        int attack;
        int defense;
        int speed;
        int special_attack;
        int special_defense;
        int gender;
        bool shiny;
        int base_values[6];
    public:
        encountered_pokemon() : level(0), p(pokemon()), number_of_moves(0), p_moves(std::vector<moves>()), 
                                hp(0), attack(0), defense(0), speed(0), special_attack(0), special_defense(0),
                                gender(0), shiny(false), base_values{0, 0, 0, 0, 0, 0} {};
        void set_level(int level) { this-> level = level; }
        void set_pokemon(pokemon p) { this -> p = p; }
        void add_move(moves m) { 
            p_moves.push_back(m);
            number_of_moves++;
        }
        void set_hp(int hp) { this-> hp = hp; }
        void set_max_hp(int hp) { health = hp; }
        void set_attack(int attack) { this-> attack = attack; }
        void set_defense(int defense) { this-> defense = defense; }
        void set_speed(int speed) { this-> speed = speed; }
        void set_special_attack(int special_attack) { this-> special_attack = special_attack; }
        void set_special_defense(int special_defense) { this-> special_defense = special_defense; }
        void set_gender(int gender) { this -> gender = gender; }
        void set_shiny(bool shiny) { this -> shiny = shiny; }
        void set_base_values(int index, int value) { base_values[index] = value; }
        int get_level() const { return level; }
        pokemon get_pokemon() const { return p; }
        std::vector<moves> get_p_moves() const { return p_moves; }
        int get_number_of_moves() const { return number_of_moves; }
        int get_hp() const { return hp; }
        int get_attack() const { return attack; }
        int get_max_hp() const { return health; }
        int get_defense() const { return defense; }
        int get_speed() const { return speed; }
        int get_special_attack() const { return special_attack; }
        int get_special_defense() const { return special_defense; }
        int get_gender() const { return gender; }
        bool get_shiny() const { return shiny; }
        int get_base_values(int index) const { return base_values[index]; }
};

class pokedex : public encountered_pokemon {};

pokedex transfer_pokemon_to_pokedex(encountered_pokemon const ep);
std::vector<encountered_pokemon> generate_starting_pokemon();
pokedex select_first_pokemon();
encountered_pokemon pokemon_encountered();
void display_pokemon_info(const encountered_pokemon p);
int calculate_pokemon_level();
void one_moves(pokemon_moves pm, encountered_pokemon &ep);
void two_moves(pokemon_moves pm1, pokemon_moves pm2, encountered_pokemon &ep);
void get_encountered_pokemon_moves(encountered_pokemon &ep);
std::vector<pokemon_moves> filter_pokemon_moves(int level, encountered_pokemon &ep);
void get_encountered_pokemon_stats(encountered_pokemon &ep);
void get_encountered_pokemon_gender(encountered_pokemon &ep);
void get_encountered_pokemon_shiny(encountered_pokemon &ep);
void level_up_pokemon();
int get_pokemon_HP(int level, int base_stat);
int get_pokemon_other_stat(int level, int base_stat);

#endif