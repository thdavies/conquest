#include <math.h>
#include <stdio.h>
#include <curses.h>
#include "defs.h"
#include "structs.h"
#include "vars.h"

void blast(tplanet * planet_ptr, int factors)
{
    int killed;

    killed = min(planet_ptr->capacity, factors / 4);
    planet_ptr->inhabitants = min(planet_ptr->inhabitants, planet_ptr->capacity) - killed;
    planet_ptr->iu = min(planet_ptr->iu - killed, planet_ptr->inhabitants * iu_ratio);
    planet_ptr->capacity = planet_ptr->capacity - killed;
    if ( planet_ptr->inhabitants <=0 ) {
        planet_ptr->inhabitants = 0;
        planet_ptr->iu = 0;
        planet_ptr->mb = 0;
        planet_ptr->amb = 0;
        if ( planet_ptr->team != none ) {
            col_stars[planet_ptr->pstar][planet_ptr->team]--;
            planet_ptr->team = none;
            planet_ptr->esee_team = none;
            planet_ptr->conquered = false;
            on_board(stars[planet_ptr->pstar].x, stars[planet_ptr->pstar].y);
        }
    }
}



void fire_salvo(tteam att_team, struct sttf *task, int tfnum, struct stplanet *planet, boolean first_time)
//tteam att_team;
//struct sttf *task;
//struct stplanet *planet;
//boolean first_time;
{
        int     bases, att_forces, def_forces; 
        boolean a_lose_none, p_lose_none;
        float   att_odds, def_odds,attack_save,defend_save;
        tteam   def_team;

#if 0 /* THD */
        if ( left_line[24] ) {
                point(1,24);
                printw(blank_line);
                left_line[24] = false;
        }
#endif
        if ( att_team==ENEMY )
                def_team = player;
        else
                def_team = ENEMY;
        att_forces = weapons[att_team]*(task->c*c_guns + task->b*b_guns);
        def_forces = weapons[def_team]*(planet->mb*c_guns+planet->amb*b_guns);
        if ( def_forces > 0 ) {
                att_odds = fmin ( (((float) def_forces) / att_forces) , 14.0);
                attack_save = exp(log(0.8) * (att_odds));
                def_odds = fmin( (float)att_forces/def_forces, 14.0);
                defend_save=exp(log(0.8)* (def_odds));
                point(1,20);
                if ( att_team == player )
                        printw("TF%c", tfnum+'a'-1);
                else
                        printw(" EN");
                printw(": %4d(weap %2d)sur:%3d", att_forces, 
                        weapons[att_team], (int) (attack_save * 100) );
                point(1,21);
                printw(" %c%d: %4d(weap %2d)sur:%3d", planet->pstar + 'A' - 1,
                        planet->number,
                        def_forces, weapons[def_team], (int) (defend_save * 100) );
                point(1,22);
                printw("Attacker losses:              ");
                point(1,23);
                left_line[23]=true;
                printw(" Planet losses :              ");
                a_lose_none = true;
                p_lose_none = true;
                do {
                        point(17,22);
                        lose(&task->c,&a_lose_none,'c',attack_save);
                        lose(&task->b,&a_lose_none,'b',attack_save);
                        point(17,23);
                        bases = planet->mb;
                        lose(&planet->mb,&p_lose_none,'m',defend_save);
                        if ( planet->mb != bases ) printw("b");
                        bases = planet->amb;
                        lose(&planet->amb,&p_lose_none,'a',defend_save);
                        if ( planet->amb != bases ) printw("mb");
                }  while (!first_time && p_lose_none && a_lose_none);
                if ( a_lose_none ) {
                        point(17,22);
                        printw("(none)");
                };
                if ( p_lose_none ) {
                        point(17,23);
                        printw("(none)");
                };
        };
        if ( (planet->mb+planet->amb==0)&& (any_bc(att_team,planet->pstar)) ) {
                point(1,24);
                printw("Planet %d falls!               ", planet->number);
                planet->team = att_team;
                planet->esee_team = att_team;
                planet->conquered = true;
                col_stars[task->dest][def_team]--;
                col_stars[task->dest][att_team]++;
                point(50,1);
                pr5nt_star(planet->pstar);
                clear_field();
                on_board(stars[task->dest].x, stars[task->dest].y);
        };
}

void play_salvo(int starnum, boolean *battle_ptr)
//int starnum;
//boolean *battle_ptr;
{
    boolean battle, found, first_time;
    char tf_char,planch, ch; 
    int planet_num,tf_num; 
    struct stplanet *pplanet;

    battle = *battle_ptr;
    printw("Attack planet ");
    pplanet = stars[starnum].first_planet;
    if ( (col_stars[starnum][ENEMY]>1) ) {
        printw(":");
        get_char(&planch);
        cle3r_left();
        planet_num = planch - '0';
        found = false;
        while (!found) {
            if (pplanet->number == planet_num) {
                found = true;
            } else {
                if (pplanet->next == nil) {
                    found = true;
                } else {
                    pplanet = pplanet->next;
                }
            }
        }
        if ( pplanet->number != planet_num ) {
            planet_num = 0;
            error_message("That is not a useable planet");
        } else if ( pplanet->team != ENEMY ) {
            error_message("Not an enemy colony");
            planet_num = 0;
        }
    } else {
        while ( pplanet->team != ENEMY ) {
            pplanet = pplanet->next;
        }
        planet_num = pplanet->number;
        printw("%d", planet_num);
//        get_char(&ch);
        cle3r_left();
    }
    if ( planet_num != 0 ) {
        point(1,19);
        printw(" attacking tf ");
        if ( tf_stars[starnum][player] > 1 ) {
            printw(":");
            get_char(&tf_char);
            tf_num = tf_char-'A'+1;
        } else {
            tf_num = 1;
            while (tf[player][tf_num].dest != starnum || tf[player][tf_num].eta != 0 ) {
                tf_num++;
            }
            addch(tf_num+'a'-1);
        }
        if (tf_num <1 || tf_num >26) {
            error_message("Illegal tf");
        } else if ( tf[player][tf_num].dest==0 ) {
            error_message("Nonexistent tf");
        } else if ( (tf[player][tf_num].dest!=starnum) || (tf[player][tf_num].eta!=0) ) {
            error_message("Tf is not at this star      ");
        } else if ( (tf[player][tf_num].b+tf[player][tf_num].c)==0 )  {
            error_message("Tf has no warships");
        } else {
            first_time = ! pplanet->under_attack;
            if ( ! (pplanet->under_attack) ) {
                pplanet->under_attack = true;
                point(50,1);
                pr5nt_star(starnum);
                clear_field();
            }
            fire_salvo(player,&tf[player][tf_num],tf_num, pplanet,first_time);
            zero_tf(player,tf_num);
            battle = (col_stars[starnum][ENEMY]>0) && (any_bc(player,starnum));
        }
    }
    *battle_ptr = battle;
}
