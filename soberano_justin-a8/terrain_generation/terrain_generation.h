#ifndef TERRAIN_GENERATION_H
#define TERRAIN_GENERATION_H

#include "../global_variables/global_variables.h"
#include "../dijkstra_algorithms/dijkstra_algorithms.h"
#include "../data_structures/random/random.h"
#include "../data_structures/queue/queue.h"

void init_map();
void expand_terrain(int x, int y, int type);
void generate_terrain();
void print_map();
void verify_map();
void add_trees();
void path_ns();
void path_ew();
void path_generation();
void generate_centers();
Point generate_pc();
void gen();

#endif