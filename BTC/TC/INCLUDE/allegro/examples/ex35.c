/*
 *       Example program for the Allegro library, by Richard Mitton.
 *
 *             or "How to get a 12-bit mode on an 8-bit card"
 *
 * This program sets up a 12-bit mode on any 8-bit card, by setting up
 * a 256-colour palette that will fool the eye into grouping two 8-bit pixels
 * into one 12-bit pixel.
 *
 * It's quite simple (sort of). You make your 256-colour palette with all the
 * combinations of blue and green, assuming green ranges from 0-15 and blue
 * from 0-14. This takes up 16x15=240 colours. This leaves 16 colours to use
 * as red (red ranges from 0-15).
 *
 * Then you put your green/blue in one pixel, and your red in the pixel next
 * to it. The eye gets fooled into thinking it's all one pixel.
 *
 * It's all very simple really. Honest.
 *
 * To start with, you set a normal 256 color VESA mode, and construct a 
 * special palette for it. But then comes the trick: you need to write to 
 * a set of two adjacent pixels to form a single 12 bit dot. Two eight bit
 * pixels is the same as one 16 bit pixel, so after setting the video mode
 * you need to hack the screen bitmap about, halving the width and changing
 * it to use the 16 bit drawing code. Then, once you have packed a color
 * into the correct format (using the makecol12() function below), any of
 * the normal Allegro drawing functions can be used with this 12 bit display!
 *
 * Things to note:
 *
 *    The horizontal width is halved, so you get resolutions like 320x480,
 *    400x600, and 512x768.
 *
 *    Because each dot is spread over two actual pixels, the display will
 *    be darker than in a normal video mode.
 *
 *    Any bitmap data will obviously need converting to the correct 12
 *    bit format: regular 15 or 16 bit images won't display correctly...
 *
 *    Although this works like a truecolor mode, it is actually using a
 *    256 color palette, so palette fades are still possible!
 *
 * Note: This code only works in linear screen modes (so don't try Mode-X).
 */

#include <stdio.h>
#include <stdlib.h>

#include "allegro.h"

// Declare the screen size and mask colour we will use
#define GFXW 320
#define GFXH 480
#define MASK_COLOR_12 0xFFE0

// Theses are specific to this example. They say how big the vector-balls are.
#define BALLW 12
#define BALLH 24
#define BIGBALLW 20
#define BIGBALLH 40
#define MESSAGE_STR "Allegro"

typedef struct {
   fixed x, y;
   int c;
} point_t;

// These functions can be used in any 12-bit program.
int makecol12(int r, int g, int b);
void set_12bit_palette(void);
BITMAP *create_bitmap_12(int w, int h);

// These functions are just for this example. 
void blur_12(BITMAP *bmp, BITMAP *back);
void rgb_scales_12(BITMAP *bmp, int ox, int oy, int w, int h);
point_t *make_points(int *numpoints, char *msg);
BITMAP *make_ball(int w, int h, int br, int bg, int bb);

// Construct the magic palette that makes it all work.
// You need to call this after set_gfx_mode to get it to work.
void set_12bit_palette(void)
{
   int r,g,b;
   PALETTE pal;
   for (b=0;b<15;b++)
   {
      for (g=0;g<16;g++)
      {
	 pal[b*16+g].r = 0;
	 pal[b*16+g].g = g*63/15;
	 pal[b*16+g].b = b*63/15;
      }
   }
   for (r=0;r<16;r++)
   {
      pal[r+240].r = r*63/15;
      pal[r+240].g = 0;
      pal[r+240].b = 0;
   }
   set_palette(pal);
}

// The other magic routine - use this to make colours instead of makecol
int makecol12(int r, int g, int b)
{
/* returns a 16-bit integer - here's the format:

   0xARBG - where A=0xf (reserved, if you like),
		  R=red (0-15)
		  B=blue (0-14)
		  G=green (0-15)
   */

   r = r*16/256;
   g = g*16/256;
   b = b*16/256 - 1;
   if (b < 0) b = 0;

   return (r << 8) | (b << 4) | g | 0xf000;
}

// Extract red component from color.
int getr12(int color)
{
   return (color >> 4) & 0xF0;
}
// Extract green component from color.
int getg12(int color)
{
   return (color << 4) & 0xF0;
}
// Extract blue component from color.
int getb12(int color)
{
   return (color & 0xF0);
}

// Use this instead of create_bitmap, because the vtable needs changing
// so that the drawing functions will use the 16-bit functions
BITMAP *create_bitmap_12(int w, int h)
{
   BITMAP *bmp;
   bmp = create_bitmap_ex(16, w, h);
   if (bmp)
   {
      bmp->vtable->color_depth = 12;
      bmp->vtable->mask_color = MASK_COLOR_12;
   }
   return bmp;
}

