#include "file.h"
#include "csv_classes/import_csv_classes.h"

using namespace std;


void file_parsing() { 
    /* File path for the CSV files in Pyrite. */
    string path = "/share/cs327/pokedex/pokedex/data/csv/";

    /* File path for the CSV files in WSL. */
    // string home = getenv("HOME");
    // string path = home + "/documents/327/csv/";
    
    string pokemon_stats_path = path + "pokemon_stats.csv";
    string pokemon_types_path = path + "pokemon_types.csv";
    string experience_path = path + "experience.csv";
    string stats_path = path + "stats.csv";
    string pokemon_path = path + "pokemon.csv";
    string moves_path = path + "moves.csv";
    string pokemon_moves_path = path + "pokemon_moves.csv";
    string pokemon_species_path = path + "pokemon_species.csv";
    string type_names_path = path + "type_names.csv";

    cout << "\033[2J\033[1;1H";
    cout << "Parsing CSV files..." << endl;
    parse_pokemon_stats(pokemon_stats_path);
    parse_pokemon_types(pokemon_types_path);
    parse_experience(experience_path);
    parse_stats(stats_path);
    parse_pokemon(pokemon_path);
    parse_moves(moves_path);
    parse_pokemon_moves(pokemon_moves_path);
    parse_pokemon_species(pokemon_species_path);
    parse_type_names(type_names_path);
    cout << "\033[2J\033[1;1H";
};