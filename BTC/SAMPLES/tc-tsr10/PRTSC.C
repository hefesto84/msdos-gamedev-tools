/*--------------------------------------------------------------------------*
 |   PrtSc.C                                                                |
 |     contains screen, printer, keyboard, and disk I/O routines            |
 |   PrtSC.c is supplied as is with no warranty, expressed or implied.      |
 |   It is not copyrighted, either.                                         |
 *--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*
 | Author:                                                                  |
 | Sherif El-Kassas        .       :.                                       |
 | EB dept           \_____o__/ __________                                  |
 | Eindhoven U of Tec       .. /                                            |
 | The Netherlands            /             Email: elkassas@eb.ele.tue.nl   |
 *--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*
 | The author shall not be liable to the user for any direct, indirect      |
 | or consequential loss arising from the use of, or inability to use,      |
 | any program or file howsoever caused.  No warranty is given that the     |
 | programs will work under all circumstances.                              |
 *--------------------------------------------------------------------------*/

#include <dos.h>
#include <bios.h>
#include <fcntl.h>
#include <stat.h>
#include <io.h>
#include <stdlib.h>
#include "prtsc.h"

/*------------------------------------------------------------------------*
 |                           GLOBAL VARIABLES                             |
 *------------------------------------------------------------------------*/

unsigned screen_pos = 0;  /* pointer to the current screen location         */
char far *screen    = (char far *) 0xB8000000L; /* pointer to video RAM     */
                          /* assume (for now) we have a colored monitor !   */

char far * save_buffer;   /* pointer to a buffer that is used to save       */
                          /* box borders. (save_buffer is declared as a     */
                          /* far pointer to make sure that it's compatible  */
                          /* with the screen pointer                        */

char box_buffer[((MAX_X+1)*2+(MAX_Y+1)*2)*2]; /* reserve the maximum amount */
                                              /* of memory needed to save   */
                                              /* the rubber-band box        */

byte old_cursor_color;                        /* to save cursor location    */
int  old_cursor_pos;                          /* and attribute              */

/*------------------------------------------------------------------------*
 |                  Initialize screen and buffer pointers                 |
 *------------------------------------------------------------------------*/
void  initialize_video(void){
     union REGS reg;

     reg.h.ah = 0x0F;         /* interrupt 0x10 function 0x0F get current   */
     int86(0x10, &reg, &reg); /* display mode. if al==7 i.e. mono screen    */
     if (reg.h.al == 7)       /* change the value of the screen pointer     */
       screen = (char far *) 0xB0000000L;
     save_buffer = MK_FP(_DS, (unsigned) box_buffer); /* point to box buffer*/
     old_cursor_pos   = 0;
     old_cursor_color = screen[1];

}/* initialize_video() */

/*------------------------------------------------------------------------*
 |       Read a key using BIOS function (interrupt 0x16 function 0)       |
 *------------------------------------------------------------------------*/
int  getkey(void){
     register  int  c;
     c = bioskey(0);
     return( (c&0x00ff) ? (c & 0x00ff) : c );
} /* getkey() */

/*------------------------------------------------------------------------*
 |                  Move inverse video cursor to col and row.             |
 *------------------------------------------------------------------------*/
void move_cursor( byte col, byte row ){
     screen[old_cursor_pos+1] = old_cursor_color; /* restore old color      */
     old_cursor_pos = row*160+col*2;              /* save cursor position   */
     old_cursor_color = screen[old_cursor_pos+1]; /* save color             */
/*     screen[old_cursor_pos+1] = CURSOR_COLOR; /* put cursor at col and row  */
    if(screen[old_cursor_pos+1]==CURSOR_COLOR)
         screen[old_cursor_pos+1] = CURSOR_COLOR1;
    else screen[old_cursor_pos+1] = CURSOR_COLOR;
}/* move_cursor */

/*------------------------------------------------------------------------*
 |           Make col and row the current screen position                 |
 *------------------------------------------------------------------------*/
void gotoxy( byte col, byte row ){
     screen_pos = row*((MAX_X+1)*2)+col*2;
}/* gotoxy */

/*------------------------------------------------------------------------*
 |   Write character and attribute at current location and move pointer   |
 *------------------------------------------------------------------------*/
void putc(char c, byte color){
     screen[screen_pos] = c;    /* write character                        */
     screen_pos++;              /* point to color location                */
     screen[screen_pos] = color;/* write color                            */
     screen_pos++;              /* point to next position                 */
}/* putc */

