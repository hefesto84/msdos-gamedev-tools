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
 *      Table of functions for drawing onto 24 bit linear bitmaps.
 *
 *      By Shawn Hargreaves.
 *
 *      See readme.txt for copyright information.
 */


#include "internal.h"



#ifdef ALLEGRO_COLOR24


void _linear_draw_sprite24_end();
void _linear_blit24_end();


GFX_VTABLE __linear_vtable24 =
{
   BMP_TYPE_LINEAR,
   24,
   MASK_COLOR_24,
   FALSE,

   NULL,
   _linear_getpixel24,
   _linear_putpixel24,
   _linear_vline24,
   _linear_hline24,
   _normal_line,
   _normal_rectfill,
   NULL,
   _linear_draw_sprite24,
   _linear_draw_256_sprite24,
   _linear_draw_sprite_v_flip24,
   _linear_draw_sprite_h_flip24,
   _linear_draw_sprite_vh_flip24,
   _linear_draw_trans_sprite24,
   _linear_draw_lit_sprite24,
   _linear_draw_rle_sprite24,
   _linear_draw_trans_rle_sprite24,
   _linear_draw_lit_rle_sprite24,
   _linear_draw_character24,
   _linear_textout_fixed24,
   _linear_blit24,
   _linear_blit24,
   _linear_blit24,
   _linear_blit24,
   _linear_blit_backward24,
   _linear_masked_blit24,
   _linear_clear_to_color24,
   _linear_draw_sprite24_end,
   _linear_blit24_end
};


#endif