// This merges 'bmp' into 'back'. This is how the trails work.
void blur_12(BITMAP *bmp, BITMAP *back)
{
   int x,y, r1,g1,b1, r2,g2,b2, c1,c2;
   for (y=0;y<bmp->h;y++)
   {
      unsigned short* backline = (unsigned short*) (back->line[y]);
      unsigned short* bmpline = (unsigned short*) (bmp->line[y]);

      for (x=0;x<bmp->w;x++)
      {
	 // First get the pixel from each bitmap.
	 // Then move the first colour values slightly towards the second.
	 c1 = bmpline[x];
	 c2 = backline[x];
         r1 = c1 & 0xF00;
         r2 = c2 & 0xF00;
	 if (r1 < r2)
            c1 += 0x100;
	 else if (r1 > r2)
            c1 -= 0x100;

	 b1 = c1 & 0xF0;
	 b2 = c2 & 0xF0;
	 if (b1 < b2)
            c1 += 0x10;
	 else if (b1 > b2)
            c1 -= 0x10;

	 g1 = c1 & 0x0F;
	 g2 = c2 & 0x0F;
	 if (g1 < g2)
            c1 += 0x01;
	 else if (g1 > g2)
            c1 -= 0x01;

	 // Then put it back in the bitmap.
         bmpline[x] = c1;
      }
   }
}

// Generates some nice RGB scales onto the specified bitmap.
// Draws at ox,oy, with width 'w' and height 'h'.
void rgb_scales_12(BITMAP *bmp, int ox, int oy, int w, int h)
{
   int x,y;
   for (y=0;y<h;y++)
      for (x=0;x<w;x++)
	 putpixel(bmp, ox+x, oy+y,     makecol12(x*256/w, y*256/h, 0));
   for (y=0;y<h;y++)
      for (x=0;x<w;x++)
	 putpixel(bmp, ox+x+w, oy+y,   makecol12(x*256/w, 0, y*256/h));
   for (y=0;y<h;y++)
      for (x=0;x<w;x++)
	 putpixel(bmp, ox+x, oy+y+h,   makecol12(0, x*256/w, y*256/h));
   for (y=0;y<h;y++)
      for (x=0;x<w;x++)
	 putpixel(bmp, ox+x+w, oy+y+h, makecol12(x*128/w+y*128/h,
						 x*128/w+y*128/h,
						 x*128/w+y*128/h));
}

// Turns the string in 'msg' into a series of 2D points. These can then
// be drawn with the vector balls.
// numpoints will be filled with how many points there are.
point_t *make_points(int *numpoints, char *msg)
{
   BITMAP *bmp;
   int n, x,y;
   point_t *points;

   // This routine only needs to be 256 colour, as we are only using the
   // bitmap to temporarily store the font

   bmp = create_bitmap_ex(8, text_length(font, msg), text_height(font));
   if (!bmp) return NULL;
   clear(bmp);
   textout(bmp, font, msg, 0,0, 1);

   // First, count how much memory we will need to reserve.
   n = 0;
   for (y=0;y<bmp->h;y+=1)
   {
      for (x=0;x<bmp->w;x+=1)
      {
	 if (_getpixel(bmp, x, y) != 0) n++;
      }
   }
   points = (point_t *)malloc(n * sizeof(point_t));
   if (!points)
   {
      strcpy(allegro_error, "Out of memory");
      return NULL;
   }

   // Then redo it all, but actually store the points this time.
   n = 0;
   for (y=0;y<bmp->h;y+=1)
   {
      for (x=0;x<bmp->w;x+=1)
      {
	 if (_getpixel(bmp, x, y) != 0)
	 {
	    points[n].x = itofix(x - bmp->w/2) * 6;
	    points[n].y = itofix(y - bmp->h/2) * 12;
	    points[n].c = rand()%4;
	    n++;
	 }
      }
   }
   *numpoints = n;
   destroy_bitmap(bmp);
   return points;
}

// This draws a vector ball. w and h are the dimensions, br/bg/bg is the
// colour of the brightest spot.
BITMAP *make_ball(int w, int h, int br, int bg, int bb)
{
   BITMAP *bmp;
   int r, rx,ry;
   bmp = create_bitmap_12(w, h); 

   if (!bmp) return NULL;

   clear_to_color(bmp, MASK_COLOR_12);
   for (r=0;r<16;r++)
   {
      rx = w*(15-r)/32;
      ry = h*(15-r)/32;
      ellipsefill(bmp, w/2,h/2, rx, ry, makecol12(br*r/15, bg*r/15, bb*r/15));
   }
   return bmp;
}

