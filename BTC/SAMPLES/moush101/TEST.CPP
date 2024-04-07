
/****************************************************************************

      Mouse example program

	originally by Erik Kangas, in MMOUSE10.ZIP

	modified to use C++ encapsulation by Michael Chen, 7/24/1992
	modified to use my own driver, 7/26/1992
	modified to use check_() functions, 7/26/1992

	program draws random lines;
	press either LMB or RMB to continue.

	drag with LMB to draw lines;
	press RMB to quit.

****************************************************************************/

#include <graphics.h>          // Mouse driver requires BGI graphics
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mouse.hpp"
#include "bgi.hpp"

void main() {

	BGI Graphics;

	Mouse mouse;

	// Display default cursor.

	mouse.show();


	// Draw random lines until either LMB or RMB released.

	randomize();

	mouse.update_region(0,0,getmaxx(),getmaxy());

	do
	{
		setcolor(random(getmaxcolor()+1));
		line(random(getmaxx()+1),random(getmaxy()+1),
		     random(getmaxx()+1),random(getmaxy()+1));
	} while (!(mouse.check_up(LEFT_BUTTON)
		|| mouse.check_up(RIGHT_BUTTON)));

	mouse.clear_all();	// Clear any pending button checks
				// and moved checks

	clearviewport();
	mouse.show();

	// Start drawing lines by dragging with the LMB down; end with RMB.

	setcolor(WHITE);
	int drag=0;
	int x,y,x1,y1;

	do
	{
		mouse.status();

		if (mouse.check_down(LEFT_BUTTON))
		{
		   mouse.hide();
		   x = x1 = mouse.x();		// Store position of cursor
		   y = y1 = mouse.y();
		   drag=1;
		   setwritemode(XOR_PUT);
		   line(x,y,x1,y1);
		   mouse.show();
		};


		if (mouse.check_up(LEFT_BUTTON))
		{
		   mouse.hide();
		   line(x,y,x1,y1);
		   setwritemode(COPY_PUT);
		   line(x,y,mouse.x(),mouse.y());
		   mouse.show();
		   drag=0;
		};

		if (drag
		&& ((mouse.x() != x1) || (mouse.y() != y1)))
		{
		   mouse.hide();
		   setwritemode(XOR_PUT);
		   line (x,y,x1,y1);
		   x1 = mouse.x();
		   y1 = mouse.y();
		   line (x,y,x1,y1);
		   mouse.show();
		};
	}
	while (!mouse.check_up(RIGHT_BUTTON));


	// Exit graphics system (destructor for BGI)


	// Destructor for Mouse mouse calls EndMouse when variable is out of scope

}

