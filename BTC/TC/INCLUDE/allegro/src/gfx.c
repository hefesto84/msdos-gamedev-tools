/*         ______   ___    ___ 
 *        /\  _  \ /\_ \  /\_ \ 
 *        \ \ \L\ \\//\ \ \//\ \      __     __   _ __   ___ 
 *         \ \  __ \ \ \ \  \ \ \   /'__`\ /'_ `\/\`'__\/ __`\
 *          \ \ \/\ \ \_\ \_ \_\ \_/\  __//\ \L\ \ \ \//\ \L\ \
 *           \ \_\ \_\/\____\/\____\ \____\ \____ \ \_\\ \____/
 *            \/_/\/_/\/____/\/____/\/____/\/___L\ \/_/ \/___/
 *                                           /\____/
 *                                           \_/__/
 *
 *      Graphics routines: pallete fading, circles, etc.
 *
 *      By Shawn Hargreaves.
 *
 *      Optimised line drawer by Michael Bukin.
 *
 *      Bresenham arc routine by Romano Signorelli.
 *
 *      See readme.txt for copyright information.
 */


#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <limits.h>

#include "internal.h"



/* drawing_mode:
 *  Sets the drawing mode. This only affects routines like putpixel,
 *  lines, rectangles, triangles, etc, not the blitting or sprite
 *  drawing functions.
 */
void drawing_mode(int mode, BITMAP *pattern, int x_anchor, int y_anchor)
{
   _drawing_mode = mode;
   _drawing_pattern = pattern;
   _drawing_x_anchor = x_anchor;
   _drawing_y_anchor = y_anchor;

   if (pattern) {
      _drawing_x_mask = 1; 
      while (_drawing_x_mask < (unsigned)pattern->w)
	 _drawing_x_mask <<= 1;        /* find power of two greater than w */

      if (_drawing_x_mask > (unsigned)pattern->w)
	 _drawing_x_mask >>= 1;        /* round down if required */

      _drawing_x_mask--;               /* convert to AND mask */

      _drawing_y_mask = 1;
      while (_drawing_y_mask < (unsigned)pattern->h)
	 _drawing_y_mask <<= 1;        /* find power of two greater than h */

      if (_drawing_y_mask > (unsigned)pattern->h)
	 _drawing_y_mask >>= 1;        /* round down if required */

      _drawing_y_mask--;               /* convert to AND mask */
   }
   else
      _drawing_x_mask = _drawing_y_mask = 0;

   if ((gfx_driver) && (gfx_driver->drawing_mode))
      gfx_driver->drawing_mode();
}



/* set_blender_mode:
 *  Specifies a custom set of blender functions for interpolating between
 *  truecolor pixels. Provide a table of 256 interpolation routines (one for
 *  every alpha value), for each color depth you are going to use (the 24
 *  bit blender is shared between the 24 and 32 bit modes). Pass a NULL
 *  table for unused color depths (you must not draw translucent graphics 
 *  in modes without a table, though!). Your blender will be passed two
 *  32 bit colors in the appropriate format (5.5.5, 5.6.5, or 8.8.8), and
 *  should return the result of combining them. In translucent drawing modes,
 *  the two colors are taken from the source and destination images and the
 *  alpha is specified by this function. In lit modes, the alpha is specified
 *  when you call the drawing routine, and the interpolation is between the
 *  source color and the RGB values you pass to this function.
 */
void set_blender_mode(BLENDER_MAP *b15, BLENDER_MAP *b16, BLENDER_MAP *b24, int r, int g, int b, int a)
{
   _blender_map15 = b15;
   _blender_map16 = b16;
   _blender_map24 = b24;

   _blender_col_15 = makecol15(r, g, b);
   _blender_col_16 = makecol16(r, g, b);
   _blender_col_24 = makecol24(r, g, b);
   _blender_col_32 = makecol32(r, g, b);

   _blender_alpha = a;
}



/* xor_mode:
 *  Shortcut function for toggling XOR mode on and off.
 */
void xor_mode(int xor)
{
   drawing_mode(xor ? DRAW_MODE_XOR : DRAW_MODE_SOLID, NULL, 0, 0);
}



/* solid_mode:
 *  Shortcut function for selecting solid drawing mode.
 */
void solid_mode()
{
   drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
}



/* clear:
 *  Clears the bitmap to color 0.
 */
