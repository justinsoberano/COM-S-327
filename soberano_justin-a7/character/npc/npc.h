#ifndef NPC_H
#define NPC_H

#include "../../classes/character.h"
#include "../../global_variables/global_variables.h"
#include "../../data_structures/priority_queue/priority_queue.h"
#include "../../data_structures/random/random.h"
#include "../../dijkstra_algorithms/dijkstra_algorithms.h"

void spawn_npc(int trainer);
void generate_trainers(int numtrainers);
int is_valid_move(int x, int y);
Character npc_movement(Character c);
int is_trainer_defeated(char trainer);
int connect_enum_to_trainer(char trainer);
void trainer_page(int x);
void initialize_trainer_queue_at_map();

#endif