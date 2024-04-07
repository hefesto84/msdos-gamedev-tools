/* 
 *    Example program for the Allegro library, by Shawn Hargreaves.
 *
 *    This program demonstrates how to load and display bitmap files
 *    in truecolor video modes, and how to crossfade between them.
 */


#include <stdlib.h>
#include <stdio.h>

#include "allegro.h"



int show(char *name)
{
   BITMAP *bmp, *buffer;
   PALLETE pal;
   int alpha;

   /* load the file */
   bmp = load_bitmap(name, pal);
   if (!bmp)
      return -1;

   buffer = create_bitmap(SCREEN_W, SCREEN_H);
   blit(screen, buffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

   set_pallete(pal);

   /* fade it in on top of the previous picture */
   for (alpha=0; alpha<256; alpha+=8) {
      set_trans_blender(0, 0, 0, alpha);
      draw_trans_sprite(buffer, bmp, (SCREEN_W-bmp->w)/2, (SCREEN_H-bmp->h)/2);
      vsync();
      blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
      if (keypressed()) {
	 destroy_bitmap(bmp);
	 destroy_bitmap(buffer);
	 if ((readkey() & 0xFF) == 27)
	    return 1;
	 else
	    return 0;
      }
   }

   blit(bmp, screen, 0, 0, (SCREEN_W-bmp->w)/2, (SCREEN_H-bmp->h)/2, bmp->w, bmp->h);

   destroy_bitmap(bmp);
   destroy_bitmap(buffer);

   if ((readkey() & 0xFF) == 27)
      return 1;
   else
      return 0;
}



int main(int argc, char *argv[])
{
   int i;

   if (argc < 2) {
      printf("Usage: 'ex32 files.[bmp|lbm|pcx|tga]'\n");
      return 1;
   }

   allegro_init();
   install_keyboard(); 

   /* set the best color depth that we can find */
   set_color_depth(16);
   if (set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0) != 0) {
      set_color_depth(15);
      if (set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0) != 0) {
	 set_color_depth(32);
	 if (set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0) != 0) {
	    set_color_depth(24);
	    if (set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0) != 0) {
	       allegro_exit();
	       printf("Error setting graphics mode\n%s\n\n", allegro_error);
	       return 1;
	    }
	 }
      }
   }

   /* load all images in the same color depth as the display */
   set_color_conversion(COLORCONV_TOTAL);

   /* process all the files on our command line */
   for (i=1; i<argc; i++) {
      switch (show(argv[i])) {

	 case -1:
	    /* error */
	    allegro_exit();
	    printf("Error loading image file '%s'\n\n", argv[i]);
	    return 1;

	 case 0:
	    /* ok! */
	    break;

	 case 1:
	    /* quit */
	    allegro_exit();
	    return 0;
      }
   }

   return 0;
}

