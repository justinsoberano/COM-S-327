#include "terrain_generation.h"

void init_map() {
    for(int j = 0; j < ROWS; j++) {
        for(int i = 0; i < COLS; i++) {
            if(j == 0 || i == 0 || i == COLS - 1 || j == ROWS - 1) {
                earth[LY][LX].map[j][i] = BORDER;
            } else {
                earth[LY][LX].map[j][i] = BLANK;
            }
        }
    }
}

void expand_terrain(int x, int y, int type) {
    if(x > 0 && earth[LY][LX].map[y][x - 1] == BLANK) {
        push(x - 1, y, type);
    }
    if(x < COLS - 1 && earth[LY][LX].map[y][x + 1] == BLANK) {
        push(x + 1, y, type);
    }
    if(y > 0 && earth[LY][LX].map[y - 1][x] == BLANK) {
        push(x, y - 1, type);
    }
    if(y < ROWS - 1 && earth[LY][LX].map[y + 1][x] == BLANK) {
        push(x, y + 1, type);
    }
}

void generate_terrain() {
    while(count > 0) {
        tile t = pop();
        int x = t.x;
        int y = t.y;
        int type = t.type;

        switch(type) {
            case 1:
                earth[LY][LX].map[y][x] = TALL_GRASS;
                expand_terrain(x, y, type);
                break;
            case 2:
                earth[LY][LX].map[y][x] = SHORT_GRASS;
                expand_terrain(x, y, type);
                break;
            case 3:
                earth[LY][LX].map[y][x] = WATER;
                expand_terrain(x, y, type);
                break;
        }
    }
}

void print_map() {

    clear();

    for (int j = 0; j < ROWS; j++) {
        for (int i = 0; i < COLS; i++) {
            if (earth[LY][LX].char_map[j][i] != 0) {
                if(earth[LY][LX].char_map[j][i] == PLAYER) {
                    attron(COLOR_PAIR(6));
                    mvaddch(j + 1, i, earth[LY][LX].char_map[j][i]);
                    attroff(COLOR_PAIR(6));
                } else {
                    attron(COLOR_PAIR(1));
                    mvaddch(j + 1, i, earth[LY][LX].char_map[j][i]);
                    attroff(COLOR_PAIR(1));
                }
            } else {
                if(earth[LY][LX].map[j][i] == BORDER) {
                    attron(COLOR_PAIR(3));
                    mvaddch(j + 1, i, earth[LY][LX].map[j][i]);
                    attroff(COLOR_PAIR(3));
                } else if (earth[LY][LX].map[j][i] == TREE) {
                    attron(COLOR_PAIR(2));
                    mvaddch(j + 1, i, earth[LY][LX].map[j][i]);
                    attroff(COLOR_PAIR(2));
                } else if (earth[LY][LX].map[j][i] == WATER) {
                    attron(COLOR_PAIR(4));
                    mvaddch(j + 1, i, earth[LY][LX].map[j][i]);
                    attroff(COLOR_PAIR(4));   
                } else if (earth[LY][LX].map[j][i] == SHORT_GRASS) {
                    attron(COLOR_PAIR(5));
                    mvaddch(j + 1, i, earth[LY][LX].map[j][i]);
                    attroff(COLOR_PAIR(5));
                } else if (earth[LY][LX].map[j][i] == TALL_GRASS) {
                    attron(COLOR_PAIR(2));
                    mvaddch(j + 1, i, earth[LY][LX].map[j][i]);
                    attroff(COLOR_PAIR(2));
                } else if (earth[LY][LX].map[j][i] == PATH) {
                    attron(COLOR_PAIR(7));
                    mvaddch(j + 1, i, earth[LY][LX].map[j][i]);
                    attroff(COLOR_PAIR(7));
                } else {
                    mvaddch(j + 1, i, earth[LY][LX].map[j][i]);
                }
            }
        }
    }
    refresh();
}

void verify_map() {
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            if(earth[LY][LX].map[i][j] == BLANK) {
                earth[LY][LX].map[i][j] = TALL_GRASS;
            }
        }
    }
}
void add_trees() {
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            int r = rand_int(1, 20);
            if(earth[LY][LX].map[i][j] == SHORT_GRASS && r == 3) {
                earth[LY][LX].map[i][j] = TREE;
            }
        }
    }
}

