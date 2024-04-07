/* 
 *    Example program for the Allegro library, by Shawn Hargreaves.
 *
 *    This program demonstrates how to use the 3d matrix functions.
 *    It isn't a very elegant or efficient piece of code, but it does 
 *    show the stuff in action. I'll leave it to you to design a proper 
 *    model structure and rendering pipeline: after all, the best way to 
 *    do that sort of stuff varies hugely from one game to another.
 */


#include <stdlib.h>
#include <stdio.h>

#include "allegro.h"


#define NUM_SHAPES         8     /* number of bouncing cubes */

#define NUM_VERTICES       8     /* a cube has eight corners */
#define NUM_FACES          6     /* a cube has six faces */


typedef struct VTX
{
   fixed x, y, z;
} VTX;


typedef struct QUAD              /* four vertices makes a quad */
{
   VTX *vtxlist;
   int v1, v2, v3, v4;
} QUAD;


typedef struct SHAPE             /* store position of a shape */
{
   fixed x, y, z;                /* x, y, z position */
   fixed rx, ry, rz;             /* rotations */
   fixed dz;                     /* speed of movement */
   fixed drx, dry, drz;          /* speed of rotation */
} SHAPE;


VTX points[] =                   /* a cube, centered on the origin */
{
   /* vertices of the cube */
   { -32 << 16, -32 << 16, -32 << 16 },
   { -32 << 16,  32 << 16, -32 << 16 },
   {  32 << 16,  32 << 16, -32 << 16 },
   {  32 << 16, -32 << 16, -32 << 16 },
   { -32 << 16, -32 << 16,  32 << 16 },
   { -32 << 16,  32 << 16,  32 << 16 },
   {  32 << 16,  32 << 16,  32 << 16 },
   {  32 << 16, -32 << 16,  32 << 16 },
};


QUAD faces[] =                   /* group the vertices into polygons */
{
   { points, 0, 3, 2, 1 },
   { points, 4, 5, 6, 7 },
   { points, 0, 1, 5, 4 },
   { points, 2, 3, 7, 6 },
   { points, 0, 4, 7, 3 },
   { points, 1, 2, 6, 5 }
};


SHAPE shapes[NUM_SHAPES];        /* a list of shapes */


/* somewhere to put translated vertices */
VTX output_points[NUM_VERTICES * NUM_SHAPES];
QUAD output_faces[NUM_FACES * NUM_SHAPES];


enum { 
   wireframe,
   flat,
   gcol,
   grgb,
   atex,
   ptex,
   atex_mask,
   ptex_mask,
   atex_lit,
   ptex_lit,
   atex_mask_lit,
   ptex_mask_lit,
   last_mode
} render_mode = wireframe;


int render_type[] = {
   0,
   POLYTYPE_FLAT,
   POLYTYPE_GCOL,
   POLYTYPE_GRGB,
   POLYTYPE_ATEX,
   POLYTYPE_PTEX,
   POLYTYPE_ATEX_MASK,
   POLYTYPE_PTEX_MASK,
   POLYTYPE_ATEX_LIT,
   POLYTYPE_PTEX_LIT,
   POLYTYPE_ATEX_MASK_LIT,
   POLYTYPE_PTEX_MASK_LIT
};

char *mode_desc[] = {
   "Wireframe",
   "Flat shaded",
   "Single color gouraud shaded",
   "Gouraud shaded",
   "Texture mapped",
   "Perspective correct texture mapped",
   "Masked texture mapped",
   "Masked persp. correct texture mapped",
   "Lit texture map",
   "Lit persp. correct texture map",
   "Masked lit texture map",
   "Masked lit persp. correct texture map"
};


BITMAP *texture;



/* initialise shape positions */
void init_shapes()
{
   int c;

   for (c=0; c<NUM_SHAPES; c++) {
      shapes[c].x = (random() & 0xFFFFFF) - 0x800000;
      shapes[c].y = (random() & 0xFFFFFF) - 0x800000;
      shapes[c].z = itofix(768);
      shapes[c].rx = 0;
      shapes[c].ry = 0;
      shapes[c].rz = 0;
      shapes[c].dz =  (random() & 0xFFFFF) - 0x80000;
      shapes[c].drx = (random() & 0x1FFFF) - 0x10000;
      shapes[c].dry = (random() & 0x1FFFF) - 0x10000;
      shapes[c].drz = (random() & 0x1FFFF) - 0x10000;
   }
}


/* update shape positions */
void animate_shapes()
{
   int c;

   for (c=0; c<NUM_SHAPES; c++) {
      shapes[c].z += shapes[c].dz;

      if ((shapes[c].z > itofix(1024)) ||
	  (shapes[c].z < itofix(192)))
	 shapes[c].dz = -shapes[c].dz;

      shapes[c].rx += shapes[c].drx;
      shapes[c].ry += shapes[c].dry;
      shapes[c].rz += shapes[c].drz;
   }
}


