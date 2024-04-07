/* TCHK 2.1 - Howard Kapustein's Turbo C library        6-6-89      */
/* Copyright (C) 1988,1989 Howard Kapustein.  All rights reserved.  */

/* demolite.c  - used for testing TCHK mouse functions */

#include <howard.h>
#include <mousehk.h>
#include <video.h>
#include <stdio.h>
#include <stdlib.h>
#include <keyboard.h>
#include <keycode.h>
#include <dos.h>

/* Global variables */
boolean ishidden, restrict = FALSE;
int left, top, right, bottom;

/* function prototypes */
void main();
void Setup(void);
void TestCoord(void);
void RestrictCoord(void);
void TestCursor(void);
void Bye(void);

void main()
{
    Setup();
    TestCoord();

    TestCursor();

    Bye();
}


void Setup(void)
{
    extern int _mouse2;
    extern boolean ishidden;    /* is Mouse cursor hidden */

    if (!ismouse()) {           /* check for mouse */
        printf("This demo requires a mouse\n");
        exit(0);
    }
    cursor_off();
    MCursorOn();                /* ismouse() sets internal mouse cursor variable to -1, we want it on */
    ishidden = FALSE;
    cls();
}


void TestCoord(void)
{
    extern int _mouse2, _mouse3, _mouse4;
    extern boolean ishidden, restrict;
    int x,y, status, key;
    char savescreen[scrbuff(1,1,80,24)];
    boolean done;

    gotohv(0,0);
    printf("Button pressed at: \n");
    printf("Mickeys from last: \n");
    gotohv(60,0);
    printf("Current: ");
    gotohv(0,3);
    printf("Press left button to end\n");
    printf("Press right button to display mouse coordinates\n");
    gotohv(0,6);
    printf("Press left/right arrow to change horizontal mickey/pixel ratio\n");
    printf("Press up/down arrow to change vertical mickey/pixel ratio\n");
    gotohv(0,9);
    printf("Press C to hide cursor\n");
    printf("Press R to restrict mouse range\n");
    printf("Press ALT-R to reset the mouse driver\n");
    printf("Press ESC to abort demo\n");

    MCursorText(FALSE,0x00FF,0x1F00);
    status = MButtonStatus();           /* clear button status */
    if (status & LBPRESSED) {
        MButtonPress(LEFTBUTTON);
        MButtonRelease(LEFTBUTTON);
    }
    if (status & RBPRESSED) {
        MButtonPress(RIGHTBUTTON);
        MButtonRelease(RIGHTBUTTON);
    }

    for (done = FALSE; !done;) {
        status = MButtonStatus();
        x = _mouse3;    y = _mouse4;
        delay(100);                     /* needed to insure cursor is displayed on screen      */
                                        /* don't believe me? comment out this function and see */
                                        /* See note below for more details                     */
        if ((key = inkeyc(FALSE)) != 0) {
            switch (key) {
                case LEFTARROW: { MGetSensitivity();
                                  _mouse2--;                    /* beware of underflow */
                                  MSetRatio(_mouse2,_mouse3);
                                  break; }
                case RIGHTARROW:{ MGetSensitivity();
                                  _mouse2++;                    /* beware of overflow */
                                  MSetRatio(_mouse2,_mouse3);
                                  break; }
                case DOWNARROW: { MGetSensitivity();
                                  _mouse3--;                    /* beware of underflow */
                                  MSetRatio(_mouse2,_mouse3);
                                  break; }
                case UPARROW:   { MGetSensitivity();
                                  _mouse3++;                    /* beware of overflow */
                                  MSetRatio(_mouse2,_mouse3);
                                  break; }
                case 'C': { ishidden = !ishidden;
                            ishidden ? MCursorOff() : MCursorOn();
                            gotohv(0,9);
                            printf(ishidden ? "Press C to show cursor" :
                                              "Press C to hide cursor");
                            break; }
                case 'R': { restrict = !restrict;
                            if (restrict) {
                                gettext(1,1,80,24,savescreen);
                                RestrictCoord();
                                puttext(1,1,80,24,savescreen);
                                gotohv(60,1);
                                printf("(%3d,%3d) (%3d,%3d)",left/8,top/8,right/8,bottom/8);
                                gotohv(0,10);
                                printf("Press R to unrestrict mouse range\n");
                            } else {
                                MCursorRangex(0,79*8);  /* each character is 8x8 pixels */
                                MCursorRangey(0,24*8);
                                gotohv(60,1);
                                printf("                   ");
                                gotohv(0,10);
                                printf("Press R to restrict mouse range  \n");
                            }
                            break; }
                case ALT_R: { MouseReset();
                              MCursorText(FALSE,0x00FF,0x1F00);
                              if (!ishidden)
                                  MCursorOn();
                              break; }
                case ESC: cursor_on();  exit(1);
            }
        }
        gotohv(69,0);
        printf("(%3d,%3d)",x/8,y/8);
        if (status & RBPRESSED) {
            MButtonPress(RIGHTBUTTON);
            gotohv(20,0);
            printf("(%4d,%4d)",_mouse3/8,_mouse4/8);
            MMickeysMovedx();
            gotohv(20,1);
            printf("(%4d,%4d)  ",_mouse3,_mouse4);
        }
        if ((status & LBPRESSED) && (MButtonRelease(LEFTBUTTON) != 0)) {
            /* left button pressed AND released */
            MButtonPress(LEFTBUTTON);   /* clear Left Button Presses */
            done = TRUE;
            break;
        }
    }
}


