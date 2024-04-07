/***************************************************************************
*                               MouseLib.c                                 *
***************************************************************************/
#include <dos.h>
#include <stdio.h>
#include <graphics.h>
#include "mouselib.h"

unsigned interceptX, interceptY ;
void far (*interrupt OldExitProc)(void);

enum boolean  mouse_present    ;
enum mouseType  mouse_buttons  ;
unsigned  eventX,eventY,eventButtons ; /* any event handler should update */
enum boolean eventhappened  ;        /* these vars to use getLastEvent   */
unsigned XMotions, YMotions ;        /*          per 8 pixels            */

static unsigned data ;            /* data segment (DS) */

struct box HideBox ;             /* Do not change field order !!! */
struct REGPACK reg ;             /* general registers used */
static struct SREGS sreg ;              /* segment registers */

int grMode,grDrv   ;             /* detect graphic mode if any */
int grCode         ;             /* return initgraph code in here */
u_int LastMask = 0 ;
enum boolean mouseGraph = FALSE; /* assume text mode upon entry */

signed int mouseCursorLevel ;        /* if > 0 mouse cursor is visiable,
                                        otherwise not, containes the level
                                        of showMouseCursor/hideMouseCursor */

void far interrupt (*lastHandler)(void) ; /* when changing the interrupt
                                             handler temporarily, save BEFORE
                                             the change these to variables,
                                             and restore them when neccessary */

struct grCursorType lastCursor  ; /* when changing graphic cursor temporarily,
                                     save these values BEFORE the change, and
                                     restore when neccessary */


/***************************************************************************/
/*                mouseLib - C version  -      Release 5                   */
/*                                                                         */
/* because of quirks in hercules graphic mode that is not detectable       */
/*  by the mouse driver we have to know when we initMouse if we want       */
/*  to check for graphic mode or not, if we do we must perform a           */
/*  setMouseGraph before initGraph, to initGraph in text mode we must      */
/*  resetMouseGraph before.. , if these calling conventions are not        */
/*  taken we might have problems in hercules cards!                        */
/*                                                                         */
/* each call to hideMouseCursor must be balanced by a matching call        */
/*  to showMouseCursor, 2 calls to hideMou.. and only 1 to showM..         */
/*  will not show the mouse cursor on the screen!                          */
/***************************************************************************/

static u_int WatchData[] = {
    0xE007,0xC003,0x8001,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x8001,
    0xC003,0xE007,0x0,0x1FF8,0x318C,0x6186,0x4012,0x4022,0x4042,0x718C,
    0x718C,0x4062,0x4032,0x4002,0x6186,0x318C,0x1FF8,0x0
   };

static u_int newWatchCursor[] = {
    0xffff, 0xc003, 0x8001, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x8001, 0xc003, 0xffff, 0x0, 0x0,
    0x1ff8, 0x2004, 0x4992, 0x4022, 0x4042, 0x518a, 0x4782,
    0x4002, 0x4992, 0x4002, 0x2004, 0x1ff8, 0x0, 0x0
   };

static u_int ArrowData[] = {
    0xFFFF,0x8FFF,0x8FFF,0x87FF,0x83FF,0x81FF,0x80FF,0x807F,0x803F,
    0x801F,0x800F,0x801F,0x807F,0x887F,0xDC3F,0xFC3F,
    0x0,0x0,0x2000,0x3000,0x3800,0x3C00,0x3E00,0x3F00,0x3F80,0x3FC0,
    0x3FE0,0x3E00,0x3300,0x2300,0x0180,0x0180
   };

static u_int UpArrowCursor[] = {
    0xf9ff,0xf0ff,0xe07f,0xe07f,0xc03f,0xc03f,0x801f,0x801f,
    0xf,0xf,0xf0ff,0xf0ff,0xf0ff,0xf0ff,0xf0ff,0xf0ff,
    0x0,0x600,0xf00,0xf00,0x1f80,0x1f80,0x3fc0,0x3fc0,
    0x7fe0,0x600, 0x600, 0x600, 0x600, 0x600, 0x600, 0x600
   };

static u_int LeftArrowCursor[] = {
    0xfe1f,0xf01f,0x0,   0x0,   0x0,   0xf01f,0xfe1f,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0x0,   0xc0,  0x7c0, 0x7ffe,0x7c0, 0xc0,  0x0,   0x0,
    0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0
   };

