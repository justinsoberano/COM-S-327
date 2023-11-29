#include "pokemon_functions.h"
#include "../../terrain_generation/terrain_generation.h"

using namespace std;

/* All functions implemented for assignment 1.08 are here. */

/* Debug Flags */
int debug_total_nums_moves = 0;
int debug_pm1;
int debug_pm2;

/* ----------- */

pokedex transfer_pokemon_to_pokedex(encountered_pokemon const ep) {
    pokedex p;
    p.set_level(ep.get_level());
    p.set_pokemon(ep.get_pokemon());
    p.set_hp(ep.get_hp());
    p.set_max_hp(ep.get_max_hp());
    p.set_attack(ep.get_attack());
    p.set_defense(ep.get_defense());
    p.set_speed(ep.get_speed());
    p.set_special_attack(ep.get_special_attack());
    p.set_special_defense(ep.get_special_defense());
    p.set_gender(ep.get_gender());
    p.set_shiny(ep.get_shiny());
    for(int i = 0; i < 6; i++) {
        p.set_base_values(i, ep.get_base_values(i));
    }
    for(int i = 0; i < ep.get_number_of_moves(); i++) {
        p.add_move(ep.get_p_moves()[i]);
    }
    return p;
}

vector<encountered_pokemon> generate_starting_pokemon() {

    vector<encountered_pokemon> starting_pokemon;
    encountered_pokemon ep = encountered_pokemon();

    for(int i = 0; i < 3; i++) {
        ep.set_pokemon(get_pokemon(rand_int(0, 1092)));
        ep.set_level(calculate_pokemon_level());
        get_encountered_pokemon_moves(ep);
        get_encountered_pokemon_stats(ep);
        get_encountered_pokemon_gender(ep);
        get_encountered_pokemon_shiny(ep);
        starting_pokemon.push_back(ep);
        ep = encountered_pokemon();
    }

    return starting_pokemon;
}

pokedex select_first_pokemon() {
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
    mvprintw(1, 27, "SELECT YOUR FIRST POKEMON");
    attroff(COLOR_PAIR(8));
    refresh();


    vector<encountered_pokemon> starting_pokemon = generate_starting_pokemon();

    int spacer = 5;
    for(int i = 0; i < 3; i++) {
        encountered_pokemon p = starting_pokemon[i];
        mvprintw(spacer * (i + 1), 3, "%d. [%d][%c] %s | Moves: %s + %s", (i + 1), p.get_level(), ((p.get_gender() == 0) ? 'F' : 'M'), p.get_pokemon().get_identifier().c_str(), p.get_p_moves()[0].get_identifier().c_str(), p.get_p_moves()[1].get_identifier().c_str());
        mvprintw((spacer * (i + 1) + 1), 3, "Stats | HP: %d ATK: %d DEF %d SPD %d S-ATK: %d S-DEF: %d ", p.get_hp(), p.get_attack(), p.get_defense(), p.get_speed(), p.get_special_attack(), p.get_special_defense());
        refresh();
    }

    int pokemon_selected = 0;
    int ch;

    while(pokemon_selected == 0) {
        ch = getch();
        if(ch == '1') {
            pokemon_selected = 1;
        } else if (ch == '2') {
            pokemon_selected = 2;
        } else if (ch == '3') {
            pokemon_selected = 3;
        }
    }

    return transfer_pokemon_to_pokedex(starting_pokemon[pokemon_selected - 1]);
}

encountered_pokemon pokemon_encountered() {
    encountered_pokemon ep;
    ep.set_pokemon(get_pokemon(rand_int(0, 1092)));
    ep.set_level(calculate_pokemon_level());
    get_encountered_pokemon_moves(ep);
    get_encountered_pokemon_stats(ep);
    get_encountered_pokemon_gender(ep);
    get_encountered_pokemon_shiny(ep);
    return ep;
}

int calculate_pokemon_level() {
    int distance = (abs(200 - LY) + abs(200 - LX));

    // int max_level = (distance != 0) ? distance / 2 : 1;
    // int min_level = (distance - 200 != 0) ? (distance - 200) / 2 : 1;
    /* Prevents a floating point exception error. Distance being 0 can
       cause a floating point exception. */

    if(distance <= 200) {
        int max_level = (distance != 0) ? distance / 2 : 1; 
        max_level = (max_level == 0) ? 1 : max_level;
        return (rand_int(1, max_level));
    } else {
        int min_level = (distance - 200 != 0) ? (distance - 200) / 2 : 1;
        min_level = (min_level == 0) ? 1 : min_level;
        return (rand_int(min_level, 100));
    }
}

void display_pokemon_info(const encountered_pokemon p) {
    print_map();
    if(p.get_shiny()) {
        attron(COLOR_PAIR(6));
        mvprintw(0, 0, "Shiny Pokemon Information!");
        attroff(COLOR_PAIR(6));
    } else {
        mvprintw(0, 0, "Pokemon Information");
    }
    mvprintw(23, 0, "Stats | HP: %d ATK: %d DEF %d SPD %d S-ATK: %d S-DEF: %d ", p.get_hp(), p.get_attack(), p.get_defense(), p.get_speed(), p.get_special_attack(), p.get_special_defense());
    if(p.get_number_of_moves() == 1) {
        mvprintw(22, 0, "Pokemon: [%d][%c] %s Move: %s", p.get_level(), ((p.get_gender() == 0) ? 'F' : 'M'), p.get_pokemon().get_identifier().c_str(), p.get_p_moves()[0].get_identifier().c_str());
    } else if(p.get_number_of_moves() == 0) {
        mvprintw(22, 40, "Moves: None");
    } else {
        mvprintw(22, 0, "Pokemon: [%d][%c] %s | Moves: %s + %s", p.get_level(), ((p.get_gender() == 0) ? 'F' : 'M'), p.get_pokemon().get_identifier().c_str(), p.get_p_moves()[0].get_identifier().c_str(), p.get_p_moves()[1].get_identifier().c_str());
    }
}

