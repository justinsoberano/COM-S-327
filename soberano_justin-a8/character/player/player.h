#ifndef PLAYER_H
#define PLAYER_H

#include "../../global_variables/global_variables.h"
#include "../npc/npc.h"
#include "../../dijkstra_algorithms/dijkstra_algorithms.h"
#include "../../terrain_generation/terrain_generation.h"
#include "../../game/file_parsing/csv_classes/import_csv_classes.h"
#include "../pokemon/pokemon_functions.h"

extern std::vector<pokedex> owned_pokemon;
extern Character currently_battling;

int valid_move_for_pc(int x, int y);
void check_for_trainer(int x, int y);
void check_if_at_gate(int x, int y);
void fly();
void game();

#endif