static u_int CheckMarkCursor[] = {
    0xfff0,0xffe0,0xffc0,0xff81,0xff03,0x607, 0xf,   0x1f,
    0xc03f,0xf07f,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0x0,   0x6,   0xc,   0x18,  0x30,  0x60,  0x70c0,0x1d80,
    0x700, 0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0
   };

static u_int PointingHandCursor[] = {
    0xe1ff,0xe1ff,0xe1ff,0xe1ff,0xe1ff,0xe000,0xe000,0xe000,
    0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,
    0x1e00,0x1200,0x1200,0x1200,0x1200,0x13ff,0x1249,0x1249,
    0xf249,0x9001,0x9001,0x9001,0x8001,0x8001,0x8001,0xffff
   };

static u_int DiagonalCrossCursor[] = {
    0x7e0, 0x180, 0x0,   0xc003,0xf00f,0xc003,0x0,   0x180,
    0x7e0, 0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0x0,   0x700e,0x1c38,0x660, 0x3c0, 0x660, 0x1c38,0x700e,
    0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0
   };

static u_int RectangularCrossCursor[] = {
    0xfc3f,0xfc3f,0xfc3f,0x0,0x0,   0x0,   0xfc3f,0xfc3f,
    0xfc3f,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0x0,   0x180, 0x180, 0x180, 0x7ffe,0x180, 0x180, 0x180,
    0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0
   };

static u_int HourGlassCursor[] = {
    0x0,   0x0,   0x0,   0x0,   0x8001,0xc003,0xe007,0xf00f,
    0xe007,0xc003,0x8001,0x0,   0x0,   0x0,   0x0,   0xffff,
    0x0,   0x7ffe,0x6006,0x300c,0x1818,0xc30, 0x660, 0x3c0,
    0x660, 0xc30, 0x1998,0x33cc,0x67e6,0x7ffe,0x0,   0x0
   };


/***************************************************************************
*                               callMouse();                               *
*                                                                          *
* used to call mouse interrupt with global data reg - used as parameters   *
***************************************************************************/
void callMouse(void)
{
        intr(MOUSEINT,&reg);

}; /* callMouse(); */

/******************************************************************************
*                                  initMouse                                  *
* For some reason grCode is assigned a value of -11,(0xFFF5) in the second time*
*  we call initmouse after we allready are in graphics mode, override.. was   *
*  born because of that situation.                                            *
******************************************************************************/
void initMouse (void)
{
    enum boolean overRideDriver ; /*  TRUE if we over-ridden stupid driver
                                      hercules bug  */
    overRideDriver = FALSE;

    if (mouseGraph && (peek (0,0x449) == 7)) { /*  assume no mda - hercules  */
      poke (0,0x449,6);
      overRideDriver = TRUE;
    };
 /* trick stupid mouse driver to know we are in graphic mode */
    reg.r_ax = 0;          /* detect genius mouse */
    reg.r_bx = 0;          /* be sure what mode we get */
    callMouse();
    mouse_present = (reg.r_ax != 0) ? TRUE : FALSE ; /* not an iret.. */
    if ((reg.r_bx & 2) != 0)
        mouse_buttons = twoButton;
    else if ((reg.r_bx & 3) != 0)
        mouse_buttons = threeButton;
    else mouse_buttons = another; /* unknown to us */

    if (overRideDriver) poke (0,0x449,7) ;

 /* restore the stupid situation */
    eventX = eventY = eventButtons = 0;
    eventhappened = FALSE;
    XMotions = 8;
    YMotions = 16;
    mouseCursorLevel = 0; /*  not visiable, one show to appear  */
}; /* initMouse */

/***************************************************************************
*                                doMouse                                   *
*  First function one should call, before even thinking of using the mouse *
****************************************************************************/
void doMouse (void)
{
   segread(&sreg) ;
   data = sreg.ds ;              /* data is a global variable */

   eventX = eventY = 0;

   eventhappened = FALSE;        /* initialize ... */
	initMouse();                  /*detect in global variables*/
	setArrowCursor();             /*start like that in graphic mode*/
/*	OldExitProc = ExitProc;
	ExitProc    = MyExitProc; */
} /*doMouse*/


