#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <ncurses.h>

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

/* 0,0 : Starting Map */
int LX = 200;
int LY = 200;

typedef struct {
    int x;
    int y;
    int type;
} tile;

typedef struct {
    char map[ROWS][COLS];
    int north_gate;
    int south_gate;
    int east_gate;
    int west_gate;
} world;

typedef struct {
    int x, y;
} Point;

Point pc; 
int pc_in_building = 0;

int trainer_page_open = 0;

int battle_with_trainer = 0;
int current_battle_trainer = 0;

int defeated_trainers[6] = {0, 0, 0, 0, 0, 0};

typedef enum {Hkr, Rvl, Pcr, Wndr, Expl, Stry} TrainerType;

typedef struct {
    int x, y, dist;
} Node;

world earth[401][401] = {{{{{'\0'}}, 0, 0, 0, 0}}};

int char_map[ROWS][COLS];
int dist_map[ROWS][COLS];

/* Djikstra's Algorithm for Path Creation*/
int is_valid(int x, int y) {
    return x >= 0 && x < ROWS && y >= 0 && y < COLS && (x == 0 || x == ROWS - 1 || y == 0 || y == COLS - 1 || earth[LY][LX].map[x][y] != BLANK);
}
int dijkstra(Point start, Point end) {

    int dist[ROWS][COLS];
    Node prev[ROWS][COLS];
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            dist[i][j] = INT_MAX;
        }
    }

    dist[start.x][start.y] = 0;

    Node queue[ROWS * COLS];
    int front = 0, rear = 0;
    queue[rear++] = (Node){start.x, start.y, 0};

    while (front != rear) {
        Node curr = queue[front++];
        if (dist[curr.x][curr.y] < curr.dist) {
            continue;
        }
        int dx[] = {-1, 0, 1, 0};
        int dy[] = {0, 1, 0, -1};
        for (int i = 0; i < 4; i++) {
            int nx = curr.x + dx[i];
            int ny = curr.y + dy[i];
            if (is_valid(nx, ny)) {
                int ndist = curr.dist + 1;
                if (ndist < dist[nx][ny]) {
                    dist[nx][ny] = ndist;
                    prev[nx][ny] = curr;
                    queue[rear++] = (Node){nx, ny, ndist};
                }
            }
        }
    }

    if (dist[end.x][end.y] == INT_MAX) {
        return -1;
    }

    Point curr = end;
    while (curr.x != start.x || curr.y != start.y) {
        earth[LY][LX].map[curr.x][curr.y] = PATH;
        curr = (Point){prev[curr.x][curr.y].x, prev[curr.x][curr.y].y};
    }
    earth[LY][LX].map[start.x][start.y] = PATH;
    earth[LY][LX].map[end.x][end.y] = PATH;

    return dist[end.x][end.y];
}
/* -------------------- */

/* Dijkstra's Algorithm for Trainer Path Finding */

                    // BRDR,TREE,PATH,PMART,CMART,TGRAS,SGRAS,WATER,PLAYER
int hiker_costs[] = {INT_MAX, INT_MAX, 10, 50, 50, 15, 10, INT_MAX, INT_MAX, 0};
int rival_costs[] = {INT_MAX, INT_MAX, 10, 50, 50, 20, 10, INT_MAX, INT_MAX, 0};

int dist_map_hiker[ROWS][COLS] = {{INT_MAX}};
int dist_map_rival[ROWS][COLS] = {{INT_MAX}};
int dist_map_pacer[ROWS][COLS] = {{INT_MAX}};
int dist_map_wanderer[ROWS][COLS] = {{INT_MAX}};
int dist_map_explorer[ROWS][COLS] = {{INT_MAX}};

typedef enum { Border, Tree, Path, PokeMart, CenterMart, TallGrass, ShortGrass, Water, Player } TerrainType;

TerrainType m[ROWS][COLS] = {0};

typedef enum {Hiker, Rival} CharacterType;

