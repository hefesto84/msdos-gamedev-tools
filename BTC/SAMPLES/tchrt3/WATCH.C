/*---------------------------------------------------------------------------
 |  Program WATCH                                                           |
 |                                                                          |
 |  This program creates a stopwatch on the display with "Taylor" split     |
 |  capability.                                                             |
 |                                                                          |
 |  This program uses two timers to keep track of total and lap times.      |
 |  This method can result in a least significant digit "jitter" of .01 sec |
 |  on the lap timer.  A better, but more complex method is to use a single |
 |  timer to keep track of both.  The dual timer method makes for better    |
 |  demo code and was chosen for that reason only.                          |
 |                                                                          |
 |  NOTE: Compile with stack checking disabled.                             |
 |                                                                          |
 |  (c)1989 Ryle Design, P.O. Box 22, Mt. Pleasant, Michigan 48804          |
 |                                                                          |
 |  V3.00  Turbo C Shareware Evaluation Series                              |                                    |
 ---------------------------------------------------------------------------*/

#include <stdio.h>
#include <conio.h>
#include <dos.h>

#include "pchrt.h"

#ifndef TRUE
    #define TRUE    1
    #define FALSE   0
#endif

#define TIMEROFF    0
#define TIMERON     1
#define LAPSTOP     0
#define LAPRUN      1
#define F1          59
#define F2          60
#define KEYPORT     0x60

#define MSMINS      60000000L;
#define MSSECS      1000000L;
#define MSHUNDS     10000L;

char            tstring[9] = {"00:00.00"};
char            lstring[9] = {"00:00.00"};

int             tstate = TIMEROFF;
int             lstate = LAPSTOP;
int             laps;
unsigned long   ltime = 0;
unsigned long   ttime = 0;

pchrt_type      dtime[2];

void interrupt  (*old_keybd_int)(void);             /* pointer to old keyboard interrupt */
void interrupt  new_keybd_int(void);                /* our new keyboard interrupt        */


int hide_cursor(void)
/*--------------------------------------------------------------------------
 |  This function disables the cursor.                                     |
 |                                                                         |
 |  Globals referenced: none                                               |
 |                                                                         |
 |  Arguments : void                                                       |
 |                                                                         |
 |  Returns   : (int) cursor shape for later restoration                   |
 --------------------------------------------------------------------------*/
{
    union REGS  regs;
    int         cursortype;

    regs.h.ah = 15;                                         /* get current video page */
    int86(0x10,&regs,&regs);

    regs.h.ah = 3;                                          /* request cursor shape */
    int86(0x10,&regs,&regs);                                /* regs.bh has video page from last int86() call */

    cursortype = regs.h.cl + ( (int) regs.h.ch << 8);       /* store cursor start & stop rasters */

    regs.h.ah = 1;                                          /* set cursor shape */
    regs.h.ch = 32;                                         /* set bit 5 - turns cursor off */
    int86(0x10,&regs,&regs);                                /* and disable cursor */

    return(cursortype);

} /* hide_cursor */



void set_cursor(int cursortype)
/*---------------------------------------------------------------------------
 |  This function sets the cursor to a new shape.                           |
 |                                                                          |
 |  Globals referenced: none                                                |
 |                                                                          |
 |  Arguments : (int) cursortype - high 8 bits is start raster              |
 |                                 low 8 bits is stop raster                |
 |  Returns   : void                                                        |
 ---------------------------------------------------------------------------*/
{
    union REGS  regs;

    regs.h.ah = 1;                                          /* set cursor shape */
    regs.h.ch = (cursortype & 0xFF00) >> 8;                 /* cursor start raster */
    regs.h.cl = (cursortype & 0x00FF);                      /* cursor stop raster */
    int86(0x10,&regs,&regs);                                /* call BIOS interupt 10h */

} /* set_cursor */



void restore_old_keybd_int(void)
/*---------------------------------------------------------------------------
 |  This function restores the original keyboard interrupt and must be      |
 |  called prior to program completion.                                     |
 |                                                                          |
 |  Globals referenced: old_keybd_int                                       |
 |                                                                          |
 |  Arguments: void                                                         |
 |                                                                          |
 |  Returns  : void                                                         |
 ---------------------------------------------------------------------------*/
{
    setvect(9,old_keybd_int);                       /* restore old ISR vector */

} /* restore_old_keybd_int */