/***************************************************************************
*                            showMouseCursor                               *
****************************************************************************/
void showMouseCursor (void)
{
    reg.r_ax = 1; /* enable cursor display */
    callMouse();
    mouseCursorLevel++;
}; /* showMouseCursor */

/***************************************************************************
*                            hideMouseCursor                               *
****************************************************************************/
void hideMouseCursor (void)
{
    reg.r_ax = 2; /* disable cursor display */
    callMouse();
    mouseCursorLevel--;
}; /* hideMouseCursor */

/***************************************************************************
*                               getMouseX                                  *
****************************************************************************/
unsigned getMouseX  (void)
{
    reg.r_ax = 3;
    callMouse();
    return reg.r_cx;
}; /* getMouseX */

/***************************************************************************
*                               getMouseY                                  *
****************************************************************************/
unsigned getMouseY  (void)
{
    reg.r_ax = 3;
    callMouse();
    return reg.r_dx;
}; /* getMouseX */

/***************************************************************************
*                               getButton                                  *
****************************************************************************/
enum buttonState getButton(char Button)
{
    reg.r_ax = 3;
    callMouse();
    if ((reg.r_bx & Button) != 0)
        return buttonDown;
        /* bit 0 = left, 1 = right, 2 = middle */
    else return buttonUp;
}; /* getButton */

/***************************************************************************
*                             buttonPressed                                *
****************************************************************************/
enum boolean buttonPressed (void)
{
    reg.r_ax = 3;
    callMouse();
    if ((reg.r_bx & 7) != 0)
        return TRUE;
    else return FALSE;
}; /* buttonPressed */

/***************************************************************************
*                             setMouseCursor                               *
****************************************************************************/
void setMouseCursor(unsigned x, unsigned y)

{
   reg.r_ax = 4;
   reg.r_cx = x;
   reg.r_dx = y; /* prepare parameters */
   callMouse();
}; /* setMouseCursor */

/***************************************************************************
*                               lastXPress                                 *
****************************************************************************/
unsigned lastXPress(char Button)
{
    reg.r_ax = 5;
    reg.r_bx = Button;
    callMouse();
    return reg.r_cx;
}; /* lastXpress */

/***************************************************************************
*                               lastYPress                                 *
****************************************************************************/
unsigned lastYPress(char Button)
{
    reg.r_ax = 5;
    reg.r_bx = Button;
    callMouse();
    return reg.r_dx;
}; /* lastYpress */

/***************************************************************************
*                             buttonPresses                                *
****************************************************************************/
unsigned buttonPresses(char Button) /* from last check */
{
    reg.r_ax = 5;
    reg.r_bx = Button;
    callMouse();
    return reg.r_bx;
}; /* buttonPresses */

/***************************************************************************
*                              lastXRelease                                *
****************************************************************************/
unsigned lastXRelease(char Button)
{
    reg.r_ax = 6;
    reg.r_bx = Button;
    callMouse();
    return reg.r_cx;
}; /* lastXRelease */

/***************************************************************************
*                              lastYRelease                                *
****************************************************************************/
unsigned lastYRelease(char Button)

{
    reg.r_ax = 6;
    reg.r_bx = Button;
    callMouse();
    return reg.r_dx;
}; /* lastYRelease */

/***************************************************************************
*                             buttonReleases                               *
****************************************************************************/
unsigned buttonReleases(char Button) /* from last check */
{
    reg.r_ax = 6;
    reg.r_bx = Button;
    callMouse();
    return reg.r_bx;
}; /* buttonReleases */

/***************************************************************************
*                                  swap                                    *
****************************************************************************/
void swap(unsigned *a, unsigned *b)
{
    unsigned c;
     c = *a;
    *a = *b;
    *b =  c;  /* swap a and b */
}; /* swap */

/***************************************************************************
*                                mouseBox                                  *
****************************************************************************/
void mouseBox(unsigned left, unsigned top, unsigned right, unsigned bottom)

{
    if (left > right)
       swap(&left,&right);
    if (top > bottom)
       swap(&top,&bottom); /* make sure they are ordered */
    reg.r_ax = 7;
    reg.r_cx = left;
    reg.r_dx = right;
    callMouse(); /* set x range */
    reg.r_ax = 8;
    reg.r_cx = top;
    reg.r_dx = bottom;
    callMouse(); /* set y range */
}; /* mouseBox */

