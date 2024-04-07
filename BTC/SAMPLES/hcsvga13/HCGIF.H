/*
 * HCGIF.H
 *
 * Copyright 1990,1991 Synergrafix Consulting
 *          All Rights Reserved.
 *
 * November 19,1991
 *
 */

#ifndef _HC_GIFINCLUDE

#define _HC_GIFINCLUDE

#include "hicolor.h"

#define HCGIFCANTOPEN	-1
#define HCGIFNOMEM	-2
#define HCGIFBADGIF	-3
#define HCGIFNOTGIF	-4

int hcgifsize(char *filename,int *width,int *height,long offset);
int hcgifview(char *filename,int xs,int ys,long offset,int autoscale);

#endif