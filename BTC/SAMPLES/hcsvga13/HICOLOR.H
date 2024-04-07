/*
 * HiColor.H
 *
 * Copyright 1990,1991 Synergrafix Consulting
 *          All Rights Reserved.
 *
 * November 19,1991
 *
 */

#ifndef HC_INCLUDE

#ifdef __386__
#define ADD2LINADD(a)       ((a)<<4)
#endif

#define HC_INCLUDE	1

#include <dos.h>
#include <malloc.h>
#include <string.h>
#include <conio.h>

#define HC_VESA		1
#define HC_TSENG4   2
#define HC_ATIXL    3

#define HC_SVGALO	0   /* 640x350 */ /* Not all modes supported on all */
#define HC_SVGAMED	1   /* 640x400 */ /* graphics hardware!!! */
#define HC_SVGAHI	2   /* 640x480 */
#define HC_SVGAHI2	3   /* 800x600 */
#define HC_SVGAHI3	4   /* 1024x768 */
#define HC_SVGAHI4	5   /* 1280x1024 */

#define	XOR	0
#define OR	1
#define COPY	2

/* TSENG Modes: 0x2D=640x350; 0x2E=640x480; 0x2F=640x400; 0x30=800x600 */

#ifndef FALSE
#define FALSE	0
#define TRUE	!FALSE
#endif

#define SCREENSEG	0xA000
#define GCSEGSEL	0x3CD

#define DAC_MASK  0x3C6 /* DAC pixel mask reg address, also Sierra
			   command reg address when enabled */

#define DAC_WADDR 0x3C8  /* DAC write address reg address */

#ifndef BYTE
typedef unsigned char BYTE;
#endif
#ifdef __386__
#include <i86.h>
#define ax eax
#define bx ebx
#define cx ecx
#define dx edx
#define di edi
#define si esi
#define int86 int386
#define int86x int386x
#ifndef WORD
typedef unsigned short int WORD;
#endif
#ifndef DWORD
typedef unsigned int DWORD;
#endif
#else
#ifndef WORD
typedef unsigned int WORD;
#endif
#ifndef DWORD
typedef unsigned long DWORD;
#endif
#endif

#define RGB256INT(r,g,b)	(WORD)( ( ((r)>>3)<<10 ) | ( ((g)>>3)<<5 ) | ((b)>>3) )
//#define RGB256INT(r,g,b)	(WORD)( _rlu[r] | _glu[g] | _blu[b] )
#define RGBINT(r,g,b)		(WORD)( ((r)<<10) | ((g)<<5) | (b) )
#define INTRED(c)			((WORD)(c)>>10)
#define INTGREEN(c)			(((WORD)(c)<<5)>>10)
#define INTBLUE(c)			(((WORD)(c)<<10)>>10)
#define INTRED256(c)		(INTRED(c)<<3)
#define INTGREEN256(c)		(INTGREEN(c)<<3)
#define INTBLUE256(c)       (INTBLUE(c)<<3)
#define RGB256GREY(r,g,b)	(((WORD)r*77+(WORD)g*150+(WORD)b*29)>>8)
#define RGBGREY(r,g,b)		(((WORD)(r)*10+(WORD)(g)*18+((WORD)(b)<<2))>>5)
#define SWAP(a,b)		{ a^=b; b^=a; a^=b; }

void hcmodinx(WORD pt,WORD inx,WORD mask,WORD nwv);

#define BANK(b)		{ if (b!=hcoldbank) {      			\
				hcoldbank=b;				\
				switch (hcgrcard) {                     \
					case HC_VESA:                   \
						hcbankregs.x.ax=0x4f05; \
						hcbankregs.x.bx=0;      \
						hcbankregs.x.dx=b;      \
						int86(0x10, &hcbankregs,&hcbankregs);\
						break;                  \
					case HC_TSENG4:                 \
						outp(GCSEGSEL, (b<<4)|b);\
						break;                  \
					case HC_ATIXL:                  \
						hcmodinx(hcatireg,0xb2,0xee,b*0x22);\
						break;                  \
					}                               \
				}                               	\
			}

#define DOBANK		BANK(*(((WORD *)&hcbitmap)+1));

extern WORD hcoldbank;
extern DWORD hcbitmap;
extern WORD hcmaxx,hcmaxy,hcmaxc;
extern union REGS hcbankregs;

/* HICOLOR.C 19/12/91 10.11.22 */

