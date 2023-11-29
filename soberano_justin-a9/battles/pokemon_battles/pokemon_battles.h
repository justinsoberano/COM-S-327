#ifndef POKEMON_BATTLES_H
#define POKEMON_BATTLES_H

#include "../../global_variables/global_variables.h"

extern encountered_pokemon wild_pokemon;

pokedex swap_pokemon();
void use_potion(pokedex &p);
void use_revive();
int use_pokeball(encountered_pokemon p);

void use_items();
void pokemon_battle_screen(pokedex p);
int calculate_damage(pokedex p, moves m);
int calculate_damage(encountered_pokemon p, moves m);
void encountered_wild_pokemon_screen();
void player_encountered_pokemon(int x, int y);

#endif