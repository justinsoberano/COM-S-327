#ifndef TRAINER_BATTLES_H
#define TRAINER_BATTLES_H

#include "../../global_variables/global_variables.h"
#include "../../data_structures/random/random.h"
#include "../pokemon_battles/pokemon_battles.h"
#include "../../terrain_generation/terrain_generation.h"

void view_trainer_pokemon(Character const &trainer);
void battle_trainer_view(Character const &trainer);
void trainer_battle(Character const &trainer);

#endif