/*
 * HCTGAV.C
 *
 * Copyright 1990,1991 Synergrafix Consulting
 *          All Rights Reserved.
 *
 * December 31,1991
 *
 */

#include <stdio.h>
#include "hctarga.h"

char infilename[128];
char errmsg[256];

void error(char *s) {
	fcloseall();
	hctextmode();
	printf("%s\n",s);
	exit(

1);
	}

int main(int argc,char *argv[]) {

	int err,mode,w,h,ox,oy,pixsize,ttype,comp,map,commentsize;
	unsigned char comment[256];

	if (argc!=2)
		error("Usage: HCVTGA tgafilename");

	strcpy(infilename,argv[1]);

						/* Get size of TGA file */

	err=hctgasize(infilename,&w,&h,&pixsize,&ox,&oy,&ttype,&comp,&map,&commentsize,&comment);
	switch (err) {
		case HCTGACANTOPEN:
			error("Can't open input file!");
			break;
		case HCTGANOMEM:
			error("Not enough memory to load file!");
		case HCTGANOTSUPPORTED:
			error("File type not supported!");
		case HCTGACANTREAD:
			error("Error reading file!");
		}

						/* Set Hicolor mode */
	mode=hcmodesize(w,h);
	if (mode<2) mode=2;
	if (mode>3) mode=3;

	if (!hcsetmode(mode,FALSE))
		error("Can't set HiColor mode.");


						/* View TGA file */

	err=hctgaview(infilename,-1,-1,-1,-1,0,0);
	switch (err) {
		case HCTGACANTOPEN:
			error("Can't open input file!");
			break;
		case HCTGANOMEM:
			error("Not enough memory to load file!");
		case HCTGANOTSUPPORTED:
			error("File type not supported!");
		case HCTGACANTREAD:
			error("Error reading file!");
		}

	getch();                           	/* Exit */

	hctextmode();

	return 0;
	}