vector<pokemon_moves> filter_pokemon_moves(int level, encountered_pokemon &ep) {
    vector<pokemon_moves> epm;
    debug_total_nums_moves = 0;
    for(pokemon_moves pm : pokemon_moves_database) {
        if((pm.get_pokemon_id() == ep.get_pokemon().get_id()) && pm.get_pokemon_move_method_id() == 1 && pm.get_level() <= level) {
            epm.push_back(pm);
            debug_total_nums_moves++;
        }
    }
    return epm;
}

void one_move(pokemon_moves pm, encountered_pokemon &ep) {
    for(moves m : moves_database) {
        if(m.get_id() == pm.get_move_id()) {
            ep.add_move(m);
        }
    }
}

void two_moves(pokemon_moves pm1, pokemon_moves pm2, encountered_pokemon &ep) {
    for(moves m : moves_database) {
        if(m.get_id() == pm1.get_move_id()) {
            ep.add_move(m);
        }
    }
    for(moves m : moves_database) {
        if(m.get_id() == pm2.get_move_id()) {
            ep.add_move(m);
        }
    }
}

void get_encountered_pokemon_moves(encountered_pokemon &ep) {
    vector<pokemon_moves> epm = filter_pokemon_moves(ep.get_level(), ep);
    int l = 0;

    while(epm.size() == 0 || epm.size() == 1) {
        epm = filter_pokemon_moves((l++), ep);
        if (l > 100) {
            break;
        }
    }

    int pokemon_moves_index_one = rand_int(0, epm.size() - 1);
    debug_pm1 = pokemon_moves_index_one;
    int pokemon_moves_index_two;
    while(1) {
        pokemon_moves_index_two = rand_int(0, epm.size() - 1);
        if(pokemon_moves_index_one != pokemon_moves_index_two) {
            break;
        }
    };
    debug_pm2 = pokemon_moves_index_two;
    two_moves(epm[pokemon_moves_index_one], epm[pokemon_moves_index_two], ep);
}

// 1 -> hp
// 2 -> attack
// 3 -> defense
// 4 -> special-attack
// 5 -> special-defense
// 6 -> speed

void level_up_pokemon(pokedex p) {
    int curr_level = p.get_level();
    int next_level = curr_level + 1;
    p.set_level(next_level);

    p.set_hp(get_pokemon_HP(next_level, p.get_base_values(0)));
    p.set_attack(get_pokemon_other_stat(next_level, p.get_base_values(1)));
    p.set_defense(get_pokemon_other_stat(next_level, p.get_base_values(2)));
    p.set_special_attack(get_pokemon_other_stat(next_level, p.get_base_values(3)));
    p.set_special_defense(get_pokemon_other_stat(next_level, p.get_base_values(4)));
    p.set_speed(get_pokemon_other_stat(next_level, p.get_base_values(5)));
}

int get_pokemon_HP(int level, int base_HP) {
    int pokemon_hp = 0;
    pokemon_hp = floor((((base_HP + rand_int(0, 15)) * 2) * level) / 100) + level + 10;
    return pokemon_hp;
}

int get_pokemon_other_stat(int level, int base_stat) {
    int other_stat = 0;
    other_stat = floor((((base_stat + rand_int(0, 15)) * 2) * level) / 100) + 5;
    return other_stat;
}

/* Use formula for getting stats */
void get_encountered_pokemon_stats(encountered_pokemon &ep) {

    vector<pokemon_stats> eps;

    for(pokemon_stats ps : pokemon_stats_database) {
        if(ps.get_pokemon_id() == ep.get_pokemon().get_id()) {
            eps.push_back(ps);
        }
    }

    for(int i = 0; i < 6; i++) {
        ep.set_base_values(i, eps[i].get_base_stat());
    }

    for(int i = 1; i <= 6; i++) {
        if(i == 1) {
            ep.set_hp(get_pokemon_HP(ep.get_level(), eps[0].get_base_stat()));
            ep.set_max_hp(get_pokemon_HP(ep.get_level(), eps[0].get_base_stat()));
        } else if (i == 2) {
            ep.set_attack(get_pokemon_other_stat(ep.get_level(), eps[1].get_base_stat()));
        } else if (i == 3) {
            ep.set_defense(get_pokemon_other_stat(ep.get_level(), eps[2].get_base_stat()));
        } else if (i == 4) {
            ep.set_special_attack(get_pokemon_other_stat(ep.get_level(), eps[3].get_base_stat()));
        } else if (i == 5) {
            ep.set_special_defense(get_pokemon_other_stat(ep.get_level(), eps[4].get_base_stat()));
        } else if (i == 6) {
            ep.set_speed(get_pokemon_other_stat(ep.get_level(), eps[5].get_base_stat()));
        }
    }
}

void get_encountered_pokemon_gender(encountered_pokemon &ep) {
    ep.set_gender(rand_int(0, 1));
}

void get_encountered_pokemon_shiny(encountered_pokemon &ep) {
    if(rand_int(0, 8192) == 0) {
        ep.set_shiny(true);
    }
}