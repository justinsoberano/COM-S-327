#include "pokemon_species.h"
#include "../helper_functions/helper_functions_for_csv.h"

using namespace std;

vector<pokemon_species> pokemon_species_database;

void print_pokemon_species() {
    for(const auto &p : pokemon_species_database) {
        cout << p;
    }
}

pokemon_species get_pokemon_species(int index) {
    return pokemon_species_database[index];
}

pokemon_species parse_pokemon_species_line(const string &line) {
    istringstream sstream(line);
    string field;
    vector<int> values(20, INT_MAX);
    string id;

    int i = 0;

    while(getline(sstream, field, ',')) {
        if(i == 1) {
            id = field;
        } else if (is_integer(field)) {
            values[i] = stoi(field);
        } 
        i++;
    }

    return pokemon_species(values[0], id, values[2], values[3], values[4], values[5],
                           values[6], values[7], values[8], values[9], values[10],
                           values[11], values[12], values[13], values[14], values[15],
                           values[16], values[17], values[18], values[19]);
}

void parse_pokemon_species(string path) {
    ifstream f(path);
    if(f.is_open()) {
        string line;
        getline(f, line);
        while(getline(f, line)) {
            pokemon_species_database.push_back(parse_pokemon_species_line(line));
        }
    } else {
        cout << "[Pokemon Species] Unable to locate file in Pyrite: " << path << endl;
        cout << "[Pokemon Species] Fall back to $HOME directory..." << endl;
        home_directory_parse_pokemon_species();
    }
}

void home_directory_parse_pokemon_species() {
    string home = getenv("HOME");
    string path = home + "/.poke327/pokedex/pokedex/data/csv/";
    string pokemon_species_path = path + "pokemon_species.csv";
    ifstream f(pokemon_species_path);
    if(f.is_open()) {
        parse_pokemon_species(pokemon_species_path);
    } else {
        cout << "[Pokemon Species] Unable to locate file in $HOME directory: " << pokemon_species_path << endl;
        cout << "[Pokemon Species] Enter directory: " + home;

        string custom_path = home;
        string input;
        cin >> input;
        custom_path += input;

        custom_directory_parse_pokemon_species(custom_path);
    }
}

void custom_directory_parse_pokemon_species(string path) {
    ifstream f(path);
    if(f.is_open()) {
        parse_pokemon_species(path);
    } else {
        cout << "[Pokemon Species] Unable to locate file in custom directory: " << path << endl;
        cout << "[Pokemon Species] Terminating program..." << endl;
        exit(1);
    }
}

ostream &operator<<(ostream &os, const pokemon_species &p) {
    os << "[Pokemon Species] | ID: " << (p.get_id() == INT_MAX ? "NA" : to_string(p.get_id()))
    << ", Identifier: " << (p.get_identifier() == "" ? "NA" : p.get_identifier())
    << ", Generation ID: " << (p.get_generation_id() == INT_MAX ? "NA" : to_string(p.get_generation_id()))
    << ", Evolves From Species ID: " << (p.get_evolves_from_species_id() == INT_MAX ? "NA" : to_string(p.get_evolves_from_species_id()))
    << ", Evolution Chain ID: " << (p.get_evolution_chain_id() == INT_MAX ? "NA" : to_string(p.get_evolution_chain_id()))
    << ", Color ID: " << (p.get_color_id() == INT_MAX ? "NA" : to_string(p.get_color_id()))
    << ", Shape ID: " << (p.get_shape_id() == INT_MAX ? "NA" : to_string(p.get_shape_id()))
    << ", Habitat ID: " << (p.get_habitat_id() == INT_MAX ? "NA" : to_string(p.get_habitat_id()))
    << ", Gender Rate: " << (p.get_gender_rate() == INT_MAX ? "NA" : to_string(p.get_gender_rate()))
    << ", Capture Rate: " << (p.get_capture_rate() == INT_MAX ? "NA" : to_string(p.get_capture_rate()))
    << ", Base Happiness: " << (p.get_base_happiness() == INT_MAX ? "NA" : to_string(p.get_base_happiness()))
    << ", Is Baby: " << (p.get_is_baby() == INT_MAX ? "NA" : to_string(p.get_is_baby()))
    << ", Hatch Counter: " << (p.get_hatch_counter() == INT_MAX ? "NA" : to_string(p.get_hatch_counter()))
    << ", Has Gender Differences: " << (p.get_has_gender_differences() == INT_MAX ? "NA" : to_string(p.get_has_gender_differences()))
    << ", Growth Rate ID: " << (p.get_growth_rate_id() == INT_MAX ? "NA" : to_string(p.get_growth_rate_id()))
    << ", Forms Switchable:" << (p.get_forms_switchable() == INT_MAX ? "NA" : to_string(p.get_forms_switchable()))
    << ", Is Legendary: " << (p.get_is_legendary() == INT_MAX ? "NA" : to_string(p.get_forms_switchable()))
    << ", Is Mythical: " << (p.get_is_mythical() == INT_MAX ? "NA" : to_string(p.get_is_mythical()))
    << ", Order: " << (p.get_order() == INT_MAX ? "NA" : to_string(p.get_order()))
    << ", Conquest Order: " << (p.get_conquest_order() == INT_MAX ? "NA" : to_string(p.get_conquest_order())) << endl;
    return os;
}