/* --------- thelp.c ----------- */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "twindow.h"
#include "keys.h"

#define MAXHELPS 25
#define HBG WHITE
#define HFG BLACK
#define HINT DIM

#define TRUE 1
#define FALSE 0

static struct helps {
	char hname [9];
	int h, w;
	long hptr;
} hps [MAXHELPS+1];

static int hp = 0;
static int ch = 0;
static int hx, hy;
FILE *helpfp = NULL;
long ftell();
char *fgets();
void help();
char helpname[64];
void getline(char *lineh);
/*page*/
/* ----------- load the HELP! definition file ------------ */
void load_help(char *hn)
{
	extern void (*helpfunc)();
	extern int helpkey;
	char lineh [80];

	if (strcmp(helpname, hn) == 0)
		return;
	helpfunc = help;
	helpkey = F1;
	hp = 0;
	strcpy(helpname, hn);
	if ((helpfp = fopen(helpname, "r")) == NULL)
		return;
	getline(lineh);
	while (1)	{
		if (hp == MAXHELPS)
			break;
		if (strncmp(lineh, "<end>", 5) == 0)
			break;
		if (*lineh != '<')
			continue;
		hps[hp].h = 3;
		hps[hp].w = 18;
		strncpy(hps[hp].hname, lineh+1, 8);
		hps[hp].hptr = ftell(helpfp);
		getline(lineh);
		while (*lineh != '<')	{
			hps[hp].h++;
			hps[hp].w = max(hps[hp].w, strlen(lineh)+2);
			getline(lineh);
		}
		hp++;
	}
}
/*page*/
/* -------- get a line of text from the help file -------- */
static void getline(char *lineh)
{
	if (fgets(lineh, 80, helpfp) == NULL)
		strcpy(lineh, "<end>");
}
/* -------- set the current active help screen ----------- */
void set_help(char *s, int x, int y)
{
	for (ch = 0; ch < hp; ch++)
		if (strncmp(s, hps[ch].hname, 8) == 0)
			break;
	hx = x;
	hy = y;
}
/* ---------- display the current help window ----------- */
void help()
{
	char ln [80];
	int i, xx, yy;
	WINDOW *wnd;
	extern int helpkey;
	if (hp && ch != hp)	{
		curr_cursor(&xx, &yy);
		cursor(0, 25);
		wnd = establish_window(hx, hy, hps[ch].h, hps[ch].w);
		set_colors(wnd, ALL, HBG, HFG, HINT);
		display_window(wnd);
		fseek(helpfp, hps[ch].hptr, 0);
		for (i = 0; i < hps[ch].h-3; i++)	{
			getline(ln);
			wprintf(wnd, ln);
		}
		wprintf(wnd, " [Help] to return");
		while (get_char() != helpkey)
			putchar(BELL);
		delete_window(wnd);
		cursor(xx, yy);
	}
}

