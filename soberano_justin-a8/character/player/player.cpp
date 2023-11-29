#include "player.h"

using namespace std;

vector<pokedex> owned_pokemon;
Character currently_battling;

void view_own_pokemon() {

    clear();
    pokedex_open = 1;

    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            if(i == 0 || j == 0 || i == ROWS - 1 || j == COLS - 1) {
                attron(COLOR_PAIR(3));
                mvprintw(i + 1, j, "#");
                attroff(COLOR_PAIR(3));
            }
        }
    }

    attron(COLOR_PAIR(8));
    mvprintw(1, 36, "POKEDEX");
    attroff(COLOR_PAIR(8));

    int spacer = 3;
    for(int i = 0; i < (int)owned_pokemon.size(); i++) {
        pokedex p = owned_pokemon[i];
        mvprintw((spacer * (i + 1)), 3, "%d. [%d][%c] %s | Moves: %s + %s", (i + 1), p.get_level(), ((p.get_gender() == 0) ? 'F' : 'M'), p.get_pokemon().get_identifier().c_str(), 
                                                                                     p.get_p_moves()[0].get_identifier().c_str(), p.get_p_moves()[1].get_identifier().c_str());
        mvprintw((spacer * (i + 1) + 1), 3, "Stats | HP: %d ATK: %d DEF %d SPD %d S-ATK: %d S-DEF: %d ", p.get_hp(), p.get_attack(), p.get_defense(), p.get_speed(), p.get_special_attack(), p.get_special_defense());
        mvprintw(22, 0, "[1] | Back");
        refresh();
    }

}

void battle_trainer_view(Character const &c) {
    clear();

    character_battle_page_open = 1;

    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            if(i == 0 || j == 0 || i == ROWS - 1 || j == COLS - 1) {
                attron(COLOR_PAIR(3));
                mvprintw(i + 1, j, "#");
                attroff(COLOR_PAIR(3));
            }
        }
    }

    attron(COLOR_PAIR(8));
    mvprintw(1, 32, "POKEMON BATTLE");
    mvprintw(21, 27, "TO BE IMPLEMENTED IN 1.09");
    attroff(COLOR_PAIR(8));

    mvprintw(22, 0, "[1] | Back");
    mvprintw(23, 0, "[ESC] | Defeat");

    refresh();
}

void view_trainer_pokemon(Character const &c) {

    clear();

    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            if(i == 0 || j == 0 || i == ROWS - 1 || j == COLS - 1) {
                attron(COLOR_PAIR(3));
                mvprintw(i + 1, j, "#");
                attroff(COLOR_PAIR(3));
            }
        }
    }

    attron(COLOR_PAIR(8));
    mvprintw(1, 31, "TRAINER'S POKEMON");
    attroff(COLOR_PAIR(8));
    refresh();

    int spacer = 3;
    for(int i = 0; i < (int)c.trainer_pokemon.size(); i++) {
        pokedex p = c.trainer_pokemon[i];
        mvprintw((spacer * (i + 1)), 3, "%d. [%d][%c] %s | Moves: %s + %s", (i + 1), p.get_level(), ((p.get_gender() == 0) ? 'F' : 'M'), p.get_pokemon().get_identifier().c_str(), 
                                                                                     p.get_p_moves()[0].get_identifier().c_str(), p.get_p_moves()[1].get_identifier().c_str());
        mvprintw((spacer * (i + 1) + 1), 3, "Stats | HP: %d ATK: %d DEF %d SPD %d S-ATK: %d S-DEF: %d ", p.get_hp(), p.get_attack(), p.get_defense(), p.get_speed(), p.get_special_attack(), p.get_special_defense());
        mvprintw(22, 0, "[1] | Back");
        refresh();
    }
}

