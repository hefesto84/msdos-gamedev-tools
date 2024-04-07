/* 
 *    Example program for the Allegro library, by Grzegorz Adam Hankiewicz.
 *
 *    This program demonstrates how to create custom graphic effects with
 *    the create_color_table function, this time a greyscale effect.
 */


#include <stdio.h>
#include <time.h>

#include "allegro.h"


/* RGB -> color mapping table. Not needed, but speeds things up */
RGB_MAP rgb_table;

/* greyscale & negative color mapping table */
COLOR_MAP greyscale_table,negative_table;

PALLETE pal;
BITMAP *background;
BITMAP *temp;

/* progress indicator for the color table calculations */
void callback_func()
{
   printf(".");
   fflush(stdout);
}

/* Here comes our custom function. It's designed to take the input colors
 * (red, green & blue) and return a greyscale color for it. This way, when
 * any drawing function draws say over green, it draws the greyscale color
 * for green.
 * 'pal' is the palette we are looking in to find the colors.
 * Now, imagine we want to draw a pixel with color A, over color B.
 * Once the table is created, set, and the drawing mode is TRANSLUCENT, then
 * A is the 'x' color passed to the function and B is the 'y' color passed
 * to the function.
 * Since we want a greyscale effect with no matter what A (or 'x') color, we
 * ignore it and use y to look at the palette.
 * The x=x line is there to avoid compiler warnings.
 * NOTE:
 * When you return the rgb value, you don't need to search the palette for
 * the nearest color, Allegro does this automatically.
 */

RGB return_grey_color(PALETTE pal, int x, int y)
{
   int c;
   RGB rgb;

   // First create the greyscale color
   c= (pal[y].r*0.3+pal[y].g*0.5+pal[y].b*0.2);

   // Now assign to our rgb triplet the palette greyscale color...
   rgb.r = rgb.g = rgb.b = c;
   x=x;

   // ...and return it.
   return rgb;
}

/* The negative_color function is quite the same like the grayscale one,
 * since we are ignoring the value of the drawn color (aka x).
 */

RGB return_negative_color(PALETTE pal, int x, int y)
{
   RGB rgb;

   /* To get the nevative color, substract the color values of red, green and
    * blue from the full (63) color value
    */

   rgb.r = 63- pal[y].r;
   rgb.g = 63- pal[y].g;
   rgb.b = 63- pal[y].b;

   x=x;

   return rgb;
}

void generate_background(void)
{
   int i;

   // First get some usual colors
   generate_332_palette(pal);

   // Now remap the first 64 for a perfect greyscale gradient
   for(i=0;i<64;i++) {
      pal[i].r=i;
      pal[i].g=i;
      pal[i].b=i;
   }

   // Draws some things on the screen using not-greyscale colors
   for(i=0;i<3000;i++)
      circlefill(background,random()%320,random()%200,random()%25,64+random()%192);
}

int main (void)
{
   int x,y,deltax=1,deltay=1;
   srandom(time(NULL));

   allegro_init();
   install_keyboard(); 

   temp=create_bitmap(320,200);
   background=create_bitmap(320,200);

   if(temp==NULL || background==NULL) {
      printf("Sorry, not enough memory\n");
      return 1;
   }

   printf("Generating background graphic and palette.Please wait...\n");
   generate_background();

   /* this isn't needed, but it speeds up the color table calculations */
   printf("Generating RGB Table (3.25 lines to go)\n");
   create_rgb_table(&rgb_table, pal, callback_func);
   rgb_map = &rgb_table;

   /* build a color lookup table for greyscale effect */
   printf("\nGenerating Greyscale Table (3.25 lines to go)\n");
   create_color_table(&greyscale_table, pal, return_grey_color, callback_func);

   /* build a color lookup table for greyscale effect */
   printf("\nGenerating Negative Table (3.25 lines to go)\n");
   create_color_table(&negative_table, pal, return_negative_color, callback_func);

   if(set_gfx_mode(GFX_VGA,320,200,0,0) != 0) {
      printf("Sorry, couldn't set graphic mode\n");
      return 2;
   }
   set_palette(pal);

   /* Look, we have set the drawing mode to TRANS. This makes all the drawing
    * functions use the general color_map table, which is _NOT_ translucent,
    * since we are using a custom color_map table.
    */
   drawing_mode(DRAW_MODE_TRANS,NULL,0,0);

   text_mode(-1);

   // Now here comes the part with the greyscale examples.

   /* select the greyscale table */
   color_map = &greyscale_table;

   x=y=50;
   blit(background,temp,0,0,0,0,320,200);
   rectfill(temp,x,y,x+50,y+50,0);

   blit(temp,screen,0,0,0,0,320,200);

   while(!keypressed()) {
      x+=deltax;
      y+=deltay;

      if(x<1 || x>320-50) deltax *= -1;
      if(y<1 || y>200-50) deltay *= -1;
      blit(background,temp,0,0,0,0,320,200);
      textout_centre(temp,font,"Greyscale effect",SCREEN_W>>1,SCREEN_H>>1,makecol(0,0,255));
      rectfill(temp,x,y,x+50,y+50,0);
      vsync();
      blit(temp,screen,0,0,0,0,320,200);
   }

   clear_keybuf();

   /* Now it's time for the negative part. The negative example is easier to
    * see with greyscale colors. Therefore we will change the color of the
    * background to a greyscale one, but only in a restricted area...
    */

   rectfill(background,SCREEN_H >> 2,SCREEN_H >> 2,
	    background->w - (SCREEN_H >> 2),background->h - (SCREEN_H >> 2),0);

   /* This should go inside the next loop, but since we won't use the
    * background image any more, we can optimize it's speed printing the
    * text now.
    */
   textout_centre(background,font,"Negative effect",SCREEN_W>>1,SCREEN_H>>1,makecol(0,0,0));

   // Switch the active color table...
   color_map = &negative_table;

   blit(background,temp,0,0,0,0,320,200);
   rectfill(temp,x,y,x+50,y+50,0);

   blit(temp,screen,0,0,0,0,320,200);

   while(!keypressed()) {
      x+=deltax;
      y+=deltay;

      if(x<1 || x>320-50) deltax *= -1;
      if(y<1 || y>200-50) deltay *= -1;
      blit(background,temp,0,0,0,0,320,200);
      rectfill(temp,x,y,x+50,y+50,0);
      vsync();
      blit(temp,screen,0,0,0,0,320,200);
   }

   return 0;
}
