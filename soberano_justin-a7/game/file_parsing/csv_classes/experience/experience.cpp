#include "experience.h"
#include "../helper_functions/helper_functions_for_csv.h"

using namespace std;

vector<experience> experience_database;

void print_experience() {
    for(const auto &e : experience_database) {
        cout << e;
    }
}

experience get_experience(int index) {
    return experience_database[index];
}

experience parse_experience_line(const string &line) {
    istringstream sstream(line);
    string field;
    vector<int> values(3, INT_MAX);

    int i = 0;

    while(getline(sstream, field, ',')) {
        if (is_integer(field)) {
            values[i] = stoi(field);
        }
        i++;
    }

    return experience(values[0], values[1], values[2]);
}

void parse_experience(string path) {
    ifstream f(path);
    if(f.is_open()) {
        string line;
        getline(f, line);
        while(getline(f, line)) {
            experience_database.push_back(parse_experience_line(line));
        }
    } else {
        cout << "[Experience] Unable to locate file in Pyrite: " << path << endl;
        cout << "[Experience] Fall back to $HOME directory..." << endl;
        home_directory_parse_experience();
    }
}

void home_directory_parse_experience() {
    string home = getenv("HOME");
    string path = home + "/.poke327/pokedex/pokedex/data/csv/";
    string experience_path = path + "experience.csv";
    ifstream f(experience_path);
    if(f.is_open()) {
        parse_experience(experience_path);
    } else {
        cout << "[Experience] Unable to locate file in $HOME directory: " << experience_path << endl;
        cout << "[Experience] Enter directory: " + home;

        string custom_path = home;
        string input;
        cin >> input;
        custom_path += input;
        custom_directory_parse_experience(custom_path);
    }
}

void custom_directory_parse_experience(string path) {
    ifstream f(path);
    if(f.is_open()) {
        parse_experience(path);
    } else {
        cout << "[Experience] Unable to locate file in custom directory: " << path << endl;
        cout << "[Experience] Terminating Program..." << endl;
        exit(1);
    }
}

ostream& operator<<(ostream &os, const experience &e) {
    os << "[Experience] | Growth Rate ID: " << (e.get_growth_rate_id() == INT_MAX ? "NA" : to_string(e.get_growth_rate_id())) 
    << ", Level: " << (e.get_level()  == INT_MAX ? "NA" : to_string(e.get_level()))
    << ", Experience: " << (e.get_experience() == INT_MAX ? "NA" : to_string(e.get_experience())) << endl;
    return os;
}