#define GLOBAL_DEFINITION_OR_EXTERN /**/
#include "defs.h"
#include "structs.h"
#include "vars.h"
#include <stdio.h>
#include <curses.h>

void check_game_over();

void main()
{
	initscr();

	printw("\n *** CONQUEST *** \n");
	initconst();
	initmach();
	do {
		inputplayer();
		if (!game_over) {
			inputmach();
			move_ships();
			battle();
			if (production_year == 4 && turn < 100) {
				invest();
			}
			up_year();
		}
		check_game_over();
	}
	while (!game_over);
}

void point(int col, int row)
{
	move(row - 1, col - 1);
	refresh();
	x_cursor = col;
	y_cursor = row;
	if ((x_cursor < 20) && (y_cursor != 18)) {
		left_line[y_cursor] = true;
	}
}

int rnd(int i)
{
	int k;
	k = lrand48() / 2;
	k = (k % i) + 1;
	return (k);
}

int conq_round(x)
float x;
{
	if (x < 0.0)
		return ((int)(x - .5));
	else
		return ((int)(x + .5));
}

int min(a, b)
int a;
int b;
{
	return a < b ? a : b;
}

void check_game_over()
{
	boolean dead[2];
	boolean quit_game;
	int total[2], transports[2], inhabs[2];
	tteam team;
	int tfnum, starnum;
	struct stplanet *pplan;

	quit_game = game_over;
	for (team = ENEMY; team <= player; team++) {
		transports[team] = 0;
		inhabs[team] = 0;
		for (tfnum = 1; tfnum <= 26; tfnum++) {
			if (tf[team][tfnum].dest != 0) {
				transports[team] = transports[team] + tf[team][tfnum].t;
			}
		}
	}
	for (starnum = 1; starnum <= nstars; starnum++) {
		pplan = stars[starnum].first_planet;
		while (pplan != nil) {
			switch (pplan->team) {
				case player:
					inhabs[player] = inhabs[player] + pplan->iu;
					break;
				case ENEMY:
					inhabs[ENEMY] = inhabs[ENEMY] + pplan->iu;
					break;
			} /*switch ( */
			pplan = pplan->next;
		}
	}
	for (team = ENEMY; team <= player; team++) {
		total[team] = inhabs[team] + transports[team];
		dead[team] = total[team] == 0;
	}
	if ((!dead[player]) && (!dead[ENEMY]) && (turn >= 40)) {
		dead[ENEMY] = total[player] / total[ENEMY] >= 8;
		dead[player] = total[ENEMY] / total[player] >= 8;
	}
	game_over = dead[player] || dead[ENEMY] || (turn > 100) || quit_game;
	if (game_over) {
		endwin();
		printf("*** Game Over : Turn %d ***\n", turn);
		printf("Player: Population in transports:%3d", transports[player]);
		printf("  IU's on colonies:%4d  TOTAL:%4d\n", inhabs[player], total[player]);
		printf("Enemy:  Population in transports:%3d", transports[ENEMY]);
		printf("  IU's on colonies:%4d  TOTAL:%4d\n", inhabs[ENEMY], total[ENEMY]);
		if ((total[ENEMY] > total[player]) || quit_game) {
			printf("**** THE ENEMY HAS CONQUERED THE GALAXY ***\n");
		} else if ((total[player] > total[ENEMY])) {
			printf("*** PLAYER WINS - YOU HAVE SAVED THE GALAXY! ***\n");
		} else {
			printf("*** DRAWN ***\n");
		}
	}
}
