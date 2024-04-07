#include <prowindc.h>

/*
	Select the "Project" Menu and enter "TESTPRO.PRJ" as the project name.
*/

void myscreen()
{
	popwindow(3, 3, 17, 76, attr(15, 4), 1, 1, 1);
	titlewindow(2, "[ ProWindows *LITE* Test Program ]");
	cwcprint(2, attr(11, 4), "The 'ProWindows' Window Logic System");
        cwcprint(14, attr(14, 4), "The Best 'Pop Up' Window Manager in the World");

        popwindow(22, 3, 3, 76, attr(15, 1), 2, 0, 0);
	cwcprint(1, attr(30, 1), "One Moment Please...");

	/*
		This is the end of the user screen.
		copyright(); is invoked automaticly, next
	*/
}

void main()
{
	clrscr();

	initpro(7, 19, myscreen, 0);

	removewindow();			/* Remove Copyright (automatic 3 second delay) */

	removewindow();			/* Remove "One Moment Please...." */

	removewindow();			/* Remove Main Window */
}
