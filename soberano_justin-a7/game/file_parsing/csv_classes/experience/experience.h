#ifndef EXPERIENCE_H
#define EXPERIENCE_H

#include <fstream>
#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include <string>
#include <climits>

class experience {
    private:
        int growth_rate_id;
        int level;
        int exp;

    public:
        experience() : growth_rate_id(0), level(0), exp(0) {};
        
        experience(int growth_rate_id, int level, int exp) : 
            growth_rate_id(growth_rate_id), level(level), exp(exp) {};

        int get_growth_rate_id() const { return growth_rate_id; }
        int get_level() const { return level; }
        int get_experience() const { return exp; }

    friend std::ostream& operator<<(std::ostream &os, const experience &e);
    
};

extern std::vector<experience> experience_database;

void parse_experience(std::string path);
experience parse_experience_line(const std::string &line);
void print_experience();
experience get_experience(int index);
void home_directory_parse_experience();
void custom_directory_parse_experience(std::string path);

#endif