/* ------------ promote.c ------------ */

#include "twindow.h"
#include "keys.h"

void promote()
{
	WINDOW *wndA, *wndB, *wndC;
	int c;

	wndA = establish_window(5, 5, 9, 19);
	wndB = establish_window(10, 3, 9, 20);
	wndC = establish_window(13, 8, 9, 12);
	set_colors(wndA, ALL, RED, YELLOW, BRIGHT);
	set_colors(wndB, ALL, AQUA, YELLOW, BRIGHT);
	set_colors(wndC, ALL, WHITE, YELLOW, BRIGHT);
	display_window(wndA);
	display_window(wndB);
	display_window(wndC);
	wprintf(wndA, "\n\n Window A");
	wprintf(wndB, "\n\n Window B");
	wprintf(wndC, "\n\n Window C");
	do	{
		c = get_char();
		switch (c)	{
			case 'a':	forefront(wndA);
						break;
			case 'b':	forefront(wndB);
						break;
			case 'c':	forefront(wndC);
						break;
			case 'A':	rear_window(wndA);
						break;
			case 'B':	rear_window(wndB);
						break;
			case 'C':	rear_window(wndC);
						break;
			default:	break;
		}
	} while (c != ESC);
	delete_window(wndA);
	get_char();
	delete_window(wndC);
	get_char();
	delete_window(wndB);
}





