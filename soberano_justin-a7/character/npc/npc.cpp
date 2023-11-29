#include "npc.h"

void spawn_npc(int trainer) {
    while(1) {
        int x = rand_int(1, ROWS - 2);
        int y = rand_int(1, COLS - 2);

        if (trainer == HIKER && 
            (earth[LY][LX].map[x][y] == PATH ||
            earth[LY][LX].map[x][y] == TALL_GRASS ||
            earth[LY][LX].map[x][y] == SHORT_GRASS)) {
            earth[LY][LX].char_map[x][y] = HIKER;
            earth[LY][LX].characters[character_array_size].type = HIKER;
            earth[LY][LX].characters[character_array_size].position = (Point){x, y};
            earth[LY][LX].characters[character_array_size].distance = 0;
            character_array_size++;
            break;
        }
        if (trainer == RIVAL &&
            (earth[LY][LX].map[x][y] == PATH || 
            earth[LY][LX].map[x][y] == TALL_GRASS ||
            earth[LY][LX].map[x][y] == SHORT_GRASS)) {
            earth[LY][LX].char_map[x][y] = RIVAL;
            earth[LY][LX].characters[character_array_size].type = RIVAL;
            earth[LY][LX].characters[character_array_size].position = (Point){x, y};
            earth[LY][LX].characters[character_array_size].distance = 0;
            character_array_size++;
            break;
        }
        if (trainer == PACER &&
            (earth[LY][LX].map[x][y] == TALL_GRASS ||
            earth[LY][LX].map[x][y] == SHORT_GRASS)) {
            earth[LY][LX].char_map[x][y] = PACER;
            int r = rand_int(0, 1) == 0 ? true : false;
            earth[LY][LX].characters[character_array_size].type = PACER;
            earth[LY][LX].characters[character_array_size].position = (Point){x, y};
            earth[LY][LX].characters[character_array_size].distance = 0;
            earth[LY][LX].characters[character_array_size].west = r;
            character_array_size++;
            break;
        }
        if (trainer == WANDERER &&
            (earth[LY][LX].map[x][y] == TALL_GRASS ||
            earth[LY][LX].map[x][y] == SHORT_GRASS)) {
            earth[LY][LX].char_map[x][y] = WANDERER;
            int r = rand_int(0, 7);
            earth[LY][LX].characters[character_array_size].type = WANDERER;
            earth[LY][LX].characters[character_array_size].position = (Point){x, y};
            earth[LY][LX].characters[character_array_size].distance = 0;
            earth[LY][LX].characters[character_array_size].direction = r;
            character_array_size++;
            break;
        }
        if (trainer == SENTRY && 
            (earth[LY][LX].map[x][y] == TALL_GRASS ||
            earth[LY][LX].map[x][y] == SHORT_GRASS)) {
            earth[LY][LX].char_map[x][y] = SENTRY;
            earth[LY][LX].characters[character_array_size].type = SENTRY;
            earth[LY][LX].characters[character_array_size].position = (Point){x, y};
            earth[LY][LX].characters[character_array_size].distance = INT_MAX;
            character_array_size++;
            break;
        }
        if (trainer == EXPLORER &&
            (earth[LY][LX].map[x][y] == TALL_GRASS ||
            earth[LY][LX].map[x][y] == SHORT_GRASS)) {
            earth[LY][LX].char_map[x][y] = EXPLORER;
            int r = rand_int(0, 7);
            earth[LY][LX].characters[character_array_size].type = EXPLORER;
            earth[LY][LX].characters[character_array_size].position = (Point){x, y};
            earth[LY][LX].characters[character_array_size].distance = 0;
            earth[LY][LX].characters[character_array_size].direction = r;
            character_array_size++;
            break;
        }
    }
}

void generate_trainers(int numtrainers) {

    earth[LY][LX].characters = (Character *)malloc(sizeof(Character) * numtrainers);

    for(int i = 0; i < numtrainers; i++) {
        if (i == 0) {
            spawn_npc(HIKER);
        } else if (i == 1) {
            spawn_npc(RIVAL);
        } else if (i == 2) {
            spawn_npc(PACER);
        } else if (i == 3) {
            spawn_npc(WANDERER);
        } else if (i == 4) {
            spawn_npc(EXPLORER);
        } else {
            int npc[6] = {PACER, HIKER, SENTRY, WANDERER, EXPLORER, RIVAL};
            int r = rand_int(0, 5);
            spawn_npc(npc[r]);
        }
    }
}