/***************************************************************************
*                          graphicMouseCursor                              *
****************************************************************************/
void graphicMouseCursor(char xHotPoint,char yHotPoint,unsigned *dataOfs)

/* define 16*16 cursor mask and screen mask, pointed by data,
    dataOfs is pointer to data of the masks. */

{
    reg.r_ax = 9;
    reg.r_bx = xHotPoint;
    reg.r_cx = yHotPoint;
    reg.r_dx = FP_OFF(dataOfs);    /* DS:DX point to masks */
    reg.r_es = FP_SEG(dataOfs);
    callMouse();
    lastCursor.xH = xHotPoint;
    lastCursor.yH = yHotPoint;
    lastCursor.data = dataOfs;
    /* save it in lastCursor, if someone needs to change cursor temporary */
}; /* graphicMouseCursor */

/***************************************************************************
*                           HardwareTextCursor                             *
****************************************************************************/
void HardwareTextCursor(char fromLine,char toLine)

/* set text cursor to text, using the scan lines from..to,
    same as intr 10 cursor set in bios :
    color scan lines 0..7, monochrome 0..13  */
{
    reg.r_ax = 10;
    reg.r_bx = 1; /* hardware text */
    reg.r_cx = fromLine;
    reg.r_dx = toLine;
    callMouse();
}; /* hardwareTextCursor */

/***************************************************************************
*                           softwareTextCursor                             *
****************************************************************************/
void softwareTextCursor(unsigned screenMask,unsigned cursorMask)

/*  when in this mode the cursor will be achived by ANDing the screen word
    with the screen mask (Attr,Char in high,low order) and
    XORing the cursor mask, ussually used by putting the screen attr
    we want preserved in screen mask (and 0 into screen mask character
    byte), and character + attributes we want to set into cursor mask */

{
    reg.r_ax = 10;
    reg.r_bx = 0;    /* software cursor */
    reg.r_cx = screenMask;
    reg.r_dx = cursorMask;
    callMouse();
}; /* softwareMouseCursor */

/***************************************************************************
*                            recentXmovement                               *
****************************************************************************/
enum direction recentXmovement (void)
/* from recent call to which direction did we move ? */

{
 int  d ;

    reg.r_ax = 11;
    callMouse();
    d = reg.r_cx;
    if (d > 0)
        return moveRight;
    else if (d < 0)
        return moveLeft;
    else return noMove;
}; /* recentXmovement */

/***************************************************************************
*                            recentYmovement                               *
****************************************************************************/
enum direction recentYmovement(void)
/* from recent call to which direction did we move ? */
{
   int d;
    reg.r_ax = 11;
    callMouse();
    d = reg.r_dx;
    if (d > 0)
        return moveDown;
    else if (d < 0)
        return moveUp;
    else return noMove;
}; /* recentYmovement */

/***************************************************************************
*                             setWatchCursor                               *
****************************************************************************/
void setWatchCursor (void)
{
    graphicMouseCursor(0, 0, WatchData);
}; /* setWatchCursor */

/***************************************************************************
*                             setNewWatchCursor                            *
****************************************************************************/
void setNewWatchCursor (void)
{
    graphicMouseCursor(0, 0, newWatchCursor);
}; /* setNewWatchCursor */
/***************************************************************************
*                            setUpArrowCursor                              *
****************************************************************************/
void setUpArrowCursor (void)
{
    graphicMouseCursor(5, 0, UpArrowCursor);
}; /* setUpArrowCursor */

/***************************************************************************
*                           setLeftArrowCursor                             *
****************************************************************************/
void setLeftArrowCursor (void)
{
    graphicMouseCursor(0, 3, LeftArrowCursor);
}; /* setLeftArrowCursor */

/***************************************************************************
*                           setCheckMarkCursor                             *
****************************************************************************/
void setCheckMarkCursor (void)
{
    graphicMouseCursor(6, 7, CheckMarkCursor);
}; /* setCheckMarkCursor */

/***************************************************************************
*                         setPointingHandCursor                            *
****************************************************************************/
void setPointingHandCursor (void)
{
    graphicMouseCursor(5, 0, PointingHandCursor);
}; /* setPointingHandCursor */