int hicolorDAC (void);
int tseng4000 (void);
int hcsetmodetseng (int Mode,int hardcheck);
int atixl (void);
int hcsetmodeatixl(int Mode,int hardcheck);
int vesagetinfo(void);
int vesagetmodeinfo(int nmode);
int vesabytesperline(int mode);
int vesacard (void);
int vesasetmode(int nmode);
int hcsetmodevesa(int Mode,int hardcheck);
int hcgetcard(void);
int hcsetmode(int mode,int hardcheck);
int hcmodesize(int picwidth,int picheight);
char *hccardtype(int card);
char *hcmodetype(int card,int mode);
WORD hcgetcrtmode();
void hctextmode (void);
void hcrestoretxtmode(void);
void hcputpoint (WORD x, WORD y, WORD color);
void hcputpointxor (WORD x, WORD y, WORD color);
void hcputpointrgb (WORD x, WORD y, WORD r, WORD g, WORD b);
WORD rgb265int (WORD r, WORD g, WORD b);
WORD rgbint (WORD r, WORD g, WORD b);
void intrgb256 (WORD color, WORD *r, WORD *g, WORD *b);
void intrgb (WORD color, WORD *r, WORD *g, WORD *b);
WORD hcgetpoint (WORD x, WORD y);
void hcgetpointrgb (WORD x, WORD y, WORD *r, WORD *g, WORD *b);
WORD hcgetmaxx (void);
WORD hcgetmaxy (void);
WORD hcgetmaxcolor (void);
void hchline (WORD x, WORD y, WORD x1, register WORD color);
void hcvline (WORD x, WORD y, WORD y1, register WORD color);
void hchlinexor (WORD x, WORD y, WORD x1, register WORD color);
void hcvlinexor (WORD x, WORD y, WORD y1, register WORD color);
void hcrectangle (WORD x, WORD y, WORD x1, WORD y1, WORD color);
void hcrectanglexor (WORD x, WORD y, WORD x1, WORD y1, WORD color);
void hcline(int a1, int b1, int a2, int b2, WORD lcolor);
void hclinexor(int a1, int b1, int a2, int b2, WORD lcolor);
void hccircle(WORD xc,WORD yc,WORD rad,WORD color);
void hcfillcircle(WORD xc,WORD yc,WORD rad,WORD color);
void hcbar (WORD x, WORD y, WORD x1, WORD y1, register WORD color);
void hcclrscr (WORD color);
void hcputrow (WORD x, WORD y, register WORD w, register WORD *buf);
void hcputrowxor (WORD x, WORD y, register WORD w, register WORD *buf);
void hcputrowor (WORD x, WORD y, register WORD w, register WORD *buf);
void hcgetrow (WORD x, WORD y, register WORD w, register WORD *buf);
DWORD hcgetimagesize (register WORD left, register WORD top, register WORD right,register WORD bottom);
void hcgetimage (WORD left, WORD top, WORD right, WORD bottom, WORD *bitmap);
void hcputimage (WORD left, WORD top, WORD *bitmap, WORD drawtype);
void hcputchr (unsigned char ch, WORD xpos, WORD ypos, WORD fg, WORD background,WORD drawtype);
void hcputstr (char *str, WORD xpos, WORD ypos, WORD fg, WORD background,WORD drawtype);
void hcfill(int x,int y,int ncolor);
void hcpfill(int x,int y,int bordcol,WORD *pat,int pw,int ph);
WORD *hcimagetopattern(WORD *bitmap,WORD *w,WORD *h);

/* MOUSE.C */

#define GRCURSORSIZEX	11	/* Max. graphics cursor width in pixels */
#define GRCURSORSIZEY	15	/* Max. cursor height in pixels + 1 */
#define GRCURSORNUM	10	/* Max number of default and definable cursors */

#define ARROWCURSOR		0	/* Graphics cursor types */
#define SMALLARROWCURSOR	1
#define CROSSHAIR		2
#define SMALLCROSSHAIR		3
#define DIAGCROSSHAIR		4

typedef int	grcursortype[GRCURSORSIZEX][GRCURSORSIZEY];
typedef char	mousestr[80];

	   /* Mouse driver control */

int  initmouse(void);				/* Init. mouse driver */
void showmouse(void);				/* Show (text) cursor */
void hidemouse(void);				/* Unshow (text) cursor */
void getmouse(int *x,int *y,int *buttons);/* Get position */
void putmouse(int x,int y);			/* Place mouse */
void getmousebuttonon(int button,int *buttonstate,
		      int *numpresses,int *x,int *y);/* Get buttons pressed */
void getmousebuttonoff(int button,int *buttonstate,
		       int *numreleases,int  *x,int *y);/* Get buttons released */
void setmouserange(int x,int y,int x1,int y1);	/* Set range */
void getmousemotion(int *x,int *y);		/* Get movement */
void setmousemove(int x,int y);			/* Set sensitivity */

       /* Mouse cursor routines for BGI Graphics */

void setgrcursor(int index,grcursortype newgrcursor);/* Set cursor bitmap */
void putgrcursor(int x,int y);		/* Place graphics cursor */
void unputgrcursor(void);		/* Remove graphics cursor */
int getgrcursorx(void);                 /* Get current postn */
int getgrcursory(void);
int initgrcursor(int grcursorindex,int color);/* Init. graphics cursor */
void closegrcursor(void);

#endif