#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <ncurses.h>

/* Global Constants */
#define ROWS 20
#define COLS 10
#define SHAPE_TYPES 7

/* Global Variables */
char tetris_table[ROWS][COLS] = {0};
int game_score = 0;
char game_running = 1;
suseconds_t timer = 400000;
int decrease = 1000;

/* Global Struct for time evaluation for Tetriminos falling */
struct timeval before_now;

/* Main Tetrimino class */
class Tetrimino {
	public:
    	char **array;
    	int width, row, col, color;
};

/* Global Tetrimino variable for current shape falling */
Tetrimino current_tetrimino;

/* Tetris Shapes defined in 2D arrays */
const Tetrimino tetrimino_shapes[SHAPE_TYPES] = {
	{(char *[]){(char []){0,1,1},(char []){1,1,0}, (char []){0,0,0}}, 3, 3},                           // S     
	{(char *[]){(char []){1,1,0},(char []){0,1,1}, (char []){0,0,0}}, 3, 2},                           // Z     
	{(char *[]){(char []){0,1,0},(char []){1,1,1}, (char []){0,0,0}}, 3, 6},                           // T     
	{(char *[]){(char []){0,0,1},(char []){1,1,1}, (char []){0,0,0}}, 3, 5},                           // L     
	{(char *[]){(char []){1,0,0},(char []){1,1,1}, (char []){0,0,0}}, 3, 1},                           // J  
	{(char *[]){(char []){1,1},(char []){1,1}}, 2, 4},                                                 // O 
	{(char *[]){(char []){0,0,0,0}, (char []){1,1,1,1}, (char []){0,0,0,0}, (char []){0,0,0,0}}, 4, 7} // I
};

/* Function Prototypes */
Tetrimino grab_shape(Tetrimino tet);
void remove_shape(Tetrimino tet);
int check_shape_position(Tetrimino tet);
void set_new_random_shape();
void rotate_shape(Tetrimino *tet);
void write_shape_to_table();
void remove_full_rows();
void print_table();
void manipulate_current(int action);
int time_to_update(struct timeval *before, struct timeval *now);
void initialize_game();
void initialize_color_pairs();
void end_game();


/* Main Function */
int main() {
    initialize_game();
    struct timeval before, now;
    int c;
    while (game_running) {
        if ((c = getch()) != ERR) {
            manipulate_current(c);
        }
        gettimeofday(&now, NULL);
        if (time_to_update(&before, &now)) {
            manipulate_current('s');
            gettimeofday(&before, NULL);
        }
    }
    end_game();
    return 0;
}

/* This function grabs the next shape when the current one is placed. */
Tetrimino grab_shape(Tetrimino tet) {
    Tetrimino new_tetrimino = tet;
    new_tetrimino.array = (char**) malloc(new_tetrimino.width * sizeof(char*));
	new_tetrimino.color = tet.color;

    for (int i = 0; i < new_tetrimino.width; i++) {
        new_tetrimino.array[i] = (char*) malloc(new_tetrimino.width * sizeof(char));
        for (int j = 0; j < new_tetrimino.width; j++) {
            new_tetrimino.array[i][j] = tet.array[i][j];
        }
    }
    return new_tetrimino;
}

/* This removes the tetrimino when it is placed at the bottom of the table */
void remove_shape(Tetrimino tet) {
    for (int i = 0; i < tet.width; i++) {
        free(tet.array[i]);
    }
    free(tet.array);
}

/* This makes sure that the tetrimino is inside of the bounds. */
int check_shape_position(Tetrimino tet) {
    for (int i = 0; i < tet.width; i++) {
        for (int j = 0; j < tet.width; j++) {
            if ((tet.col + j < 0 || tet.col + j >= COLS || tet.row + i >= ROWS) && tet.array[i][j]) {
                return 0;
            } else if (tetris_table[tet.row + i][tet.col + j] && tet.array[i][j]) {
                return 0;
            }
        }
    }
    return 1;
}


void set_new_random_shape() {
    Tetrimino new_tetrimino = grab_shape(tetrimino_shapes[rand() % SHAPE_TYPES]);
    new_tetrimino.col = rand() % (COLS - new_tetrimino.width + 1);
    new_tetrimino.row = 0;
	new_tetrimino.color = rand() % 7 + 1;
    remove_shape(current_tetrimino);
    current_tetrimino = new_tetrimino;

    if (!check_shape_position(current_tetrimino)) {
        game_running = 0;
    }
}


