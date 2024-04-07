/* 
 *    Example program for the Allegro library, by Shawn Hargreaves.
 *
 *    This program demonstrates the use of memory bitmaps. It creates
 *    a small temporary bitmap in memory, draws some circles onto it,
 *    and then blits lots of copies of it onto the screen.
 */


#include <stdlib.h>
#include <stdio.h>

#include "allegro.h"


int main()
{
   BITMAP *memory_bitmap;
   int x, y;

   allegro_init();
   install_keyboard(); 
   set_gfx_mode(GFX_VGA, 320, 200, 0, 0);
   set_pallete(desktop_pallete);

   /* make a memory bitmap sized 20x20 */
   memory_bitmap = create_bitmap(20, 20);

   /* draw some circles onto it */
   clear(memory_bitmap);
   for (x=0; x<16; x++)
      circle(memory_bitmap, 10, 10, x, x);

   /* blit lots of copies of it onto the screen */
   for (y=0; y<SCREEN_H; y+=20)
      for (x=0; x<SCREEN_W; x+=20)
	 blit(memory_bitmap, screen, 0, 0, x, y, 20, 20);

   /* free the memory bitmap */
   destroy_bitmap(memory_bitmap);

   readkey();
   return 0;
}