void clear(BITMAP *bitmap)
{
   clear_to_color(bitmap, 0);
}



/* set_color:
 *  Sets a single pallete entry.
 */
void set_color(int index, RGB *p)
{
   set_pallete_range(p-index, index, index, FALSE);
}



/* set_pallete:
 *  Sets the entire color pallete.
 */
void set_pallete(PALLETE p)
{
   set_pallete_range(p, 0, PAL_SIZE-1, TRUE);
}



/* set_pallete_range:
 *  Sets a part of the color pallete.
 */
void set_pallete_range(PALLETE p, int from, int to, int vsync)
{
   int c;

   _check_gfx_virginity();

   if (gfx_driver) {
      if (screen->vtable->color_depth == 8)
	 gfx_driver->set_pallete(p, from, to, vsync);
   }
   else
      _vga_set_pallete_range(p, from, to, vsync);

   for (c=from; c<=to; c++) {
      _current_pallete[c] = p[c];

      if (_color_depth != 8)
	 pallete_color[c] = makecol(_rgb_scale_6[p[c].r], _rgb_scale_6[p[c].g], _rgb_scale_6[p[c].b]);
   }
}



/* previous palette, so the image loaders can restore it when they are done */
static PALLETE prev_current_pallete;
static int prev_pallete_color[256];



/* select_pallete:
 *  Sets the aspects of the palette tables that are used for converting
 *  between different image formats, without altering the display settings.
 *  The previous settings are copied onto a one-deep stack, from where they
 *  can be restored by calling unselect_pallete().
 */
void select_pallete(PALLETE p)
{
   int c;

   for (c=0; c<256; c++) {
      prev_current_pallete[c] = _current_pallete[c];
      prev_pallete_color[c] = pallete_color[c];

      _current_pallete[c] = p[c];

      if (_color_depth != 8)
	 pallete_color[c] = makecol(_rgb_scale_6[p[c].r], _rgb_scale_6[p[c].g], _rgb_scale_6[p[c].b]);
   }
}



/* unselect_pallete:
 *  Restores pallete settings from before the last call to select_pallete().
 */
void unselect_pallete()
{
   int c;

   for (c=0; c<256; c++) {
      _current_pallete[c] = prev_current_pallete[c];

      if (_color_depth != 8)
	 pallete_color[c] = prev_pallete_color[c];
   }
}



/* generate_332_palette:
 *  Used when loading a truecolor image into an 8 bit bitmap, to generate
 *  a 3.3.2 RGB palette.
 */
void generate_332_palette(PALETTE pal)
{
   int c;

   for (c=0; c<256; c++) {
      pal[c].r = ((c>>5)&7) * 63/7;
      pal[c].g = ((c>>2)&7) * 63/7;
      pal[c].b = (c&3) * 63/3;
   }

   pal[0].r = 63;
   pal[0].g = 0;
   pal[0].b = 63;

   pal[254].r = pal[254].g = pal[254].b = 0;
}



/* get_color:
 *  Retrieves a single color from the pallete.
 */
void get_color(int index, RGB *p)
{
   get_pallete_range(p-index, index, index);
}



/* get_pallete:
 *  Retrieves the entire color pallete.
 */
void get_pallete(PALLETE p)
{
   get_pallete_range(p, 0, PAL_SIZE-1);
}



/* get_pallete_range:
 *  Retrieves a part of the color pallete.
 */
void get_pallete_range(PALLETE p, int from, int to)
{
   int c;

   _check_gfx_virginity();

   for (c=from; c<=to; c++)
      p[c] = _current_pallete[c];
}



/* fade_interpolate: 
 *  Calculates a pallete part way between source and dest, returning it
 *  in output. The pos indicates how far between the two extremes it should
 *  be: 0 = return source, 64 = return dest, 32 = return exactly half way.
 *  Only affects colors between from and to (inclusive).
 */
void fade_interpolate(PALLETE source, PALLETE dest, PALLETE output, int pos, int from, int to)
{
   int c;

   for (c=from; c<=to; c++) { 
      output[c].r = ((int)source[c].r * (63-pos) + (int)dest[c].r * pos) / 64;
      output[c].g = ((int)source[c].g * (63-pos) + (int)dest[c].g * pos) / 64;
      output[c].b = ((int)source[c].b * (63-pos) + (int)dest[c].b * pos) / 64;
   }
}



