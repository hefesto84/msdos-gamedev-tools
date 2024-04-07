/* ------------ tmenu.c ------------ */

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include "keys.h"
#include "twindow.h"

extern int VSG;

WINDOW *open_menu(char *mnm, MENU *mn, int hsel);
int gethmenu(MENU *mn, WINDOW *hmenu, int hsel);
int getvmn(MENU *mn, WINDOW *hmenu, int *hsel, int vsel);
int haccent(MENU *mn, WINDOW *hmenu, int hsel, int vsel);
void dimension(char *sl[], int *ht, int *wd);
void light(MENU *mn, WINDOW *hmenu, int hsel, int d);

/* ------------- display & process a menu ----------- */
void menu_select(char *name, MENU *mn)
{
	WINDOW *open_menu();
	WINDOW *hmenu;
	int sx, sy;
	int hsel = 1, vsel;

	curr_cursor(&sx, &sy);
	cursor(0, 26);
	hmenu = open_menu(name, mn, hsel);
	while (hsel = gethmenu(mn, hmenu, hsel))	{
		vsel = 1;
		while (vsel = getvmn(mn, hmenu, &hsel, vsel))	{
			delete_window(hmenu);
			set_help("", 0, 0);
			(*(mn+hsel-1)->func [vsel-1])(hsel, vsel);
			hmenu = open_menu(name, mn, hsel);
		}
	}
	delete_window(hmenu);
	cursor(sx, sy);
}

/* ------- open a horizontal menu ------- */
static WINDOW *open_menu(char *mnm, MENU *mn, int hsel)
{
	int i = 0;
	WINDOW *hmenu;

	set_help("menu    ", 30, 10);
	hmenu = establish_window(0, 0, 3, 80);
	set_title(hmenu, mnm);
	set_colors(hmenu, ALL, BLUE, AQUA, BRIGHT);
	set_colors(hmenu, ACCENT, WHITE, BLACK, DIM);
	display_window(hmenu);
	while ((mn+i)->mname)
		wprintf(hmenu, " %-10.10s ", (mn+i++)->mname);
	light(mn, hmenu, hsel, 1);
	cursor(0, 26);
	return hmenu;
}

/* ------- get a horizontal selection ---------- */
static int gethmenu(MENU *mn, WINDOW *hmenu, int hsel)
{
	int sel;

	light(mn, hmenu, hsel, 1);
	while (TRUE)	{
		switch (sel = get_char())	{
			case FWD:
			case BS:	hsel = haccent(mn, hmenu, hsel, sel);
						break;
			case ESC:	return 0;
			case '\r':	return hsel;
			default:	putch(BELL);
						break;
		}
	}
}
/*page*/
/* ---------pop down a vertical menu --------- */
static int getvmn(MENU *mn,WINDOW *hmenu,int *hsel,int vsel)
{
	WINDOW *vmenu;
	int ht = 10, wd = 20;
	char **mp;

	while (1)	{
		dimension((mn+*hsel-1)->mselcs, &ht, &wd);
		vmenu = establish_window(2+(*hsel-1)*12, 2, ht, wd);
		set_colors(vmenu, ALL, BLUE, AQUA, BRIGHT);
		set_colors(vmenu, ACCENT, WHITE, BLACK, DIM);
		set_border(vmenu, 4);
		display_window(vmenu);
		mp = (mn+*hsel-1)->mselcs;
		while(*mp)
			wprintf(vmenu, "\n%s", *mp++);
		vsel = get_selection(vmenu, vsel, "");
		delete_window(vmenu);
		if (vsel == FWD || vsel == BS)	{
			*hsel = haccent(mn, hmenu, *hsel, vsel);
			vsel = 1;
		}
		else
			return vsel;
	}
}

/* ----- manage the horizontal menu selection accent ----- */
static int haccent(MENU *mn,WINDOW *hmenu,int hsel,int sel)
{
	switch (sel)	{
		case FWD:
			light(mn, hmenu, hsel, 0);
			if ((mn+hsel)->mname)
				hsel++;
			else
				hsel = 1;
			light(mn, hmenu, hsel, 1);
			break;
		case BS:
			light(mn, hmenu, hsel, 0);
			if (hsel == 1)
				while ((mn+hsel)->mname)
					hsel++;
			else
				--hsel;
			light(mn, hmenu, hsel, 1);
			break;
		default:
			break;
	}
	return hsel;
}

/* ---------- compute a menu's height & width --------- */
static void dimension(char *sl[], int *ht, int *wd)
{
	unsigned strlen(char *);

	*ht = *wd = 0;
	while (sl [*ht])	{
		*wd = max(*wd, strlen(sl [*ht]));
		(*ht)++;
	}
	*ht += 2;
	*wd += 2;
}

/* --------- accent a horizontal menu selection ---------- */
static void light(MENU *mn, WINDOW *hmenu, int hsel, int d)
{
	if (d)
		reverse_video(hmenu);
	wcursor(hmenu, (hsel-1)*12+2, 0);
	wprintf(hmenu, (mn+hsel-1)->mname);
	normal_video(hmenu);
	cursor(0, 26);
}


							   
