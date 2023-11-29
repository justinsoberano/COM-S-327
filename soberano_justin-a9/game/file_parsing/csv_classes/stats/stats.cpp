#include "stats.h"
#include "../helper_functions/helper_functions_for_csv.h"

using namespace std;

vector<stats> stats_database;

void print_stats() {
    for(const auto &s : stats_database) {
        cout << s;
    }
}

stats get_stat(int index) {
    return stats_database[index];
}

stats parse_stats_line(const string &line) {
    istringstream sstream(line);
    string field;
    vector<int> values(5, INT_MAX);
    string id;

    int i = 0;

    while(getline(sstream, field, ',')) {
        if(i == 2) {
            id = field;
        } else if (is_integer(field)) {
            values[i] = stoi(field);
        }
        i++;
    }

    return stats(values[0], values[1], id, values[3], values[4]);
}

void parse_stats(string path) {
    ifstream f(path);
    if(f.is_open()) {
        string line;
        getline(f, line); // this skips the header line.
        while(getline(f, line)) {
            stats_database.push_back(parse_stats_line(line));
        }
    } else {
        cout << "[Stats] Unable to locate file in Pyrite: " << path << endl;
        cout << "[Stats] Fall back to $HOME directory..." << endl;
        home_directory_parse_stats();
    }
}

void home_directory_parse_stats() {
    string home = getenv("HOME");
    string path = home + "/.poke327/pokedex/pokedex/data/csv/";
    string stats_path = path + "stats.csv";
    ifstream f(stats_path);
    if(f.is_open()) {
        parse_stats(stats_path);
    } else {
        cout << "[Stats] Unable to locate file in $HOME directory: " << stats_path << endl;
        cout << "[Stats] Enter directory: " + home;

        string custom_path = home;
        string input;
        cin >> input;
        custom_path += input;

        custom_directory_parse_stats(custom_path);
    }
}

void custom_directory_parse_stats(string path) {
    ifstream f(path);
    if(f.is_open()) {
        parse_stats(path);
    } else {
        cout << "[Stats] Unable to locate file in custom directory: " << path << endl;
        cout << "[Stats] Terminating program..." << endl;
        exit(1);
    }
}


ostream& operator<<(ostream &os, const stats &s) {
    os << "[Stats] | ID: " << (s.get_id() == INT_MAX ? "NA" : to_string(s.get_id())) 
    << ", Damage Class ID: " << (s.get_damage_class_id() == INT_MAX ? "NA" : to_string(s.get_damage_class_id()))
    << ", Identifier: " << (s.get_identifier() == "" ? "NA" : s.get_identifier())
    << ", Is Battle Only: " << (s.get_is_battle_only() == INT_MAX ? "NA" : to_string(s.get_is_battle_only()))
    << ", Game Index: " << (s.get_game_index() == INT_MAX ? "NA" : to_string(s.get_game_index())) << endl;
    return os;
}