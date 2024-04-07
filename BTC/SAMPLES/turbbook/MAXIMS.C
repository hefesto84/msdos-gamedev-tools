/* ------ maxims.c --------- */

#include "twindow.h"
#include "keys.h"

void maxims()
{
	int c;
	WINDOW *wnd;

	set_help("maxims  ", 50, 10);
	wnd = establish_window(5, 10, 3, 50);
	set_title(wnd, "Press F1 for help");
	set_colors(wnd, ALL, RED, WHITE, DIM);
	display_window(wnd);
	while ((c = get_char()) != ESC)	{
		switch (c)		{
			case '1':
				wprintf(wnd, "\nA stitch in time \
saves nine    ");
				break;
			case '2':
				wprintf(wnd, "\nA rolling stone \
gathers no moss");
				break;
			case '3':
				wprintf(wnd, "\nA penny saved \
is a penny earned");
				break;
			default:
				break;
		}
	}
	delete_window(wnd);
}