/* fade_from_range:
 *  Fades from source to dest, at the specified speed (1 is the slowest, 64
 *  is instantaneous). Only affects colors between from and to (inclusive,
 *  pass 0 and 255 to fade the entire pallete).
 */
void fade_from_range(PALLETE source, PALLETE dest, int speed, int from, int to)
{
   PALLETE temp;
   int c;

   for (c=0; c<PAL_SIZE; c++)
      temp[c] = source[c];

   for (c=0; c<64; c+=speed) {
      fade_interpolate(source, dest, temp, c, from, to);
      set_pallete_range(temp, from, to, TRUE);
      set_pallete_range(temp, from, to, TRUE);
   }

   set_pallete_range(dest, from, to, TRUE);
}



/* fade_in_range:
 *  Fades from a solid black pallete to p, at the specified speed (1 is
 *  the slowest, 64 is instantaneous). Only affects colors between from and 
 *  to (inclusive, pass 0 and 255 to fade the entire pallete).
 */
void fade_in_range(PALLETE p, int speed, int from, int to)
{
   fade_from_range(black_pallete, p, speed, from, to);
}



/* fade_out_range:
 *  Fades from the current pallete to a solid black pallete, at the 
 *  specified speed (1 is the slowest, 64 is instantaneous). Only affects 
 *  colors between from and to (inclusive, pass 0 and 255 to fade the 
 *  entire pallete).
 */
void fade_out_range(int speed, int from, int to)
{
   PALLETE temp;

   get_pallete(temp);
   fade_from_range(temp, black_pallete, speed, from, to);
}



/* fade_from:
 *  Fades from source to dest, at the specified speed (1 is the slowest, 64
 *  is instantaneous).
 */
void fade_from(PALLETE source, PALLETE dest, int speed)
{
   fade_from_range(source, dest, speed, 0, PAL_SIZE-1);
}



/* fade_in:
 *  Fades from a solid black pallete to p, at the specified speed (1 is
 *  the slowest, 64 is instantaneous).
 */
void fade_in(PALLETE p, int speed)
{
   fade_in_range(p, speed, 0, PAL_SIZE-1);
}



/* fade_out:
 *  Fades from the current pallete to a solid black pallete, at the 
 *  specified speed (1 is the slowest, 64 is instantaneous).
 */
void fade_out(int speed)
{
   fade_out_range(speed, 0, PAL_SIZE-1);
}



/* rect:
 *  Draws an outline rectangle.
 */
void rect(BITMAP *bmp, int x1, int y1, int x2, int y2, int color)
{
   if (x2 < x1) {
      x1 ^= x2;
      x2 ^= x1;
      x1 ^= x2;
   }

   if (y2 < y1) {
      y1 ^= y2;
      y2 ^= y1;
      y1 ^= y2;
   }

   hline(bmp, x1, y1, x2, color);

   if (y2 > y1)
      hline(bmp, x1, y2, x2, color);

   if (y2-1 >= y1+1) {
      vline(bmp, x1, y1+1, y2-1, color);

      if (x2 > x1)
	 vline(bmp, x2, y1+1, y2-1, color);
   }
}



/* _normal_rectfill:
 *  Draws a solid filled rectangle, using hline() to do the work.
 */
void _normal_rectfill(BITMAP *bmp, int x1, int y1, int x2, int y2, int color)
{
   int t;

   if (y1 > y2) {
      t = y1;
      y1 = y2;
      y2 = t;
   }

   if (bmp->clip) {
      if (x1 > x2) {
	 t = x1;
	 x1 = x2;
	 x2 = t;
      }

      if (x1 < bmp->cl)
	 x1 = bmp->cl;

      if (x2 >= bmp->cr)
	 x2 = bmp->cr-1;

      if (x2 < x1)
	 return;

      if (y1 < bmp->ct)
	 y1 = bmp->ct;

      if (y2 >= bmp->cb)
	 y2 = bmp->cb-1;

      if (y2 < y1)
	 return;

      bmp->clip = FALSE;
      t = TRUE;
   }
   else
      t = FALSE;

   while (y1 <= y2) {
      hline(bmp, x1, y1, x2, color);
      y1++;
   };

   bmp->clip = t;
}



