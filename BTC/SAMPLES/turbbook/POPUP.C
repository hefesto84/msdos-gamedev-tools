/* ---------- popup.c ---------- */

#include <dos.h>
#include <stdio.h>
#include <string.h>
#include <dir.h>
#include "twindow.h"

static union REGS rg;

unsigned sizeprogram = 48000/16;
unsigned scancode = 52;
unsigned keymask = 8;
char signature [] = "POPUP";

char notefile[64];

/* -------- local prototypes ---------- */
int resident(char *, void interrupt (*)());
void resinit(void);
void terminate(void);
void restart(void);
void wait(void);
void resident_psp(void);
void interrupted_psp(void);
void exec(void);

main(argc, argv)
char *argv[];
{
	void interrupt ifunc();
	int ivec;

	if ((ivec = resident(signature, ifunc)) != 0)	{
		/* ----- TSR is resident ------- */
		if (argc > 1)	{
			/* ---- there is a command line parameter --- */
			rg.x.ax = 0;
			if (strcmp(argv[1], "quit") == 0)
				rg.x.ax = 1;
			else if (strcmp(argv[1], "restart") == 0)
				rg.x.ax = 2;
			else if (strcmp(argv[1], "wait") == 0)
				rg.x.ax = 3;
			if (rg.x.ax)	{
				/* -- call the communications interrupt -- */
				int86(ivec, &rg, &rg);
				return;
			}
		}
		printf("\nPopup is already resident");
	}
	else	{
		/* ------ initial load of TSR program ------ */
		load_help("tcprogs.hlp");
		getcwd(notefile, 64);
		if (*(notefile+strlen(notefile)-1) != '\\')
			strcat(notefile, "\\");
		strcat(notefile, "note.pad");
		printf("\nResident popup is loaded");
		/* ---- T&SR --------- */
		resinit();
	}
}

/* -------- TSR communications ISR ---------- */
void interrupt ifunc(bp,di,si,ds,es,dx,cx,bx,ax)
{
	if (ax == 1)			/* "quit" */
		terminate();
	else if (ax == 2)		/* "restart" */
		restart();
	else if (ax == 3)		/* "wait" */
		wait();
}
/*page*/
/* -------- close files when terminating ---------- */
closefiles()
{
	extern FILE *helpfp;

	resident_psp();		/* switch to TSR PID */
	if (helpfp)
		fclose(helpfp);	/* close the help file */
	interrupted_psp();	/* switch to int'd PID */
}

/* -------- the popup TSR utility function --------- */
popup()
{
	int x, y;

	curr_cursor(&x, &y);
	exec();					/* call the TSR C program here */
	cursor(x, y);
}

