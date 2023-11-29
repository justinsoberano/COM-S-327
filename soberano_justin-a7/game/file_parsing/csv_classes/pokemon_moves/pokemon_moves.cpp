#include "pokemon_moves.h"
#include "../helper_functions/helper_functions_for_csv.h"

using namespace std;

vector<pokemon_moves> pokemon_moves_database;

void print_pokemon_moves() {
    for(const auto &p : pokemon_moves_database) {
        cout << p;
    }
}

pokemon_moves get_pokemon_moves(int index) {
    return pokemon_moves_database[index];
}

pokemon_moves parse_pokemon_moves_line(const string &line) {
    istringstream sstream(line);
    string field;
    vector<int> values(6, INT_MAX);

    int i = 0;

    while(getline(sstream, field, ',')) {
        if(is_integer(field)) {
            values[i] = stoi(field);
        }
        i++;
    }

    return pokemon_moves(values[0], values[1], values[2], values[3], values[4], values[5]);
}

void parse_pokemon_moves(string path) {
    ifstream f(path);
    if(f.is_open()) {
        string line;
        getline(f, line);
        while(getline(f, line)) {
            pokemon_moves_database.push_back(parse_pokemon_moves_line(line));
        }
    } else {
        cout << "[Pokemon Moves] Unable to locate file in Pyrite: " << path << endl;
        cout << "[Pokemon Moves] Fall back to $HOME directory..." << endl;
        home_directory_parse_pokemon_moves();   
    }
}

void home_directory_parse_pokemon_moves() {
    string home = getenv("HOME");
    string path = home + "/.poke327/pokedex/pokedex/data/csv/";
    string pokemon_moves_path = path + "pokemon_moves.csv";
    ifstream f(pokemon_moves_path);
    if(f.is_open()) {
        parse_pokemon_moves(pokemon_moves_path);
    } else {
        cout << "[Pokemon Moves] Unable to locate file in $HOME directory: " << pokemon_moves_path << endl;
        cout << "[Pokemon Moves] Enter directory: " + home;

        string custom_path = home;
        string input;
        cin >> input;
        custom_path += input;
        custom_directory_parse_pokemon_moves(custom_path);
    }
}

void custom_directory_parse_pokemon_moves(string path) {
    ifstream f(path);
    if(f.is_open()) {
        parse_pokemon_moves(path);
    } else {
        cout << "[Pokemon Moves] Unable to locate file in custom directory: " << path << endl;
        cout << "[Pokemon Moves] Terminating program..." << endl;
        exit(1);
    }

}

ostream& operator<<(ostream &os, const pokemon_moves &p) {
    os << "[Pokemon Moves] | Pokemon ID: " << (p.get_pokemon_id() == INT_MAX ? "NA" : to_string(p.get_pokemon_id()))
    << ", Version Group ID: " << (p.get_version_group_id() == INT_MAX ? "NA" : to_string(p.get_version_group_id()))
    << ", Move ID: " << (p.get_move_id() == INT_MAX ? "NA" : to_string(p.get_move_id()))
    << ", Pokemon Move Method ID: " << (p.get_pokemon_move_method_id() == INT_MAX ? "NA" : to_string(p.get_pokemon_move_method_id()))
    << ", Level: " << (p.get_level() == INT_MAX ? "NA" : to_string(p.get_level()))
    << ", Order: " << (p.get_order() == INT_MAX ? "NA" : to_string(p.get_order())) << endl;
    return os;
}