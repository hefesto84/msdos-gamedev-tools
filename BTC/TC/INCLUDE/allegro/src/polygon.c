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
 *      The 2d polygon rasteriser.
 *
 *      By Shawn Hargreaves.
 *
 *      See readme.txt for copyright information.
 */


#include <limits.h>

#include "internal.h"



/* fill_edge_structure:
 *  Polygon helper function: initialises an edge structure for the 2d
 *  rasteriser.
 */
void fill_edge_structure(POLYGON_EDGE *edge, int *i1, int *i2)
{
   if (i2[1] < i1[1]) {
      int *it;

      it = i1;
      i1 = i2;
      i2 = it;
   }

   edge->top = i1[1];
   edge->bottom = i2[1] - 1;
   edge->dx = ((i2[0] - i1[0]) << POLYGON_FIX_SHIFT) / (i2[1] - i1[1]);
   edge->x = (i1[0] << POLYGON_FIX_SHIFT) + (1<<(POLYGON_FIX_SHIFT-1)) - 1;
   edge->prev = NULL;
   edge->next = NULL;

   if (edge->dx < 0)
      edge->x += MIN(edge->dx+(1<<POLYGON_FIX_SHIFT), 0);

   edge->w = MAX(ABS(edge->dx)-(1<<POLYGON_FIX_SHIFT), 0);
}



/* _add_edge:
 *  Adds an edge structure to a linked list, returning the new head pointer.
 */
POLYGON_EDGE *_add_edge(POLYGON_EDGE *list, POLYGON_EDGE *edge, int sort_by_x)
{
   POLYGON_EDGE *pos = list;
   POLYGON_EDGE *prev = NULL;

   if (sort_by_x) {
      while ((pos) && ((pos->x + (pos->w + pos->dx) / 2) < 
		       (edge->x + (edge->w + edge->dx) / 2))) {
	 prev = pos;
	 pos = pos->next;
      }
   }
   else {
      while ((pos) && (pos->top < edge->top)) {
	 prev = pos;
	 pos = pos->next;
      }
   }

   edge->next = pos;
   edge->prev = prev;

   if (pos)
      pos->prev = edge;

   if (prev) {
      prev->next = edge;
      return list;
   }
   else
      return edge;
}



/* _remove_edge:
 *  Removes an edge structure from a list, returning the new head pointer.
 */
POLYGON_EDGE *_remove_edge(POLYGON_EDGE *list, POLYGON_EDGE *edge)
{
   if (edge->next) 
      edge->next->prev = edge->prev;

   if (edge->prev) {
      edge->prev->next = edge->next;
      return list;
   }
   else
      return edge->next;
}



/* polygon:
 *  Draws a filled polygon with an arbitrary number of corners. Pass the 
 *  number of vertices, then an array containing a series of x, y points 
 *  (a total of vertices*2 values).
 */
void polygon(BITMAP *bmp, int vertices, int *points, int color)
{
   int c;
   int top = INT_MAX;
   int bottom = INT_MIN;
   int *i1, *i2;
   POLYGON_EDGE *edge, *next_edge;
   POLYGON_EDGE *active_edges = NULL;
   POLYGON_EDGE *inactive_edges = NULL;

   /* allocate some space and fill the edge table */
   _grow_scratch_mem(sizeof(POLYGON_EDGE) * vertices);

   edge = (POLYGON_EDGE *)_scratch_mem;
   i1 = points;
   i2 = points + (vertices-1) * 2;

   for (c=0; c<vertices; c++) {
      if (i1[1] != i2[1]) {
	 fill_edge_structure(edge, i1, i2);

	 if (edge->bottom >= edge->top) {

	    if (edge->top < top)
	       top = edge->top;

	    if (edge->bottom > bottom)
	       bottom = edge->bottom;

	    inactive_edges = _add_edge(inactive_edges, edge, FALSE);
	    edge++;
	 }
      }
      i2 = i1;
      i1 += 2;
   }

   if (bottom >= bmp->cb)
      bottom = bmp->cb-1;

   /* for each scanline in the polygon... */
   for (c=top; c<=bottom; c++) {

      /* check for newly active edges */
      edge = inactive_edges;
      while ((edge) && (edge->top == c)) {
	 next_edge = edge->next;
	 inactive_edges = _remove_edge(inactive_edges, edge);
	 active_edges = _add_edge(active_edges, edge, TRUE);
	 edge = next_edge;
      }

      /* draw horizontal line segments */
      edge = active_edges;
      while ((edge) && (edge->next)) {
	 hline(bmp, edge->x>>POLYGON_FIX_SHIFT, c, 
	       (edge->next->x+edge->next->w)>>POLYGON_FIX_SHIFT, color);
	 edge = edge->next->next;
      }

      /* update edges, sorting and removing dead ones */
      edge = active_edges;
      while (edge) {
	 next_edge = edge->next;
	 if (c >= edge->bottom) {
	    active_edges = _remove_edge(active_edges, edge);
	 }
	 else {
	    edge->x += edge->dx;
	    while ((edge->prev) && 
		   (edge->x+edge->w/2 < edge->prev->x+edge->prev->w/2)) {
	       if (edge->next)
		  edge->next->prev = edge->prev;
	       edge->prev->next = edge->next;
	       edge->next = edge->prev;
	       edge->prev = edge->prev->prev;
	       edge->next->prev = edge;
	       if (edge->prev)
		  edge->prev->next = edge;
	       else
		  active_edges = edge;
	    }
	 }
	 edge = next_edge;
      }
   }
}



/* triangle:
 *  Draws a filled triangle between the three points. Note: this depends 
 *  on a dodgy assumption about parameter passing conventions. I assume that 
 *  the point coordinates are all on the stack in consecutive locations, so 
 *  I can pass that block of stack memory as the array for polygon() without 
 *  bothering to copy the data to a temporary location.
 */
void triangle(BITMAP *bmp, int x1, int y1, int x2, int y2, int x3, int y3, int color)
{
#if !defined __GNUC__ || !defined __i386__
   int point[6];
#endif

   if (bmp->vtable->triangle)
      if (bmp->vtable->triangle(bmp, x1, y1, x2, y2, x3, y3, color))
	 return;

#if defined __GNUC__ && defined __i386__
   polygon(bmp, 3, &x1, color);
#else
   point[0] = x1; point[1] = y1;
   point[2] = x2; point[3] = y2;
   point[4] = x3; point[5] = y3;
   polygon(bmp, 3, point, color);
#endif
}