/* do_line:
 *  Calculates all the points along a line between x1, y1 and x2, y2, 
 *  calling the supplied function for each one. This will be passed a 
 *  copy of the bmp parameter, the x and y position, and a copy of the 
 *  d parameter (so do_line() can be used with putpixel()).
 */
void do_line(BITMAP *bmp, int x1, int y1, int x2, int y2, int d, void (*proc)())
{
   int dx = x2-x1;
   int dy = y2-y1;
   int i1, i2;
   int x, y;
   int dd;

   /* worker macro */
   #define DO_LINE(pri_sign, pri_c, pri_cond, sec_sign, sec_c, sec_cond)     \
   {                                                                         \
      if (d##pri_c == 0) {                                                   \
	 proc(bmp, x1, y1, d);                                               \
	 return;                                                             \
      }                                                                      \
									     \
      i1 = 2 * d##sec_c;                                                     \
      dd = i1 - (sec_sign (pri_sign d##pri_c));                              \
      i2 = dd - (sec_sign (pri_sign d##pri_c));                              \
									     \
      x = x1;                                                                \
      y = y1;                                                                \
									     \
      while (pri_c pri_cond pri_c##2) {                                      \
	 proc(bmp, x, y, d);                                                 \
									     \
	 if (dd sec_cond 0) {                                                \
	    sec_c sec_sign##= 1;                                             \
	    dd += i2;                                                        \
	 }                                                                   \
	 else                                                                \
	    dd += i1;                                                        \
									     \
	 pri_c pri_sign##= 1;                                                \
      }                                                                      \
   }

   if (dx >= 0) {
      if (dy >= 0) {
	 if (dx >= dy) {
	    /* (x1 <= x2) && (y1 <= y2) && (dx >= dy) */
	    DO_LINE(+, x, <=, +, y, >=);
	 }
	 else {
	    /* (x1 <= x2) && (y1 <= y2) && (dx < dy) */
	    DO_LINE(+, y, <=, +, x, >=);
	 }
      }
      else {
	 if (dx >= -dy) {
	    /* (x1 <= x2) && (y1 > y2) && (dx >= dy) */
	    DO_LINE(+, x, <=, -, y, <=);
	 }
	 else {
	    /* (x1 <= x2) && (y1 > y2) && (dx < dy) */
	    DO_LINE(-, y, >=, +, x, >=);
	 }
      }
   }
   else {
      if (dy >= 0) {
	 if (-dx >= dy) {
	    /* (x1 > x2) && (y1 <= y2) && (dx >= dy) */
	    DO_LINE(-, x, >=, +, y, >=);
	 }
	 else {
	    /* (x1 > x2) && (y1 <= y2) && (dx < dy) */
	    DO_LINE(+, y, <=, -, x, <=);
	 }
      }
      else {
	 if (-dx >= -dy) {
	    /* (x1 > x2) && (y1 > y2) && (dx >= dy) */
	    DO_LINE(-, x, >=, -, y, <=);
	 }
	 else {
	    /* (x1 > x2) && (y1 > y2) && (dx < dy) */
	    DO_LINE(-, y, >=, -, x, <=);
	 }
      }
   }
}



/* _normal_line:
 *  Draws a line from x1, y1 to x2, y2, using putpixel() to do the work.
 */
void _normal_line(BITMAP *bmp, int x1, int y1, int x2, int y2, int color)
{
   int sx, sy, dx, dy, t;

   if (x1 == x2) {
      vline(bmp, x1, y1, y2, color);
      return;
   }

   if (y1 == y2) {
      hline(bmp, x1, y1, x2, color);
      return;
   }

   /* use a bounding box to check if the line needs clipping */
   if (bmp->clip) {
      sx = x1;
      sy = y1;
      dx = x2;
      dy = y2;

      if (sx > dx) {
	 t = sx;
	 sx = dx;
	 dx = t;
      }

      if (sy > dy) {
	 t = sy;
	 sy = dy;
	 dy = t;
      }

      if ((sx >= bmp->cr) || (sy >= bmp->cb) || (dx < bmp->cl) || (dy < bmp->ct))
	 return;

      if ((sx >= bmp->cl) && (sy >= bmp->ct) && (dx < bmp->cr) && (dy < bmp->cb))
	 bmp->clip = FALSE;

      t = TRUE;
   }
   else
      t= FALSE;

   do_line(bmp, x1, y1, x2, y2, color, bmp->vtable->putpixel);

   bmp->clip = t;
}



/* do_circle:
 *  Helper function for the circle drawing routines. Calculates the points
 *  in a circle of radius r around point x, y, and calls the specified 
 *  routine for each one. The output proc will be passed first a copy of
 *  the bmp parameter, then the x, y point, then a copy of the d parameter
 *  (so putpixel() can be used as the callback).
 */
void do_circle(BITMAP *bmp, int x, int y, int radius, int d, void (*proc)())
{
   int cx = 0;
   int cy = radius;
   int df = 1 - radius; 
   int d_e = 3;
   int d_se = -2 * radius + 5;

   do {
      proc(bmp, x+cx, y+cy, d); 

      if (cx) 
	 proc(bmp, x-cx, y+cy, d); 

      if (cy) 
	 proc(bmp, x+cx, y-cy, d);

      if ((cx) && (cy)) 
	 proc(bmp, x-cx, y-cy, d); 

      if (cx != cy) {
	 proc(bmp, x+cy, y+cx, d); 

	 if (cx) 
	    proc(bmp, x+cy, y-cx, d);

	 if (cy) 
	    proc(bmp, x-cy, y+cx, d); 

	 if (cx && cy) 
	    proc(bmp, x-cy, y-cx, d); 
      }

      if (df < 0)  {
	 df += d_e;
	 d_e += 2;
	 d_se += 2;
      }
      else { 
	 df += d_se;
	 d_e += 2;
	 d_se += 4;
	 cy--;
      } 

      cx++; 

   } while (cx <= cy);
}



/* circle:
 *  Draws a circle.
 */
void circle(BITMAP *bmp, int x, int y, int radius, int color)
{
   int clip, sx, sy, dx, dy;

   if (bmp->clip) {
      sx = x-radius-1;
      sy = y-radius-1;
      dx = x+radius+1;
      dy = y+radius+1;

      if ((sx >= bmp->cr) || (sy >= bmp->cb) || (dx < bmp->cl) || (dy < bmp->ct))
	 return;

      if ((sx >= bmp->cl) && (sy >= bmp->ct) && (dx < bmp->cr) && (dy < bmp->cb))
	 bmp->clip = FALSE;

      clip = TRUE;
   }
   else
      clip = FALSE;

   do_circle(bmp, x, y, radius, color, bmp->vtable->putpixel);

   bmp->clip = clip;
}



/* circlefill:
 *  Draws a filled circle.
 */
void circlefill(BITMAP *bmp, int x, int y, int radius, int color)
{
   int cx = 0;
   int cy = radius;
   int df = 1 - radius; 
   int d_e = 3;
   int d_se = -2 * radius + 5;
   int clip, sx, sy, dx, dy;

   if (bmp->clip) {
      sx = x-radius-1;
      sy = y-radius-1;
      dx = x+radius+1;
      dy = y+radius+1;

      if ((sx >= bmp->cr) || (sy >= bmp->cb) || (dx < bmp->cl) || (dy < bmp->ct))
	 return;

      if ((sx >= bmp->cl) && (sy >= bmp->ct) && (dx < bmp->cr) && (dy < bmp->cb))
	 bmp->clip = FALSE;

      clip = TRUE;
   }
   else
      clip = FALSE;

   do {
      hline(bmp, x-cy, y-cx, x+cy, color);

      if (cx)
	 hline(bmp, x-cy, y+cx, x+cy, color);

      if (df < 0)  {
	 df += d_e;
	 d_e += 2;
	 d_se += 2;
      }
      else { 
	 if (cx != cy) {
	    hline(bmp, x-cx, y-cy, x+cx, color);

	    if (cy)
	       hline(bmp, x-cx, y+cy, x+cx, color);
	 }

	 df += d_se;
	 d_e += 2;
	 d_se += 4;
	 cy--;
      } 

      cx++; 

   } while (cx <= cy);

   bmp->clip = clip;
}



/* do_ellipse:
 *  Helper function for the ellipse drawing routines. Calculates the points
 *  in an ellipse of radius rx and ry around point x, y, and calls the 
 *  specified routine for each one. The output proc will be passed first a 
 *  copy of the bmp parameter, then the x, y point, then a copy of the d 
 *  parameter (so putpixel() can be used as the callback).
 */
void do_ellipse(BITMAP *bmp, int x, int y, int rx, int ry, int d, void (*proc)())
{
   int ix, iy;
   int h, i, j, k;
   int oh, oi, oj, ok;

   if (rx < 1) 
      rx = 1; 

   if (ry < 1) 
      ry = 1;

   h = i = j = k = 0xFFFF;

   if (rx > ry) {
      ix = 0; 
      iy = rx * 64;

      do {
	 oh = h;
	 oi = i;
	 oj = j;
	 ok = k;

	 h = (ix + 32) >> 6; 
	 i = (iy + 32) >> 6;
	 j = (h * ry) / rx; 
	 k = (i * ry) / rx;

	 if (((h != oh) || (k != ok)) && (h < oi)) {
	    proc(bmp, x+h, y+k, d); 
	    if (h) 
	       proc(bmp, x-h, y+k, d);
	    if (k) {
	       proc(bmp, x+h, y-k, d); 
	       if (h)
		  proc(bmp, x-h, y-k, d);
	    }
	 }

	 if (((i != oi) || (j != oj)) && (h < i)) {
	    proc(bmp, x+i, y+j, d); 
	    if (i)
	       proc(bmp, x-i, y+j, d);
	    if (j) {
	       proc(bmp, x+i, y-j, d); 
	       if (i)
		  proc(bmp, x-i, y-j, d);
	    }
	 }

	 ix = ix + iy / rx; 
	 iy = iy - ix / rx;

      } while (i > h);
   } 
   else {
      ix = 0; 
      iy = ry * 64;

      do {
	 oh = h;
	 oi = i;
	 oj = j;
	 ok = k;

	 h = (ix + 32) >> 6; 
	 i = (iy + 32) >> 6;
	 j = (h * rx) / ry; 
	 k = (i * rx) / ry;

	 if (((j != oj) || (i != oi)) && (h < i)) {
	    proc(bmp, x+j, y+i, d); 
	    if (j)
	       proc(bmp, x-j, y+i, d);
	    if (i) {
	       proc(bmp, x+j, y-i, d); 
	       if (j)
		  proc(bmp, x-j, y-i, d);
	    }
	 }

	 if (((k != ok) || (h != oh)) && (h < oi)) {
	    proc(bmp, x+k, y+h, d); 
	    if (k)
	       proc(bmp, x-k, y+h, d);
	    if (h) {
	       proc(bmp, x+k, y-h, d); 
	       if (k)
		  proc(bmp, x-k, y-h, d);
	    }
	 }

	 ix = ix + iy / ry; 
	 iy = iy - ix / ry;

      } while(i > h);
   }
}



/* ellipse:
 *  Draws an ellipse.
 */
void ellipse(BITMAP *bmp, int x, int y, int rx, int ry, int color)
{
   int clip, sx, sy, dx, dy;

   if (bmp->clip) {
      sx = x-rx-1;
      sy = y-ry-1;
      dx = x+rx+1;
      dy = y+ry+1;

      if ((sx >= bmp->cr) || (sy >= bmp->cb) || (dx < bmp->cl) || (dy < bmp->ct))
	 return;

      if ((sx >= bmp->cl) && (sy >= bmp->ct) && (dx < bmp->cr) && (dy < bmp->cb))
	 bmp->clip = FALSE;

      clip = TRUE;
   }
   else
      clip = FALSE;

   do_ellipse(bmp, x, y, rx, ry, color, bmp->vtable->putpixel);

   bmp->clip = clip;
}



/* ellipsefill:
 *  Draws a filled ellipse.
 */
void ellipsefill(BITMAP *bmp, int x, int y, int rx, int ry, int color)
{
   int ix, iy;
   int a, b, c, d;
   int da, db, dc, dd;
   int na, nb, nc, nd;
   int clip, sx, sy, dx, dy;

   if (bmp->clip) {
      sx = x-rx-1;
      sy = y-ry-1;
      dx = x+rx+1;
      dy = y+ry+1;

      if ((sx >= bmp->cr) || (sy >= bmp->cb) || (dx < bmp->cl) || (dy < bmp->ct))
	 return;

      if ((sx >= bmp->cl) && (sy >= bmp->ct) && (dx < bmp->cr) && (dy < bmp->cb))
	 bmp->clip = FALSE;

      clip = TRUE;
   }
   else
      clip = FALSE;

   if (rx < 1)
      rx = 1;

   if (ry < 1) 
      ry = 1;

   if (rx > ry) {
      dc = -1;
      dd = 0xFFFF;
      ix = 0; 
      iy = rx * 64;
      na = 0; 
      nb = (iy + 32) >> 6;
      nc = 0; 
      nd = (nb * ry) / rx;

      do {
	 a = na; 
	 b = nb; 
	 c = nc; 
	 d = nd;

	 ix = ix + (iy / rx);
	 iy = iy - (ix / rx);
	 na = (ix + 32) >> 6; 
	 nb = (iy + 32) >> 6;
	 nc = (na * ry) / rx; 
	 nd = (nb * ry) / rx;

	 if ((c > dc) && (c < dd)) {
	    hline(bmp, x-b, y+c, x+b, color);
	    if (c)
	       hline(bmp, x-b, y-c, x+b, color);
	    dc = c;
	 }

	 if ((d < dd) && (d > dc)) { 
	    hline(bmp, x-a, y+d, x+a, color);
	    hline(bmp, x-a, y-d, x+a, color);
	    dd = d;
	 }

      } while(b > a);
   } 
   else {
      da = -1;
      db = 0xFFFF;
      ix = 0; 
      iy = ry * 64; 
      na = 0; 
      nb = (iy + 32) >> 6;
      nc = 0; 
      nd = (nb * rx) / ry;

      do {
	 a = na; 
	 b = nb; 
	 c = nc; 
	 d = nd; 

	 ix = ix + (iy / ry); 
	 iy = iy - (ix / ry);
	 na = (ix + 32) >> 6; 
	 nb = (iy + 32) >> 6;
	 nc = (na * rx) / ry; 
	 nd = (nb * rx) / ry;

	 if ((a > da) && (a < db)) {
	    hline(bmp, x-d, y+a, x+d, color); 
	    if (a)
	       hline(bmp, x-d, y-a, x+d, color);
	    da = a;
	 }

	 if ((b < db) && (b > da)) { 
	    hline(bmp, x-c, y+b, x+c, color);
	    hline(bmp, x-c, y-b, x+c, color);
	    db = b;
	 }

      } while(b > a);
   }

   bmp->clip = clip;
}



/* do_arc:
 *  Helper function for the arc function. Calculates the points in an arc
 *  of radius r around point x, y, going anticlockwise from fixed point
 *  binary angle ang1 to ang2, and calls the specified routine for each one. 
 *  The output proc will be passed first a copy of the bmp parameter, then 
 *  the x, y point, then a copy of the d parameter (so putpixel() can be 
 *  used as the callback).
 */
void do_arc(BITMAP *bmp, int x, int y, fixed ang1, fixed ang2, int r, int d, void (*proc)(BITMAP *, int, int, int))
{
   unsigned long rr = r*r;
   unsigned long rr1, rr2, rr3;
   int px, py;
   int ex, ey;
   int px1, px2, px3;
   int py1, py2, py3;
   int d1, d2, d3;
   int ax, ay;
   int q, qe;
   long tg_cur, tg_end;
   int done = FALSE;

   rr1 = r;
   rr2 = itofix(x);
   rr3 = itofix(y);

   /* evaluate the start point and the end point */
   px = fixtoi(rr2 + rr1 * fcos(ang1));
   py = fixtoi(rr3 - rr1 * fsin(ang1));
   ex = fixtoi(rr2 + rr1 * fcos(ang2));
   ey = fixtoi(rr3 - rr1 * fsin(ang2));

   /* start quadrant */
   if (px >= x) {
      if (py <= y)
	 q = 1;                           /* quadrant 1 */
      else
	 q = 4;                           /* quadrant 4 */
   }
   else {
      if (py < y)
	 q = 2;                           /* quadrant 2 */
      else
	 q = 3;                           /* quadrant 3 */
   }

   /* end quadrant */
   if (ex >= x) {
      if (ey <= y)
	 qe = 1;                          /* quadrant 1 */
      else
	 qe = 4;                          /* quadrant 4 */
   }
   else {
      if (ey < y)
	 qe = 2;                          /* quadrant 2 */
      else
	 qe = 3;                          /* quadrant 3 */
   }

   #define loc_tg(_y, _x)  (_x-x) ? itofix(_y-y)/(_x-x) : itofix(_y-y)

   tg_end = loc_tg(ey, ex);

   while (!done) {
      proc(bmp, px, py, d);

      /* from here, we have only 3 possible direction of movement, eg.
       * for the first quadrant:
       *
       *    OOOOOOOOO
       *    OOOOOOOOO
       *    OOOOOO21O
       *    OOOOOO3*O
       */

      /* evaluate the 3 possible points */
      switch (q) {

	 case 1:
	    px1 = px;
	    py1 = py-1;
	    px2 = px-1;
	    py2 = py-1;
	    px3 = px-1;
	    py3 = py;

	    /* 2nd quadrant check */
	    if (px != x) {
	       break;
	    }
	    else {
	       /* we were in the end quadrant, changing is illegal. Exit. */
	       if (qe == q)
		  done = TRUE;
	       q++;
	    }
	    /* fall through */

	 case 2:
	    px1 = px-1;
	    py1 = py;
	    px2 = px-1;
	    py2 = py+1;
	    px3 = px;
	    py3 = py+1;

	    /* 3rd quadrant check */
	    if (py != y) {
	       break;
	    }
	    else {
	       /* we were in the end quadrant, changing is illegal. Exit. */
	       if (qe == q)
		  done = TRUE;
	       q++;
	    }
	    /* fall through */

	 case 3:
	    px1 = px;
	    py1 = py+1;
	    px2 = px+1;
	    py2 = py+1;
	    px3 = px+1;
	    py3 = py;

	    /* 4th quadrant check */
	    if (px != x) {
	       break;
	    }
	    else {
	       /* we were in the end quadrant, changing is illegal. Exit. */
	       if (qe == q)
		  done = TRUE;
	       q++;
	    }
	    /* fall through */

	 case 4:
	    px1 = px+1;
	    py1 = py;
	    px2 = px+1;
	    py2 = py-1;
	    px3 = px;
	    py3 = py-1;

	    /* 1st quadrant check */
	    if (py == y) {
	       /* we were in the end quadrant, changing is illegal. Exit. */
	       if (qe == q)
		  done = TRUE;

	       q = 1;
	       px1 = px;
	       py1 = py-1;
	       px2 = px-1;
	       py2 = py-1;
	       px3 = px-1;
	       py3 = py;
	    }
	    break;

	 default:
	    return;
      }

      /* now, we must decide which of the 3 points is the right point.
       * We evaluate the distance from center and, then, choose the
       * nearest point.
       */
      ax = x-px1;
      ay = y-py1;
      rr1 = ax*ax + ay*ay;

      ax = x-px2;
      ay = y-py2;
      rr2 = ax*ax + ay*ay;

      ax = x-px3;
      ay = y-py3;
      rr3 = ax*ax + ay*ay;

      /* difference from the main radius */
      if (rr1 > rr)
	 d1 = rr1-rr;
      else
	 d1 = rr-rr1;
      if (rr2 > rr)
	 d2 = rr2-rr;
      else
	 d2 = rr-rr2;
      if (rr3 > rr)
	 d3 = rr3-rr;
      else
	 d3 = rr-rr3;

      /* what is the minimum? */
      if (d1 <= d2) {
	 px = px1;
	 py = py1;
      }
      else if (d2 <= d3) {
	 px = px2;
	 py = py2;
      }
      else {
	 px = px3;
	 py = py3;
      }

      /* are we in the final quadrant? */
      if (qe == q) {
	 tg_cur = loc_tg(py, px);

	 /* is the arc finished? */
	 switch (q) {

	    case 1:
	       /* end quadrant = 1? */
	       if (tg_cur <= tg_end)
		  done = TRUE;
	       break;

	    case 2:
	       /* end quadrant = 2? */
	       if (tg_cur <= tg_end)
		  done = TRUE;
	       break;

	    case 3:
	       /* end quadrant = 3? */
	       if (tg_cur <= tg_end)
		  done = TRUE;
	       break;

	    case 4:
	       /* end quadrant = 4? */
	       if (tg_cur <= tg_end)
		  done = TRUE;
	       break;
	 }
      }
   }

   /* draw the last evaluated point */
   proc(bmp, px, py, d);
}



/* arc:
 *  Draws an arc.
 */
void arc(BITMAP *bmp, int x, int y, fixed ang1, fixed ang2, int r, int color)
{
   do_arc(bmp, x, y, ang1, ang2, r, color, bmp->vtable->putpixel);
}

