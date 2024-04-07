/* ------------ testmove.c ------------ */

#include "twindow.h"
#include "keys.h"

void testmove()
{
	WINDOW *wndA, *wndB, *wndC;
	int c;

	wndA = establish_window(5, 5, 9, 19);
	wndB = establish_window(10, 3, 9, 23);
	wndC = establish_window(13, 8, 9, 12);
	set_colors(wndA, ALL, RED, YELLOW, BRIGHT);
	set_colors(wndB, ALL, AQUA, YELLOW, BRIGHT);
	set_colors(wndC, ALL, WHITE, YELLOW, BRIGHT);
	display_window(wndA);
	display_window(wndB);
	display_window(wndC);
	wprintf(wndB, "\n I wouldn't care who");
	wprintf(wndB, "\n wrote the laws if I");
	wprintf(wndB, "\n could write the");
	wprintf(wndB, "\n ballads.");
	wprintf(wndB, "\n\n    Thomas Jefferson");
	do	{
		int x = 0, y = 0;
		c = get_char();
		switch (c)	{
			case FWD:	x++;
						break;
			case BS:	--x;
						break;
			case UP:	--y;
						break;
			case DN:	y++;
			default:	break;
		}
		if (x || y)
			rmove_window(wndB, x, y);
	} while (c != ESC);
	delete_window(wndA);
	get_char();
	delete_window(wndC);
	get_char();
	delete_window(wndB);
}



