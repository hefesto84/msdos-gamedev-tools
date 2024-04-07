#include <dos.h>
#include "blit.h"

#define video(x)	{_AX = x;geninterrupt(0x10);}
#define mouse(x)	{_AX = x;geninterrupt(0x33);}

/*
 * this code assumes an EGA and a mouse.
 */

main()
{
	struct bitmap src, dest;
	int	buttons;

	video(0x10);
	printf("hello");

	src.bounds.left = 8;
	src.bounds.right = 28;
	src.bounds.top = 0;
	src.bounds.bot = 14;
	src.bytes = dest.bytes = 80;
	src.pntr = dest.pntr = MK_FP(0xa000, 0);

	mouse(1);
	do {
		mouse(3);
		buttons = _BX;
		dest.bounds.left = _CX;
		dest.bounds.top = _DX;
		dest.bounds.right = dest.bounds.left + 20;
		dest.bounds.bot = dest.bounds.top + 14;
		mouse(2);
		blit(&src, &dest, or_verb);
		mouse(1);
	} while (buttons == 0);
	mouse(2);

	video(0x3);
}