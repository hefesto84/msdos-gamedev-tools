/* ------------ ccolor.c ------------ */

#include "twindow.h"
#include "keys.h"

void ccolor()
{
	WINDOW *wndA, *wndB, *wndC;
	int c;

	wndA = establish_window(8, 8, 9, 19);
	wndB = establish_window(13, 6, 9, 20);
	wndC = establish_window(16, 11, 9, 12);
	set_colors(wndA, ALL, RED, YELLOW, BRIGHT);
	set_colors(wndB, ALL, AQUA, YELLOW, BRIGHT);
	set_colors(wndC, ALL, WHITE, YELLOW, BRIGHT);
	display_window(wndA);
	display_window(wndB);
	display_window(wndC);
	do	{
		c = get_char();
		switch (c)	{
			case 'r':
				set_title(wndB, " RED ");
				set_colors(wndB, ALL, RED, WHITE, BRIGHT);
				break;
			case 'b':
				set_title(wndB, " BLUE ");
				set_colors(wndB, ALL, BLUE, WHITE, BRIGHT);
				break;
			case 'g':
				set_title(wndB, " GREEN ");
				set_colors(wndB, ALL, GREEN, WHITE, BRIGHT);
				break;
			default:
				break;
		}
	} while (c != ESC);
	delete_window(wndA);
	get_char();
	delete_window(wndC);
	get_char();
	delete_window(wndB);
}
