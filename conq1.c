#include <stdio.h>
#include <curses.h>
#include <math.h>
#include "defs.h"
#include "structs.h"
#include "vars.h"

/*
 * are there any attack forces belonging to team at current star?
 */
boolean any_bc(tteam team, int starnum)
{
	boolean any;
	int tf_number;

	any = false;
	if (tf_stars[starnum][team] > 0) {
		tf_number = 1;
		while ((!any) && (tf_number < 27)) {
			any = (tf[team][tf_number].dest == starnum) && (tf[team][tf_number].eta == 0)
			    && ((tf[team][tf_number].c > 0) || (tf[team][tf_number].b > 0));
			tf_number = tf_number + 1;
		}
	}
	return (any);
}

void best_plan(int *starnum_ptr, int *size_ptr, tteam *team_ptr)
{
	tplanet *pplanet;
	int starnum, size;
	tteam team;

	team = none;
	starnum = *starnum_ptr;
	size = 0;
	pplanet = stars[starnum].first_planet;
	while (pplanet != nil) {
		if (pplanet->capacity > size) {
			size = pplanet->capacity;
			team = pplanet->team;
		};
		pplanet = pplanet->next;
	};
	*team_ptr = team;
	*starnum_ptr = starnum;
	*size_ptr = size;
}

void disp_tf(struct sttf *taskf)
{
	if (taskf->t != 0)
		printw("%2dt", taskf->t);
	else
		printw("   ");
	if (taskf->s != 0)
		printw("%2ds", taskf->s);
	else
		printw("   ");
	if (taskf->c != 0)
		printw("%2dc", taskf->c);
	else
		printw("   ");
	if (taskf->b != 0)
		printw("%2db", taskf->b);
	else
		printw("   ");
}
