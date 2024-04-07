/* 
 *    Example program for the Allegro library, by Shawn Hargreaves.
 *
 *    This program demonstrates the use of patterned drawing and sub-bitmaps.
 */


#include <stdlib.h>
#include <stdio.h>

#include "allegro.h"


void draw_pattern(BITMAP *bitmap, char *message, int color)
{
   BITMAP *pattern;

   /* create a pattern bitmap */
   pattern = create_bitmap(text_length(font, message), text_height(font));
   clear(pattern);
   textout(pattern, font, message, 0, 0, 255);

   /* cover the bitmap with the pattern */
   drawing_mode(DRAW_MODE_SOLID_PATTERN, pattern, 0, 0);
   rectfill(bitmap, 0, 0, bitmap->w, bitmap->h, color);
   solid_mode();

   /* destroy the pattern bitmap */
   destroy_bitmap(pattern);
}



int main()
{
   BITMAP *bitmap;

   allegro_init();
   install_keyboard(); 
   set_gfx_mode(GFX_VGA, 320, 200, 0, 0);
   set_pallete(desktop_pallete);

   /* first cover the whole screen with a pattern */
   draw_pattern(screen, "<screen>", 255);

   /* draw the pattern onto a memory bitmap and then blit it to the screen */
   bitmap = create_bitmap(128, 32);
   draw_pattern(bitmap, "<memory>", 1);
   blit(bitmap, screen, 0, 0, 32, 32, 128, 32);
   destroy_bitmap(bitmap);

   /* or we could use a sub-bitmap. These share video memory with their
    * parent, so the drawing will be visible without us having to blit it
    * across onto the screen.
    */
   bitmap = create_sub_bitmap(screen, 224, 64, 64, 128);
   draw_pattern(bitmap, "<subbmp>", 4);
   destroy_bitmap(bitmap);

   readkey();
   return 0;
}
