/*
  Example program 1. Copyright 1997 Liouros Thanasis.

  This file is part of PLATFORMTRON game library. See the file license.txt
  for more details. If that file is missing then please contact me.

  This program demonstrates some very basic concepts of PLATFORMTRON game
  library. There are comments everywhere so it should be self-explanatory.
*/



#include "world.h"
#include "sprite.h"
#include "mem.h"
#include "plvga.h"
#include <stdlib.h>
#include <conio.h>

// my world
TOworld myworld;

// this is an array to store the data of 3 tiles
char mytiles[3][256];

// my background layer
unsigned short mybacklayer[22*15]= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0,
			   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			   0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			   0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			   0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0,
			   0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0,
			   0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0,
			   0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			   0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			   0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			   0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0,
			   0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0,
			   0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0,
			   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
			};

void exitaction(TOobject *s)
{
 char ch;
 static char ddx=1,ddy=1;
 TOworld *w=s->owner;       // get a pointer to myworld

 if ( kbhit()) ch=getch(); else ch='\0';

 switch (ch)
 {
  case '\033': w->exitbit=1;           // if ESCAPE then set exitbit to 1
  case 'j' : ddx=-abs(ddx);break;      // scroll left
  case 'l' : ddx=abs(ddx);break;       // scroll right
  case 'i':  ddy=-abs(ddy);break;      // scroll up
  case 'k':  ddy=abs(ddy);break;       // scroll down
 };

 w->scrollR(ddx);
 w->scrollD(ddy);
};


void main()
{
 int i,j;
 TOobject exitobj;     // this is the exit object
		       // it is used to make TOworld::animate() exit
		       // and in this example to do some scrolling too

 // vga and 386 required
 if ( !pl_isvga() || !pl_is386())
  {  printf("386 and VGA card required\n") ;return;
  }
 // check the memory left to avoid checking what individual routines return
 // i tried to guess how much memory this program needs, 10000 bytes should
 // be enough
 if (coreleft() < 10000)
 { printf("Not enough memory \n"); return;
 }

// set the dimensions of the world in tiles
// length can never be less than 22 and height can never be less than 15
 myworld.setdimensions(22,15);
// tell myworld to use mybacklayer as the background layer
 myworld.setlayer(LAY_BACK,mybacklayer);
// let myworld reserve memory for the foreground layer
// you must always have background AND foreground layer
 myworld.makelayer(LAY_FORE);
// Make the foreground layer ,an empty layer
// value 0xffff means dont display any tile
 for (i=0;i<22*15;i++) myworld.Pforelayer[i]=0xffff;

// tell myworld to make an array of 3 pointers to tiles
 myworld.maketiles(3);
// set color 246 to red
 myworld.rgbset(246,56,0,0);
// make a funny image for the first tile
 for (i=0;i<16;i++)
 for (j=0;j<16;j++)
  mytiles[0][i*16+j]=67+j+i;
// tile 1 is a red tile
 memset(mytiles[1],246,256);

 // convert the tile data to 4PLANES format
 // tile data must always be in 4PLANES format
 for (i=0;i<3;i++) myworld.tileto4planes(mytiles[i]);

 // tell myworld which tiles to use
 for (i=0;i<2;i++) myworld.settileno(i,mytiles[i]);

 // initialize the exit object
 exitobj.init(0,0,0,0,0);
// set the active action of exit object to exitaction()
 exitobj.nextaction=exitaction;
// add exitobject to myworld, object list: 0 , priority list: 0
 myworld.addobj(0,&exitobj,0);


 clrscr();
 printf("Example program 1. Copyright 1997 Liouros Thanasis\n");
 printf("--------------------------------------------------\n");
 printf("Use i,j,k,l to move the view window. ESC to exit.\n");
 printf("\nPress <ENTER> ...\n");
 getchar();


// set the special mode required by PLATFORMTRON
 mx320x200(44);
// jump to position (0,0) in the world
 myworld.jumpto(0,0);
// and GO!!!!!
 myworld.animate();
// set text mode
 pl_setvideomode(3);
}
