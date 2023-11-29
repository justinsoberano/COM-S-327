#ifndef POKEMON_SPECIES_H
#define POKEMON_SPECIES_H

#include <fstream>
#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include <string>
#include <climits>

class pokemon_species {
    private:
        int id;
        std::string identifier;
        int generation_id;
        int evolves_from_species_id;
        int evolution_chain_id;
        int color_id;
        int shape_id;
        int habitat_id;
        int gender_rate;
        int capture_rate;
        int base_happiness;
        int is_baby;
        int hatch_counter;
        int has_gender_differences;
        int growth_rate_id;
        int forms_switchable;
        int is_legendary;
        int is_mythical;
        int order;
        int conquest_order;

    public:
        pokemon_species() : id(0), identifier(""), generation_id(0), evolves_from_species_id(0), evolution_chain_id(0), 
                            color_id(0), shape_id(0), habitat_id(0), gender_rate(0), capture_rate(0), base_happiness(0),
                            is_baby(0), hatch_counter(0), has_gender_differences(0), growth_rate_id(0), forms_switchable(0),
                            is_legendary(0), is_mythical(0), order(0), conquest_order(0) {};

        pokemon_species(
            int id, std::string identifier, int generation_id,
            int evolves_from_species_id, int evolution_chain_id,
            int color_id, int shape_id, int habitat_id, int gender_rate,
            int capture_rate, int base_happiness, int is_baby, int hatch_counter,
            int has_gender_differences, int growth_rate_id, int forms_switchable,
            int is_legendary, int is_mythical, int order, int conquest_order) : id(id), identifier(identifier), generation_id(generation_id), evolves_from_species_id(evolves_from_species_id), evolution_chain_id(evolution_chain_id), 
                                                                                color_id(color_id), shape_id(shape_id), habitat_id(habitat_id), gender_rate(gender_rate), capture_rate(capture_rate), base_happiness(base_happiness),
                                                                                is_baby(is_baby), hatch_counter(hatch_counter), has_gender_differences(has_gender_differences), growth_rate_id(growth_rate_id), forms_switchable(forms_switchable),
                                                                                is_legendary(is_legendary), is_mythical(is_mythical), order(order), conquest_order(conquest_order) {};
            
        int get_id() const { return id; }
        std::string get_identifier() const { return identifier; }
        int get_generation_id() const { return generation_id; }
        int get_evolves_from_species_id() const { return evolves_from_species_id; }
        int get_evolution_chain_id() const { return evolution_chain_id; }
        int get_color_id() const { return color_id; }
        int get_shape_id() const { return shape_id; }
        int get_habitat_id() const { return habitat_id; }
        int get_gender_rate() const { return gender_rate; }
        int get_capture_rate() const { return capture_rate; }
        int get_base_happiness() const { return base_happiness; }
        int get_is_baby() const { return is_baby; }
        int get_hatch_counter() const { return hatch_counter; }
        int get_has_gender_differences() const { return has_gender_differences; }
        int get_growth_rate_id() const { return growth_rate_id; }
        int get_forms_switchable() const { return forms_switchable; }
        int get_is_legendary() const { return is_legendary; }
        int get_is_mythical() const { return is_mythical; }
        int get_order() const { return order; }
        int get_conquest_order() const { return conquest_order; }
    
    friend std::ostream &operator<<(std::ostream &os, const pokemon_species &p);

};

extern std::vector<pokemon_species> pokemon_species_database;

void print_pokemon_species();
pokemon_species get_pokemon_species(int index);
void parse_pokemon_species(std::string path);
pokemon_species parse_pokemon_species_line(const std::string &line);
void home_directory_parse_pokemon_species();
void custom_directory_parse_pokemon_species(std::string path);

#endif