void rotate_shape(Tetrimino *tet) {
    Tetrimino temp = grab_shape(*tet);
    int width = tet->width;

    for (int i = 0; i < width; i++) {
        for (int j = 0, k = width - 1; j < width; j++, k--) {
            tet->array[i][j] = temp.array[k][i];
        }
    }
    remove_shape(temp);
}

void write_shape_to_table() {
    for (int i = 0; i < current_tetrimino.width; i++) {
        for (int j = 0; j < current_tetrimino.width; j++) {
            if (current_tetrimino.array[i][j]) {
                tetris_table[current_tetrimino.row + i][current_tetrimino.col + j] = current_tetrimino.array[i][j];
            }
        }
    }
}

void remove_full_rows() {
    int full_row_count = 0;

    for (int i = 0; i < ROWS; i++) {
        int row_sum = 0;
        for (int j = 0; j < COLS; j++) {
            row_sum += tetris_table[i][j];
        }

        if (row_sum == COLS) {
            full_row_count++;
            for (int k = i; k >= 1; k--) {
                for (int l = 0; l < COLS; l++) {
                    tetris_table[k][l] = tetris_table[k - 1][l];
                }
            }
            for (int l = 0; l < COLS; l++) {
                tetris_table[0][l] = 0;
            }
            timer -= decrease--;
        }
    }
    game_score += 100 * full_row_count;
}


void print_table() {
    char buffer[ROWS][COLS] = {0};

    for (int i = 0; i < current_tetrimino.width; i++) {
        for (int j = 0; j < current_tetrimino.width; j++) {
            if (current_tetrimino.array[i][j]) {
                buffer[current_tetrimino.row + i][current_tetrimino.col + j] = current_tetrimino.array[i][j];
            }
        }
    }

    clear();

    printw("Score: %d\n\n", game_score);

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (tetris_table[i][j] + buffer[i][j]) {
                attron(COLOR_PAIR(current_tetrimino.color));
                printw("  "); // Print a block of color
                attroff(COLOR_PAIR(current_tetrimino.color));
            } else {
                printw(". "); // Print a dot for empty spaces
            }
        }
        printw("\n");
    }

}


void manipulate_current(int action) {
    Tetrimino temp = grab_shape(current_tetrimino);

    switch (action) {
        case 's': // Move down
            temp.row++;
            if (check_shape_position(temp)) current_tetrimino.row++;
            else {
                write_shape_to_table();
                remove_full_rows();
                set_new_random_shape();
            }
            break;
        case 'd': // Move right
            temp.col++;
            if (check_shape_position(temp)) current_tetrimino.col++;
            break;
        case 'a': // Move left
            temp.col--;
            if (check_shape_position(temp)) current_tetrimino.col--;
            break;
        case 'w': // Rotate clockwise
            rotate_shape(&temp);
            if (check_shape_position(temp)) rotate_shape(&current_tetrimino);
            break;
    }

    remove_shape(temp);
    print_table();
}


int time_to_update(struct timeval *before, struct timeval *now) {
    return ((suseconds_t)(now->tv_sec * 1000000 + now->tv_usec) -
            ((suseconds_t)before->tv_sec * 1000000 + before->tv_usec)) > timer;
}

void initialize_color_pairs() {
	init_pair(1, COLOR_WHITE, COLOR_WHITE);
	init_pair(2, COLOR_RED, COLOR_RED);
	init_pair(3, COLOR_GREEN, COLOR_GREEN);
	init_pair(4, COLOR_YELLOW, COLOR_YELLOW);
	init_pair(5, COLOR_BLUE, COLOR_BLUE);
	init_pair(6, COLOR_MAGENTA, COLOR_MAGENTA);
	init_pair(7, COLOR_CYAN, COLOR_CYAN);
}


void initialize_game() {
    srand(time(0)); // Seed the random number generator
    game_score = 0; // Reset the game score
    game_running = 1; // Set the game as running
    initscr(); // Initialize the screen for ncurses
	noecho(); // Don't echo user input
	curs_set(0); // Hide the cursor
	start_color(); // Enable color
	initialize_color_pairs(); // Initialize color pairs
	keypad(stdscr, TRUE); // Enable the keypad
    timeout(1); // Set a timeout for user input

    set_new_random_shape(); // Set the first random tet
    gettimeofday(&before_now, NULL); // Get the current time

    print_table(); // Print the initial game table
}


void end_game() {
    remove_shape(current_tetrimino);
    endwin();

    printf("\nScore: %d\n", game_score);
}