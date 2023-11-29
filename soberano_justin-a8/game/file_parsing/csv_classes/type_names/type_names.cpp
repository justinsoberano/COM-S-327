#include "type_names.h"
#include "../helper_functions/helper_functions_for_csv.h"

using namespace std;

vector<type_names> type_names_database;

void print_type_names() {
    for(const auto &t : type_names_database) {
        cout << t;
    }
}

type_names get_type_name(int index) {
    return type_names_database[index];
}

type_names parse_type_names_line(const string &line) {
    istringstream sstream(line);
    string field;
    vector<int> values(3, INT_MAX);
    string id;

    int i = 0;

    while(getline(sstream, field, ',')) {
        if (i == 2) {
            id = field;
        } else if(is_integer(field)) {
            values[i] = stoi(field);
        }
        i++;
    }
    return type_names(values[0], values[1], id);
}

bool check_local_language_for_type_names(const type_names &t_n) {
    if(t_n.get_local_language_id() == 9) {
        return true;
    } else {
        return false;
    }
}

void parse_type_names(string path) {
    ifstream f(path);
    if(f.is_open()) {
        string line;
        getline(f, line);
        while(getline(f, line)) {
            type_names t_n = parse_type_names_line(line);
            if(check_local_language_for_type_names(t_n)) {
                type_names_database.push_back(t_n);
            }
        }
    } else {
        cout << "[Type Names] Unable to locate file in Pyrite: " << path << endl;
        cout << "[Type Names] Fall back to $HOME directory..." << endl;
        home_directory_parse_type_names();
    }
}

void home_directory_parse_type_names() {
    string home = getenv("HOME");
    string path = home + "/.poke327/pokedex/pokedex/data/csv/";
    string type_names_path = path + "type_names.csv";
    ifstream f(type_names_path);
    if(f.is_open()) {
        parse_type_names(type_names_path);
    } else {
        cout << "[Type Names] Unable to locate file in $HOME directory: " << type_names_path << endl;
        cout << "[Type Names] Enter directory: " + home;

        string custom_path = home;
        string input;
        cin >> input;
        custom_path += input;

        custom_directory_parse_type_names(custom_path);
    }
}

void custom_directory_parse_type_names(string path) {
    ifstream f(path);
    if(f.is_open()) {
        parse_type_names(path);
    } else {
        cout << "[Type Names] Unable to locate file in custom directory: " << path << endl;
        cout << "[Type Names] Terminating program..." << endl;
        exit(1);
    }
}

ostream& operator<<(ostream &os, const type_names &t) {
    os << "[Type Names] | Type ID: " << (t.get_type_id() == INT_MAX ? "NA" : to_string(t.get_type_id()))
    << ", Local Language ID: " << (t.get_local_language_id() == INT_MAX ? "NA" : to_string(t.get_local_language_id()))
    << ", Name: " << (t.get_name() == "" ? "NA" : t.get_name()) << endl;
    return os;
}