int dijkstra_for_pc(Point start, CharacterType character_type, TerrainType map[ROWS][COLS], int distance_map[ROWS][COLS], int dist[ROWS][COLS]) {
    
    dist[start.x][start.y] = 0;
    distance_map[start.x][start.y] = 0;

    Node queue[ROWS * COLS * 50];
    int front = 0, rear = 0;
    queue[rear++] = (Node){start.x, start.y, 0};

    int dx[] = {-1, 0, 1, 0, -1, -1, 1, 1};
    int dy[] = {0, 1, 0, -1, -1, 1, -1, 1};

    while (front != rear) {
        Node curr = queue[front++];
        if (dist[curr.x][curr.y] < curr.dist) {
            continue;
        }
        
        for (int i = 0; i < 8; i++) {
            int nx = curr.x + dx[i];
            int ny = curr.y + dy[i];
            
            if (is_valid(nx, ny)) {
                int move_cost = 0;
                if (character_type == Hiker) {
                    move_cost = hiker_costs[map[nx][ny]];
                } else if (character_type == Rival) {
                    move_cost = rival_costs[map[nx][ny]];
                }

                int ndist = curr.dist + move_cost;
                if (ndist < 0) {
                    ndist = INT_MAX;
                    distance_map[nx][ny] = INT_MAX;
                    dist[nx][ny] = INT_MAX;
                }

                if (ndist < dist[nx][ny]) {
                    dist[nx][ny] = ndist;
                    distance_map[nx][ny] = ndist;
                    queue[rear++] = (Node){nx, ny, ndist};
                }
            }
        }
    }

    distance_map[start.x][start.y] = 0;

    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            if(map[i][j] != Player && distance_map[i][j] == 0) {
                distance_map[i][j] = INT_MAX;
            }
            if(i == 0 || j == 0 || i == ROWS - 1 || j == COLS - 1) {
                distance_map[i][j] = INT_MAX;
            }
        }
    }

    // for(int i = 0; i < ROWS; i++) {
    //     for(int j = 0; j < COLS; j++) {
    //         if (dist_map[i][j] == INT_MAX) {
    //             printf("   ");
    //         } else {
    //             printf("%02d ", dist_map[i][j] % 100);
    //         }
    //     }
    //     printf("\n");
    // }
    return 0;
}
/* --------------------------------------------- */

/* Initialization of Random */
void init_rand() { srand(time(NULL)); }
int rand_int(int a, int b) { return rand() % (b - a + 1) + a; }
/* ------------------------ */

/* Queue */ 
tile q[512];
int count = 0;
void push(int x, int y, int type) {
    if (count == 256) return;
    tile t = {x, y, type};
    q[count] = t;
    count++;
}
tile pop() {
    if(count == 0) {
        tile empty = {0, 0, 0};
        return empty;
    }
    tile t = q[0];
    for (int i = 0; i < count - 1; i++) q[i] = q[i + 1]; 
    count--;
    return t;
}
void init_queue(int max_size) {
    int size = 0;
    while (size < max_size) {
        int type = rand_int(1, 3);
        int x = rand_int(5, 75);
        int y = rand_int(2, 19);
        push(x, y, type);
        size++;
    }
}
/* ------ */

/* Assignment 1.04 */

typedef struct {
    int type;
    Point position;
    int distance;
    bool west;
    int direction;
} Character;

typedef struct PriorityQueueNode {
    Character character;
    struct PriorityQueueNode* next;
} PriorityQueueNode;

typedef struct PriorityQueue {
    PriorityQueueNode* head;
} PriorityQueue;

PriorityQueue* create_priority_queue() {
    PriorityQueue* queue = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    queue->head = NULL;
    return queue;
}

void insert(PriorityQueue* queue, Character character) {
    PriorityQueueNode* new_node = (PriorityQueueNode*)malloc(sizeof(PriorityQueueNode));
    new_node->character = character;
    new_node->next = NULL;

    if (!queue->head || character.distance < queue->head->character.distance) {
        new_node->next = queue->head;
        queue->head = new_node;
    } else {
        PriorityQueueNode* temp = queue->head;
        while (temp->next && character.distance >= temp->next->character.distance) {
            temp = temp->next;
        }
        new_node->next = temp->next;
        temp->next = new_node;
    }
}