int is_valid_move(int x, int y) {
    return x > 0 && x < ROWS - 1 && y > 0 && y < COLS - 1 && earth[LY][LX].map[x][y] != BORDER && earth[LY][LX].char_map[x][y] == 0;
}

Character npc_movement(Character c) {
    int current_gradient;
    int lowest_gradient;
    int lowest_gradient_y = 0;
    int lowest_gradient_x = 0;
    int dx[] = {-1, 0, 1, 0, -1, -1, 1, 1};
    int dy[] = {0, 1, 0, -1, -1, 1, -1, 1};

    switch(c.type) {
        case HIKER:
        if(defeated_trainers[Hkr] == 0) {
            current_gradient = dist_map_hiker[c.position.x][c.position.y];
            lowest_gradient = current_gradient;

            for(int i = 0; i < 8; i++) {
                int next_x = c.position.x + dx[i];
                int next_y = c.position.y + dy[i];
                
                if(is_valid_move(next_x, next_y) && dist_map_hiker[next_x][next_y] < lowest_gradient) {
                    lowest_gradient = dist_map_hiker[next_x][next_y];
                    lowest_gradient_x = next_x;
                    lowest_gradient_y = next_y;
                }
            }
            if(lowest_gradient < current_gradient) {
                earth[LY][LX].char_map[c.position.x][c.position.y] = 0;
                earth[LY][LX].char_map[lowest_gradient_x][lowest_gradient_y] = HIKER;
                c.position.x = lowest_gradient_x;
                c.position.y = lowest_gradient_y;
            }

            c.distance += dist_map_hiker[c.position.x][c.position.y];

            return c;
        } else { 
            int pacer_x = 0;
            int pacer_y = c.west ? 1 : -1;

            int next_x = c.position.x + pacer_x;
            int next_y = c.position.y + pacer_y;

            if(is_valid_move(next_x, next_y) && dist_map_pacer[next_x][next_y] != INT_MAX) {
                earth[LY][LX].char_map[c.position.x][c.position.y] = 0;
                earth[LY][LX].char_map[next_x][next_y] = HIKER;
                c.position.x = next_x;
                c.position.y = next_y;

            } else {
                c.west = !c.west;
            }
            c.distance = INT_MAX;
            return c;
        }
            break;
        case RIVAL:
            if(defeated_trainers[Rvl] == 0) {
                current_gradient = dist_map_hiker[c.position.x][c.position.y];
                lowest_gradient = current_gradient;

                for(int i = 0; i < 8; i++) {
                    int next_x = c.position.x + dx[i];
                    int next_y = c.position.y + dy[i];
                    
                    if(is_valid_move(next_x, next_y) && dist_map_hiker[next_x][next_y] < lowest_gradient) {
                        lowest_gradient = dist_map_hiker[next_x][next_y];
                        lowest_gradient_x = next_x;
                        lowest_gradient_y = next_y;
                    }
                }
                if(lowest_gradient < current_gradient) {
                    earth[LY][LX].char_map[c.position.x][c.position.y] = 0;
                    earth[LY][LX].char_map[lowest_gradient_x][lowest_gradient_y] = RIVAL;
                    c.position.x = lowest_gradient_x;
                    c.position.y = lowest_gradient_y;
                }
                
                c.distance += dist_map_rival[c.position.x][c.position.y];
                
                return c;
            } else {
                int pacer_x = 0;
                int pacer_y = c.west ? 1 : -1;

                int next_x = c.position.x + pacer_x;
                int next_y = c.position.y + pacer_y;

                if(is_valid_move(next_x, next_y) && dist_map_pacer[next_x][next_y] != INT_MAX) {
                    earth[LY][LX].char_map[c.position.x][c.position.y] = 0;
                    earth[LY][LX].char_map[next_x][next_y] = RIVAL;
                    c.position.x = next_x;
                    c.position.y = next_y;

                } else {
                    c.west = !c.west;
                }
                c.distance = INT_MAX;
                return c;
            }
            break;
        case PACER: {
            int pacer_x = 0;
            int pacer_y = c.west ? 1 : -1;

            int next_x = c.position.x + pacer_x;
            int next_y = c.position.y + pacer_y;

            if(is_valid_move(next_x, next_y) && dist_map_pacer[next_x][next_y] != INT_MAX) {
                earth[LY][LX].char_map[c.position.x][c.position.y] = 0;
                earth[LY][LX].char_map[next_x][next_y] = PACER;
                c.position.x = next_x;
                c.position.y = next_y;

            } else {
                c.west = !c.west;
            }
            c.distance += dist_map_pacer[c.position.x][c.position.y];
            return c;
            break;
        }
        case WANDERER: {
            int next_x = c.position.x + dx[c.direction];
            int next_y = c.position.y + dy[c.direction];

            int current_terrain = m[c.position.x][c.position.y];
            if(is_valid_move(next_x, next_y) && m[next_x][next_y] == current_terrain) {
                earth[LY][LX].char_map[c.position.x][c.position.y] = 0;
                earth[LY][LX].char_map[next_x][next_y] = WANDERER;
                c.position.x = next_x;
                c.position.y = next_y;
            } else {
                c.direction = rand_int(0, 7);
            }
            c.distance += dist_map_wanderer[c.position.x][c.position.y];
            return c;
            break;
        }
        case EXPLORER: {
            int next_x = c.position.x + dx[c.direction];
            int next_y = c.position.y + dy[c.direction];

            if(is_valid_move(next_x, next_y) && dist_map_explorer[next_x][next_y] != INT_MAX) {
                earth[LY][LX].char_map[c.position.x][c.position.y] = 0;
                earth[LY][LX].char_map[next_x][next_y] = EXPLORER;
                c.position.x = next_x;
                c.position.y = next_y;
            } else {
                c.direction = rand_int(0, 7);
            }
            c.distance += dist_map_explorer[c.position.x][c.position.y];
        }
    }
    return c;
}