void battle(int x, int y) {
    int dx[] = {-1, 0, 1, 0, -1, -1, 1, 1};
    int dy[] = {0, 1, 0, -1, -1, 1, -1, 1};
    int nx = 0;
    int ny = 0;
    for(int i = 0; i < 8; i++) {
        nx = x + dx[i];
        ny = y + dy[i];
        for(int j = 0; j < character_array_size; j++) {
            if(earth[LY][LX].characters[j].position.get_position_x() == nx && earth[LY][LX].characters[j].position.get_position_y() == ny) {
                currently_battling = earth[LY][LX].characters[j];
                mvprintw(22, 0, "[p] | View Trainer's Pokemon");
                mvprintw(23, 0, "[o] | Battle Trainer! ");
                refresh();
            }
        }
    }
}

int valid_move_for_pc(int x, int y) {
    if(x >= 0 && x < ROWS && y >= 0 && y < COLS && earth[LY][LX].map[x][y] != BORDER) {
        if(pc_in_building == 1) {
            mvprintw(0, 0, "Leave the building first [<].");
            return 0;
        } else if (battle_with_trainer == 1) {
            mvprintw(0, 0, "You have encountered a trainer.");
            battle(pc.get_position_x(), pc.get_position_y());
            return 0;
        } else if (earth[LY][LX].char_map[x][y] != 0) {
            if(is_trainer_defeated(earth[LY][LX].char_map[x][y])) {
                mvprintw(0, 0, "You have defeated this trainer.");
                return 0;
            } else {
                battle_with_trainer = 1;
                current_battle_trainer = connect_enum_to_trainer(earth[LY][LX].char_map[x][y]);
                return 0;
            }
        } else if (earth[LY][LX].map[x][y] == TREE) {
            mvprintw(0, 0, "A tree blocks your path.");
            return 0;
        } else if (earth[LY][LX].map[x][y] == WATER) {
            mvprintw(0, 0, "You cannot swim.");
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
        if(earth[LY][LX].char_map[next_x][next_y] != 0) {
            if(is_trainer_defeated(earth[LY][LX].char_map[next_x][next_y])) {
                mvprintw(0, 0, "You have defeated this trainer.");
            } else {
                battle_with_trainer = 1;
                current_battle_trainer = connect_enum_to_trainer(earth[LY][LX].char_map[next_x][next_y]);
            }
        }
    }   
}

void check_if_at_gate(int x, int y) {
    if(earth[LY][LX].west_gate == x && y + 1 == 80) {
        LX++;
        gen();
        if(earth[LY][LX].characters == NULL) {
            character_array_size = 0;
            generate_trainers(numtrainers);
            earth[LY][LX].smart_queue = create_priority_queue();
            earth[LY][LX].other_queue = create_priority_queue();
            initialize_trainer_queue_at_map();
        }
        calculate_distance_maps();
        update_dijkstra_npc();
        pc.set_position_x(earth[LY][LX].east_gate);
        pc.set_position_y(0);
        print_map();
    } else if (earth[LY][LX].east_gate == x && y == 0) {
        LX--;
        gen();
        if(earth[LY][LX].characters == NULL) {
            character_array_size = 0;
            generate_trainers(numtrainers);
            earth[LY][LX].smart_queue = create_priority_queue();
            earth[LY][LX].other_queue = create_priority_queue();
            initialize_trainer_queue_at_map();
        }
        calculate_distance_maps();
        update_dijkstra_npc();
        pc.set_position_x(earth[LY][LX].west_gate);
        pc.set_position_y(79);
        print_map();
    } else if (x == 0 && earth[LY][LX].north_gate == y) {
        LY--;
        gen();
        if(earth[LY][LX].characters == NULL) {
            character_array_size = 0;
            generate_trainers(numtrainers);
            earth[LY][LX].smart_queue = create_priority_queue();
            earth[LY][LX].other_queue = create_priority_queue();
            initialize_trainer_queue_at_map();
        }
        calculate_distance_maps();
        update_dijkstra_npc();
        pc.set_position_x(20);
        pc.set_position_y(earth[LY][LX].south_gate);
        print_map();
    } else if (x == 20 && earth[LY][LX].south_gate == y) {
        LY++;
        gen();
        if(earth[LY][LX].characters == NULL) {
            character_array_size = 0;
            generate_trainers(numtrainers);
            earth[LY][LX].smart_queue = create_priority_queue();
            earth[LY][LX].other_queue = create_priority_queue();
            initialize_trainer_queue_at_map();
        }
        calculate_distance_maps();
        update_dijkstra_npc();
        pc.set_position_x(0);
        pc.set_position_y(earth[LY][LX].north_gate);
        print_map();
    }
}

void fly() {
    int x;
    int y;
    print_map();    
    mvprintw(0, 0, "COORDS: [%d, %d]", LX - 200, LY - 200);
    mvprintw(22, 0, "Fly: ");
    echo();
    scanw(const_cast<char*>("%d %d"), &x, &y);
    noecho();

    LX = 200;
    LY = 200;

    if(LX + x <= 400 && LX + x >= 0 && LY + y <= 400 && LY + y >= 0) {

        earth[LY][LX].char_map[pc.get_position_x()][pc.get_position_y()] = 0;

        LX = x + 200;
        LY = y + 200;

        if(earth[LY][LX].map[0][0] == '\0') {
            gen();
        }

        if(earth[LY][LX].characters == NULL) {
            character_array_size = 0;
            generate_trainers(numtrainers);
            earth[LY][LX].smart_queue = create_priority_queue();
            earth[LY][LX].other_queue = create_priority_queue();
            initialize_trainer_queue_at_map();
        }
        pc.set_position_x(rand_int(5, 15));
        for(int i = 0; i < COLS; i++) {
            if(earth[LY][LX].map[pc.get_position_x()][i] == PATH) {
                pc.set_position_y(i);
                break;
            }
        }
        calculate_distance_maps();
        update_dijkstra_npc();
        earth[LY][LX].char_map[pc.get_position_x()][pc.get_position_y()] = PLAYER;
        print_map();
        mvprintw(0, 0, "You have teleported to %d, %d", LX - 200, LY - 200);
    } else {
        mvprintw(22, 0, "Invalid coordinates, choose between [-200, 200]");
    }
}

void game() {

    print_map();

    int iter = 0;
    int ch;
    int gameEnded = 1;
    int trainer_page_start_index = 0;
    int invalid_move = 0;

    while(gameEnded) {
        if(iter % 2 == 0) {
            mvprintw(0, 69, "Pokedex [m]");
            ch = getch();
            switch(ch) {
                case '7':
                case 'y':
                    earth[LY][LX].char_map[pc.get_position_x()][pc.get_position_y()] = 0;
                    if(valid_move_for_pc(pc.get_position_x() - 1, pc.get_position_y() - 1) && pc_in_building == 0) {
                        check_if_at_gate(pc.get_position_x() - 1, pc.get_position_y() - 1);
                        pc.set_position_x(pc.get_position_x() - 1);
                        pc.set_position_y(pc.get_position_y() - 1);
                        earth[LY][LX].char_map[pc.get_position_x()][pc.get_position_y()] = PLAYER;
                        update_dijkstra_npc();
                        print_map();
                        invalid_move = 0;
                        mvprintw(0, 0, "Player moved upper left");
                        pokemon_encountered(pc.get_position_x(), pc.get_position_y());
                    } else {
                        invalid_move = 1;
                        earth[LY][LX].char_map[pc.get_position_x()][pc.get_position_y()] = PLAYER;
                        print_map();
                        valid_move_for_pc(pc.get_position_x() - 1, pc.get_position_y() - 1);
                    }
                    break;
                case '8':
                case 'k':
                    earth[LY][LX].char_map[pc.get_position_x()][pc.get_position_y()] = 0;
                    if(valid_move_for_pc(pc.get_position_x() - 1, pc.get_position_y()) && pc_in_building == 0) {
                        check_if_at_gate(pc.get_position_x() - 1, pc.get_position_y());
                        pc.set_position_x(pc.get_position_x() - 1);
                        earth[LY][LX].char_map[pc.get_position_x()][pc.get_position_y()] = PLAYER;
                        update_dijkstra_npc();
                        print_map();
                        mvprintw(0, 0, "Player moved up");
                        pokemon_encountered(pc.get_position_x(), pc.get_position_y());
                        invalid_move = 0;
                    } else {
                        invalid_move = 1;
                        earth[LY][LX].char_map[pc.get_position_x()][pc.get_position_y()] = PLAYER;
                        print_map();
                        valid_move_for_pc(pc.get_position_x() - 1, pc.get_position_y());
                    }
                    break;
                case '9':
                case 'u':
                    earth[LY][LX].char_map[pc.get_position_x()][pc.get_position_y()] = 0;
                    if(valid_move_for_pc(pc.get_position_x() - 1, pc.get_position_y() + 1) && pc_in_building == 0) {
                        check_if_at_gate(pc.get_position_x() - 1, pc.get_position_y() + 1);
                        pc.set_position_x(pc.get_position_x() - 1);
                        pc.set_position_y(pc.get_position_y() + 1);
                        earth[LY][LX].char_map[pc.get_position_x()][pc.get_position_y()] = PLAYER;
                        update_dijkstra_npc();
                        print_map();
                        mvprintw(0, 0, "Player moved upper right");
                        pokemon_encountered(pc.get_position_x(), pc.get_position_y());
                        invalid_move = 0;
                    } else {
                        invalid_move = 1;
                        earth[LY][LX].char_map[pc.get_position_x()][pc.get_position_y()] = PLAYER;
                        print_map();
                        valid_move_for_pc(pc.get_position_x() - 1, pc.get_position_y() + 1);
                    }
                    break;
                case '6':
                case 'l':
                    earth[LY][LX].char_map[pc.get_position_x()][pc.get_position_y()] = 0;
                    if(valid_move_for_pc(pc.get_position_x(), pc.get_position_y() + 1) && pc_in_building == 0) {
                        check_if_at_gate(pc.get_position_x(), pc.get_position_y() + 1);
                        pc.set_position_y(pc.get_position_y() + 1);
                        earth[LY][LX].char_map[pc.get_position_x()][pc.get_position_y()] = PLAYER;
                        update_dijkstra_npc();
                        print_map();
                        mvprintw(0, 0, "Player moved right");
                        pokemon_encountered(pc.get_position_x(), pc.get_position_y());
                        invalid_move = 0;
                    } else {
                        invalid_move = 1;
                        earth[LY][LX].char_map[pc.get_position_x()][pc.get_position_y()] = PLAYER;
                        print_map();
                        valid_move_for_pc(pc.get_position_x(), pc.get_position_y() + 1);
                    }
                    break;
                case '3':
                case 'n':
                    earth[LY][LX].char_map[pc.get_position_x()][pc.get_position_y()] = 0;
                    if(valid_move_for_pc(pc.get_position_x() + 1, pc.get_position_y() + 1) && pc_in_building == 0) {
                        check_if_at_gate(pc.get_position_x() + 1, pc.get_position_y() + 1);
                        pc.set_position_x(pc.get_position_x() + 1);
                        pc.set_position_x(pc.get_position_y() + 1);
                        earth[LY][LX].char_map[pc.get_position_x()][pc.get_position_y()] = PLAYER;
                        update_dijkstra_npc();
                        print_map();
                        mvprintw(0, 0, "Player moved lower right");
                        pokemon_encountered(pc.get_position_x(), pc.get_position_y());
                        invalid_move = 0;
                    } else {
                        invalid_move = 1;
                        earth[LY][LX].char_map[pc.get_position_x()][pc.get_position_y()] = PLAYER;
                        print_map();
                        valid_move_for_pc(pc.get_position_x() + 1, pc.get_position_y() + 1);
                    }
                    break;
                case '2':
                case 'j':
                    earth[LY][LX].char_map[pc.get_position_x()][pc.get_position_y()] = 0;
                    if(valid_move_for_pc(pc.get_position_x() + 1, pc.get_position_y()) && pc_in_building == 0) {
                        check_if_at_gate(pc.get_position_x() + 1, pc.get_position_y());
                        pc.set_position_x(pc.get_position_x() + 1);
                        earth[LY][LX].char_map[pc.get_position_x()][pc.get_position_y()] = PLAYER;
                        update_dijkstra_npc();
                        print_map();
                        mvprintw(0, 0, "Player moved down");
                        pokemon_encountered(pc.get_position_x(), pc.get_position_y());
                        invalid_move = 0;
                    } else {
                        invalid_move = 1;
                        earth[LY][LX].char_map[pc.get_position_x()][pc.get_position_y()] = PLAYER;
                        print_map();
                        valid_move_for_pc(pc.get_position_x() + 1, pc.get_position_y());
                    }
                    break;
                case '1':
                case 'b':
                    character_battle_page_open = 0;
                    pokedex_open = 0;
                    earth[LY][LX].char_map[pc.get_position_x()][pc.get_position_y()] = 0;
                    if(valid_move_for_pc(pc.get_position_x() + 1, pc.get_position_y() - 1) && pc_in_building == 0) {
                        check_if_at_gate(pc.get_position_x() + 1, pc.get_position_y() - 1);
                        pc.set_position_x(pc.get_position_x() + 1);
                        pc.set_position_y(pc.get_position_y() - 1);
                        earth[LY][LX].char_map[pc.get_position_x()][pc.get_position_y()] = PLAYER;
                        update_dijkstra_npc();
                        print_map();
                        mvprintw(0, 0, "Player moved lower left");
                        pokemon_encountered(pc.get_position_x(), pc.get_position_y());
                        invalid_move = 0;
                    } else {
                        invalid_move = 1;
                        earth[LY][LX].char_map[pc.get_position_x()][pc.get_position_y()] = PLAYER;
                        print_map();
                        valid_move_for_pc(pc.get_position_x() + 1, pc.get_position_y() - 1);
                    }
                    break;
                case '4':
                case 'h':
                    earth[LY][LX].char_map[pc.get_position_x()][pc.get_position_y()] = 0;
                    if(valid_move_for_pc(pc.get_position_x(), pc.get_position_y() - 1) && pc_in_building == 0) {
                        check_if_at_gate(pc.get_position_x(), pc.get_position_y() - 1);
                        pc.set_position_y(pc.get_position_y() - 1);
                        earth[LY][LX].char_map[pc.get_position_x()][pc.get_position_y()] = PLAYER;
                        update_dijkstra_npc();
                        print_map();
                        mvprintw(0, 0, "Player moved left");
                        pokemon_encountered(pc.get_position_x(), pc.get_position_y());
                        invalid_move = 0;
                    } else {
                        invalid_move = 1;
                        earth[LY][LX].char_map[pc.get_position_x()][pc.get_position_y()] = PLAYER;
                        print_map();
                        valid_move_for_pc(pc.get_position_x(), pc.get_position_y() - 1);
                    }
                    break;
                case '>':
                    if(earth[LY][LX].map[pc.get_position_x()][pc.get_position_y()] == POKECENTER) {
                        mvprintw(0, 0, "Player entered Pokecenter.");
                        pc_in_building = 1;
                        invalid_move = 0;
                    } else if(earth[LY][LX].map[pc.get_position_x()][pc.get_position_y()] == POKEMART) {
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
                        currently_battling = Character();
                        print_map();
                    } else if (battle_with_trainer == 1 && character_battle_page_open == 1) {
                        battle_with_trainer = 0;
                        defeated_trainers[current_battle_trainer] = 1;
                        current_battle_trainer = 0;
                        invalid_move = 0;
                        print_map();
                        mvprintw(0, 0, "You defeated the trainer!");
                    }
                    break;
                case 'p':
                    if(battle_with_trainer == 1) {
                        view_trainer_pokemon(currently_battling);
                        refresh();
                    } else {
                        mvprintw(0, 0, "Not currently battling.");
                    }
                    break;
                case 'o':
                    if(battle_with_trainer == 1) {
                        battle_trainer_view(currently_battling);
                        refresh();
                    }
                    break;
                case 'm':
                    view_own_pokemon();
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
                case 'f':
                    fly();
                    break;
            }
        }
        if(iter % 2 == 1 && battle_with_trainer == 0 && pc_in_building == 0 && invalid_move == 0) {
            for(int i = 0; i < character_array_size; i++) {
                npc_movement(earth[LY][LX].characters[i]);
            }
        }
        iter++;
        check_for_trainer(pc.get_position_x(), pc.get_position_y());
    };
}

