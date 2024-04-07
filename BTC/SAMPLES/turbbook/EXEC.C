/* ----------- exec.c --------- */
#include <stdio.h>
#include "twindow.h"
/*   ------ local prototypes --------  */
void testmove(void);
void promote(void);
void ccolor(void);
void fasttest(void);
void notepad(void);
void ordent(void);
void poems(void);
void maxims(void);
/* ---------- menu tables --------- */
char *dselcs[] = {
	" move ",
	" promote ",
	" colors ",
	" fast ",
	NULL
};
char *pselcs[] = {
	" notepad ",
	" orders ",
	" poetry ",
	" sayings ",
	NULL
};
static void (*dfuncs[])()={testmove,promote,ccolor,fasttest};
static void (*pfuncs[])()={notepad,ordent,poems,maxims};
static MENU tmn [] = {
	{" demos ", 	dselcs, dfuncs},
	{" programs ",	pselcs, pfuncs},
	{NULL,NULL,NULL}
};

void exec()
{
	menu_select(" TC Executive ", tmn);
}

