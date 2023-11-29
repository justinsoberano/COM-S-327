#ifndef TYPE_NAMES_H
#define TYPE_NAMES_H

#include <fstream>
#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include <string>
#include <climits>


class type_names {
    private:
        int type_id;
        int local_language_id;
        std::string name;

    public:
        type_names() : type_id(0), local_language_id(0), name("") {};

        type_names(int type_id, int local_language_id, std::string name) : 
                   type_id(type_id), local_language_id(local_language_id), name(name) {};

        int get_type_id() const { return type_id; };
        int get_local_language_id() const { return local_language_id; };
        std::string get_name() const { return name; };

    friend std::ostream& operator<<(std::ostream &os, const type_names &t);

};

extern std::vector<type_names> type_names_database;

void print_type_names();
type_names parse_type_names_line(const std::string &line);
bool check_language_id_for_type_names(type_names t_n);
void parse_type_names(std::string path);
type_names get_type_name(int index);
void home_directory_parse_type_names();
void custom_directory_parse_type_names(std::string path);

#endif