void RestrictCoord(void)
{
    extern int left,top,right,bottom;
    int status;

    cls();
    printf("We're about to restrict the mouse coordinates\n");
    printf("\nMove the mouse to the upper left corner of the boundary\n");
    printf("    and press the right button\n");

    if (MButtonStatus() & RBPRESSED) {  /* clear button status */
        MButtonPress(RIGHTBUTTON);
        MButtonRelease(RIGHTBUTTON);
    }
    do {
        status = MButtonStatus();
        if (status & RBPRESSED) {
            MButtonPress(RIGHTBUTTON);
            left = _mouse3;
            top = _mouse4;
        }
        if ((status & RBPRESSED) && (MButtonRelease(RIGHTBUTTON)!=0)) {
            MButtonRelease(RIGHTBUTTON);    /* clear Right Button Presses */
            break;
        }
    } while (TRUE);

    gotohv(0,5);
    printf("\nMove the mouse to the lower right corner of the boundary\n");
    printf("    and press the right button\n");

    if (MButtonStatus() & RBPRESSED) {  /* clear button status */
        MButtonPress(RIGHTBUTTON);
        MButtonRelease(RIGHTBUTTON);
    }
    do {
        status = MButtonStatus();
        if (status & RBPRESSED) {
            MButtonPress(RIGHTBUTTON);
            right = _mouse3;
            bottom = _mouse4;
        }
        if ((status & RBPRESSED) && (MButtonRelease(RIGHTBUTTON)!=0)) {
            MButtonRelease(RIGHTBUTTON);    /* clear Right Button Presses */
            break;
        }
    } while (TRUE);

    MCursorRangex(left,right);          /* restrict cursor range */
    MCursorRangey(top,bottom);

    MButtonPress(LEFTBUTTON);           /* clear internal mouse counters */
    MButtonRelease(LEFTBUTTON);
    MMickeysMovedx();
}


void TestCursor(void)
{
    extern unsigned int _Bitmap_StandardCursor[],
                        _Bitmap_UpArrow[],
                        _Bitmap_LeftArrow[],
                        _Bitmap_CheckMark[],
                        _Bitmap_PointingHand[],
                        _Bitmap_DiagonalCross[],
                        _Bitmap_RectangularCross[],
                        _Bitmap_Hourglass[];
    int savemode, status, gcursor, rightpressed=0;
    boolean done;
    unsigned int *bitmap;

    cls();
    if ((savemode = MODE) == 7) {
        printf("Sorry, but the cursor test requires a graphic (non-mono) display");
        cursor_on();
        exit(99);
    }
    set_mode(6);
    printf("Press left button to quit\n");
    printf("Press right button to change cursor\n");

    ismouse();
    MCursorOn();
    status = MButtonStatus();           /* clear button status */
    if (status & LBPRESSED) {
        MButtonPress(LEFTBUTTON);
        MButtonRelease(LEFTBUTTON);
    }
    if (status & RBPRESSED) {
        MButtonPress(RIGHTBUTTON);
        MButtonRelease(RIGHTBUTTON);
    }

    MCursorGraphic((int)_Bitmap_StandardCursor[0],
                   (int)_Bitmap_StandardCursor[1],
                   _Bitmap_StandardCursor+2);
    for (gcursor = 0, done = FALSE; !done;) {
        status = MButtonStatus();
        if ((rightpressed > 0) || (status & RBPRESSED)) {
            if (rightpressed == 0)
                rightpressed = MButtonPress(RIGHTBUTTON);
            if (MButtonRelease(RIGHTBUTTON)) {
                rightpressed = 0;
                gcursor = (++gcursor) % 8;
                switch (gcursor) {
                    case 0: bitmap = _Bitmap_StandardCursor;  break;
                    case 1: bitmap = _Bitmap_UpArrow;  break;
                    case 2: bitmap = _Bitmap_LeftArrow;  break;
                    case 3: bitmap = _Bitmap_CheckMark;  break;
                    case 4: bitmap = _Bitmap_PointingHand;  break;
                    case 5: bitmap = _Bitmap_DiagonalCross;  break;
                    case 6: bitmap = _Bitmap_RectangularCross;  break;
                    case 7: bitmap = _Bitmap_Hourglass;  break;
                }
                MCursorGraphic((int)*bitmap,(int)*(bitmap+1),bitmap+2);
            }
        }
        if ((status & LBPRESSED) && (MButtonRelease(LEFTBUTTON) != 0)) {
            /* left button pressed AND released */
            MButtonPress(LEFTBUTTON);   /* clear Left Button Presses */
            done = TRUE;
            break;
        }
    }
    set_mode(savemode);
}


void Bye(void)
{
    extern unsigned char _TCHKmajor, _TCHKminor;

    cls();
    printf("Thank you for sampling TCHK %u.%u\n",_TCHKmajor,_TCHKminor);
    exit(0);
}
