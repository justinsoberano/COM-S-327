#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

/* Header files */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <stdbool.h>
#include <cstring>
#include <ncurses.h>
#include <iostream>
#include <fstream>

/* Class Files */
#include "../classes/point.h"
#include "../classes/node.h"
#include "../classes/character.h"
#include "../classes/world.h"

#define ROWS 21
#define COLS 80

/* Terrain Types */
#define TREE '^'
#define BORDER '%'
#define PATH '#'
#define POKECENTER 'C'
#define POKEMART 'M'
#define TALL_GRASS ':'
#define SHORT_GRASS '.'
#define WATER '~' 
#define BLANK ' '

/* Characters */
#define PLAYER '@'
#define SENTRY 's'
#define HIKER 'h'
#define RIVAL 'r'
#define PACER 'p'
#define WANDERER 'w'
#define EXPLORER 'e'

/* Global Variables */
extern int LX;
extern int LY;
extern int numtrainers;
extern int pc_in_building;
extern int pc_generated;
extern int trainer_page_open;
extern int battle_with_trainer;
extern int current_battle_trainer;
extern int character_array_size;
extern int character_battle_page_open;
extern int pokedex_open;

extern Point pc; 
extern int defeated_trainers[6];

extern world earth[401][401];
extern int dist_map[ROWS][COLS];

#endif