/* translate shapes from 3d world space to 2d screen space */
void translate_shapes()
{
   int c, d;
   MATRIX matrix;
   VTX *outpoint = output_points;
   QUAD *outface = output_faces;

   for (c=0; c<NUM_SHAPES; c++) {
      /* build a transformation matrix */
      get_transformation_matrix(&matrix, itofix(1),
				shapes[c].rx, shapes[c].ry, shapes[c].rz,
				shapes[c].x, shapes[c].y, shapes[c].z);

      /* output the vertices */
      for (d=0; d<NUM_VERTICES; d++) {
	 apply_matrix(&matrix, points[d].x, points[d].y, points[d].z, &outpoint[d].x, &outpoint[d].y, &outpoint[d].z);
	 persp_project(outpoint[d].x, outpoint[d].y, outpoint[d].z, &outpoint[d].x, &outpoint[d].y);
      }

      /* output the faces */
      for (d=0; d<NUM_FACES; d++) {
	 outface[d] = faces[d];
	 outface[d].vtxlist = outpoint;
      }

      outpoint += NUM_VERTICES;
      outface += NUM_FACES;
   }
}


/* draw a line (for wireframe display) */
void wire(BITMAP *b, VTX *v1, VTX *v2)
{
   int col = MID(128, 255 - fixtoi(v1->z+v2->z) / 16, 255);
   line(b, fixtoi(v1->x), fixtoi(v1->y), fixtoi(v2->x), fixtoi(v2->y), palette_color[col]);
}


/* draw a quad */
void quad(BITMAP *b, VTX *v1, VTX *v2, VTX *v3, VTX *v4, int mode)
{
   int col;

   /* four vertices */
   V3D vtx1 = { v1->x, v1->y, v1->z, 0,      0,      0 };
   V3D vtx2 = { v2->x, v2->y, v2->z, 31<<16, 0,      0 };
   V3D vtx3 = { v3->x, v3->y, v3->z, 31<<16, 31<<16, 0 };
   V3D vtx4 = { v4->x, v4->y, v4->z, 0,      31<<16, 0 };

   /* cull backfaces */
   if ((mode != POLYTYPE_ATEX_MASK) && (mode != POLYTYPE_PTEX_MASK) &&
       (mode != POLYTYPE_ATEX_MASK_LIT) && (mode != POLYTYPE_PTEX_MASK_LIT) &&
       (polygon_z_normal(&vtx1, &vtx2, &vtx3) < 0))
      return;

   /* set up the vertex color, differently for each rendering mode */
   switch (mode) {

      case POLYTYPE_FLAT:
	 col = MID(128, 255 - fixtoi(v1->z+v2->z) / 16, 255);
	 vtx1.c = vtx2.c = vtx3.c = vtx4.c = palette_color[col];
	 break;

      case POLYTYPE_GCOL:
	 vtx1.c = palette_color[0xD0];
	 vtx2.c = palette_color[0x80];
	 vtx3.c = palette_color[0xB0];
	 vtx4.c = palette_color[0xFF];
	 break;

      case POLYTYPE_GRGB:
	 vtx1.c = 0x000000;
	 vtx2.c = 0x7F0000;
	 vtx3.c = 0xFF0000;
	 vtx4.c = 0x7F0000;
	 break;

      case POLYTYPE_ATEX_LIT:
      case POLYTYPE_PTEX_LIT:
      case POLYTYPE_ATEX_MASK_LIT:
      case POLYTYPE_PTEX_MASK_LIT:
	 vtx1.c = MID(0, 255 - fixtoi(v1->z) / 4, 255);
	 vtx2.c = MID(0, 255 - fixtoi(v2->z) / 4, 255);
	 vtx3.c = MID(0, 255 - fixtoi(v3->z) / 4, 255);
	 vtx4.c = MID(0, 255 - fixtoi(v4->z) / 4, 255);
	 break; 
   }

   /* draw the quad */
   quad3d(b, mode, texture, &vtx1, &vtx2, &vtx3, &vtx4);
}


/* callback for qsort() */
int quad_cmp(const void *e1, const void *e2)
{
   QUAD *q1 = (QUAD *)e1;
   QUAD *q2 = (QUAD *)e2;

   fixed d1 = q1->vtxlist[q1->v1].z + q1->vtxlist[q1->v2].z +
	      q1->vtxlist[q1->v3].z + q1->vtxlist[q1->v4].z;

   fixed d2 = q2->vtxlist[q2->v1].z + q2->vtxlist[q2->v2].z +
	      q2->vtxlist[q2->v3].z + q2->vtxlist[q2->v4].z;

   return d2 - d1;
}


