#include "global_variables.h"

#include <vector>

int LX = 200;
int LY = 200;
int numtrainers = 6;
int pc_in_building = 0;
int pc_generated = 0;
int trainer_page_open = 0;
int battle_with_trainer = 0;
int current_battle_trainer = 0;
int character_battle_page_open = 0;
int pokedex_open = 0;
int wild_pokemon_battle_page = 0;
int wild_pokemon_encountered_page = 0;
int items_page_open = 0;
int pokemon_captured = 0;

int potions = 3;
int revives = 3;
int pokeballs = 3;

pokedex current_pokemon_in_battle;

Point pc = Point(0, 0); 
int defeated_trainers[6] = {0, 0, 0, 0, 0, 0};

world earth[401][401] = {{{{{'\0'}}, {{0}}, 0, 0, 0, 0}}};
int dist_map[ROWS][COLS];

std::vector<pokedex> owned_pokemon;

Character currently_battling;

int character_array_size = 0;

int battle_ongoing = 0;