Character dequeue(PriorityQueue* queue) {
    if (!queue->head) {
        Character empty = {0, {0, 0}, 0};
        return empty;
    }

    PriorityQueueNode* temp = queue->head;
    Character character = temp->character;
    queue->head = temp->next;
    free(temp);
    return character;
}

int is_empty(PriorityQueue* queue) {
    return queue->head == NULL;
}

void free_priority_queue(PriorityQueue* queue) {
    while (!is_empty(queue)) {
        dequeue(queue);
    }
    free(queue);
}

Character* character_array;
int character_array_size = 0;

void spawn_npc(int trainer) {

    while(1) {
        int x = rand_int(1, ROWS - 2);
        int y = rand_int(1, COLS - 2);

        if (trainer == HIKER && 
            (earth[LY][LX].map[x][y] == PATH ||
            earth[LY][LX].map[x][y] == TALL_GRASS ||
            earth[LY][LX].map[x][y] == SHORT_GRASS)) {
            char_map[x][y] = HIKER;
            character_array[character_array_size].type = HIKER;
            character_array[character_array_size].position = (Point){x, y};
            character_array[character_array_size].distance = 0;
            character_array_size++;
            break;
        }
        if (trainer == RIVAL &&
            (earth[LY][LX].map[x][y] == PATH || 
            earth[LY][LX].map[x][y] == TALL_GRASS ||
            earth[LY][LX].map[x][y] == SHORT_GRASS)) {
            char_map[x][y] = RIVAL;
            character_array[character_array_size].type = RIVAL;
            character_array[character_array_size].position = (Point){x, y};
            character_array[character_array_size].distance = 0;
            character_array_size++;
            break;
        }
        if (trainer == PACER &&
            (earth[LY][LX].map[x][y] == TALL_GRASS ||
            earth[LY][LX].map[x][y] == SHORT_GRASS)) {
            char_map[x][y] = PACER;
            character_array[character_array_size].type = PACER;
            character_array[character_array_size].position = (Point){x, y};
            character_array[character_array_size].distance = 0;
            character_array[character_array_size].west = rand_int(0, 1) == 0 ? true : false;
            character_array_size++;
            break;
        }
        if (trainer == WANDERER &&
            (earth[LY][LX].map[x][y] == TALL_GRASS ||
            earth[LY][LX].map[x][y] == SHORT_GRASS)) {
            char_map[x][y] = WANDERER;
            character_array[character_array_size].type = WANDERER;
            character_array[character_array_size].position = (Point){x, y};
            character_array[character_array_size].distance = 0;
            character_array[character_array_size].direction = rand_int(0, 7);
            character_array_size++;
            break;
        }
        if (trainer == SENTRY && 
            (earth[LY][LX].map[x][y] == TALL_GRASS ||
            earth[LY][LX].map[x][y] == SHORT_GRASS)) {
            char_map[x][y] = SENTRY;
            character_array[character_array_size].type = SENTRY;
            character_array[character_array_size].position = (Point){x, y};
            character_array[character_array_size].distance = INT_MAX;
            character_array_size++;
            break;
        }
        if (trainer == EXPLORER &&
            (earth[LY][LX].map[x][y] == TALL_GRASS ||
            earth[LY][LX].map[x][y] == SHORT_GRASS)) {
            char_map[x][y] = EXPLORER;
            character_array[character_array_size].type = EXPLORER;
            character_array[character_array_size].position = (Point){x, y};
            character_array[character_array_size].distance = 0;
            character_array[character_array_size].direction = rand_int(0, 7);
            character_array_size++;
            break;
        }
    }
}

