#ifndef GAME_H
#define GAME_H

#include "../global_variables/global_variables.h"
#include "../classes/character.h"
#include "../data_structures/priority_queue/priority_queue.h"
#include "../classes/world.h"
#include "../classes/node.h"
#include "../data_structures/queue/queue.h"
#include "../data_structures/random/random.h"
#include "../dijkstra_algorithms/dijkstra_algorithms.h"
#include "../terrain_generation/terrain_generation.h"
#include "../character/npc/npc.h"
#include "../character/player/player.h"
#include "file_parsing/file.h"

void initialize_ncurses();
void handle_args(int argc, char const *argv[]);
int start(int argc, char const *argv[]);
int parse_files(int argc, char const *argv[]);

#endif