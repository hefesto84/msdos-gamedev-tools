/*
 * HCTarga.H
 *
 * Copyright 1990,1991 Synergrafix Consulting
 *          All Rights Reserved.
 *
 * November 19,1991
 *
 */

#ifndef _HC_TGAINCLUDE

#define _HC_TGAINCLUDE

#include "hicolor.h"

#define HCTGACANTOPEN	-1
#define HCTGANOMEM	-2
#define HCTGANOTSUPPORTED -3
#define HCTGACANTREAD	-4
#define HCTGACANTWRITE	-5

#define MAXTGAXWIDTH	1024
#define MAXTGAXHEIGHT	1024
#define MAXTCOMMENT	256

int hctgaview(char *fname,int x,int y,int x1,int y1,int dither,int jitter);
int hctgasave(char *fname,int x,int y,int x1,int y1,int compressed,BYTE csize,BYTE *comm);
int hctgasize(char *fname,int *w,int *h,int *pixsize,int *ox,int *oy,int *type,int *comp,int *mapp,BYTE *csize,BYTE *comm);

#endif