void generate_trainers(int numtrainers) {

    character_array = malloc(sizeof(Character) * numtrainers);

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
/* --------------- */

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
            if (char_map[j][i] != 0) {
                if(char_map[j][i] == PLAYER) {
                    attron(COLOR_PAIR(6));
                    mvaddch(j + 1, i, char_map[j][i]);
                    attroff(COLOR_PAIR(6));
                } else {
                    attron(COLOR_PAIR(1));
                    mvaddch(j + 1, i, char_map[j][i]);
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
                char_map[i][j] = PLAYER;
                Point result = {i, j};
                return result;
            }
        }
    }

    return (Point){0, 0};
}
void reset_distance_map() {
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            dist_map[i][j] = INT_MAX;
        }
    }
}

void update_dijkstra_npc() {
    reset_distance_map();
    dijkstra_for_pc(pc, Hiker, m, dist_map_hiker, dist_map);
    reset_distance_map();
    dijkstra_for_pc(pc, Rival, m, dist_map_rival, dist_map);
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


    pc = generate_pc();

    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            if(earth[LY][LX].map[i][j] == BORDER) {
                m[i][j] = Border;
                dist_map_pacer[i][j] = INT_MAX;
                dist_map_wanderer[i][j] = INT_MAX;
                dist_map_explorer[i][j] = INT_MAX;
            } else if (earth[LY][LX].map[i][j] == TREE) {
                m[i][j] = Tree;
                dist_map_pacer[i][j] = INT_MAX;
                dist_map_wanderer[i][j] = INT_MAX;
                dist_map_explorer[i][j] = INT_MAX;
            } else if (earth[LY][LX].map[i][j] == PATH) {
                m[i][j] = Path;
                dist_map_pacer[i][j] = 10;
                dist_map_wanderer[i][j] = 10;
                dist_map_explorer[i][j] = 10;
            } else if (earth[LY][LX].map[i][j] == POKECENTER) {
                m[i][j] = CenterMart;
                dist_map_pacer[i][j] = 50;
                dist_map_wanderer[i][j] = 50;
                dist_map_explorer[i][j] = 50;
            } else if (earth[LY][LX].map[i][j] == POKEMART) {
                m[i][j] = PokeMart;
                dist_map_pacer[i][j] = 50;
                dist_map_wanderer[i][j] = 50;
                dist_map_explorer[i][j] = 50;
            } else if (earth[LY][LX].map[i][j] == TALL_GRASS) {
                m[i][j] = TallGrass;
                dist_map_pacer[i][j] = 20;
                dist_map_wanderer[i][j] = 20;
                dist_map_explorer[i][j] = 20;
            } else if (earth[LY][LX].map[i][j] == SHORT_GRASS) {
                m[i][j] = ShortGrass;
                dist_map_pacer[i][j] = 10;
                dist_map_wanderer[i][j] = 10;
                dist_map_explorer[i][j] = 10;
            } else if (earth[LY][LX].map[i][j] == WATER) {
                m[i][j] = Water;
                dist_map_pacer[i][j] = INT_MAX;
                dist_map_wanderer[i][j] = INT_MAX;
                dist_map_explorer[i][j] = INT_MAX;
            } else if (earth[LY][LX].map[i][j] == PLAYER) {
                m[i][j] = Player;
                dist_map_pacer[i][j] = INT_MAX;
                dist_map_wanderer[i][j] = INT_MAX;
                dist_map_explorer[i][j] = INT_MAX;
            }
        }
    }

    update_dijkstra_npc();
}

