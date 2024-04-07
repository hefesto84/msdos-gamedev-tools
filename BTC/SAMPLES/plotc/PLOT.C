/****************************************************************
*                                                               *
*     This program is an example of using Turbo C inline        *
*     assembly statements.                                      *
*                                                               *
*     The program should be compiled with the command line      *
*     version of Turbo C with the compile with assembly         *
*     flag on (tcc -B plot).                                    *
*                                                               *
****************************************************************/

#include <stdio.h>

/* define some crt modes */

#define BW40     0
#define COLOR40  1
#define BW80     2
#define COLOR80  3
#define CGA320   4
#define CGA320CB 5
#define CGA640   6
#define MONO     7
#define EGA320   0x0D
#define EGA640   0x0E
#define EGAMono  0x0F
#define EGAFull  0x10

void set_crtmode(int mode) {
/****************************************************************
*                                                               *
*    Set the crt mode with interrupt 16 function 0              *
*                                                               *
****************************************************************/

     asm mov ah, 0       /* function 0, set video mode */
     asm mov al, mode
     asm int 16          /* call interrupt 16 */
}


void plotpixel(int x, int y, int pixel_val) {
/****************************************************************
*                                                               *
*    Plot a pixel at x,y                                        *
*                                                               *
****************************************************************/

     asm mov ah, 12           /* function 12     */
     asm mov al, pixel_val    /* pixel attribute */
     asm mov cx, x
     asm mov dx, y
     asm int 16               /* call interrupt 16 */

} /* plotpixel */

void draw(int _x1, int _y1, int _x2, int _y2, int attr){
/****************************************************************
*                                                               *
*    Draw a line from _x1,_y1 to _x2,_y2                        *
*                                                               *
****************************************************************/

     int x, y, deltax, deltay, xstep, ystep, direction;
     int  x1, y1, x2, y2;

     x = x1 = _x1;
     y = y1 = _y1;

     xstep = (x1 > (x2 = _x2)) ? -1 : 1;
     ystep = (y1 > (y2 = _y2)) ? -1 : 1;

     deltax = abs(x2 - x1);
     deltay = abs(y2 - y1);

     direction = (deltax == 0) ? -1 : 0;

     while ( !( (x == x2) && (y == y2) ) ) {

         plotpixel(x, y, attr);

         if (direction < 0) {
              y += ystep;
              direction += deltax;
         } else {
              x += xstep;
              direction -= deltay;
         }
     }
} /* draw */


void box(int x1, int y1, int x2, int y2, int attr){
/****************************************************************
*                                                               *
*     Draw a box with corners x1,y1 & x2,y2                     *
*                                                               *
****************************************************************/
     int i;

     /* draw top and bottom */
     for (i = x1 ; i <= x2 ; i++) {
         plotpixel(i,y1,attr);
         plotpixel(i,y2,attr);
     }

     /* draw sides */
     for (i = y1 ; i <= y2 ; i++) {
         plotpixel(x1,i,attr);
         plotpixel(x2,i,attr);
     }

}

main(){
     /* turn on CGA 640 X 200 */
     set_crtmode(CGA640);

     printf("This is an example of using 640 X 200 CGA");

     box(20,20,630,190,1);

     draw(20,20,630,190,1);

     getch();

     /* turn on color text mode */
     set_crtmode(COLOR80);
}