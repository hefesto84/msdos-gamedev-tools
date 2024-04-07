/*
 * HCGIFV.C
 *
 * Copyright 1990,1991 Synergrafix Consulting
 *          All Rights Reserved.
 *
 * December 31,1991
 *
 */

#include <stdio.h>
#include "hcgif.h"

char infilename[128];
char errmsg[256];

void error(char *s) {
	fcloseall();
	hctextmode();
	printf("%s\n",s);
	exit(1);
	}

int main(int argc,char *argv[]) {

	int err,mode,w,h;

	if (argc!=2)
		error("Usage: HCGIFV giffilename");

	strcpy(infilename,argv[1]);

						/* Get size of GIF file */

	err=hcgifsize(infilename,&w,&h,0L);
	switch (err) {
		case HCGIFCANTOPEN:
			error("Can't open input file!");
			break;
		case HCGIFNOMEM:
			error("Not enough memory to load file!");
		case HCGIFNOTGIF:
			error("Not a GIF File!");
		case HCGIFBADGIF:
			error("Error reading file!");
		}

						/* Set Hicolor mode */
	mode=hcmodesize(w,h);
	if (mode<2) mode=2;
	if (mode>3) mode=3;

	if (!hcsetmode(mode,FALSE))
		error("No HiColor DAC, or can't set mode!");


						/* View GIF file */

	err=hcgifview(infilename,-1,-1,0L,1);
	switch (err) {
		case HCGIFCANTOPEN:
			error("Can't open input file!");
			break;
		case HCGIFNOMEM:
			error("Not enough memory to load file!");
		case HCGIFNOTGIF:
			error("Not a GIF File!");
		case HCGIFBADGIF:
			error("Error reading file!");
		}

	getch();                           	/* Exit */

	hctextmode();

	return 0;
	}