void path_ns(int NG, int SG) {

    int random_start;
    if (NG == 0) { random_start = rand_int(5, 75); }
    else { 
        random_start = NG; 
        earth[LY][LX].north_gate = random_start;
    }

    int start_displacement = rand_int(2, 5);
    Point start = {0 + start_displacement, random_start};
    
    int random_end;
    if (SG == 0) { random_end = rand_int(5, 75); }
    else { 
        random_end = SG;
        earth[LY][LX].south_gate = random_end;
    }

    int end_displacement = rand_int(2, 5);
    Point end = {(ROWS - 1) - end_displacement, random_end};
    dijkstra(start, end);

    for(int i = start_displacement; i >= 0; i--) { earth[LY][LX].map[i][random_start] = PATH; }
    for(int i = end_displacement; i >= 0; i--) { earth[LY][LX].map[ROWS - 1 - i][random_end] = PATH; }

    if (LY == 400) { earth[LY][LX].map[0][random_start] = BORDER;}
    if (LY == 0) { earth[LY][LX].map[ROWS - 1][random_end] = BORDER;}

    if (SG == 0) { earth[LY][LX].south_gate = random_end; }
    if (NG == 0) { earth[LY][LX].north_gate = random_start; }

}

void path_ew(int EG, int WG) {

    int random_start;
    if(EG == 0) { random_start = rand_int(7, 17); }
    else { 
        random_start = EG;
        earth[LY][LX].east_gate = random_start;
    }

    int start_displacement = rand_int(30, 35);
    Point start = {random_start, 0 + start_displacement};
    
    int random_end;
    if(WG == 0) { random_end = rand_int(7, 17); }
    else { 
        random_end = WG;
        earth[LY][LX].west_gate = random_end;
    }

    int end_displacement = rand_int(30, 35);
    Point end = {random_end, (COLS - 1) - end_displacement};
    dijkstra(start, end);

    for(int i = start_displacement; i >= 0; i--) { earth[LY][LX].map[random_start][i] = PATH; }
    for(int i = end_displacement; i >= 0; i--) { earth[LY][LX].map[random_end][COLS - 1 - i] = PATH; }

    if (LX == 400) { earth[LY][LX].map[random_end][COLS - 1] = BORDER;}
    if (LX == 0) { earth[LY][LX].map[random_start][0] = BORDER;}

    if (WG == 0) { earth[LY][LX].west_gate = random_end; }
    if (EG == 0) { earth[LY][LX].east_gate = random_start; }

}

void path_generation(int NG, int SG, int EG, int WG) {
    path_ns(NG, SG);
    path_ew(EG, WG);
}

void generate_centers() {
    int x = rand_int(5, 75);
    int y = rand_int(5, 75);
    int spawn_chance = abs(((-45 * (abs(LX - 200 + LY - 200) / 2)) / 200) + 50);
    int spawn_center = rand_int(1, 100);
    int spawn_mart = rand_int(1, 100);
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            if(j == x && ((LX == 200 && LY == 200) || spawn_center <= spawn_chance)) {
                if(earth[LY][LX].map[i][j] == PATH && earth[LY][LX].map[i - 1][j] != PATH) {
                    earth[LY][LX].map[i - 1][j] = POKECENTER;
                    x = -1;
                }
            }
            if(j == y && ((LX == 200 && LY == 200) || spawn_mart <= spawn_chance)) {
                if(earth[LY][LX].map[i][j] == PATH && earth[LY][LX].map[i - 1][j] != PATH) {
                    earth[LY][LX].map[i - 1][j] = POKEMART;
                    y = -1;
                }
            }
        }
    }
}

Point generate_pc() {
    int x = rand_int(5, 75);
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            if(j == x && earth[LY][LX].map[i][j] == PATH) {
                earth[LY][LX].char_map[i][j] = PLAYER;
                Point result = {i, j};
                return result;
            }
        }
    }

    return (Point){0, 0};
}

void gen() {
    if(earth[LY][LX].map[0][0] != '\0') {
        print_map();
        return;
    }
    init_map();
    init_queue(20);
    generate_terrain();
    verify_map();
    add_trees();

    int north_gate = 0, south_gate = 0, east_gate = 0, west_gate = 0;
    if (LY + 1 <= 400) south_gate = earth[LY + 1][LX].south_gate;
    if (LY - 1 >= 0) north_gate = earth[LY - 1][LX].north_gate;
    if (LX - 1 >= 0) west_gate = earth[LY][LX - 1].west_gate;
    if (LX + 1 <= 400) east_gate = earth[LY][LX + 1].east_gate;

    path_generation(south_gate, north_gate, west_gate, east_gate);
    generate_centers();

    if(pc_generated == 0) {
        pc = generate_pc();
        pc_generated = 1;
    }

    calculate_distance_maps();
    update_dijkstra_npc();
}
