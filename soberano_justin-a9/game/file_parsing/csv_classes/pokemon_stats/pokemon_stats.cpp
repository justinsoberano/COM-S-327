#include "pokemon_stats.h"
#include "../helper_functions/helper_functions_for_csv.h"

using namespace std;

vector<pokemon_stats> pokemon_stats_database;

void print_pokemon_stats() {
    for(const auto &p : pokemon_stats_database) {
        cout << p;
    }
}

pokemon_stats get_pokemon_stat(int index) {
    return pokemon_stats_database[index];
}

pokemon_stats parse_pokemon_stats_line(const string &line) {
    istringstream sstream(line);
    string field;
    vector<int> values(4, INT_MAX);

    int i = 0;

    while(getline(sstream, field, ',')) {
        if(is_integer(field)) {
            values[i] = stoi(field);
        }
        i++;
    }

    return pokemon_stats(values[0], values[1], values[2], values[3]);
}

void parse_pokemon_stats(string path) {
    ifstream f(path);

    if(f.is_open()) {
        string line;
        getline(f, line); // this skips the header line.
        while(getline(f, line)) {
            pokemon_stats_database.push_back(parse_pokemon_stats_line(line));
        }
    } else {
        cout << "[Pokemon Stats] Unable to locate file in Pyrite: " << path << endl;
        cout << "[Pokemon Stats] Fall back to $HOME directory..." << endl;
        home_directory_parse_pokemon_stats();
    }
}

void home_directory_parse_pokemon_stats() {
    string home = getenv("HOME");
    string path = home + "/.poke327/pokedex/pokedex/data/csv/";
    string pokemon_stats_path = path + "pokemon_stats.csv";
    ifstream f(pokemon_stats_path);
    if(f.is_open()) {
        parse_pokemon_stats(pokemon_stats_path);
    } else {
        cout << "[Pokemon Stats] Unable to locate file in $HOME directory: " << pokemon_stats_path << endl;
        cout << "[Pokemon Stats] Enter directory: " + home;

        string custom_path;
        string input;
        cin >> input;
        custom_path = home + input;
        custom_directory_parse_pokemon_stats(custom_path);
    }
}

void custom_directory_parse_pokemon_stats(string path) {
    ifstream f(path);
    if(f.is_open()) {
        parse_pokemon_stats(path);
    } else {
        cout << "[Pokemon Stats] Unable to locate file in custom directory: " << path << endl;
        cout << "[Pokemon Stats] Terminating program..." << endl;
        exit(1);
    }
}

ostream& operator<<(ostream &os, const pokemon_stats &p) {
    os << "[Pokemon Stat] | Pokemon ID: " << (p.get_pokemon_id() == INT_MAX ? "NA" : to_string(p.get_pokemon_id()))
    << ", Stat ID: " << (p.get_stat_id() == INT_MAX ? "NA" : to_string(p.get_stat_id()))
    << ", Base Stat: " << (p.get_base_stat() == INT_MAX ? "NA" : to_string(p.get_base_stat()))
    << ", Effort: " << (p.get_effort() == INT_MAX ? "NA" : to_string(p.get_effort())) << endl;
    return os;
}