/*------------------------------------------------------------------------*
 |         Write character and attribute "num" times                      |
 *------------------------------------------------------------------------*/
void repeat_char(char c, register int num, byte color){
     register  int i;

     for(i=0;i<num;i++) putc(c, color);
}/* repeat_char */

/*------------------------------------------------------------------------*
 |                        Write string                                    |
 *------------------------------------------------------------------------*/
void puts(register char *p, byte color){
     while (*p) putc(*p++, color);
}/* puts */

/*------------------------------------------------------------------------*
 |           Copy data from far source to far destination                 |
 *------------------------------------------------------------------------*/
void move( char far *src, char far *dest, int amount ){
     register int i;

     for (i=0; i<amount; i++) dest[i] = src[i];
}/* move */


/*------------------------------------------------------------------------*
 |                Copy data from/to the screen                            |
 *------------------------------------------------------------------------*/

void scr_move( char far *screen, char far *buffer,
               unsigned amount,
               byte  direction )
{
     if (direction == SAVE)
          move( screen, buffer, amount );
     else move( buffer, screen, amount );
}/* scr_move */

/*------------------------------------------------------------------------*
 |                          Exchange two bytes                            |
 *------------------------------------------------------------------------*/
void exchange(byte *a, byte *b){

     int temp;
     temp = *a; *a = *b; *b = temp;

}/* exchange */

/*------------------------------------------------------------------------*
 |                      Order two bytes                                   |
 *------------------------------------------------------------------------*/
void order( byte *a, byte *b){

  if (*a > *b)
    exchange( a, b );

}/* order */

/*------------------------------------------------------------------------*
 |       Save/Restore the box defined by sx, sy, lx, ly  to/from buff     |
 *------------------------------------------------------------------------*/
void box_move( byte sx, byte sy, byte lx, byte ly,
               char far *buff,
               byte direction )
{
  int y, x, i;

  order( &sy, &ly );
  x = min(sx, lx);
  i = (abs(lx-sx)+1) << 1;
  scr_move( &screen[(160*sy)+(2*x)], &buff[0], i, direction);
  scr_move( &screen[(160*ly)+(2*x)], &buff[i], i, direction);
  i <<=  1;
  for (y = sy+1; y<ly; y++){
    scr_move( &screen[(160*y)+(2*sx)], &buff[i], 2, direction);
    i += 2;
    scr_move( &screen[(160*y)+(2*lx)], &buff[i], 2, direction);
    i += 2;
  }
}/* box_move */

/*------------------------------------------------------------------------*
 |                           Draw box                                     |
 *------------------------------------------------------------------------*/
void draw_box( byte sx, byte sy, byte lx, byte ly ){
     byte x, y;

     order( &sy, &ly );
     x = min(sx,lx);

     if (sx != lx ){
       gotoxy(x, sy);
       putc(C1, BOX_COLOR); repeat_char(H, abs(lx-sx)-1, BOX_COLOR);
       putc(C2, BOX_COLOR);
       gotoxy(x, ly);
       putc(C3, BOX_COLOR); repeat_char(H, abs(lx-sx)-1, BOX_COLOR);
       putc(C4, BOX_COLOR);
     }
     for (y = sy+1; y <ly; y++){
       gotoxy(sx, y); putc(V, BOX_COLOR); gotoxy(lx, y); putc(V, BOX_COLOR);
     }
}/* draw_box */

/*------------------------------------------------------------------------*
 |                         Get box corr.                                  |
 *------------------------------------------------------------------------*/
