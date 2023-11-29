#include "moves.h"
#include "../helper_functions/helper_functions_for_csv.h"

using namespace std;

vector<moves> moves_database;

void print_moves() {
    for(const auto &m : moves_database) {
        cout << m;
    }
}

moves get_moves(int index) {
    return moves_database[index];
}

moves parse_moves_line(const string &line) {
    istringstream sstream(line);
    string field;
    vector<int> values(15, INT_MAX);
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

    return moves(values[0], id, values[2], values[3], values[4], values[5], values[6], 
                   values[7], values[8], values[9], values[10], values[11], values[12], 
                   values[13], values[14]);
}

void parse_moves(string path) {
    ifstream f(path);
    if(f.is_open()) {
        string line;
        getline(f, line);
        while(getline(f, line)) {
            moves_database.push_back(parse_moves_line(line));
        }
    } else {
        cout << "[Moves] Unable to locate file in Pyrite: " << path << endl;
        cout << "[Moves] Fall back to $HOME directory..." << endl;
        home_directory_parse_moves();
    }
}

void home_directory_parse_moves() {
    string home = getenv("HOME");
    string path = home + "/.poke327/pokedex/pokedex/data/csv/";
    string moves_path = path + "moves.csv";
    ifstream f(moves_path);
    if(f.is_open()) {
        parse_moves(moves_path);
    } else {
        cout << "[Moves] Unable to locate file in $HOME directory: " << moves_path << endl;
        cout << "[Moves] Enter directory: " + home;

        string custom_path = home;
        string input;
        cin >> input;
        custom_path += input;
        custom_directory_parse_moves(custom_path);
    }
}

void custom_directory_parse_moves(string path) {
    ifstream f(path);
    if(f.is_open()) {
        parse_moves(path);
    } else {
        cout << "[Moves] Unable to locate file in custom directory: " << path << endl;
        cout << "[Moves] Terminating Program..." << endl;
        exit(1);
    }
}



ostream& operator<<(ostream &os, const moves m) {
    os << "[Moves] | ID: " << (m.get_id() == INT_MAX ? "NA" : to_string(m.get_id()))
    << ", Identifier: " << (m.get_identifier() == "" ? "NA" : m.get_identifier())
    << ", Generation ID: " << (m.get_generation_id() == INT_MAX ? "NA" : to_string(m.get_generation_id()))
    << ", Type ID: " << (m.get_type_id() == INT_MAX ? "NA" : to_string(m.get_type_id()))
    << ", Power: " << (m.get_power() == INT_MAX ? "NA" : to_string(m.get_power()))
    << ", PP: " << (m.get_pp() == INT_MAX ? "NA" : to_string(m.get_pp()))
    << ", Accuracy: " << (m.get_accuracy() == INT_MAX ? "NA" : to_string(m.get_accuracy()))
    << ", Priority: " << (m.get_priority() == INT_MAX ? "NA" : to_string(m.get_priority()))
    << ", Target ID: " << (m.get_target_id() == INT_MAX ? "NA" : to_string(m.get_target_id()))
    << ", Damage Class ID: " << (m.get_damage_class_id() == INT_MAX ? "NA" : to_string(m.get_damage_class_id()))
    << ", Effect ID: " << (m.get_effect_id() == INT_MAX ? "NA" : to_string(m.get_effect_id()))
    << ", Effect Chance: " << (m.get_effect_chance() == INT_MAX ? "NA" : to_string(m.get_effect_chance()))
    << ", Contest Type ID: " << (m.get_contest_type_id() == INT_MAX ? "NA" : to_string(m.get_contest_type_id()))
    << ", Contest Effect ID: " << (m.get_contest_effect_id() == INT_MAX ? "NA" : to_string(m.get_contest_effect_id()))
    << ", Super Contest Effect ID: " << (m.get_super_contest_effect_id() == INT_MAX ? "NA" : to_string(m.get_super_contest_effect_id())) << endl;
    return os;
}