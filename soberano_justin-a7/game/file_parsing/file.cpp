#include "file.h"
#include "csv_classes/import_csv_classes.h"

using namespace std;


void file_parsing() { 
    /* File path for the CSV files in Pyrite. */
    string path = "/share/cs327/pokedex/pokedex/data/csv/";
    string pokemon_stats_path = path + "pokemon_stats.csv";
    string pokemon_types_path = path + "pokemon_types.csv";
    string experience_path = path + "experience.csv";
    string stats_path = path + "stats.csv";
    string pokemon_path = path + "pokemon.csv";
    string moves_path = path + "moves.csv";
    string pokemon_moves_path = path + "pokemon_moves.csv";
    string pokemon_species_path = path + "pokemon_species.csv";
    string type_names_path = path + "type_names.csv";

    parse_pokemon_stats(pokemon_stats_path);
    parse_pokemon_types(pokemon_types_path);
    parse_experience(experience_path);
    parse_stats(stats_path);
    parse_pokemon(pokemon_path);
    parse_moves(moves_path);
    parse_pokemon_moves(pokemon_moves_path);
    parse_pokemon_species(pokemon_species_path);
    parse_type_names(type_names_path);

    print_pokemon_stats();
    print_pokemon_types();
    print_experience();
    print_stats();
    print_pokemon();
    print_moves();
    print_pokemon_moves();
    print_pokemon_species();
    print_type_names();

};

// Have the command line to take argument in which file to parse and which line to display. If nothing is types in, display all.