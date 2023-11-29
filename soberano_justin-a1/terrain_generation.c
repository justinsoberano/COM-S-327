#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>

#define ROWS 21
#define COLS 80

#define TREE '^'
#define MOUNTAIN '%'

#define PATH '#'
#define POKECENTER 'C'
#define POKEMART 'M'

#define TALL_GRASS ':'
#define SHORT_GRASS '.'
#define WATER '~'

#define BLANK ' '

typedef struct {
    int x;
    int y;
    int type;
} tile;

typedef struct {
    int x, y;
} Point;

typedef struct {
    int x, y, dist;
} Node;

char map[80][21];

/* Implementation of Dijkstra's Algorithm ------------------------------------------ */
int is_valid(int x, int y, char map[80][21]) {
    return x >= 0 && x < ROWS && y >= 0 && y < COLS && (x == 0 || x == ROWS - 1 || y == 0 || y == COLS - 1 || map[x][y] != '#');
}

int dijkstra(Point start, Point end, char map[80][21]) {
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
            if (is_valid(nx, ny, map)) {
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
        map[curr.x][curr.y] = '#';
        curr = (Point){prev[curr.x][curr.y].x, prev[curr.x][curr.y].y};
    }
    map[start.x][start.y] = '#';
    map[end.x][end.y] = '#';

    return dist[end.x][end.y];
}


/* Initialization of Random ------------------------------------------------------- */
void initialize_random_seed() { srand(time(NULL)); }                                //
int random_number(int min, int max) { return (rand() % ((max + 1) - min) + min); }  // 
/* -------------------------------------------------------------------------------- */

/* Implementation of a Queue in C for the X and Y coordinates and type of tile ---- */
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
        int type = random_number(1, 3);
        int x = random_number(5, 75);
        int y = random_number(2, 19);
        push(x, y, type);
        size++;
    }
}
/* -------------------------------------------------------------------------------- */

void initialize_map() {
    for(int i = 0; i < 21; i++) {
        for(int j = 0; j < 80; j++) {
            if (j == 0 || i == 0 || i == 20 || j == 79) map[j][i] = MOUNTAIN; else map[j][i] = BLANK;
        }
    }
}

void expand_terrain(int x, int y, int type) {
    // Make a for loop that checks for all surrounding BLANK tiles.
    // If empty push x, y, and type into queue and expand terrain.
    if (x > 0 && map[x - 1][y] == BLANK) {
        push(x - 1, y, type);
    }
    if (x < 79 && map[x + 1][y] == BLANK) {
        push(x + 1, y, type);
    }
    if (y > 0 && map[x][y - 1] == BLANK) {
        push(x, y - 1, type);
    }
    if (y < 20 && map[x][y + 1] == BLANK) {
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
                map[x][y] = TALL_GRASS;
                expand_terrain(x, y, type);
                break;
            case 2:
                map[x][y] = SHORT_GRASS;
                expand_terrain(x, y, type);
                break;
            case 3:
                map[x][y] = WATER;
                expand_terrain(x, y, type);
                break;
        }
    }
}

void print_map() {
    for (int i = 0; i < 21; i++) {
        for (int j = 0; j < 80; j++) {
            printf("%c", map[j][i]);
        }
        printf("\n");
    }
}

void pokecenter() {
    int x = random_number(5, 75);
    for(int j = 0; j < 21; j++) {
        for(int i = 0; i < 80; i++) {
            if (i == x) {
                if (map[i][j] == PATH && map[i][j + 1] != PATH) {
                    map[i][j + 1] = POKECENTER;
                    return;
                }
            }
        }
    }
}

void pokemart() {
    int y = random_number(5, 75);
    for(int j = 0; j < 21; j++) {
        for(int i = 0; i < 80; i++) {
            if (i == y) {
                if (map[i][j] == PATH && map[i][j - 1] != PATH) {
                    map[i][j - 1] = POKEMART;
                    return;
                }
            }
        }
    }
}

void verify() {
    for(int j = 0; j < 21; j++) {
        for(int i = 0; i < 80; i++) {
            if (map[i][j] == BLANK) {
                map[i][j] = TALL_GRASS;
            }
        }
    }
}

void add_trees() {
    for(int j = 0; j < 20; j++) {
        for(int i = 0; i < 80; i++) {
            int r = random_number(1, 20);
            if(map[i][j] == SHORT_GRASS && r == 3) {
                map[i][j] = TREE;
            }
        }
    }
}

void generate() {
    initialize_map();
    init_queue(20);
    generate_terrain();
    pokecenter();
    pokemart();
    add_trees();
    verify();
}

void generate_path(int sX, int sY, int eX, int eY) {
    Point start = {sX, sY};
    Point end = {eX, eY};
    dijkstra(start, end, map);
}

int main(int argc, char *argv[]) {
    initialize_random_seed();
    generate();
    generate_path(5, 70, 15, 10);
    print_map();
    return 0;
}