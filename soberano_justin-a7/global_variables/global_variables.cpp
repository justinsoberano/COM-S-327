#include "global_variables.h"

int LX = 200;
int LY = 200;
int numtrainers = 6;
int pc_in_building = 0;
int pc_generated = 0;
int trainer_page_open = 0;
int battle_with_trainer = 0;
int current_battle_trainer = 0;
Point pc; 
int defeated_trainers[6] = {0, 0, 0, 0, 0, 0};

world earth[401][401] = {{{{{'\0'}}, {{0}}, 0, 0, 0, 0}}};
int dist_map[ROWS][COLS];

int character_array_size = 0;
