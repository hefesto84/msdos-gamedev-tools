/*

 MOUSE.H

 Header file for Microsoft Mouse routines for Turbo C/C++

 Michael Chen	7/26/1992

*/


#ifndef __MOUSE_H
#define __MOUSE_H


/****************************************************************************

 Include files

****************************************************************************/

#include <dos.h>
#include <alloc.h>


/****************************************************************************

 Defines, bit masks and flags

****************************************************************************/

#define MOUSE_INTR	0x33	/* interrupt number for MS mouse driver    */

/* Bit masks for button state */

#define S_LEFT_BUTTON	1	/* bit 0 = left button status              */
#define S_RIGHT_BUTTON	2	/* bit 1 = right button status             */
#define S_MIDDLE_BUTTON	4	/* bit 2 = middle button status            */
#define S_ALL_BUTTONS	7

/* Button IDs for check_down and check_up */

#define LEFT_BUTTON		0
#define RIGHT_BUTTON		1
#define MIDDLE_BUTTON		2

/* Bit masks for Get/SetMouseHandler */

#define H_MOVED		1
#define H_LEFT_DOWN	2
#define H_LEFT_UP	4
#define H_RIGHT_DOWN	8
#define H_RIGHT_UP	16
#define H_MIDDLE_DOWN	32
#define H_MIDDLE_UP	64

/* Bit masks for Get/SetAlternateMouseHandler */

#define A_MOVED		128
#define A_LEFT_DOWN	64
#define A_LEFT_UP	32
#define A_RIGHT_DOWN	16
#define A_RIGHT_UP	8
#define A_SHIFT		4
#define A_CTRL		2
#define A_ALT		1


/****************************************************************************

 Typedefs and related info

****************************************************************************/

typedef struct
{
  char HotX;		/* X value of hotspot                    */
  char HotY;		/* Y value of hotspot                    */
  int ScreenMask[16];	/* screen mask (int per line, LSB right) */
  int CursorMask[16];	/* cursor mask (same)                    */
} MouseCursor;

typedef struct
{
  char Width;		/* width (in words) of the masks       */
  char Height;		/* height of masks                     */
  char HotX;		/* hotspot X value                     */
  char HotY;		/* hotspot Y value                     */
  void far* Data;	/* pointer to screen and cursor masks  */
} LargeMouseCursor;

typedef void far (*MouseHandler)();

/*

Structure for large cursor data

0	char Width;	width (in words) of the masks
1	char Height;	height of the masks
2	char HotX;	hotspot X value
3	char HotY;	hotspot Y value
4	void far* Data;	far pointer to data of the following form:

Data	int ScreenMask	screen mask (can this be in color?)
	 [Width*Height];

	int CursorMask	cursor mask (same) (masks must be contiguous)
	 [Width*Height];
*/


/****************************************************************************

 External variables

****************************************************************************/

extern int
	 DriverPresent;		/* not present until initialized           */
extern int
	 NumberOfButtons;	/* number of buttons mouse has             */
extern int
	 MouseX;		/* x position of mouse                     */
extern int
	 MouseY;		/* y position of mouse                     */
extern int
	 MouseButton;		/* button state of mouse                   */
extern int
	 WhichButton;		/* which button the down/up info refers to */
extern int
	 MousePressed;		/* number of presses since last call       */
extern int
	 MouseWhereX;		/* x where pressed                         */
extern int
	 MouseWhereY;		/* y where pressed                         */
extern int
	 MouseMovedX;		/* x since last move                       */
extern int
	 MouseMovedY;		/* y since last move                       */
extern MouseHandler
	 LastMouseHandler;	/* result of GetMouseHandler		   */
extern int
	 MouseXSpeed;		/* horizontal mouse speed setting	   */
extern int
	 MouseYSpeed;		/* vertical mouse speed setting		   */
extern int
	 MouseDoubleThreshold;	/* double speed threshold setting	   */


/****************************************************************************

 Function prototypes

****************************************************************************/

extern int
	 InitMouseDriver(void);			/* initialize mouse driver */
extern void
	 ShowMouseCursor(void);                 /* show mouse cursor       */
extern void                                     
	 HideMouseCursor(void);                 /* hide mouse cursor       */
extern int
	 GetMouseStatus(void);                  /* get mouse status        */
extern void
	 SetMousePosition(int x, int y);        /* set mouse position      */
extern int
	 GetMouseDown(int b);                   /* check button press      */
extern int
	 GetMouseUp(int b);                     /* check button release    */
extern void
	 SetMouseXRange(int l, int h);          /* set horizontal range    */
extern void
	 SetMouseYRange(int l, int h);          /* set vertical range      */
extern void
	 SetMouseCursor(MouseCursor mc);        /* set mouse cursor        */
extern int
	 GetMouseMove(void);                    /* check mouse movement    */
extern void
	 SetMouseHandler
	   (MouseHandler mh, int mask);		/* set mouse handler       */
extern void
	 SetLightPenEmulation(int flag);        /* set light pen emulation */
extern void
	 SetMickeyRatio(int x,int y);           /* set "mickey" ratio      */
extern void
	 SetUpdateRegion
	   (int x0, int y0, int x1, int y1);    /* set update region       */
extern int
	 SetLargeMouseCursor
	   (LargeMouseCursor mc);               /* set large mouse cursor  */
extern void
	 ExchangeMouseHandler
	   (MouseHandler mh, int mask);         /* exchange mouse handler  */
extern int
	 GetDriverStateSize(void);              /* get driver state size   */
extern void far*
	 GetDriverState(void);                  /* get driver state        */
extern void
	 SetDriverState(void far* fp);		/* set driver state        */
extern int
	 SetAltMouseHandler
	   (MouseHandler mh, int mask);         /* set handler, alternate  */
extern MouseHandler
	 GetAltMouseHandler(int mask);          /* get handler, alternate  */
extern void
	 SetMouseSensitivity
	   (int xs, int ys, int dt);            /* set mouse sensitivity   */
extern void
	 GetMouseSensitivity(void);             /* get mouse sensitivity   */
extern void
	 SetMouseDisplayPage(int page);         /* set display page        */
extern int
	 GetMouseDisplayPage(void);             /* get display page        */
extern int
	 DisableMouseDriver(void);              /* disable mouse driver    */
extern int
	 EnableMouseDriver(void);               /* enable mouse driver     */
extern int
	 ResetMouse(void);                      /* reset mouse             */
extern int
	 StartMouse(void);			/* start using mouse       */
extern void
	 EndMouse(void);			/* stop using mouse        */


#endif