/***************************************************************************
*                         setDiagonalCrossCursor                           *
****************************************************************************/
void setDiagonalCrossCursor (void)
{
    graphicMouseCursor(7, 4, DiagonalCrossCursor);
}; /* setDiagonalCrossCursor */

/***************************************************************************
*                        setRectangularCrossCursor                          *
****************************************************************************/
void setRectangularCrossCursor (void)
{
    graphicMouseCursor(7, 4, RectangularCrossCursor);
}; /* setRectangularCrossCursor */

/***************************************************************************
*                           setHourGlassCursor                             *
****************************************************************************/
void setHourGlassCursor (void)
{
    graphicMouseCursor(7, 7, HourGlassCursor);
}; /* setHourGlassCursor */

/***************************************************************************
*                             setArrowCursor                               *
****************************************************************************/
void setArrowCursor (void)
{
    graphicMouseCursor(1, 1, ArrowData);
}; /* setArrowCursor */

/***************************************************************************
*                            setEventHandler                               *
****************************************************************************/
void setEventHandler (unsigned mask ,void interrupt (*handler)())
/* handler must be a far interrupt routine  */

{
    reg.r_ax = 12; /* set event handler function in mouse driver */
    reg.r_cx = mask;
    reg.r_es = FP_SEG(handler);
    reg.r_dx = FP_OFF(handler);
    callMouse();
    LastMask = mask;
    lastHandler = handler;
}; /* set event Handler */


/******************************************************************************
*                               defaultHandler                               *
******************************************************************************/
void far interrupt defaultHandler (void)
{
   asm   push ds;          /* save TP mouse driver */
   asm   mov ax, SEG _data;
   asm   mov ds, ax;       /* ds = TP:ds, not the driver's ds */
   asm   mov eventX, cx;   /* where in the x region did it occur */
   asm   mov eventY, dx;
   asm   mov eventButtons, bx;
   asm   mov eventhappened, 1; /* eventhapppened = TRUE */
   asm   pop ds;               /* restore driver's ds */
   asm   ret;

/*   this is the default event handler , it simulates :

      {
	       eventX = cx;
	       eventY = dx;
	       eventButtons = bx;
	       eventhappened = True;
      };
*/
}


/***************************************************************************
*                              GetLastEvent                                *
****************************************************************************/
enum boolean GetLastEvent(unsigned *x, unsigned *y, enum buttonState *left_button,
             enum buttonState *right_button, enum buttonState *middle_button)
{
  enum boolean event = eventhappened ;

    eventhappened = FALSE; /* clear to next read/event */
    *x = eventX;
    *y = eventY;
    if ((eventButtons && LEFTBUTTON) != 0)
        *left_button = buttonDown;
    else *left_button = buttonUp;

    if ((eventButtons && RIGHTBUTTON) != 0)
        *right_button = buttonDown;
    else *right_button = buttonUp;
    if ((eventButtons && MIDDLEBUTTON) != 0)
        *middle_button = buttonDown;
    else *middle_button = buttonUp;

    return event ;
}; /* getLastEvent */

/***************************************************************************
*                            setDefaultHandler                              *
****************************************************************************/
void setDefaultHandler (unsigned mask)

/* get only event mask, and set event handler to defaultHandler */

{
    setEventHandler(mask, defaultHandler);
}; /* setDefaultHandler */

/******************************************************************************
*                           enableLightPenEmulation                           *
******************************************************************************/
void enableLightPenEmulation (void)

{
    reg.r_ax = 13;
    callMouse();
}; /* enableLightPenEmulation */

/***************************************************************************
*                        disableLightPenEmulation                          *
****************************************************************************/
void disableLightPenEmulation (void)

{
    reg.r_ax = 14;
    callMouse();
};  /* disableLightPenEmulation */

/***************************************************************************
*                           defineSensetivity                              *
****************************************************************************/
void defineSensetivity(unsigned x, unsigned y)

{
    reg.r_ax = 15;
    reg.r_cx = x; /* # of mouse motions to horizontal 8 pixels */
    reg.r_dx = y; /* # of mouse motions to vertical 8 pixels */
    callMouse();
    XMotions = x;
    YMotions = y; /* update global unit variables */
}; /* defineSensetivity */

