/**************************************************************************
*         MODULE NAME:        MOUSELIB                                    *
**************************************************************************/

#define    MOUSEINT               0x33  /* mouse driver interrupt */
#define    LEFTBUTTON               1   /* bit 0 */ 
#define    RIGHTBUTTON              2   /* bit 1 */  
#define    MIDDLEBUTTON             4   /* bit 2 */

#define    CURSOR_LOCATION_CHANGED  1   /* event mask bits */
#define    LEFT_BUTTON_PRESSED      2  
#define    LEFT_BUTTON_RELEASED     4  
#define    RIGHT_BUTTON_PRESSED     8
#define    RIGHT_BUTTON_RELEASED    16 
#define    MIDDLE_BUTTON_PRESSED    32 
#define    MIDDLE_BUTTON_RELEASED   64 

#define    click_repeat   10  /* Recommended value for waitForRelease timeOut */

struct box {
        unsigned left   ;
        unsigned top    ;
        unsigned right  ;
        unsigned bottom ;
    } ; /* Do not change field order !!! */

struct grCursorType {
     char xH,yH       ;               /* x,y Hot Point */
     unsigned  *data  ;         /* cursor look pointer */
 } ;

typedef unsigned int u_int;
enum mouseType   {twoButton,threeButton,another};
enum buttonState {buttonDown,buttonUp};
enum direction   {moveRight,moveLeft,moveUp,moveDown,noMove};
enum boolean     {FALSE, TRUE};

/* External variable (can be used by user) */

extern unsigned interceptX, interceptY ;
extern void far (*interrupt OldExitProc)(void);
extern enum boolean  mouse_present    ;
extern enum mouseType  mouse_buttons  ;
extern unsigned  eventX,eventY,eventButtons ; /* any event handler should update */
extern enum boolean eventhappened  ;        /* these vars to use getLastEvent   */
extern unsigned XMotions, YMotions ;        /*          per 8 pixels            */

extern unsigned data ;            /* data segment (DS) */

extern struct box HideBox ;             /* Do not change field order !!! */
extern struct REGPACK reg ;             /* general registers used */
extern struct SREGS sreg ;              /* segment registers */

extern int grMode,grDrv   ;             /* detect graphic mode if any */
extern int grCode         ;             /* return initgraph code in here */

extern signed int mouseCursorLevel ;        /* if > 0 mouse cursor is visiable,
                                        otherwise not, containes the level
                                        of showMouseCursor/hideMouseCursor */

extern void far interrupt (*lastHandler)(void) ; /* when changing the interrupt
                                             handler temporarily, save BEFORE
                                             the change these to variables,
                                             and restore them when neccessary */

extern struct grCursorType lastCursor  ; /* when changing graphic cursor temporarily,
                                     save these values BEFORE the change, and
                                     restore when neccessary */


/* ----------------- Function prototypes ----------------- */

void initMouse (void)                       ; /* when replacing mouse mode do that..! */
void doMouse(void)                          ;
void showMouseCursor (void)                 ;
void hideMouseCursor (void)                 ;
unsigned getMouseX (void)                   ;
unsigned getMouseY (void)                   ;
enum buttonState getButton(char Button)     ;
enum boolean buttonPressed (void)           ;
void setMouseCursor(unsigned x,unsigned y)  ;
unsigned LastXPress(char Button)            ;
unsigned LastYPress(char Button)            ;
unsigned ButtonPresses(char Button)         ; /* from last last check */
unsigned LastXRelease(char Button)          ;
unsigned LastYRelease(char Button)          ;
unsigned ButtonReleases(char Button)        ; /* from last last check */
void mouseBox(unsigned left,unsigned top,
              unsigned right,unsigned bottom) ; /* limit mouse rectangle */
void graphicMouseCursor(char xHotPoint,char yHotPoint, unsigned *dataOfs)  ;
void HardwareTextCursor(char fromLine,char toLine)    ;
void softwareTextCursor(unsigned screenMask,unsigned cursorMask)  ;
enum direction recentXmovement (void)       ;
enum direction recentYmovement (void)       ;
void setArrowCursor (void)                  ;
void setEventHandler (unsigned mask ,void interrupt (*handler)());
void far interrupt defaultHandler (void);
enum boolean GetLastEvent(unsigned *x, unsigned *y, enum buttonState *left_button,
             enum buttonState *right_button, enum buttonState *middle_button);
void setDefaultHandler (unsigned mask);
void setWatchCursor (void)                  ;
void setNewWatchCursor (void)               ;
void setUpArrowCursor (void)                ;
void setLeftArrowCursor (void)              ;
void setCheckMarkCursor (void)              ;
void setPointingHandCursor (void)           ;
void setDiagonalCrossCursor (void)          ;
void setRectangularCrossCursor (void)       ;
void setHourGlassCursor (void)              ;
void setDefaultHandler(unsigned mask)               ;
void enableLightPenEmulation (void)                 ;
void disableLightPenEmulation (void)                ;
void defineSensetivity(unsigned x,unsigned y)       ;
void setHideCursorBox(unsigned left,unsigned top,
                      unsigned right,unsigned bottom)  ;
void defineDoubleSpeedTreshHold(unsigned treshHold)    ;
void disableTreshHold (void)                           ;
void defaultTreshHold (void)                           ;
void setMouseGraph (void)                              ;
void resetMouseGraph (void)                            ;
void waitForRelease(unsigned timeOut)                  ;
void swapEventHandler (unsigned mask ,void far interrupt (*handler)());
int getMouseSaveStateSize (void);
void interceptMouse (void);
void restoreMouse (void);
void MyExitProc (void);


