//#define new(x) x=(tplanet *)alloc(sizeof(tplanet))
#include <stdio.h>
#include <curses.h>
#include "defs.h"
#include "structs.h"
#include "vars.h"

void enemy_attack(int starnum);
void pl2yerattack(int starnum);

void battle()
{
	boolean first;
	int starnum;
	first = true;
	for (starnum = 1; starnum <= nstars; starnum++) {
		if (tf_stars[starnum][ENEMY] > 0 && tf_stars[starnum][player] > 0
		  && (any_bc(ENEMY, starnum) || any_bc(player, starnum))) {
			if (first) {
				point(33, 20);
				printw("* Tf battle *   ");
				first = false;
			};
			tf_battle(starnum);
		};
		if ((any_bc(ENEMY, starnum)) && (col_stars[starnum][player]
						 > 0))
			enemy_attack(starnum);
		else if ((tf_stars[starnum][player] > 0)
			 && (col_stars[starnum][ENEMY] > 0))
			pl2yerattack(starnum);
	};
}


void enemy_attack(int starnum)
{
	int attack_factors, def_factors;
	float odds, best_score;
	struct stplanet *pplanet, *best_planet;
	int en_tf, i;
	boolean first[8];
	for (i = 1; i <= 7; i++)
		first[i] = true;
	en_tf = 1;
	while ((tf[ENEMY][en_tf].dest != starnum) || (tf[ENEMY][en_tf].eta != 0))
		en_tf = en_tf + 1;
	do {
		attack_factors = tf[ENEMY][en_tf].c + 6 * tf[ENEMY][en_tf].b;
		best_planet = nil;
		best_score = 1000;
		pplanet = stars[starnum].first_planet;
		while (pplanet != nil) {
			if ((pplanet->team == player)) {
				def_factors = pplanet->esee_def;
				odds = (float)def_factors / attack_factors;
				if (pplanet->capacity > 30)
					odds = (odds - 2) * pplanet->capacity;
				else
					odds = (odds - 1.5) * pplanet->capacity;
				if (odds < best_score) {
					best_score = odds;
					best_planet = pplanet;
				};
			};
			pplanet = pplanet->next;
		};
		if (best_score < 0) {
			cle3r_left();
			point(1, 19);
			printw("Enemy attacks: %c%d", starnum + 'A' - 1, best_planet->number);
			point(50, 1);
			pr5nt_star(starnum);
			clear_field();
			pause();
			fire_salvo(ENEMY, &tf[ENEMY][en_tf], 0, best_planet, first[best_planet->number]);
			first[best_planet->number] = false;
			zero_tf(ENEMY, en_tf);
			best_planet->esee_def = best_planet->mb + 6 * best_planet->amb;
			pause();
		};
	}
	while (best_score < 0 && any_bc(ENEMY, starnum));
	revolt(starnum);
}

void pl2yerattack(int starnum)
{
	boolean battle;
	char command;
	struct stplanet *pplanet;
	battle = any_bc(player, starnum);
	if ((battle)) {
		point(33, 20);
		printw("Attack at star %c", starnum + 'A' - 1);
		while (battle) {
			point(50, 1);
			pr5nt_star(starnum);
			clear_field();
			point(1, 18);
			printw("P?                            ");
			point(3, 18);
			get_char(&command);
			switch (command) {
			case 'S':
				starsum();
				break;
			case 'M':
				printmap();
				break;
			case 'H':
				help(3);
				pause();
				break;
			case 'N':
				make_tf();
				break;
			case 'J':
				join_tf();
				break;
			case 'C':
				print_col();
				break;
			case 'R':
				ressum();
				break;
			case 'T':
				tfsum();
				break;
			case 'G':
			case ' ':
				play_salvo(starnum, &battle);
				break;
			case 'B':
				printw("reak off attack");
				battle = false;
				break;
			default:
				cle3r_left();
				error_message("Illegal command");
				break;
			} /*switch ( */
		}
		pplanet = stars[starnum].first_planet;
		while (pplanet != nil) {
			pplanet->under_attack = false;
			pplanet = pplanet->next;
		}
		point(1, 24);
		printw("Planet attack concluded       ");
		revolt(starnum);
	}
}