/* draw the shapes calculated by translate_shapes() */
void draw_shapes(BITMAP *b)
{
   int c;
   QUAD *face = output_faces;
   VTX *v1, *v2, *v3, *v4;

   /* depth sort */
   qsort(output_faces, NUM_FACES * NUM_SHAPES, sizeof(QUAD), quad_cmp);

   for (c=0; c < NUM_FACES * NUM_SHAPES; c++) {
      /* find the vertices used by the face */
      v1 = face->vtxlist + face->v1;
      v2 = face->vtxlist + face->v2;
      v3 = face->vtxlist + face->v3;
      v4 = face->vtxlist + face->v4;

      /* draw the face */
      if (render_mode == wireframe) {
	 wire(b, v1, v2);
	 wire(b, v2, v3);
	 wire(b, v3, v4);
	 wire(b, v4, v1);
      }
      else {
	 quad(b, v1, v2, v3, v4, render_type[render_mode]);
      }

      face++;
   }
}


/* RGB -> color mapping table. Not needed, but speeds things up */
RGB_MAP rgb_table;

/* lighting color mapping table */
COLOR_MAP light_table;


void print_progress(int pos)
{
   if ((pos & 3) == 3) {
      printf("*");
      fflush(stdout);
   }
}


int main()
{
   BITMAP *buffer;
   PALLETE pal;
   int c = GFX_AUTODETECT;
   int w = 640;
   int h = 480;
   int bpp = 8;
   int last_retrace_count;

   allegro_init();
   install_keyboard();
   install_mouse();
   install_timer();

   /* color 0 = black */
   pal[0].r = pal[0].g = pal[0].b = 0;

   /* copy the desktop pallete */
   for (c=1; c<64; c++)
      pal[c] = desktop_pallete[c];

   /* make a red gradient */
   for (c=64; c<96; c++) {
      pal[c].r = (c-64)*2;
      pal[c].g = pal[c].b = 0;
   }

   /* make a green gradient */
   for (c=96; c<128; c++) {
      pal[c].g = (c-96)*2;
      pal[c].r = pal[c].b = 0;
   }

   /* set up a greyscale in the top half of the pallete */
   for (c=128; c<256; c++)
      pal[c].r = pal[c].g = pal[c].b = (c-128)/2;

   /* build rgb_map table */
   printf("Generating rgb_map table:\n");
   printf("<................................................................>\r<");
   create_rgb_table(&rgb_table, pal, print_progress);
   rgb_map = &rgb_table;
   printf("\n\n");

   /* build a lighting table */
   printf("Generating lighting table:\n");
   printf("<................................................................>\r<");
   create_light_table(&light_table, pal, 0, 0, 0, print_progress);
   color_map = &light_table;
   printf("\n");

   /* set up the truecolor blending functions */
   set_trans_blender(0, 0, 0, 128);

   /* set the graphics mode */
   set_gfx_mode(GFX_VGA, 320, 200, 0, 0);
   set_pallete(desktop_pallete);

   if (!gfx_mode_select_ex(&c, &w, &h, &bpp)) {
      allegro_exit();
      return 1;
   }

   set_color_depth(bpp);

   if (set_gfx_mode(c, w, h, 0, 0) != 0) {
      allegro_exit();
      printf("Error setting graphics mode\n%s\n\n", allegro_error);
      return 1;
   }

   if (gfx_driver->id == GFX_MODEX) {
      allegro_exit();
      printf("Can't draw 3d polygons in mode-X, sorry!\n\n");
      return 1;
   }

   set_pallete(pal);

   /* make a bitmap for use as a texture map */
   texture = create_bitmap(32, 32);
   clear_to_color(texture, bitmap_mask_color(texture));
   line(texture, 0, 0, 31, 31, palette_color[1]);
   line(texture, 0, 31, 31, 0, palette_color[1]);
   rect(texture, 0, 0, 31, 31, palette_color[1]);
   text_mode(-1);
   textout(texture, font, "dead", 0, 0, palette_color[2]);
   textout(texture, font, "pigs", 0, 8, palette_color[2]);
   textout(texture, font, "cant", 0, 16, palette_color[2]);
   textout(texture, font, "fly.", 0, 24, palette_color[2]);

   /* double buffer the animation */
   buffer = create_bitmap(SCREEN_W, SCREEN_H);

   /* set up the viewport for the perspective projection */
   set_projection_viewport(0, 0, SCREEN_W, SCREEN_H);

   /* initialise the bouncing shapes */
   init_shapes();

   last_retrace_count = retrace_count;

   for (;;) {
      clear(buffer);

      while (last_retrace_count < retrace_count) {
	 animate_shapes();
	 last_retrace_count++;
      }

      translate_shapes();
      draw_shapes(buffer);

      textprintf(buffer, font, 0, 0, palette_color[192], "%s, %d bpp", mode_desc[render_mode], bitmap_color_depth(screen));
      textout(buffer, font, "Press a key to change", 0, 12, palette_color[192]);

      vsync();
      blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H); 

      if (keypressed()) {
	 if ((readkey() & 0xFF) == 27)
	    break;
	 else {
	    render_mode++;
	    if (render_mode >= last_mode)
	       render_mode = wireframe;
	 }
      }
   }

   destroy_bitmap(buffer);
   destroy_bitmap(texture);

   return 0;
}


