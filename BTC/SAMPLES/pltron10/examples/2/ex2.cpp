/*
  Example program 2. Copyright 1997 Liouros Thanasis.

  This file is part of PLATFORMTRON game library. See the file license.txt
  for more details. If that file is missing then please contact me.

  A very simple example of a moving sprite.
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
unsigned short mybacklayer[27*15]= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0,
                           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                           2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                           2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,  2, 2, 2, 2, 2
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


// this is the action function that moves the cloud in the world

void cloudgo(TOobject *s)
{
   if ( (s->x >290) || (s->x<10) ) s->dx=-s->dx;
   s->x+=s->dx;
}


short loadpal(char *fname, TOworld *w)
{
  FILE *f;

  if ( !(f=fopen(fname,"rb")) ) return ERR_FOPEN;
  if ( fread(w->palette,768,1,f)!=1) return ERR_FREAD;

  fclose(f);
  return 0;
}


short loadraw(char *fname,TPbitmap &buf)
{
  FILE *f;
  unsigned short l,h;

  if ( !(f=fopen(fname,"rb")) ) return ERR_FOPEN;
  fread(&l,1,2,f);
  fread(&h,1,2,f);

  if ( ! ( buf= (TPbitmap)malloc( sizeof(Tbitmap)-65000+l*h)) )
   return ERR_OUTOFMEM;
  fread(buf->data,l*h,1,f);
  buf->len=l;
  buf->hei=h;
  buf->drawmethod=DR_THRU;

  fclose(f);
  return 0;
}



void main()
{
 int i,j;
 TOobject exitobj;
 TOsprite cloud;       // the cloud object

 TPbitmap mybitmap;   // the cloud bitmap


 if ( !pl_isvga() || !pl_is386())
  {  printf("386 and VGA card required\n") ;return;
  }
 if (coreleft() < 10000)
 { printf("Not enough memory \n"); return;
 }
 if (loadraw("cloud.bin",mybitmap))
 { printf("Could not load cloud.bin\n"); return;
 }
 if (loadpal("cloud.pal",&myworld))
 { printf("Could not load cloud.pal\n"); return;
 }


 clrscr();
 printf("Example program 2. Copyright 1997 Liouros Thanasis\n");
 printf("--------------------------------------------------\n");
 printf("Use i,j,k,l to move the view window. ESC to exit.\n");
 printf("\nPress <ENTER> ...\n");
 getchar();


// Layers
 myworld.setdimensions(27,15);
 myworld.setlayer(LAY_BACK,mybacklayer);
 myworld.makelayer(LAY_FORE);
 for (i=0;i<27*15;i++) myworld.Pforelayer[i]=0xffff;
// Tiles
 myworld.maketiles(3);
 memset(mytiles[0],135,256);
 memset(mytiles[1],255,256);
 memset(mytiles[2],3,256);
 for (i=0;i<3;i++) myworld.tileto4planes(mytiles[i]);
 for (i=0;i<3;i++) myworld.settileno(i,mytiles[i]);

//////////// Bitmaps
 // make an array of 1 pointer to bitmaps
 myworld.makebitmaps(1);
 // try to store the bitmap in EMS memory
 if (myworld.storebitmap(0,mybitmap))
  myworld.setbitmapi(0,mybitmap); // if error then keep the bitmap in conventional memory
 else
 {
  printf("Bitmap stored in EMS memory. Press <enter>\n");
  free(mybitmap); // else free the conventional RAM since PLATFORMTRON does not
		      // use it
  getchar();
 }


//////// frame descriptors
// make an array of 1 pointer to frame descriptors
 myworld.makeframesdescrs(1);
 // reserve memory for the first descriptor which consists of a single
 // bitmap (bitmap number 0: mybitmap) and request no collision mask
 myworld.make1bitmapdescr(0,0,0);


/////////////objects
 // initialize cloud, id:1   start coordinates: 15,25
//                  slen: mybitmap->len   shei: mybitmap->hei
//                  framenow: 0
 cloud.init(1,15,25,mybitmap->len,mybitmap->hei,0);
// set the active action function of the cloud
 cloud.nextaction=cloudgo;
// horizontal speed, 1 pixel/cycle
 cloud.dx = 1;
// add cloud to object list 0 , priority list 0
 myworld.addobj(0,&cloud,0);


// exit object
 exitobj.init(0,0,0,0,0);
 exitobj.nextaction=exitaction;
 myworld.addobj(0,&exitobj,0);



 mx320x200(44);
 myworld.jumpto(0,0);
 myworld.animate();
 pl_setvideomode(3);
}
