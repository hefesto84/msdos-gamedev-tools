/* --------------- ibmpc.c -------------- */

/*
 * Low-level functions addressing BIOS & PC Hardware
 */

#pragma inline
#include <dos.h>
static union REGS rg;

/* ----------- position the cursor ------------- */
void cursor(int x, int y)
{
	rg.x.ax = 0x0200;
	rg.x.bx = 0;
	rg.x.dx = ((y << 8) & 0xff00) + x;
	int86(16, &rg, &rg);
}

/* ------------ return the cursor position ------------- */
void curr_cursor(int *x, int *y)
{
	rg.x.ax = 0x0300;
	rg.x.bx = 0;
	int86(16, &rg, &rg);
	*x = rg.h.dl;
	*y = rg.h.dh;
}

/* ----------- set cursor type --------------- */
void set_cursor_type(int t)
{
	rg.x.ax = 0x0100;
	rg.x.bx = 0;
	rg.x.cx = t;
	int86(16, &rg, &rg);
}
/*page*/
char attrib = 7;

/* ------------- clear the screen -------------- */
void clear_screen()
{
	cursor(0, 0);
	rg.h.al = ' ';
	rg.h.ah = 9;
	rg.x.bx = attrib;
	rg.x.cx = 2000;
	int86(16, &rg, &rg);
}

/* ----------- return the video mode ------------ */
int vmode()
{
	rg.h.ah = 15;
	int86(16, &rg, &rg);
	return rg.h.al;
}

/* -------- test for scroll lock -------- */
int scroll_lock()
{
	rg.x.ax = 0x0200;
	int86(0x16, &rg, &rg);
	return rg.h.al & 0x10;
}
/*page*/
void (*helpfunc)();
int helpkey = 0;
int helping = 0;

/* ------------- get a keyboard character ---------------- */
int get_char()
{
 	int c;

	while (1)	{
		rg.h.ah = 1;
		int86(0x16, &rg, &rg);
		if (rg.x.flags & 0x40)	{
			int86(0x28, &rg, &rg);
			continue;
		}
		rg.h.ah = 0;
		int86(0x16, &rg, &rg);
		if (rg.h.al == 0)
			c = rg.h.ah | 128;
		else
			c = rg.h.al;
		if (c == helpkey && helpfunc)	{
			if (!helping)	{
				helping = 1;
				(*helpfunc)();
				helping = 0;
				continue;
			}
		}
		break;
	}
	return c;
}
int EGA = 1;
/*page*/
/* --- insert a character and attribute into video RAM --- */
void vpoke(unsigned vseg, unsigned adr, unsigned chr)
{
	if (vseg == 45056 || EGA)	/* monochrome mode */
		poke(vseg, adr, chr);
	else	{
		_DI = adr;		/* offset of video character */
		_ES = vseg;		/* video segment */
		asm cld;
		_BX = chr;		/* the attribute and character */
		_DX = 986;		/* video status port */
		/* ------ wait for video retrace to start ----- */
		do
			asm in  al,dx;
		while (_AL & 1);
		/* ------ wait for video retrace to stop ----- */
		do
			asm in  al,dx;
		while (!(_AL & 1));
		_AL = _BL;
		asm stosb;		/* store character */
		/* ------ wait for video retrace to start ----- */
		do
			asm in  al,dx;
		while (_AL & 1);
		/* ------ wait for video retrace to stop ----- */
		do
			asm in  al,dx;
		while (!(_AL & 1));
		_AL = _BH;
		asm stosb;		/* store attribute */
	}
}

/*page*/
/* ---- read a character and attribute from video RAM --- */
int vpeek(unsigned vseg, unsigned adr)
{
	int ch, at;

	if (vseg == 45056 || EGA)			/* monochrome mode */
		return peek(vseg, adr);
	asm push ds;
	_DX = 986;			/* video status port */
	_DS = vseg;			/* video segment address */
	_SI = adr;			/* video character offset */
	asm cld;
	/* ------ wait for video retrace to start ----- */
	do
		asm in  al,dx;
	while (_AL & 1);
	/* ------ wait for video retrace to stop ----- */
	do
		asm in  al,dx;
	while (!(_AL & 1));
	asm lodsb;			/* get the character */
	_BL = _AL;
	/* ------ wait for video retrace to start ----- */
	do
		asm in  al,dx;
	while (_AL & 1);
	/* ------ wait for video retrace to stop ----- */
	do
		asm in  al,dx;
	while (!(_AL & 1));
	asm lodsb;			/* get the attribute */
	_BH = _AL;
	_AX = _BX;
	asm pop ds;
	return _AX;
}

