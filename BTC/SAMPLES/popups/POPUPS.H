
#include <stdio.h>
#include <conio.h>

#define FALSE 0
#define TRUE 1

#define OFF 0					/* function toggle OFF value */
#define ON 1                    /* function toggle ON value */

#define POPUP_ERROR -1      	/* function return value if error occurs */


		/* 'bordertype' value constants */
#define NO_BORDER 0          	/* no border */
#define ONE_LINE 1              /* single line box border, i.e. ƒƒƒƒƒ */
#define TWO_LINE 2           	/* double line box border, i.e. ÕÕÕÕÕ */
#define THIN_SOLID 3            /* thin solid box border, i.e. ﬂﬂﬂﬂﬂ */
#define THICK_SOLID 4       	/* thick solid box border, i.e. €€€€€ */

    	/* 'shadow()' value constants */
#define NO_SHADOW 0          	/* no shadow */
#define TL_THIN 1               /* thin shadow behind top and left edge */
#define TR_THIN 2               /* thin shadow behind top and right edge */
#define BL_THIN 3               /* thin shadow behind bottom and left edge */
#define BR_THIN 4               /* thin shadow behind bottom and right edge */
#define TL_THICK 6              /* thick shadow behind top and left edge */
#define TR_THICK 7              /* thick shadow behind top and right edge */
#define BL_THICK 8              /* thick shadow behind bottom and left edge */
#define BR_THICK 9              /* thick shadow behind bottom and right edge */


/*  the following are defined in conio.h

	screen modes
	C40		Color, 40 column
	BW40	Black & White, 40 column
	C80		Color, 80 column
	BW80	Black & White, 80 column
	MONO	Monochrome, 80 column

    foreground colors
	BLACK
	BLUE
	GREEN
	CYAN
	RED
	MAGENTA
	BROWN
	LIGHTGRAY
	DARKGRAY
    LIGHTBLUE
	LIGHTGREEN
	LIGHTCYAN
	LIGHTRED
	LIGHTMAGENTA
	YELLOW
	BLINK

    background colors
	BLACK
	BLUE
	GREEN
	CYAN
	RED
	MAGENTA
	BROWN
	LIGHTGRAY
*/