int is_trainer_defeated(char trainer) {
    if (trainer == HIKER && defeated_trainers[Hkr] == 1) {
        return 1;
    } else if (trainer == RIVAL && defeated_trainers[Rvl] == 1) {
        return 1;
    } else if (trainer == PACER && defeated_trainers[Pcr] == 1) {
        return 1;
    } else if (trainer == WANDERER && defeated_trainers[Wndr] == 1) {
        return 1;
    } else if (trainer == EXPLORER && defeated_trainers[Expl] == 1) {
        return 1;
    } else if (trainer == SENTRY && defeated_trainers[Stry] == 1) {
        return 1;
    } else {
        return 0;
    }
}

int connect_enum_to_trainer(char trainer) {
    if (trainer == HIKER) {
        return Hkr;
    } else if (trainer == RIVAL) {
        return Rvl;
    } else if (trainer == PACER) {
        return Pcr;
    } else if (trainer == WANDERER) {
        return Wndr;
    } else if (trainer == EXPLORER) {
        return Expl;
    } else if (trainer == SENTRY) {
        return Stry;
    } else {
        return -1;
    }
}

void trainer_page(int x) {
    clear();
    mvprintw(0, 0, "Trainers: ");
    int j = 1;
    for(int i = x; i < character_array_size; i++) {
        if(j == 20) {
            break;
        }
        mvprintw(j, 0, "%c: %d away cells vertically, %d away cells horizontally", (char)earth[LY][LX].characters[i].type, 
                                                                                    earth[LY][LX].characters[i].position.x - pc.x, 
                                                                                    earth[LY][LX].characters[i].position.y - pc.y);
        j++;
    }
}

void initialize_trainer_queue_at_map() {
    for(int i = 0; i < character_array_size; i++) {
        if(earth[LY][LX].characters[i].type == HIKER || earth[LY][LX].characters[i].type == RIVAL) {
            insert(earth[LY][LX].smart_queue, earth[LY][LX].characters[i]);
        } else {
            insert(earth[LY][LX].other_queue, earth[LY][LX].characters[i]);
        }
    }
}