int main(int argc, char *argv[])
{
#ifdef ALLEGRO_COLOR16
   BITMAP *rgbpic, *ball[4], *buffer, *bigball;
   int x, r=0,g=0,b=0, numpoints, thispoint;
   point_t *points;
   fixed xangle, yangle, zangle, newx, newy, newz;
   MATRIX m;

   allegro_init();
   install_keyboard();

   // First set your graphics mode as normal, except twice as wide because
   // we are using 2-bytes per pixel, but the graphics card doesn't know this.
   if (set_gfx_mode(GFX_AUTODETECT, GFXW*sizeof(short), GFXH, 0,0) != 0)
   {
      printf("Error setting %ix%ix12 (really %ix%ix8, but we fake it):\n%s\n", GFXW, GFXH, GFXW*(int)sizeof(short), GFXH, allegro_error);
      return 1;
   }

   // Then set your magic palette. From now on you can't use the set_color
   // or set_palette functions or they will mess up this palette. You can
   // still use the fade routines, if you make sure you fade back into this
   // palette.
   set_12bit_palette();

   // Then hack the vtable so it uses the 16-bit functions.
   screen->vtable = &__linear_vtable16;
   screen->vtable->color_depth = 12;
   screen->vtable->mask_color = MASK_COLOR_12;
   screen->w /= sizeof(short); // because we use sizeof(short)-bytes per pixel

   // Reset the clip window to it's new parameters.
   set_clip(screen, 0,0, screen->w - 1, screen->h - 1);

   // Then generate 4 vector balls of different colours.
   for (x=0;x<4;x++)
   {
      switch(x)
      {
      case 0: r = 255; g = 0;   b = 0;   break;
      case 1: r = 0;   g = 255; b = 0;   break;
      case 2: r = 0;   g = 0;   b = 255; break;
      case 3: r = 255; g = 255; b = 0;   break;
      }

      ball[x] = make_ball(BALLW, BALLH, r, g, b);
      if (!ball[x])
      {
	 allegro_exit();
	 printf("Error: %s\n", allegro_error);
	 return 1;
      }
   }

   // Also make one big red vector ball.
   bigball = make_ball(BIGBALLW, BIGBALLH, 255, 0, 0);
   if (!bigball)
   {
      allegro_exit();
      printf("Error: %s\n", allegro_error);
      return 1;
   }

   // Make the off-screen buffer that everything will be drawn onto.
   buffer = create_bitmap_12(GFXW, GFXH);
   if (!buffer)
   {
      allegro_exit();
      printf("Error: %s\n", allegro_error);
      return 1;
   }

   // Convert the text message into the coordinates of the vector balls.
   points = make_points(&numpoints, MESSAGE_STR);
   if (!points)
   {
      allegro_exit();
      printf("Error: %s\n", allegro_error);
      return 1;
   }

   // Create the background picture 
   rgbpic = create_bitmap_12(GFXW, GFXH);
   if (!rgbpic)
   {
      allegro_exit();
      printf("Error: %s\n", allegro_error);
      return 1;
   }
   rgb_scales_12(rgbpic, 0, 0, GFXW/2, GFXH/2);

   // Copy the background into the buffer.
   blit(rgbpic, buffer, 0,0,0,0, GFXW,GFXH);

   xangle = yangle = zangle = 0;

   // Put a message in the top-left corner.
   text_mode(-1);
   textprintf(rgbpic, font, 3, 3, makecol12(255, 255, 255), "%ix%i 12-bit colour on an 8-bit card", GFXW, GFXH);
   textprintf(rgbpic, font, 3, 13, makecol12(255, 255, 255), "(3840 colours at once!)");

   while(!keypressed())
   {
      // First, draw some vector balls moving in a circle round the edge.
      for (x=0;x<itofix(256);x+=itofix(32))
      {
	 masked_blit(bigball, buffer, 0, 0,
		     fixtoi(150 * fcos(xangle+x)) + GFXW/2 - BALLW/2,
		     fixtoi(200 * fsin(xangle+x)) + GFXH/2 - BALLH/2,
		     BIGBALLW, BIGBALLH);
      }

      // Rotate the vector balls

      get_rotation_matrix(&m, xangle, yangle, zangle);
      for (thispoint=0;thispoint<numpoints;thispoint++)
      {
	 apply_matrix(&m, points[thispoint].x,
			  points[thispoint].y,
			  0, // All points have the same Z value (which is 0)
			  &newx, &newy, &newz);

	 masked_blit(ball[points[thispoint].c], buffer, 0,0,
			  fixtoi(newx) + GFXW/2,
			  fixtoi(newy) + GFXH/2, BALLW, BALLH);
      }

      // Then blur the buffer so it fades into the background picture.
      blur_12(buffer, rgbpic);

      // Finally copy everything to the screen.
      blit(buffer, screen, 0,0,0,0, GFXW,GFXH);

      // Rotate it a bit more.
      xangle += itofix(1);
      yangle += itofix(1);
      zangle += itofix(1);
   }
   clear_keybuf();

   // Clean it all up.
   for (x=0;x<4;x++)
   {
      destroy_bitmap(ball[x]);
   }
   destroy_bitmap(bigball);
   free(points);
   screen->vtable = &__linear_vtable8;
   fade_out(4);
#else
   printf("ALLEGRO_COLOR16 is not defined in \"allegro.h\".\n");
   printf("16-bpp color depth is not supported.\n");
#endif

   return 0;
}
