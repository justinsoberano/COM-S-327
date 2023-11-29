#include "game.h"

void initialize_ncurses() {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    start_color();
    
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_WHITE);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, 82, COLOR_BLACK);
    init_pair(6, COLOR_YELLOW, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);

    init_pair(8, COLOR_BLACK, COLOR_WHITE);

}

void handle_args(int argc, char const *argv[]) {
    if(argc == 1) {
        generate_trainers(numtrainers);
    } else {
        for(int i = 1; i < argc; i++) {
            if(strncmp(argv[i], "--", 2) == 0) {
                numtrainers = atoi(argv[i] + 2);
                if (numtrainers > 0) {
                    generate_trainers(numtrainers);
                } else {
                    printf("Invalid number of trainers.\n");
                    exit(0);
                }
            }
        }
    }
}

int start(int argc, char const *argv[]) {
    init_rand();
    file_parsing();
    gen();
    initialize_ncurses();
    handle_args(argc, argv);

    pokedex p = select_first_pokemon();
    owned_pokemon.push_back(p);
    
    game();
    endwin();
    return 0;
}