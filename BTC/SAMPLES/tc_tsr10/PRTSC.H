/*-------------------------------------------------------------------*
 | PRTSC.H                                                           |
 |   Definitions of constants and macros that are used in TSRPRTSC.C |
 |   and PRTSC.C                                                     |
 *-------------------------------------------------------------------*/


typedef unsigned char byte;

#define    TRUE      (1)
#define    FALSE     (0)

#define  SAVE        (0)
#define  RESTORE     (1)

#define  ON          (1)
#define  OFF         (0)

#define  MIN_Y       (0)
#define  MIN_X       (0)

#define  MAX_Y       (24)
#define  MAX_X       (79)

#define   ALT        (8)
#define   SCAN_CODE  (55) /* the '*' key on the right keypad */

#define   cli()     __cli__()
#define   sti()     __sti__()

/*-----------------------------------------------------------------*
 |                    Color definitions                            |
 *-----------------------------------------------------------------*/

/*
 * 0-Black     8-Gray
 * 1-Blue      9-Bright Blue
 * 2-Green     A-Bright Green
 * 3-Cyan      B-Bright Cyan
 * 4-Red       C-Bright Red
 * 5-Magenta   D-Bright Magenta
 * 6-Brown     E-Yellow
 * 7-White     F-Bright White
 */

#define    ON_COLOR       ( 0x1F )
#define    OFF_COLOR      ( 0x07 )
#define    BOX_COLOR      ( 0x40 )
#define    CURSOR_COLOR   ( 0x70 )
#define    CURSOR_COLOR1  ( 0x07 )

/*-----------------------------------------------------------------*
 |                    Box drawing characters                       |
 *-----------------------------------------------------------------*/

#define    H              ('Ä')
#define    V              ('³')
#define    C1             ('Ú')
#define    C2             ('¿')
#define    C3             ('À')
#define    C4             ('Ù')

/*-----------------------------------------------------------------*
 |                constants for keyboard entry                     |
 *-----------------------------------------------------------------*/

#define    ENTER          (13)
#define    SPACE          (32)
#define    ESC            (27)

#define    F1             (0x3b00)
#define    F2             (0x3c00)
#define    F3             (0x3d00)
#define    F4             (0x3e00)
#define    F5             (0x3f00)
#define    F6             (0x4000)
#define    F7             (0x4100)
#define    F8             (0x4200)
#define    F9             (0x4300)
#define    F10            (0x4400)

#define    UP             (0x4800)
#define    DOWN           (0x5000)
#define    RIGHT          (0x4D00)
#define    LEFT           (0x4B00)
#define    HOME           (0x4700)
#define    END            (0x4f00)
#define    PGUP           (0x4900)
#define    PGDN           (0x5100)
#define    CTRL_RIGHT     (0x7400)
#define    CTRL_LEFT      (0x7300)
#define    CTRL_HOME      (0x7700)
#define    CTRL_END       (0x7500)
#define    CTRL_PGUP      (0x8400)
#define    CTRL_PGDN      (0x7600)

void initialize_video(void);
void do_main_task(void);
void main_task(void);
