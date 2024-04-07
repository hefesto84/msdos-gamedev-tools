/* ----- note.c ------ */

#include "twindow.h"

void notepad(void);
char notefile [] = "note.pad";

main()
{
	load_help("tcprogs.hlp");
	notepad();
}


