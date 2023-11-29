#include "pokemon_battles.h"

#include "../../data_structures/random/random.h"

using namespace std;

encountered_pokemon wild_pokemon;

pokedex swap_pokemon() {

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
                    owned_pokemon.push_back(current_pokemon_in_battle);
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
                    owned_pokemon.push_back(current_pokemon_in_battle);
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
                    owned_pokemon.push_back(current_pokemon_in_battle);
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
                    owned_pokemon.push_back(current_pokemon_in_battle);
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
                    owned_pokemon.push_back(current_pokemon_in_battle);
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

void use_potion(pokedex &p) {
    if(p.get_hp() > 0) {
        p.set_hp((p.get_max_hp() > 20) ? (20 + p.get_hp()) : p.get_max_hp());
    } else {
        mvprintw(0, 0, "You cannot heal a dead Pokemon");
        refresh();
        sleep(1);
    }
}

void use_revive() {

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
    
    vector<pokedex> revivable_pokemon;

    attron(COLOR_PAIR(8));
    mvprintw(1, 33, "REVIVE POKEMON");
    attroff(COLOR_PAIR(8));
    refresh();

    mvprintw(22, 0, "                                                                                ");
    mvprintw(23, 0, "                                                                                ");
    mvprintw(22, 32, "Choose a Pokemon");
    mvprintw(23, 30, "[1] [2] [3] [4] [5]");
    mvprintw(0, 72, "back [b]");

    int rev[6];
    int rev_iter = 0;

    for(int i = 0; i < (int)owned_pokemon.size(); i++) {
        if(owned_pokemon[i].get_hp() == 0) {
            revivable_pokemon.push_back(owned_pokemon[i]);
            rev[rev_iter] = i;
        }
    }

    if(revivable_pokemon.empty()) {
        mvprintw(0, 0, "No pokemon are revivable. ");
        sleep(2);
        return;
    } else {
        for(int i = 0; i < (int)revivable_pokemon.size(); i++) {
            mvprintw(3, (i + 1) * 3, "%d. [%d] %s | Will be revived to %d/%d HP.", 
            (i + 1), revivable_pokemon[i].get_level(), revivable_pokemon[i].get_pokemon().get_identifier().c_str(), 
            (int)(revivable_pokemon[i].get_max_hp() / 2), revivable_pokemon[i].get_max_hp());
        }
    }
    refresh();

    int ch;
    int pokemon_revived = 0;

    while(pokemon_revived == 0) {
        ch = getch();
        switch(ch) {
            case '1':
                if((int)revivable_pokemon.size() >= 1) {
                    mvprintw(0, 0, "You revived %s!", revivable_pokemon[0].get_pokemon().get_identifier().c_str());
                    refresh();
                    sleep(2);
                    revivable_pokemon[0].set_hp((int)(revivable_pokemon[0].get_max_hp() / 2));
                    owned_pokemon.erase(owned_pokemon.begin() + rev[0]);
                    owned_pokemon.push_back(revivable_pokemon[0]);
                    revivable_pokemon.erase(revivable_pokemon.begin());
                    pokemon_revived = 1;
                    revives--;
                } else {
                    mvprintw(0, 0, "This slot is not occupied by a Pokemon, choose another.");
                    refresh();
                }
                break;
            case '2':
                if((int)revivable_pokemon.size() >= 2) {
                    mvprintw(0, 0, "You revived %s!", revivable_pokemon[1].get_pokemon().get_identifier().c_str());
                    refresh();
                    sleep(2);
                    revivable_pokemon[1].set_hp((int)(revivable_pokemon[1].get_max_hp() / 2));
                    owned_pokemon.erase(owned_pokemon.begin() + rev[1]);
                    owned_pokemon.push_back(revivable_pokemon[1]);
                    revivable_pokemon.erase(revivable_pokemon.begin());
                    pokemon_revived = 1;
                    revives--;
                } else {
                    mvprintw(0, 0, "This slot is not occupied by a Pokemon, choose another.");
                    refresh();
                }
                break;
            case '3':
                if((int)revivable_pokemon.size() >= 3) {
                    mvprintw(0, 0, "You revived %s!", revivable_pokemon[2].get_pokemon().get_identifier().c_str());
                    refresh();
                    sleep(2);
                    revivable_pokemon[2].set_hp((int)(revivable_pokemon[2].get_max_hp() / 2));
                    owned_pokemon.erase(owned_pokemon.begin() + rev[2]);
                    owned_pokemon.push_back(revivable_pokemon[2]);
                    revivable_pokemon.erase(revivable_pokemon.begin());
                    pokemon_revived = 1;
                    revives--;
                } else {
                    mvprintw(0, 0, "This slot is not occupied by a Pokemon, choose another.");
                    refresh();
                }
                break;
            case '4':
                if((int)revivable_pokemon.size() >= 4) {
                    mvprintw(0, 0, "You revived %s!", revivable_pokemon[3].get_pokemon().get_identifier().c_str());
                    refresh();
                    sleep(2);
                    revivable_pokemon[3].set_hp((int)(revivable_pokemon[3].get_max_hp() / 2));
                    owned_pokemon.erase(owned_pokemon.begin() + rev[3]);
                    owned_pokemon.push_back(revivable_pokemon[3]);
                    revivable_pokemon.erase(revivable_pokemon.begin());
                    pokemon_revived = 1;
                    revives--;
                } else {
                    mvprintw(0, 0, "This slot is not occupied by a Pokemon, choose another.");
                    refresh();
                }
                break;
            case '5':
                if((int)revivable_pokemon.size() >= 5) {
                    mvprintw(0, 0, "You revived %s!", revivable_pokemon[4].get_pokemon().get_identifier().c_str());
                    refresh();
                    sleep(2);
                    revivable_pokemon[4].set_hp((int)(revivable_pokemon[4].get_max_hp() / 2));
                    owned_pokemon.erase(owned_pokemon.begin() + rev[4]);
                    owned_pokemon.push_back(revivable_pokemon[4]);
                    revivable_pokemon.erase(revivable_pokemon.begin());
                    pokemon_revived = 1;
                    revives--;
                } else {
                    mvprintw(0, 0, "This slot is not occupied by a Pokemon, choose another.");
                    refresh();
                }
                break;
            case '6':
                if((int)revivable_pokemon.size() >= 6) {
                    mvprintw(0, 0, "You revived %s!", revivable_pokemon[5].get_pokemon().get_identifier().c_str());
                    refresh();
                    sleep(2);
                    revivable_pokemon[5].set_hp((int)(revivable_pokemon[5].get_max_hp() / 2));
                    owned_pokemon.erase(owned_pokemon.begin() + rev[5]);
                    owned_pokemon.push_back(revivable_pokemon[5]);
                    revivable_pokemon.erase(revivable_pokemon.begin());
                    pokemon_revived = 1;
                    revives--;
                } else {
                    mvprintw(0, 0, "This slot is not occupied by a Pokemon, choose another.");
                    refresh();
                }
                break;
            case 'b':
                return;
                break;

        }
    }
}

int use_pokeball(encountered_pokemon p) {

    if(battle_ongoing == 1) {
        mvprintw(0, 0, "You cannot use a pokeball in a trainer battle.");
        refresh();
        sleep(1);
        return 0;
    }

    int chance = 0;
    if(chance == 0) {
        owned_pokemon.push_back(transfer_pokemon_to_pokedex(p));
        return 1;   
    } else {
        mvprintw(0, 0, "You have max Pokemon in your Pokedex.");
    }
    return 0;
}

void use_items() {
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
    mvprintw(1, 37, "ITEMS");
    attroff(COLOR_PAIR(8));
    refresh();

    mvprintw(9, 19, "POTIONS: %d | REVIVES: %d | POKEBALLS: %d", potions, revives, pokeballs);
    mvprintw(22, 0, "[1] Use Potion");
    mvprintw(23, 0, "[2] Use Revive");
    mvprintw(22, 64, "Use Pokeball [3]");
}

void pokemon_battle_screen(pokedex p) {

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
    mvprintw(1, 33, "POKEMON BATTLE");
    mvprintw(21, 0, "ATTACKS");
    mvprintw(21, 69, "OTHER MOVES");
    attroff(COLOR_PAIR(8));
    refresh();

    int offset = 81 - wild_pokemon.get_pokemon().get_identifier().length() - 15;

    mvprintw(3, offset, "ENEMY:");
    mvprintw(4, offset, "HP: %d ", wild_pokemon.get_hp());
    mvprintw(5, offset, "[%d][%c] %s", wild_pokemon.get_level(), (wild_pokemon.get_gender() == 0) ? 'F' : 'M', wild_pokemon.get_pokemon().get_identifier().c_str());

    mvprintw(17, 5, "YOU:");
    mvprintw(18, 5, "HP: %d ", p.get_hp());
    mvprintw(19, 5, "[%d][%c] %s", p.get_level(), (p.get_gender() == 0) ? 'F' : 'M', p.get_pokemon().get_identifier().c_str());

    mvprintw(22, 0, "[1] %s", p.get_p_moves()[0].get_identifier().c_str());
    mvprintw(23, 0, "[2] %s", p.get_p_moves()[1].get_identifier().c_str());

    mvprintw(22, 68, "Use Item [9]");
    mvprintw(23, 63, "Escape battle [0]");

    mvprintw(0, 69, "Pokedex [m]");

}

int calculate_damage(pokedex p, moves m) {
    double critical_hit = (rand_int(0, 255) > (p.get_base_values(5) / 2)) == true ? 1.5 : 1;
    double random_value = rand_int(85, 100);
    double stab_number = rand_int(5, 10) / 3;
    double standard_damage = ((((2 * p.get_level()) / 5) + 2) * m.get_power() * (p.get_attack() / p.get_defense()) / 50) + 2;
    double damage = (standard_damage * critical_hit * random_value * stab_number) / 100;
    return (int)damage;
}

int calculate_damage(encountered_pokemon p, moves m){
    double critical_hit = (rand_int(0, 255) > (p.get_base_values(5) / 2)) == true ? 1.5 : 1;
    double random_value = rand_int(85, 100);
    double stab_number = rand_int(5, 10) / 3;
    double standard_damage = ((((2 * p.get_level()) / 5) + 2) * m.get_power() * (p.get_attack() / p.get_defense()) / 50) + 2;
    double damage = (standard_damage * critical_hit * random_value * stab_number) / 100;
    return (int)damage;
}

void encountered_wild_pokemon_screen() {
    mvprintw(0, 0, "You encountered a wild %s!", wild_pokemon.get_pokemon().get_identifier().c_str());
    mvprintw(22, 0, "[p] View Pokemon");
    mvprintw(23, 0, "[o] Battle Pokemon");
    mvprintw(22, 64, "View Pokedex [m]");
    mvprintw(23, 64, "Flee Pokemom [l]");
    refresh();
}

void player_encountered_pokemon(int x, int y) {

    int chance = rand_int(0, 100);

    if(earth[LY][LX].map[x][y] == TALL_GRASS && (chance % 10) == 0) {
        wild_pokemon_encountered_page = 1;
        wild_pokemon = pokemon_encountered();
        encountered_wild_pokemon_screen();
    }
}