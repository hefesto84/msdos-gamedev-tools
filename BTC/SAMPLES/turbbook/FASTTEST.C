/* ------------ fasttest.c ------------ */

#include <stdio.h>
#include "twindow.h"

void fasttest()
{
	int row, col;

	for (row = 0, col = 0; col < 15; row += 3, col++)	{
		establish_window(row, col, 10, 30);
		set_colors(NULL, ALL, RED, YELLOW, BRIGHT);
		display_window(NULL);
		wprintf(NULL,"\n\n\n   Hello, Dolly # %d", col);
	}
	get_char();
	while (col--)
		delete_window(NULL);
}


