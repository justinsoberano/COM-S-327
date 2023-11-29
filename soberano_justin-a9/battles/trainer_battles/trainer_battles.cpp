#include "trainer_battles.h"

using namespace std;

int starting_fighter = 0;
int first_message = 0;
vector <pokedex> battle_trainer_pokemon;
pokedex trainer_selected_pokemon;
int selected_pokemon_index = 0;
int battle_ended = 0;

void clear_line(int row) {
    mvprintw(row, 0, "                                                                                ");
}

pokedex swap_dead_pokemon() {

    pokedex p;

    int valid_pokemon_selected = 1;
    

    attron(COLOR_PAIR(8));
    mvprintw(1, 34, "SWAP POKEMON");
    attroff(COLOR_PAIR(8));

    mvprintw(22, 0, "                                                                                ");
    mvprintw(23, 0, "                                                                                ");

    mvprintw(22, 32, "Choose a Pokemon");
    mvprintw(23, 30, "[1] [2] [3] [4] [5]");

    mvprintw(0, 72, "Back [b]");
    refresh();

    int ch;
    while(valid_pokemon_selected == 1) {

        ch = getch();
        switch(ch) {
            case '1':
                if((int)owned_pokemon.size() >= 1) {
                    current_pokemon_in_battle = owned_pokemon[0];
                    owned_pokemon.erase(owned_pokemon.begin());
                    mvprintw(0, 0, "                                                                                ");
                    mvprintw(0, 0, "You swapped to %s!", current_pokemon_in_battle.get_pokemon().get_identifier().c_str());
                    refresh();
                    sleep(1);
                    valid_pokemon_selected = 0;
                } else {
                    mvprintw(0, 0, "This slot is not occupied by a Pokemon, choose another.");
                    refresh();
                }
                break;
            case '2':
                if((int)owned_pokemon.size() >= 2) {
                    current_pokemon_in_battle = owned_pokemon[1];
                    owned_pokemon.erase(owned_pokemon.begin() + 1);
                    mvprintw(0, 0, "                                                                                ");
                    mvprintw(0, 0, "You swapped to %s!", current_pokemon_in_battle.get_pokemon().get_identifier().c_str());
                    refresh();
                    sleep(1);
                    valid_pokemon_selected = 0;
                } else {
                    mvprintw(0, 0, "This slot is not occupied by a Pokemon, choose another.");
                    refresh();
                }       
                break;
            case '3':
                if((int)owned_pokemon.size() >= 3) {
                    current_pokemon_in_battle = owned_pokemon[2];
                    owned_pokemon.erase(owned_pokemon.begin() + 2);
                    mvprintw(0, 0, "                                                                                ");
                    mvprintw(0, 0, "You swapped to %s!", current_pokemon_in_battle.get_pokemon().get_identifier().c_str());
                    refresh();
                    sleep(1);
                    valid_pokemon_selected = 0;
                } else {
                    mvprintw(0, 0, "This slot is not occupied by a Pokemon, choose another.");
                    refresh();
                }
                break;
            case '4':
                if((int)owned_pokemon.size() >= 4) {
                    current_pokemon_in_battle = owned_pokemon[3];
                    owned_pokemon.erase(owned_pokemon.begin() + 3);
                    mvprintw(0, 0, "                                                                                ");
                    mvprintw(0, 0, "You swapped to %s!", current_pokemon_in_battle.get_pokemon().get_identifier().c_str());
                    refresh();
                    sleep(1);
                    valid_pokemon_selected = 0;
                } else {
                    mvprintw(0, 0, "This slot is not occupied by a Pokemon, choose another.");
                    refresh();
                }
                break;
            case '5':
                if((int)owned_pokemon.size() >= 5) {
                    current_pokemon_in_battle = owned_pokemon[4];
                    owned_pokemon.erase(owned_pokemon.begin() + 4);
                    mvprintw(0, 0, "                                                                                ");
                    mvprintw(0, 0, "You swapped to %s!", current_pokemon_in_battle.get_pokemon().get_identifier().c_str());
                    refresh();
                    sleep(1);
                    valid_pokemon_selected = 0;
                } else {
                    mvprintw(0, 0, "This slot is not occupied by a Pokemon, choose another.");
                    refresh();
                }
                break;
            case 'b':
                break;
        }
    }
    refresh();
    return p;
}

void player_battle_ui() {
    clear_line(22);
    clear_line(23);
    mvprintw(22, 0, "[1] %s", current_pokemon_in_battle.get_p_moves()[0].get_identifier().c_str());
    mvprintw(23, 0, "[2] %s", current_pokemon_in_battle.get_p_moves()[1].get_identifier().c_str());
    mvprintw(22, 68, "Use Item [9]");
    mvprintw(22, 64,  "View Pokedex [m]");
}

