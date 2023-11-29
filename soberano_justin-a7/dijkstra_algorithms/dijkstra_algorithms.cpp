#include "dijkstra_algorithms.h"

int hiker_costs[] = {INT_MAX, INT_MAX, 10, 50, 50, 15, 10, INT_MAX, INT_MAX, 0};
int rival_costs[] = {INT_MAX, INT_MAX, 10, 50, 50, 20, 10, INT_MAX, INT_MAX, 0};

int dist_map_hiker[ROWS][COLS] = {{INT_MAX}};
int dist_map_rival[ROWS][COLS] = {{INT_MAX}};
int dist_map_pacer[ROWS][COLS] = {{INT_MAX}};
int dist_map_wanderer[ROWS][COLS] = {{INT_MAX}};
int dist_map_explorer[ROWS][COLS] = {{INT_MAX}};

TerrainType m[ROWS][COLS];

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
    return 0;
}

void reset_distance_map() {
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            dist_map[i][j] = INT_MAX;
        }
    }
}

void calculate_distance_maps() {
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
}

void update_dijkstra_npc() {
    reset_distance_map();
    dijkstra_for_pc(pc, Hiker, m, dist_map_hiker, dist_map);
    reset_distance_map();
    dijkstra_for_pc(pc, Rival, m, dist_map_rival, dist_map);
}
