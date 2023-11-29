#include "player.h"

int valid_move_for_pc(int x, int y) {
    if(x >= 0 && x < ROWS && y >= 0 && y < COLS && earth[LY][LX].map[x][y] != BORDER) {
        if(pc_in_building == 1) {
            mvprintw(0, 0, "Leave the building first [<].");
            return 0;
        } else if (battle_with_trainer == 1) {
            mvprintw(0, 0, "You have encountered a trainer! [escape].");
            return 0;
        } else if (earth[LY][LX].char_map[x][y] != 0) {
            if(is_trainer_defeated(earth[LY][LX].char_map[x][y])) {
                mvprintw(0, 0, "You have defeated this trainer. Move along.");
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
                mvprintw(0, 0, "You have defeated this trainer. Move along.");
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
        pc.x = earth[LY][LX].east_gate;
        pc.y = 0;
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
        pc.x = earth[LY][LX].west_gate;
        pc.y = 79;
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
        pc.x = 20;
        pc.y = earth[LY][LX].south_gate;
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
        pc.x = 0;
        pc.y = earth[LY][LX].north_gate;
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

    if(LX + x <= 400 && LX + x >= 0 && LY + y <= 400 && LY + y >= 0) {

        earth[LY][LX].char_map[pc.x][pc.y] = 0;

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
        
        pc.x = rand_int(5, 15);
        for(int i = 0; i < COLS; i++) {
            if(earth[LY][LX].map[pc.x][i] == PATH) {
                pc.y = i;
                break;
            }
        }
        calculate_distance_maps();
        update_dijkstra_npc();
        earth[LY][LX].char_map[pc.x][pc.y] = PLAYER;
        print_map();
        mvprintw(0, 0, "You have teleported to %d, %d", LX - 200, LY - 200);
    } else {
        mvprintw(22, 0, "Invalid coordinates, choose between [-200, 200]");
    }
}

void game() {

    earth[LY][LX].smart_queue = create_priority_queue();
    earth[LY][LX].other_queue = create_priority_queue();

    for(int i = 0; i < character_array_size; i++) {
        if(earth[LY][LX].characters[i].type == HIKER || earth[LY][LX].characters[i].type == RIVAL) {
            insert(earth[LY][LX].smart_queue, earth[LY][LX].characters[i]);
        } else {
            insert(earth[LY][LX].other_queue, earth[LY][LX].characters[i]);
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
            Character nextChar = dequeue(earth[LY][LX].smart_queue);
            insert(earth[LY][LX].smart_queue, npc_movement(nextChar));
        }
        if(iter % 3 == 1) {
            ch = getch();
            switch(ch) {
                case '7':
                case 'y':
                    earth[LY][LX].char_map[pc.x][pc.y] = 0;
                    if(valid_move_for_pc(pc.x - 1, pc.y - 1) && pc_in_building == 0) {
                        check_if_at_gate(pc.x - 1, pc.y - 1);
                        pc.x--;
                        pc.y--;
                        earth[LY][LX].char_map[pc.x][pc.y] = PLAYER;
                        update_dijkstra_npc();
                        print_map();
                        invalid_move = 0;
                        mvprintw(0, 0, "Player moved upper left");
                    } else {
                        invalid_move = 1;
                        earth[LY][LX].char_map[pc.x][pc.y] = PLAYER;
                        print_map();
                        valid_move_for_pc(pc.x - 1, pc.y - 1);
                    }
                    break;
                case '8':
                case 'k':
                    earth[LY][LX].char_map[pc.x][pc.y] = 0;
                    if(valid_move_for_pc(pc.x - 1, pc.y) && pc_in_building == 0) {
                        check_if_at_gate(pc.x - 1, pc.y);
                        pc.x--;
                        earth[LY][LX].char_map[pc.x][pc.y] = PLAYER;
                        update_dijkstra_npc();
                        print_map();
                        mvprintw(0, 0, "Player moved up");
                        invalid_move = 0;
                    } else {
                        invalid_move = 1;
                        earth[LY][LX].char_map[pc.x][pc.y] = PLAYER;
                        print_map();
                        valid_move_for_pc(pc.x - 1, pc.y);
                    }
                    break;
                case '9':
                case 'u':
                    earth[LY][LX].char_map[pc.x][pc.y] = 0;
                    if(valid_move_for_pc(pc.x - 1, pc.y + 1) && pc_in_building == 0) {
                        check_if_at_gate(pc.x - 1, pc.y + 1);
                        pc.x--;
                        pc.y++;
                        earth[LY][LX].char_map[pc.x][pc.y] = PLAYER;
                        update_dijkstra_npc();
                        print_map();
                        mvprintw(0, 0, "Player moved upper right");
                        invalid_move = 0;
                    } else {
                        invalid_move = 1;
                        earth[LY][LX].char_map[pc.x][pc.y] = PLAYER;
                        print_map();
                        valid_move_for_pc(pc.x - 1, pc.y + 1);
                    }
                    break;
                case '6':
                case 'l':
                    earth[LY][LX].char_map[pc.x][pc.y] = 0;
                    if(valid_move_for_pc(pc.x, pc.y + 1) && pc_in_building == 0) {
                        check_if_at_gate(pc.x, pc.y + 1);
                        pc.y++;
                        earth[LY][LX].char_map[pc.x][pc.y] = PLAYER;
                        update_dijkstra_npc();
                        print_map();
                        mvprintw(0, 0, "Player moved right");
                        invalid_move = 0;
                    } else {
                        invalid_move = 1;
                        earth[LY][LX].char_map[pc.x][pc.y] = PLAYER;
                        print_map();
                        valid_move_for_pc(pc.x, pc.y + 1);
                    }
                    break;
                case '3':
                case 'n':
                    earth[LY][LX].char_map[pc.x][pc.y] = 0;
                    if(valid_move_for_pc(pc.x + 1, pc.y + 1) && pc_in_building == 0) {
                        check_if_at_gate(pc.x + 1, pc.y + 1);
                        pc.x++;
                        pc.y++;
                        earth[LY][LX].char_map[pc.x][pc.y] = PLAYER;
                        update_dijkstra_npc();
                        print_map();
                        mvprintw(0, 0, "Player moved lower right");
                        invalid_move = 0;
                    } else {
                        invalid_move = 1;
                        earth[LY][LX].char_map[pc.x][pc.y] = PLAYER;
                        print_map();
                        valid_move_for_pc(pc.x + 1, pc.y + 1);
                    }
                    break;
                case '2':
                case 'j':
                    earth[LY][LX].char_map[pc.x][pc.y] = 0;
                    if(valid_move_for_pc(pc.x + 1, pc.y) && pc_in_building == 0) {
                        check_if_at_gate(pc.x + 1, pc.y);
                        pc.x++;
                        earth[LY][LX].char_map[pc.x][pc.y] = PLAYER;
                        update_dijkstra_npc();
                        print_map();
                        mvprintw(0, 0, "Player moved down");
                        invalid_move = 0;
                    } else {
                        invalid_move = 1;
                        earth[LY][LX].char_map[pc.x][pc.y] = PLAYER;
                        print_map();
                        valid_move_for_pc(pc.x + 1, pc.y);
                    }
                    break;
                case '1':
                case 'b':
                    earth[LY][LX].char_map[pc.x][pc.y] = 0;
                    if(valid_move_for_pc(pc.x + 1, pc.y - 1) && pc_in_building == 0) {
                        check_if_at_gate(pc.x + 1, pc.y - 1);
                        pc.x++;
                        pc.y--;
                        earth[LY][LX].char_map[pc.x][pc.y] = PLAYER;
                        update_dijkstra_npc();
                        print_map();
                        mvprintw(0, 0, "Player moved lower left");
                        invalid_move = 0;
                    } else {
                        invalid_move = 1;
                        earth[LY][LX].char_map[pc.x][pc.y] = PLAYER;
                        print_map();
                        valid_move_for_pc(pc.x + 1, pc.y - 1);
                    }
                    break;
                case '4':
                case 'h':
                    earth[LY][LX].char_map[pc.x][pc.y] = 0;
                    if(valid_move_for_pc(pc.x, pc.y - 1) && pc_in_building == 0) {
                        check_if_at_gate(pc.x, pc.y - 1);
                        pc.y--;
                        earth[LY][LX].char_map[pc.x][pc.y] = PLAYER;
                        update_dijkstra_npc();
                        print_map();
                        mvprintw(0, 0, "Player moved left");
                        invalid_move = 0;
                    } else {
                        invalid_move = 1;
                        earth[LY][LX].char_map[pc.x][pc.y] = PLAYER;
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
                case 'f':
                    fly();
                    break;
            }
        }
        if(iter % 3 == 2 && battle_with_trainer == 0 && pc_in_building == 0 && invalid_move == 0) {
            for(int i = 0; i < rand_int(1, 3); i++) {
                Character nextChar = dequeue(earth[LY][LX].other_queue);
                insert(earth[LY][LX].other_queue, npc_movement(nextChar));
            }
        }
        iter++;
        check_for_trainer(pc.x, pc.y);
    };
}