void display_player_pokemon(pokedex const &p) {
    mvprintw(17, 5, "YOU:");
    mvprintw(18, 5, "HP: %d ", p.get_hp());
    mvprintw(19, 5, "[%d][%c] %s", p.get_level(), (p.get_gender() == 0) ? 'F' : 'M', p.get_pokemon().get_identifier().c_str());
}

void display_trainer_pokemon(pokedex const &p) {
    int offset = 81 - p.get_pokemon().get_identifier().length() - 15;
    mvprintw(3, offset, "ENEMY:");
    mvprintw(4, offset, "HP: %d ", p.get_hp());
    mvprintw(5, offset, "[%d][%c] %s", p.get_level(), (p.get_gender() == 0) ? 'F' : 'M', p.get_pokemon().get_identifier().c_str());
}

void view_pokedex_trainer_edition() {

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
    mvprintw(1, 36, "POKEDEX");
    attroff(COLOR_PAIR(8));

    int spacer = 3;
    for(int i = 0; i < (int)owned_pokemon.size(); i++) {
        pokedex p = owned_pokemon[i];
        mvprintw((spacer * (i + 1)), 3, "%d. [%d][%c] %s | Moves: %s + %s", (i + 1), p.get_level(), ((p.get_gender() == 0) ? 'F' : 'M'), p.get_pokemon().get_identifier().c_str(), 
                                                                                     p.get_p_moves()[0].get_identifier().c_str(), p.get_p_moves()[1].get_identifier().c_str());
        mvprintw((spacer * (i + 1) + 1), 3, "Stats | HP: %d ATK: %d DEF %d SPD %d S-ATK: %d S-DEF: %d ", p.get_hp(), p.get_attack(), p.get_defense(), p.get_speed(), p.get_special_attack(), p.get_special_defense());
        mvprintw(22, 0, "[b] Back");
        refresh();
    }
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
        mvprintw(22, 0, "[b] Back");
        refresh();
    }
}

void battle_trainer_view(Character const &trainer) {

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
    mvprintw(1, 33, "TRAINER BATTLE");
    attroff(COLOR_PAIR(8));

    mvprintw(22, 0, "[1] Engage!");
    mvprintw(23, 0, "[2] View Trainer Pokemon");

    mvprintw(22, 60, "View Own Pokemon [m]");

    refresh();
}