void install_new_keybd_int(void)
/*---------------------------------------------------------------------------
 |  This function saves the original keyboard interrupt vector and installs |
 |  the address of our user written interrupt handler in the ISR vector     |
 |  table.                                                                  |
 |                                                                          |
 |  Globals referenced: old_keybd_int                                       |
 |                      new_keybd_int                                       |
 |                                                                          |
 |  Arguments: void                                                         |
 |                                                                          |
 |  Returns  : void                                                         |
 ---------------------------------------------------------------------------*/
{
    old_keybd_int = getvect(9);                     /* save old ISR vector    */
    setvect(9,new_keybd_int);                       /* install new ISR vector */

} /* install_new_keybd_int */



void interrupt far new_keybd_int(void)
/*---------------------------------------------------------------------------
 |  This function is our new interrupt service routine for interrupt 9h.    |
 |  The following occurs:                                                   |
 |      1. The keyboard hardware port is read to see what key was pressed.  |
 |      2. If F1 or F2 were pressed, the watch state is checked and         |
 |         appropriate action is taken.                                     |
 |      3. The old keyboard interrupt is called.                            |
 |                                                                          |
 |  Since this function is invoked by a hardware interrupt, it functions    |
 |  asynchronously to the main program execution and provides extremely     |
 |  high timing accuracy.                                                   |
 |                                                                          |
 |  Globals referenced: tstate                                              |
 |                      lstate                                              |
 |                      ltime                                               |
 |                      ttime                                               |
 |                      laps                                                |
 |                      old_keybd_int                                       |
 |                                                                          |
 |  Arguments: void                                                         |
 |                                                                          |
 |  Returns  : void                                                         |
 ---------------------------------------------------------------------------*/
{
    char unsigned   scan_code;
    unsigned long   hits;

    scan_code = inportb(KEYPORT);                   /* read keyboard */
    if (scan_code == F1)                            /* check for F1 key */
    {
        if (tstate == TIMEROFF)                     /* if the watch is off ... */
        {
            t_entry(0);                             /* start main timer */
            t_entry(1);                             /* start lap timer  */
            tstate = TIMERON;                       /* set state flags  */
            lstate = LAPRUN;
        }
        else                                        /* the watch was on ... */
        {
            t_exit(0);                              /* stop main timer */
            t_exit(1);                              /* stop lap timer  */
            tstate = TIMEROFF;                      /* set flags       */
            lstate = LAPSTOP;
        }
    }
    else if (scan_code == F2)                       /* check for F2 key */
    {
        if (tstate == TIMEROFF)                     /* if watch was off */
        {
            t_reset(0);                             /* master reset     */
            t_reset(1);
            ttime = 0;
            ltime = 0;
            laps = 0;
        }
        else                                        /* if watch was running */
        {
            if (lstate == LAPRUN)                   /* end of lap if lap was running */
            {
                t_exit(1);
                t_ask_timer(1,&hits,&ltime);
                t_reset(1);
                t_entry(1);
                lstate = LAPSTOP;
                laps++;
            }
            else                                        /* current lap continues if lap was off */
            {
                lstate = LAPRUN;
            }
        }
    }

    (*old_keybd_int)();                             /* call the old keyboard ISR */

} /* new_keybd_int */


void make_time_string(unsigned long tval, char *tstring)
/*---------------------------------------------------------------------------
 |  This function converts a quantity of microseconds into a displayable    |
 |  string in the form MM:SS.HH .                                           |
 |                                                                          |
 |  Globals referenced: none                                                |
 |                                                                          |
 |  Arguments: (unsigned long) tval - time in microseconds to convert       |
 |             (char *) tstring[] - string to receive time conversion       |
 |                                                                          |
 |  Returns  : void                                                         |
 ---------------------------------------------------------------------------*/
{
    int     mins, secs, hunds;

    mins = tval / MSMINS;
    tval -= (long) mins * MSMINS;

    secs = tval / MSSECS;
    tval -= (long) secs * MSSECS;

    hunds = tval / MSHUNDS;

    sprintf(tstring,"%.2d:%.2d.%.2d",mins,secs,hunds);

} /* make_time_string */



