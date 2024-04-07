/*
*
*	Program to show the simplest form of window creation. It pops up a
*	window, then waits for a keypress, then exits. Because it's the first
*	window on the screen, you do not need to specify POPUP, as the DOS
*	screen is automatically restored on exit.
*
*/

#include <stdio.h>

#include "stdinc.h"
#include "wn.h"
#include "kb.h"

void	main(void);

void
main()
{
	WNopen(25,5,55,15,BLACK|BKGR(LIGHTGRAY),DOUBLE,"Press any key");
	KBgetch();
}
