#include "pokemon.h"
#include "../helper_functions/helper_functions_for_csv.h"

using namespace std;

vector<pokemon> pokemon_database;

void print_pokemon() {
    for(const auto &p : pokemon_database) {
        cout << p;
    }
}

pokemon get_pokemon(int index) {
    return pokemon_database[index];
}

pokemon parse_pokemon_line(const string &line) {
    istringstream sstream(line);
    string field;
    vector<int> values(8, INT_MAX);
    string id;

    int i = 0;

    while(getline(sstream, field, ',')) {
        if( i == 1 ) {
            id = field;
        } else if (is_integer(field)) {
            values[i] = stoi(field);
        }
        i++;
    }

    return pokemon(values[0], id, values[2], values[3], values[4], values[5], values[6], values[7]);
}

void parse_pokemon(string path) {
    ifstream f(path);
    if(f.is_open()) {
        string line;
        getline(f, line);
        while(getline(f, line)) {
            pokemon_database.push_back(parse_pokemon_line(line));
        }
    } else {
        cout << "[Pokemon] Unable to locate file in Pyrite: " << path << endl;
        cout << "[Pokemon] Fall back to $HOME directory..." << endl;
        home_directory_parse_pokemon();
    }
}

void home_directory_parse_pokemon() {
    string home = getenv("HOME");
    string path = home + "/.poke327/pokedex/pokedex/data/csv/";
    string pokemon_path = path + "pokemon.csv";
    ifstream f(pokemon_path);
    if(f.is_open()) {
        parse_pokemon(pokemon_path);
    } else {
        cout << "[Pokemon] Unable to locate file in $HOME directory: " << pokemon_path << endl;
        cout << "[Pokemon] Enter directory: " + home;

        string custom_path = home;
        string input;
        cin >> input;
        custom_path += input;
        custom_directory_parse_pokemon(custom_path);
    }
}

void custom_directory_parse_pokemon(string path) {
    ifstream f(path);
    if(f.is_open()) {
        parse_pokemon(path);
    } else {
        cout << "[Pokemon] Unable to locate file in custom directory" << path << endl;
        cout << "[Pokemon] Terminating program..." << endl;
        exit(1);
    }
}

ostream& operator<<(ostream &os, const pokemon &p) {
    os << "[Pokemon] | ID: " << (p.get_id() == INT_MAX ? "NA" : to_string(p.get_id()))
    << ", Identifier: " << (p.get_identifier() == "" ? "NA" : p.get_identifier())
    << ", Species ID: " << (p.get_species_id() == INT_MAX ? "NA" : to_string(p.get_species_id()))
    << ", Height: " << (p.get_height() == INT_MAX ? "NA" : to_string(p.get_height()))
    << ", Width: " << (p.get_width() == INT_MAX ? "NA" : to_string(p.get_width()))
    << ", Base Experience: " << (p.get_base_experience() == INT_MAX ? "NA" : to_string(p.get_base_experience()))
    << ", Order: " << (p.get_order() == INT_MAX ? "NA" : to_string(p.get_order()))
    << ", Is Default: " << (p.get_is_default() == INT_MAX ? "NA" : to_string(p.get_is_default())) << endl;
    return os;
}