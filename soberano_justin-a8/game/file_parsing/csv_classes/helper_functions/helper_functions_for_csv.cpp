#include "helper_functions_for_csv.h"

using namespace std;

bool is_integer(const string &s) {
    try {
        stoi(s);
        return true;
    } catch(...) {
        return false;
    }
}