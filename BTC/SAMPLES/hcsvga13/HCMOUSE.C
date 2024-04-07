/*
 * HCMOUSE.C
 *
 * Copyright 1990, Synergrafix Constulting
 * 	      All rights reserved.
 *
 * Version 1.0
 * Dec. 10 1991
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include "hicolor.h"

void error(char *s) {

	hctextmode();
	printf("%s\n",s);
	exit(1);
	}

main() {

	int x,y,b,oldx,oldy;
	char str[100];

	if (!hcsetmode(HC_SVGAHI,TRUE)) {
		error("Can't set HiColor mode.");
		}

	if (!initgrcursor(0,10000)) {
		error("No mouse detected.");
		}

        sprintf(str,"Use Left Mouse Button to Draw.");
	hcputstr(str,0,hcgetmaxy()-20,10000,0,0);
	sprintf(str,"Any key to exit.");
	hcputstr(str,0,hcgetmaxy()-10,10000,0,0);

	oldx=getgrcursorx(); oldy=getgrcursory();

	unputgrcursor();

	hcrectanglexor(100,100,oldx,oldy,20000);

        putgrcursor(oldx, oldy);

	do {
	   getmouse(&x, &y, &b);

	   if ((x != getgrcursorx() ) || (y != getgrcursory() )) {

	      unputgrcursor();

	      hcrectanglexor(100,100,oldx,oldy,20000);

	      if (b==1) {
		      hcline(oldx,oldy,x,y,(x+20)*(y+20));
		      }

	      hcrectanglexor(100,100,x,y,20000);

	      putgrcursor(x, y);

	      oldx=x; oldy=y;
	   }

	}  while (!kbhit());

	closegrcursor();

	getch();

	hctextmode();

	return 0;
	}

