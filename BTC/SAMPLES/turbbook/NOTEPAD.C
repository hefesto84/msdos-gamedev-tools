/* ---------------- notepad.c ------------------ */

#include <stdio.h>
#include <mem.h>
#include "twindow.h"

#define LWID 60
#define WHT 10
#define PADHT 20

char bf [PADHT] [LWID];
extern char notefile[];

void notepad()
{
	WINDOW *wnd;
	FILE *fp, *fopen();
	int i, ctr = 0;

	set_help("notepad ", 0, 0);
	setmem(bf, sizeof bf, ' ');
	if ((fp = fopen(notefile, "rt")) != NULL)	{
		while (fread(bf [ctr], LWID, 1, fp))
			ctr++;
		fclose(fp);
	}
	wnd = establish_window
		((80-(LWID+2))/2, (25-(WHT+2))/2, WHT+2, LWID+2);
	set_border(wnd, 3);
	set_title(wnd, "  Note Pad  ");
	set_colors(wnd, ALL, BLUE, AQUA, BRIGHT);
	set_colors(wnd, ACCENT, WHITE, BLACK, DIM);
	display_window(wnd);
	text_editor(wnd, bf, LWID * PADHT);
	delete_window(wnd);
	ctr = PADHT;
/*page*/
	while (--ctr)	{
		for (i = 0; i < LWID; i++)
			if (bf [ctr] [i] != ' ')
				break;
		if (i < LWID)
			break;
	}
	fp = fopen(notefile, "w");
	for (i = 0; i < ctr+1; i++)
		fwrite(bf[i], LWID, 1, fp);
	fclose(fp);
}