void draw_watch(void)
/*---------------------------------------------------------------------------
 |  This function draws the stopwatch on the display.                       |
 |                                                                          |
 |  Globals referenced: none                                                |
 |                                                                          |
 |  Arguments: void                                                         |
 |                                                                          |
 |  Returns  : void                                                         |
 ---------------------------------------------------------------------------*/
{
    int     indx;

    gotoxy(33,9);   putch(218);                     /* draw the corners */
    gotoxy(46,9);   putch(191);
    gotoxy(33,15);  putch(192);
    gotoxy(46,15);  putch(217);

    for (indx=34; indx<46; indx++)                  /* draw horizontal lines */
    {
        gotoxy(indx,9);   putch(196);
        gotoxy(indx,15);  putch(196);
        gotoxy(indx,12);  putch(196);
    }

    for(indx=10; indx<15; indx++)                   /* draw vertical lines */
    {
        gotoxy(33,indx);  putch(179);
        gotoxy(46,indx);  putch(179);
    }

    gotoxy(33,12); putch(195);                      /* draw vert/horiz intersections */
    gotoxy(46,12); putch(180);

    gotoxy(35,10); printf("Total Time");
    gotoxy(37,13); printf("Lap 00");

} /* draw_watch */



void show_total_time(unsigned long ttime)
/*---------------------------------------------------------------------------
 |  This function displays the total time accumulated by the watch in the   |
 |  appropriate area of the watch face.                                     |
 |                                                                          |
 |  Globals referenced: none                                                |
 |                                                                          |
 |  Arguments: (unsigned long) ttime - time to display                      |
 |                                                                          |
 |  Returns  : void                                                         |
 ---------------------------------------------------------------------------*/
{
    char    tstring[9];

    make_time_string(ttime,tstring);    /* convert microseconds to MM:SS.HH */

    gotoxy(36,11);
    printf("%s",tstring);

} /* show_total_time */



void show_lap_time(unsigned long ltime)
/*---------------------------------------------------------------------------
 |  This function displays the current lap time accumulated by the watch in |
 |  the appropriate area of the watch face.                                 |
 |                                                                          |
 |  Globals referenced: none                                                |
 |                                                                          |
 |  Arguments: (unsigned long) ltime - time to display                      |
 |                                                                          |
 |  Returns  : void                                                         |
 ---------------------------------------------------------------------------*/
{
    char    tstring[9];

    make_time_string(ltime,tstring);    /* convert microseconds to MM:SS.HH */
    
    gotoxy(36,14);
    printf("%s",tstring);

} /* show_lap_time */



void show_lap(int lap)
/*---------------------------------------------------------------------------
 |  This function displays the current lap the watch is timing in the       |
 |  appropriate area of the watch face.                                     |
 |                                                                          |
 |  Globals referenced: none                                                |
 |                                                                          |
 |  Arguments: (int) lap - lap to display                                   |
 |                                                                          |
 |  Returns: void                                                           |
 ---------------------------------------------------------------------------*/
{
    if (lap == 100) lap = 0;            /* roll over after 99 laps */

    gotoxy(41,13);
    printf("%2.2d",lap);

} /* show_lap */


void main(void)
{
    int             atom, cursor;
    unsigned long   hits;

    t_start();

    /* first set up the display */

    clrscr();
    cursor = hide_cursor();

    gotoxy(27,6); printf("TCHRT Demonstration Series");
    gotoxy(32,7); printf("StopWatch V3.00");

    draw_watch();

    gotoxy(14,17); printf("F1 Starts/Stops Watch          F2 Lap Splits/Resets Watch");
    gotoxy(35,19); printf("<ESC> quits");

    show_total_time(ttime);
    show_lap_time(ltime);
    show_lap(laps);

    install_new_keybd_int();

    /* watch display update loop */
    
    do
    {
        if (tstate == TIMEROFF)
        {
            t_ask_timer(0,&hits,&ttime);
            t_ask_timer(1,&hits,&ltime);
            show_total_time(ttime);
            show_lap_time(ltime);
            show_lap(laps);
        }
        else
        {
            t_get(&dtime[0]);
            ttime = t_diff(&(tdata->tstart),&dtime[0]) + tdata->elapsed;
            show_total_time(ttime);
            if (lstate == LAPRUN)
            {
                t_get(&dtime[1]);
				ltime = t_diff(&((tdata+1)->tstart),&dtime[1]) + (tdata+1)->elapsed;
                show_lap_time(ltime);
            }
            else
            {
                show_lap_time(ltime);
                show_lap(laps);
            }
        }

        if (kbhit()) atom = getch();
    }
    while (atom != 27);

    /* if user presses <ESC>, we fall through to here.  Clean up and exit */

    restore_old_keybd_int();

    t_stop();

    set_cursor(cursor);
    gotoxy(1,24);
    printf("StopWatch complete.");

} /* main */
