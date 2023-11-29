#include "game.h"

void initialize_ncurses() {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    start_color();
    
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, 232, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, 82, COLOR_BLACK);
    init_pair(6, COLOR_YELLOW, COLOR_BLACK);
    init_pair(7, 248, COLOR_BLACK);

}

void handle_args(int argc, char const *argv[]) {
    if(argc == 1) {
        generate_trainers(numtrainers);
    } else {
        for(int i = 1; i < argc; i++) {
            if(strncmp(argv[i], "--", 2) == 0) {
                numtrainers = atoi(argv[i] + 2);
                if (numtrainers > 0) {
                    generate_trainers(numtrainers);
                } else {
                    printf("Invalid number of trainers.\n");
                    exit(0);
                }
            }
        }
    }
}

int start(int argc, char const *argv[]) {
    init_rand();
    gen();
    initialize_ncurses();
    handle_args(argc, argv);
    game();
    endwin();
    return 0;
}

#include <unistd.h>
#include "file_parsing/csv_classes/import_csv_classes.h"
#include <string>

using namespace std;

int parse_files(int argc, char const *argv[]) {
    string path = "/share/cs327/pokedex/pokedex/data/csv/";
    if(argc == 1) {
        cout << "No arguments given. Parsing all files..." << endl;
        usleep(2000000);
        file_parsing();
    } else if(argc == 2) {
        cout << "Parsing file in Pyrite directory: /share/cs327/pokedex/pokedex/data/csv/..." << endl;
        if(strncmp(argv[1], "type_names", 7) == 0) {
            cout << "Parsing type_names.csv..." << endl;
            usleep(2000000);
            parse_type_names(path + "type_names.csv");
            print_type_names();
        } else if(strncmp(argv[1], "pokemon_stats", 13) == 0) {
            cout << "Parsing pokemon_stats.csv..." << endl;
            usleep(2000000);
            parse_pokemon_stats(path + "pokemon_stats.csv");
            print_pokemon_stats();
        } else if(strncmp(argv[1], "pokemon_types", 13) == 0) {
            cout << "Parsing pokemon_types.csv..." << endl;
            usleep(2000000);
            parse_pokemon_types(path + "pokemon_types.csv");
            print_pokemon_types();
        } else if(strncmp(argv[1], "experience", 10) == 0) {
            cout << "Parsing experience.csv..." << endl;
            usleep(2000000);
            parse_experience(path + "experience.csv");
            print_experience();
        } else if(strncmp(argv[1], "stats", 5) == 0) {
            cout << "Parsing stats.csv..." << endl;
            usleep(2000000);
            parse_stats(path + "stats.csv");
            print_stats();
        } else if(strncmp(argv[1], "moves", 5) == 0) {
            cout << "Parsing moves.csv..." << endl;
            usleep(2000000);
            parse_moves(path + "moves.csv");
            print_moves();
        } else if(strncmp(argv[1], "pokemon_moves", 14) == 0) {
            cout << "Parsing pokemon_moves.csv..." << endl;
            usleep(2000000);
            parse_pokemon_moves(path + "pokemon_moves.csv");
            print_pokemon_moves();
        } else if(strncmp(argv[1], "pokemon_species", 16) == 0) {
            cout << "Parsing pokemon_species.csv..." << endl;
            parse_pokemon_species(path + "pokemon_species.csv");
            usleep(2000000);
            print_pokemon_species();
        } else if(strncmp(argv[1], "pokemon", 10) == 0) {
            cout << "Parsing pokemon.csv..." << endl;
            usleep(2000000);
            parse_pokemon(path + "pokemon.csv");
            print_pokemon();
        }  else if(strncmp(argv[1], "help", 4) == 0) {
            cout << "For Pyrite: ./poke [FILE]" << endl;
            cout << "For Home: ./poke home [FILE] " << endl << endl;
            cout << "Parse CSV files for the game." << endl;
            cout << "  pokemon" << endl;
            cout << "  pokemon_stats" << endl;
            cout << "  pokemon_types" << endl;
            cout << "  experience" << endl;
            cout << "  stats" << endl;
            cout << "  moves" << endl;
            cout << "  pokemon_moves" << endl;
            cout << "  pokemon_species" << endl;
            cout << "  type_names" << endl << endl;
            cout << "Only available for home command. " << endl;
            cout << "  all" << endl << endl;
        } else {
            cout << "Invalid argument given. Type [ ./poke help ] for all commands." << endl;
            exit(0);}
    } else if(argc == 3){
        if(strncmp(argv[1], "home", 4) == 0) {
            string home = getenv("HOME");
            cout << "Parsing file in home directory: " + home + "./poke327/pokedex/pokedex/data/csv/..." << endl;
            if(strncmp(argv[2], "type_names", 7) == 0) {
                cout << "Parsing type_names.csv..." << endl;
                usleep(2000000);
                home_directory_parse_type_names();
                print_type_names();
            } else if (strncmp(argv[2], "pokemon_stats", 13) == 0) {
                cout << "Parsing pokemon_stats.csv..." << endl;
                usleep(2000000);
                home_directory_parse_pokemon_stats();
                print_pokemon_stats();
            } else if (strncmp(argv[2], "pokemon_types", 13) == 0) {
                cout << "Parsing pokemon_types.csv..." << endl;
                usleep(2000000);
                home_directory_parse_pokemon_types();
                print_pokemon_types();
            } else if (strncmp(argv[2], "experience", 10) == 0) {
                cout << "Parsing experience.csv..." << endl;
                usleep(2000000);
                home_directory_parse_experience();
                print_experience();
            } else if (strncmp(argv[2], "stats", 5) == 0) {
                cout << "Parsing stats.csv..." << endl;
                usleep(2000000);
                home_directory_parse_stats();
                print_stats();
            } else if (strncmp(argv[2], "moves", 5) == 0) {
                cout << "Parsing moves.csv..." << endl;
                usleep(2000000);
                home_directory_parse_moves();
                print_moves();
            } else if (strncmp(argv[2], "pokemon_moves", 14) == 0) {
                cout << "Parsing pokemon_moves.csv..." << endl;
                usleep(2000000);
                home_directory_parse_pokemon_moves();
                print_pokemon_moves();
            } else if (strncmp(argv[2], "pokemon_species", 16) == 0) {
                cout << "Parsing pokemon_species.csv..." << endl;
                usleep(2000000);
                home_directory_parse_pokemon_species();
                print_pokemon_species();
            } else if (strncmp(argv[2], "pokemon", 10) == 0) {
                cout << "Parsing pokemon.csv..." << endl;
                usleep(2000000);
                home_directory_parse_pokemon();
                print_pokemon();
            } else if (strncmp(argv[2], "all", 3) == 0) {
                cout << "Parsing all files..." << endl;
                usleep(2000000);
                home_directory_parse_type_names();
                home_directory_parse_pokemon_stats();
                home_directory_parse_pokemon_types();
                home_directory_parse_experience();
                home_directory_parse_stats();
                home_directory_parse_moves();
                home_directory_parse_pokemon_moves();
                home_directory_parse_pokemon_species();
                home_directory_parse_pokemon();
                print_type_names();
                print_pokemon_stats();
                print_pokemon_types();
                print_experience();
                print_stats();
                print_moves();
                print_pokemon_moves();
                print_pokemon_species();
                print_pokemon();
            } else {
                cout << "Invalid argument given. Type [ ./poke help ] for all commands." << endl;
                exit(0);
            }
        } else {
            cout << "Invalid argument given. Type [ ./poke help ] for all commands." << endl;
            exit(0);
        }
    } else {
        cout << "Too many arguments given. Type [ ./poke help ] for all commands." << endl;
        exit(0);
    }
    return 0;
}
