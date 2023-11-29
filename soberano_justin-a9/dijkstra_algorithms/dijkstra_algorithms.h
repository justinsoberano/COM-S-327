#ifndef DIJKSTRA_ALGORITHMS_H
#define DIJKSTRA_ALGORITHMS_H

#include "../classes/point.h"
#include "../global_variables/global_variables.h"
#include "../classes/enums.h"
#include "../classes/node.h"

/* Path Detection Dijkstra Algorithm */
int is_valid(int x, int y);
int dijkstra(Point start, Point end);

/* NPC Distance Map Generation */
extern int hiker_costs[];
extern int rival_costs[];

extern int dist_map_hiker[ROWS][COLS];
extern int dist_map_rival[ROWS][COLS];
extern int dist_map_pacer[ROWS][COLS];
extern int dist_map_wanderer[ROWS][COLS];
extern int dist_map_explorer[ROWS][COLS];

extern TerrainType m[ROWS][COLS];

int dijkstra_for_pc(Point start, CharacterType character_type, TerrainType map[ROWS][COLS], int dist_map[ROWS][COLS], int dist[ROWS][COLS]);

void reset_distance_map();
void calculate_distance_maps();
void update_dijkstra_npc();

#endif