/***************************************************************************
*                            setHideCursorBox                              *
****************************************************************************/
void setHideCursorBox(unsigned left, unsigned top, unsigned right, unsigned bottom)
{
    reg.r_ax = 16;
    reg.r_es = FP_SEG(&HideBox);
    reg.r_dx = FP_OFF(&HideBox);
    HideBox.left = left;
    HideBox.right = right;
    HideBox.top = top;
    HideBox.bottom = bottom;
    callMouse();
}; /* setHideCursorBox */

/***************************************************************************
*                       defineDoubleSpeedTreshHold                         *
****************************************************************************/
void defineDoubleSpeedTreshHold(unsigned treshHold)
{
    reg.r_ax = 17;
    reg.r_dx = treshHold;
    callMouse();
}; /* defineDoubleSpeedTreshHold - from what speed to double mouse movement */

/***************************************************************************
*                            disableTreshHold                              *
****************************************************************************/
void disableTreshHold (void)
{
    defineDoubleSpeedTreshHold(0x7FFF);
}; /* disableTreshHold */

/***************************************************************************
*                            defaultTreshHold                              *
****************************************************************************/
void defaultTreshHold (void)
{
    defineDoubleSpeedTreshHold(64);
}; /* defaultTreshHold */

/***************************************************************************
*                             setMouseGraph                                *
****************************************************************************/
void setMouseGraph (void)
{
    mouseGraph = TRUE;
}; /* setMouseGraph */

/***************************************************************************
*                            resetMouseGraph                               *
****************************************************************************/
void resetMouseGraph (void)
{
    mouseGraph = FALSE;
}; /* resetMouseGraph */


/***************************************************************************
*                            waitForRelease                                 *
* Wait until button is release, or timeOut 1/100 seconds pass. (might miss a*
* tenth (1/10) of a second.                                                 *
*****************************************************************************/
void waitForRelease (unsigned timeOut)
{
    struct time st, ct;
    long int stopSec, currentSec, Delta;

    gettime(&st);
	stopSec = ((long)st.ti_hour*36000L + (long)st.ti_min*600 +
			   (long)st.ti_sec*10 + (long)st.ti_hund + timeOut) % (24*360000L);
    do {
       gettime(&ct);
	   currentSec = (long)ct.ti_hour*36000L + (long)ct.ti_min*600 +
                    (long)ct.ti_sec*10 + (long)ct.ti_hund;
       Delta = currentSec - stopSec;
    }
	while (buttonPressed() && Delta > 36000L);
}; /* waitForRelease */


/****************************************************************************
*                              swapEventHandler                             *
* handler is a far routine.                                                 *
****************************************************************************/
void swapEventHandler (unsigned mask ,void far interrupt (*handler)())
{
   reg.r_ax = 0x14;
   reg.r_cx = mask;
	reg.r_es = FP_SEG(handler);
	reg.r_dx = FP_OFF(handler);
	callMouse ();
   LastMask = reg.r_cx;
   lastHandler = MK_FP(reg.r_es, reg.r_dx);
}; /*swapEventHandler*/

/****************************************************************************
*                            getMouseSaveStateSize                          *
****************************************************************************/
int getMouseSaveStateSize (void)
{
   reg.r_ax = 0x15;
   callMouse ();
   return (reg.r_bx);
}; /*getMouseSaveStateSize*/

/****************************************************************************
*                               interceptMouse                              *
****************************************************************************/
void interceptMouse (void)
{
   reg.r_ax = 3;
   callMouse (); /* get place .. */
   interceptX = reg.r_cx;
   interceptY = reg.r_dx;
   reg.r_ax = 31;
   callMouse ();
}; /*interceptMouse*/

/****************************************************************************
*                                restoreMouse                               *
****************************************************************************/
void restoreMouse (void)
{
   reg.r_ax = 32; /* restore mouse driver .. */
   callMouse ();
   reg.r_ax = 4;
   reg.r_cx = interceptX;
   reg.r_dx = interceptY;
   callMouse ();
}; /*restoreMouse*/


/****************************************************************************
*                                 MyExitProc                               *
****************************************************************************/
void MyExitProc (void)
{
/*    ExitProc = OldExitProc;    ???????????????? */
    resetMouseGraph();
    initMouse();
}; /* myExitProc */