int is_valid_move(int x, int y) {
    return x > 0 && x < ROWS - 1 && y > 0 && y < COLS - 1 && earth[LY][LX].map[x][y] != BORDER && char_map[x][y] == 0;
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
                char_map[c.position.x][c.position.y] = 0;
                char_map[lowest_gradient_x][lowest_gradient_y] = HIKER;
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
                char_map[c.position.x][c.position.y] = 0;
                char_map[next_x][next_y] = HIKER;
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
                    char_map[c.position.x][c.position.y] = 0;
                    char_map[lowest_gradient_x][lowest_gradient_y] = RIVAL;
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
                    char_map[c.position.x][c.position.y] = 0;
                    char_map[next_x][next_y] = RIVAL;
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
                char_map[c.position.x][c.position.y] = 0;
                char_map[next_x][next_y] = PACER;
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
                char_map[c.position.x][c.position.y] = 0;
                char_map[next_x][next_y] = WANDERER;
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
                char_map[c.position.x][c.position.y] = 0;
                char_map[next_x][next_y] = EXPLORER;
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

int valid_move_for_pc(int x, int y) {
    if(x > 0 && x < ROWS - 1 && y > 0 && y < COLS - 1 && earth[LY][LX].map[x][y] != BORDER) {
        if(pc_in_building == 1) {
            mvprintw(0, 0, "Leave the building first [<].");
            return 0;
        } else if (battle_with_trainer == 1) {
            mvprintw(0, 0, "You have encountered a trainer! [escape].");
            return 0;
        } else if (char_map[x][y] != 0) {
            if(is_trainer_defeated(char_map[x][y])) {
                mvprintw(0, 0, "You have defeated this trainer. Move along.");
                return 0;
            } else {
                battle_with_trainer = 1;
                current_battle_trainer = connect_enum_to_trainer(char_map[x][y]);
                return 0;
            }
        } else if (earth[LY][LX].map[x][y] == TREE) {
            mvprintw(0, 0, "A tree blocks your path.");
            return 0;
        }
        return 1;
    }
    mvprintw(0, 0, "Invalid move.");
    return 0;
}

void check_for_trainer(int x, int y) {
    int dx[] = {-1, 0, 1, 0, -1, -1, 1, 1};
    int dy[] = {0, 1, 0, -1, -1, 1, -1, 1};

    for(int i = 0; i < 8; i++) {
        int next_x = x + dx[i];
        int next_y = y + dy[i];
        if(char_map[next_x][next_y] != 0) {
            if(is_trainer_defeated(char_map[next_x][next_y])) {
                mvprintw(0, 0, "You have defeated this trainer. Move along.");
            } else {
                battle_with_trainer = 1;
                current_battle_trainer = connect_enum_to_trainer(char_map[next_x][next_y]);
            }
        }
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
        mvprintw(j, 0, "%c: %d away cells vertically, %d away cells horizontally", (char)character_array[i].type, 
                                                                                    character_array[i].position.x - pc.x, 
                                                                                    character_array[i].position.y - pc.y);
        j++;
    }
}

int main(int argc, char const *argv[]) {
    init_rand();
    gen();

    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    start_color();

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, 232, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, 82, COLOR_BLACK);
    init_pair(6, COLOR_YELLOW, COLOR_BLACK);
    init_pair(7, 248, COLOR_BLACK);

    if(argc == 1) {
        generate_trainers(10);
    } else {
        for(int i = 1; i < argc; i++) {
            if(strncmp(argv[i], "--", 2) == 0) {
                int numtrainers = atoi(argv[i] + 2);
                if (numtrainers > 0) {
                    generate_trainers(numtrainers);
                } else {
                    printf("Invalid number of trainers.\n");
                    return 0;
                }
            }
        }
    }

    PriorityQueue* other_queue = create_priority_queue();
    PriorityQueue* smart_queue = create_priority_queue();

    for(int i = 0; i < character_array_size; i++) {
        if(character_array[i].type == HIKER || character_array[i].type == RIVAL || character_array[i].type == PLAYER) {
            insert(smart_queue, character_array[i]);
        } else {
            insert(other_queue, character_array[i]);
        }
    }

    print_map();

    int iter = 0;
    int ch;
    int gameEnded = 1;
    int trainer_page_start_index = 0;
    int invalid_move = 0;

    while(gameEnded) {
        if(iter % 3 == 0 && battle_with_trainer == 0 && pc_in_building == 0 && invalid_move == 0) {
            Character nextChar = dequeue(smart_queue);
            insert(smart_queue, npc_movement(nextChar));
        }
        if(iter % 3 == 1) {
            ch = getch();
            switch(ch) {
                case '7':
                case 'y':
                    char_map[pc.x][pc.y] = 0;
                    if(valid_move_for_pc(pc.x - 1, pc.y - 1) && pc_in_building == 0) {
                        pc.x--;
                        pc.y--;
                        char_map[pc.x][pc.y] = PLAYER;
                        update_dijkstra_npc();
                        print_map();
                        invalid_move = 0;
                        mvprintw(0, 0, "Player moved upper left");
                    } else {
                        invalid_move = 1;
                        char_map[pc.x][pc.y] = PLAYER;
                        print_map();
                        valid_move_for_pc(pc.x - 1, pc.y - 1);
                    }
                    break;
                case '8':
                case 'k':
                    char_map[pc.x][pc.y] = 0;
                    if(valid_move_for_pc(pc.x - 1, pc.y) && pc_in_building == 0) {
                        pc.x--;
                        char_map[pc.x][pc.y] = PLAYER;
                        update_dijkstra_npc();
                        print_map();
                        mvprintw(0, 0, "Player moved up");
                        invalid_move = 0;
                    } else {
                        invalid_move = 1;
                        char_map[pc.x][pc.y] = PLAYER;
                        print_map();
                        valid_move_for_pc(pc.x - 1, pc.y);
                    }
                    break;
                case '9':
                case 'u':
                    char_map[pc.x][pc.y] = 0;
                    if(valid_move_for_pc(pc.x - 1, pc.y + 1) && pc_in_building == 0) {
                        pc.x--;
                        pc.y++;
                        char_map[pc.x][pc.y] = PLAYER;
                        update_dijkstra_npc();
                        print_map();
                        mvprintw(0, 0, "Player moved upper right");
                        invalid_move = 0;
                    } else {
                        invalid_move = 1;
                        char_map[pc.x][pc.y] = PLAYER;
                        print_map();
                        valid_move_for_pc(pc.x - 1, pc.y + 1);
                    }
                    break;
                case '6':
                case 'l':
                    char_map[pc.x][pc.y] = 0;
                    if(valid_move_for_pc(pc.x, pc.y + 1) && pc_in_building == 0) {
                        pc.y++;
                        char_map[pc.x][pc.y] = PLAYER;
                        update_dijkstra_npc();
                        print_map();
                        mvprintw(0, 0, "Player moved right");
                        invalid_move = 0;
                    } else {
                        invalid_move = 1;
                        char_map[pc.x][pc.y] = PLAYER;
                        print_map();
                        valid_move_for_pc(pc.x, pc.y + 1);
                    }
                    break;
                case '3':
                case 'n':
                    char_map[pc.x][pc.y] = 0;
                    if(valid_move_for_pc(pc.x + 1, pc.y + 1) && pc_in_building == 0) {
                        pc.x++;
                        pc.y++;
                        char_map[pc.x][pc.y] = PLAYER;
                        update_dijkstra_npc();
                        print_map();
                        mvprintw(0, 0, "Player moved lower right");
                        invalid_move = 0;
                    } else {
                        invalid_move = 1;
                        char_map[pc.x][pc.y] = PLAYER;
                        print_map();
                        valid_move_for_pc(pc.x + 1, pc.y + 1);
                    }
                    break;
                case '2':
                case 'j':
                    char_map[pc.x][pc.y] = 0;
                    if(valid_move_for_pc(pc.x + 1, pc.y) && pc_in_building == 0) {
                        pc.x++;
                        char_map[pc.x][pc.y] = PLAYER;
                        update_dijkstra_npc();
                        print_map();
                        mvprintw(0, 0, "Player moved down");
                        invalid_move = 0;
                    } else {
                        invalid_move = 1;
                        char_map[pc.x][pc.y] = PLAYER;
                        print_map();
                        valid_move_for_pc(pc.x + 1, pc.y);
                    }
                    break;
                case '1':
                case 'b':
                    char_map[pc.x][pc.y] = 0;
                    if(valid_move_for_pc(pc.x + 1, pc.y - 1) && pc_in_building == 0) {
                        pc.x++;
                        pc.y--;
                        char_map[pc.x][pc.y] = PLAYER;
                        update_dijkstra_npc();
                        print_map();
                        mvprintw(0, 0, "Player moved lower left");
                        invalid_move = 0;
                    } else {
                        invalid_move = 1;
                        char_map[pc.x][pc.y] = PLAYER;
                        print_map();
                        valid_move_for_pc(pc.x + 1, pc.y - 1);
                    }
                    break;
                case '4':
                case 'h':
                    char_map[pc.x][pc.y] = 0;
                    if(valid_move_for_pc(pc.x, pc.y - 1) && pc_in_building == 0) {
                        pc.y--;
                        char_map[pc.x][pc.y] = PLAYER;
                        update_dijkstra_npc();
                        print_map();
                        mvprintw(0, 0, "Player moved left");
                        invalid_move = 0;
                    } else {
                        invalid_move = 1;
                        char_map[pc.x][pc.y] = PLAYER;
                        print_map();
                        valid_move_for_pc(pc.x, pc.y - 1);
                    }
                    break;
                case '>':
                    if(earth[LY][LX].map[pc.x][pc.y] == POKECENTER) {
                        mvprintw(0, 0, "Player entered Pokecenter.");
                        pc_in_building = 1;
                        invalid_move = 0;
                    } else if(earth[LY][LX].map[pc.x][pc.y] == POKEMART) {
                        mvprintw(0, 0, "Player entered a Pokemart.");
                        invalid_move = 0;
                        pc_in_building = 1;
                    } else {
                        invalid_move = 1;
                        mvprintw(0, 0, "No building found.");
                    }
                    break;
                case '<':
                    print_map();
                    if(pc_in_building == 1) {
                        mvprintw(0, 0, "Player left building.");
                        pc_in_building = 0;
                        invalid_move = 0;
                    } else {
                        invalid_move = 1;
                        mvprintw(0, 0, "Player is not in a building.");
                    }
                    break;
                case '5':
                case ' ':
                case '.':
                    print_map();
                    mvprintw(0, 0, "Player rests.");
                    break;
                case 't':
                    trainer_page_open = 1;
                    clear();
                    trainer_page(0);
                    refresh();
                    break;
                case 27:
                    if(trainer_page_open == 1) {
                        invalid_move = 1;
                        trainer_page_open = 0;
                        print_map();
                    } else if (battle_with_trainer == 1) {
                        battle_with_trainer = 0;
                        defeated_trainers[current_battle_trainer] = 1;
                        current_battle_trainer = 0;
                        invalid_move = 0;
                        print_map();
                        mvprintw(0, 0, "You defeated the trainer!");
                    }
                    break;
                case KEY_UP:
                    invalid_move = 1;
                    if((character_array_size > 20) && (character_array_size - trainer_page_start_index) > 20 && trainer_page_open == 1){
                        trainer_page_start_index++;
                        trainer_page(trainer_page_start_index);
                    }
                    break;
                case KEY_DOWN:
                    invalid_move = 1;
                    if(trainer_page_start_index > 0 && trainer_page_open == 1) {
                        trainer_page_start_index--;
                        trainer_page(trainer_page_start_index);
                    }
                    break;
                case 'q':
                    gameEnded = 0;
                    break;
            }
        }
        if(iter % 3 == 2 && battle_with_trainer == 0 && pc_in_building == 0 && invalid_move == 0) {
            for(int i = 0; i < rand_int(3, 6); i++) {
                Character nextChar = dequeue(other_queue);
                insert(other_queue, npc_movement(nextChar));
            }
        }
        iter++;
        check_for_trainer(pc.x, pc.y);
    };

    endwin();
    return 0;
}

// TODO LIST!