void battle(Character const &trainer){
    battle_trainer_pokemon = trainer.trainer_pokemon;

    vector<pokedex> dead_pokemon;

    int ch;

    int valid_move = 1;
    battle_ongoing = 1;

    int attack_damage;
    int enemy_attack_damage;

    trainer_selected_pokemon = battle_trainer_pokemon[0];
    battle_trainer_pokemon.erase(battle_trainer_pokemon.begin());
    clear_line(0);
    mvprintw(0, 0, "Trainer has selected %s", trainer_selected_pokemon.get_pokemon().get_identifier().c_str());
    display_trainer_pokemon(trainer_selected_pokemon);
    display_player_pokemon(current_pokemon_in_battle);
    refresh();
    sleep(2);

    player_battle_ui();

    moves selected_move;
    moves enemy_selected_move;

    if(starting_fighter % 2 == 0 && first_message == 0) {
        mvprintw(0, 0, "Battle Engaged! Trainer goes first!");
        refresh();
        sleep(1);
        first_message = 1;
    } else if (starting_fighter % 2 == 1 && first_message == 0) {
        clear_line(0);
        mvprintw(0, 0, "Battle Engaged! You go first!");
        refresh();
        sleep(1);
        first_message = 1;
    }

    while(battle_ongoing) {

        battle_trainer_view(trainer);
        display_trainer_pokemon(trainer_selected_pokemon);
        display_player_pokemon(current_pokemon_in_battle);

        if(starting_fighter % 2 == 0 && valid_move == 1) {

            clear_line(22);
            clear_line(23);
            
            if(first_message != 0) {
                clear_line(0);
                mvprintw(0, 0, "Trainer's turn");
                refresh();
                sleep(1);
            }

            enemy_selected_move = trainer_selected_pokemon.get_p_moves()[rand_int(0, 1)];

            enemy_attack_damage = calculate_damage(trainer_selected_pokemon, enemy_selected_move);
            mvprintw(0, 0, "Trainer's pokemon did %d damage", enemy_attack_damage);
            refresh();
            sleep(1);
            current_pokemon_in_battle.set_hp(current_pokemon_in_battle.get_hp() - enemy_attack_damage);

        } else {

            if(first_message != 0 && valid_move == 1) {
                clear_line(0);
                mvprintw(0, 0, "Your turn.");
                refresh();       
            }

            valid_move = 0;
            player_battle_ui();
            ch = getch();
            switch(ch) {
                case '1':
                    valid_move = 1;
                    selected_move = current_pokemon_in_battle.get_p_moves()[0];
                    if(selected_move.get_accuracy() > rand() % 100) {
                        attack_damage = calculate_damage(current_pokemon_in_battle, selected_move);
                        trainer_selected_pokemon.set_hp(trainer_selected_pokemon.get_hp() - attack_damage);
                        mvprintw(0, 0, "You used %s, it did %d damage!", selected_move.get_identifier().c_str(), attack_damage);
                        clear_line(22);
                        clear_line(23);
                        refresh();
                    } else {
                        mvprintw(0, 0, "You missed!");
                        refresh();
                        sleep(2);
                    }
                    break;
                case '2':
                    valid_move = 1;
                    selected_move = current_pokemon_in_battle.get_p_moves()[1];
                    if(selected_move.get_accuracy() > rand() % 100) {
                        attack_damage = calculate_damage(current_pokemon_in_battle, selected_move);
                        trainer_selected_pokemon.set_hp(trainer_selected_pokemon.get_hp() - attack_damage);
                        mvprintw(0, 0, "You used %s, it did %d damage!", selected_move.get_identifier().c_str(), attack_damage);
                        clear_line(22);
                        clear_line(23);
                        refresh();
                        sleep(1);
                    } else {
                        mvprintw(0, 0, "You missed!");
                        refresh();
                        sleep(2);
                    }
                    break;
                case '9':
                    clear();
                    use_items();
                    valid_move = 1;
                    break;
                case 'm':
                    if(owned_pokemon.size() == 0) {
                        mvprintw(0, 0, "Your only pokemon is currently in battle.");
                        refresh();
                    } else {
                        view_pokedex_trainer_edition();
                        swap_pokemon();
                        valid_move = 1;
                    }
                    break;
                case 'q':
                    endwin();
                    exit(0);
                    break;
                
            }
        }
        if(current_pokemon_in_battle.get_hp() <= 0) {
            clear_line(0);
            mvprintw(0, 0, "Trainer defeated your %s", current_pokemon_in_battle.get_pokemon().get_identifier().c_str());
            current_pokemon_in_battle.set_hp(0);
            dead_pokemon.push_back(current_pokemon_in_battle);
            refresh();
            sleep(2);
            if(owned_pokemon.empty()) {
                clear_line(0);
                mvprintw(0, 0, "You lost the battle!");
                refresh();
                sleep(1);
                character_battle_page_open = 0;
                battle_with_trainer = 0;
                battle_ended = 1;
                battle_ongoing = 0;
                defeated_trainers[current_battle_trainer] = 1;

                for(int i = 0; i < (int)dead_pokemon.size(); i++) {
                    owned_pokemon.push_back(dead_pokemon[i]);
                }

                print_map();
                refresh();
                break;
            } else {
                clear_line(0);
                mvprintw(0, 0, "Choose a new Pokemon.");
                view_pokedex_trainer_edition();
                refresh();
                swap_dead_pokemon();
                clear();
            }
        }
        if(trainer_selected_pokemon.get_hp() <= 0) {
            clear_line(0);
            mvprintw(0, 0, "You defeated Trainer's %s", trainer_selected_pokemon.get_pokemon().get_identifier().c_str());
            refresh();
            sleep(1);
            if(battle_trainer_pokemon.size() == 0) {
                clear_line(0);
                mvprintw(0, 0, "You won the match!");
                refresh();
                sleep(1);
                character_battle_page_open = 0;
                battle_with_trainer = 0;
                battle_ended = 1;
                battle_ongoing = 0;
                defeated_trainers[current_battle_trainer] = 1;
                print_map();
                refresh();
            } else {
                trainer_selected_pokemon = battle_trainer_pokemon[0];
                battle_trainer_pokemon.erase(battle_trainer_pokemon.begin());
                clear_line(0);
                mvprintw(0,0, "Trainer swapped to %s!", trainer_selected_pokemon.get_pokemon().get_identifier().c_str());
                refresh();
                sleep(2);
            }
        }
        starting_fighter++;
    }
    battle_with_trainer = 0;
    battle_ongoing = 0;
}

void trainer_battle(Character const &trainer) {

        clear();
        battle_trainer_view(trainer);
        display_player_pokemon(current_pokemon_in_battle);
        refresh();
        sleep(2);
        starting_fighter = rand_int(0, 1);

        int ch;

        while(battle_ended != 1) {
            ch = getch();
            switch(ch) {
                case '1':
                    battle(trainer);
                    break;
                case '2':
                    view_trainer_pokemon(trainer);
                    break;
                case 'm':
                    if(owned_pokemon.size() == 0) {
                        mvprintw(0, 0, "Your only pokemon is already in battle.");
                    } else {
                        clear();
                        view_pokedex_trainer_edition();
                        refresh();
                    }
                    break;
                case 'q':
                    battle_ended = 1;
                    character_battle_page_open = 0;
                    battle_with_trainer = 0;
                    defeated_trainers[current_battle_trainer] = 1;
                    break;
                case 'b':
                    refresh();
                    battle_trainer_view(trainer);
                    display_player_pokemon(current_pokemon_in_battle);
                    refresh();
            }
        }
        battle_ended = 0;
        character_battle_page_open = 0;
        return;
}