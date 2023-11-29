#include "pokemon_types.h"
#include "../helper_functions/helper_functions_for_csv.h"

using namespace std;

vector<pokemon_types> pokemon_types_database;

void print_pokemon_types() {
    for(const auto &p : pokemon_types_database) {
        cout << p;
    }
}

pokemon_types get_pokemon_type(int index) {
    return pokemon_types_database[index];
}

pokemon_types parse_pokemon_types_line(const string &line) {
    istringstream sstream(line);
    string field;
    vector<int> values(3, INT_MAX);

    int i = 0;

    while(getline(sstream, field, ',')) {
        if(is_integer(field)) {
            values[i] = stoi(field);
        }
        i++;
    }

    return pokemon_types(values[0], values[1], values[2]);
}

void parse_pokemon_types(string path) {
    ifstream f(path);
    if(f.is_open()) {
        string line;
        getline(f, line); // this skips the header line.
        while(getline(f, line)) {
            pokemon_types_database.push_back(parse_pokemon_types_line(line));
        }
    } else {
        cout << "[Pokemon Types] Unable to locate file in Pyrite: " << path << endl;
        cout << "[Pokemon Types] Fall back to $HOME directory..." << endl;
        home_directory_parse_pokemon_types();
    }
}

void home_directory_parse_pokemon_types() {
    string home = getenv("HOME");
    string path = home + "/.poke327/pokedex/pokedex/data/csv/";
    string pokemon_types_path = path + "pokemon_types.csv";
    ifstream f(pokemon_types_path);
    if(f.is_open()) {
        parse_pokemon_types(pokemon_types_path);
    } else {
        cout << "[Pokemon Types] Unable to locate file in $HOME directory: " << pokemon_types_path << endl;
        cout << "[Pokemon Types] Enter directory: " + home;

        string custom_path = home;
        string input;
        cin >> input;
        custom_path += input;

        custom_directory_parse_pokemon_types(custom_path);
    }
}

void custom_directory_parse_pokemon_types(string path) {
    ifstream f(path);
    if(f.is_open()) {
        parse_pokemon_types(path);
    } else {
        cout << "[Pokemon Types] Unable to locate file in custom directory: " << path << endl;
        cout << "[Pokemon Types] Terminating program..." << endl;
        exit(1);
    }
}


ostream& operator<<(ostream &os, const pokemon_types &p) {
    os << "[Pokemon Types] | Pokemon ID: " << (p.get_pokemon_id() == INT_MAX ? "NA" : to_string(p.get_pokemon_id()))
    << ", Type ID: " << (p.get_type_id() == INT_MAX ? "NA" : to_string(p.get_type_id()))
    << ", Slot: " << (p.get_slot() == INT_MAX ? "NA" : to_string(p.get_slot())) << endl;
    return os;
}