void get_box_corr(byte *x1, byte *y1, byte *x2, byte *y2){

     byte quit, band_on;
     byte x, y;
     int  key;

  old_cursor_pos   = 0;
  old_cursor_color = screen[1];
  band_on = FALSE; quit = FALSE;
  x = y = 0;
  do{
    if (!band_on) move_cursor(x,y);

    key = getkey();
    if (band_on) box_move( *x1, *y1, x, y, save_buffer, RESTORE );

    switch( key ){
       case UP:         y -= (y > MIN_Y);
                        break;
       case DOWN:       y += (y < MAX_Y);
                        break;
       case RIGHT:      x += (x < MAX_X);
                        break;
       case LEFT:       x -= (x > MIN_X);
                        break;
       case HOME:       y -= (y>MIN_Y);
                        x -= (x > MIN_X);
                        break;
       case END:        x -= (x > MIN_X);
                        y += (y < MAX_Y);
                        break;
       case PGDN:       y += (y < MAX_Y);
                        x += (x < MAX_X);
                        break;
       case PGUP:       y -= (y > MIN_Y);
                        x += (x < MAX_X);
                        break;
       case CTRL_RIGHT: x = MAX_X;
                        break;
       case CTRL_LEFT:  x = MIN_X;
                        break;
       case CTRL_HOME:  x = MIN_X; y = MIN_Y;
                        break;
       case CTRL_END:   x = MIN_X; y = MAX_Y;
                        break;
       case CTRL_PGUP:  x = MAX_X; y = MIN_Y;
                        break;
       case CTRL_PGDN:  x = MAX_X; y = MAX_Y;
                        break;

       case ENTER:      if (band_on){
                          *x2 = x; *y2 = y;
                          quit = TRUE;
                        }
                        else{
                          band_on = TRUE;
                          *x1 = x; *y1 = y;
                        }
                        break;
      case ESC:         band_on = FALSE;
                        break;
    }

    if (band_on){
      box_move( *x1, *y1, x, y, save_buffer, SAVE );
      draw_box( *x1, *y1, x, y);
    }

  }while (!quit);


  if (band_on) box_move( *x1, *y1, x, y, save_buffer, RESTORE );
  screen[old_cursor_pos+1] = old_cursor_color;
  if (*x1 > *x2) exchange(x1, x2);
  if (*y1 > *y2) exchange(y1, y2);

}/* get_box_corr */

/*------------------------------------------------------------------------*
 |              Yes. return TRUE if y is pressed                          |
 *------------------------------------------------------------------------*/
int yes(char *prompt){
    register key;
    scr_move( screen, save_buffer, (MAX_X+1)*2, SAVE );
    gotoxy(0,0); puts(prompt, ON_COLOR);
    key = getkey(); putc( key, ON_COLOR);
    scr_move( screen, save_buffer, (MAX_X+1)*2, RESTORE );
    return( (key=='y') || (key=='Y') );
}/* yes */

/*------------------------------------------------------------------------*
 |           Print a character (using interrupt 0x17)                     |
 *------------------------------------------------------------------------*/
void printc(char c){
     union REGS reg;

     reg.h.ah = 0;
     reg.h.al = c;
     reg.x.dx = 0;
     int86( 0x17, &reg, &reg );
}/* printc */

/*------------------------------------------------------------------------*
 |      Print contents of screen (x1, y1) (x2, y2)                        |
 *------------------------------------------------------------------------*/
void print_screen( byte x1, byte y1, byte x2, byte y2 ){
     register x, y;

     if (yes("DO YOU WANT TO PRINT THE CURRENT SCREEN ? (Y/N)"))
     {
       gotoxy(0,0);
       for ( y = y1; y <= y2; y++){
         for ( x = x1; x <= x2; x++){
           printc(screen[y*160+x*2]);
         }
         printc(13); printc(10);
       }
     }
}/* print_screen */

/*------------------------------------------------------------------------*
 |        Save contents of screen (x1, y1)-(x2, y2)                       |
 *------------------------------------------------------------------------*/
void save_screen( byte x1, byte y1, byte x2, byte y2 ){
    static char *file_name = "OUTPUT.PRN";
    static int  handle = -1;
    static char temp[MAX_X+1];
    register x,y;

    if ( yes("DO YOU WANT TO SAVE THE CURRENT SCREEN ? (Y/N)") )
    {
      handle = open(file_name, O_WRONLY|O_APPEND|O_CREAT, S_IREAD|S_IWRITE);
      if (handle > -1){
        for ( y = y1; y <= y2; y++){
          for (x=x1; x<=x2; x++)
            temp[x-x1] = screen[y*160+x*2];
          temp[x-x1]   = 13;
          temp[x-x1+1] = 10;
          _write(handle, temp, x2-x1+3);
        }
        close(handle);
      }
    }
}/* save_screen */

/*------------------------------------------------------------------------*
 |                          Main task                                     |
 *------------------------------------------------------------------------*/
void main_task(void){
  byte x1, y1, x2, y2;

  old_cursor_pos   = 0;
  old_cursor_color = screen[1];
  get_box_corr(&x1, &y1, &x2, &y2);
  print_screen( x1, y1, x2, y2 );
  save_screen( x1, y1, x2, y2 );

}/* main_task */
