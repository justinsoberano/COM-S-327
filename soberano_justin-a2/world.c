#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <math.h>

#define ROWS 21
#define COLS 80

#define TREE '^'
#define BORDER '%'

#define PATH '#'
#define POKECENTER 'C'
#define POKEMART 'M'

#define TALL_GRASS ':'
#define SHORT_GRASS '.'
#define WATER '~'

#define BLANK ' '

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

world earth[401][401] = {{{{{'\0'}}, 0, 0, 0, 0}}};

/* Djikstra's Algorithm */
typedef struct {
    int x, y;
} Point;
typedef struct {
    int x, y, dist;
} Node;
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
    for(int j = 0; j < ROWS; j++) {
        for(int i = 0; i < COLS; i++) {
            printf("%c", earth[LY][LX].map[j][i]);
        }
        printf("\n");
    }
    printf("Position: (%d, %d)\n", LX - 200, LY - 200);
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
    print_map();
}

void move() {
    char n = '\0';
    while(n != 'q') {
        scanf(" %c", &n);
        if(n == 'n') {
            LY += 1;
            if (LY > 400) {
                LY = 400;
                printf("Out of bounds.\n");
            } else {
                gen();
            }
        } else if(n == 's') {
            LY -= 1;
            if (LY < 0) {
                LY = 0;
                printf("Out of bounds.\n");
            } else {
                gen();
            }
        } else if(n == 'e') {
            LX += 1;
            if (LX > 400) {
                LX = 400;
                printf("Out of bounds.\n");
            } else {
                gen();
            }
        } else if(n == 'w') {
            LX -= 1;
            if(LX < 0) {
                LX = 0;
                printf("Out of bounds.\n");
            } else {
                gen();
            }
        } else if(n == 'f') {
            int x, y;
            scanf("%d %d", &x, &y);
            if (x > 200 || x < -200 || y > 200 || y < -200) {
                printf("Invalid Coordinates\n");
            } else {
                LX = x + 200;
                LY = y + 200;
                gen();
            }
        } else {
            printf("Invalid Command\n");
        }
    }
    printf("Quitting...\n");
}

int main(int argc, char const *argv[]) {
    init_rand();
    gen();
    move();
    return 0;
}