/*
  Example program 3. Copyright 1997 Liouros Thanasis.

  This file is part of PLATFORMTRON game library. See the file license.txt
  for more details. If that file is missing then please contact me.

  This example program demonstrates the collision detection routine.
  It also introduces the use of the low level keyboard handler.
*/

#include "world.h"
#include "sprite.h"
#include <mem.h>
#include "plvga.h"
#include "plkeys.h"
#include <stdlib.h>
#include <conio.h>

// my world
TOworld myworld;

// extend TOsprite to include a pointer to sign1
struct  TOsign2:public TOsprite
{
  TOobject *psign1;
};

// this is an array to store the data of 3 tiles
char mytile[256];

void exitaction(TOobject *s)
{
 char ch;
 static char ddx=1,ddy=1;
 TOworld *w=s->owner;       // get a pointer to myworld

      if (pl_testkey(mcESC)) w->exitbit=1;
    else
    {
      if (pl_testkey(mcNUM4)) w->scrollR(-ddx);
 else if (pl_testkey(mcNUM6)) w->scrollR(ddx);

     if (pl_testkey(mcNUM8)) w->scrollD(-ddy);
 else if (pl_testkey(mcNUM2)) w->scrollD(ddy);
    }

};


// this is the action function that moves the cloud in the world

void sign1action(TOobject *s)
{

     if (pl_testkey(mcW)) s->y--;
else if (pl_testkey(mcS)) s->y++;
     if (pl_testkey(mcA)) s->x--;
else if (pl_testkey(mcD)) s->x++;


}

void sign2action(TOsign2 *s)
{
  TOworld *w=s->owner;

  if (w->collision(s,s->psign1) ) w->rgbset(0,34,0,0);
  else w->rgbset(0,0,0,0);
};



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
 TOsprite sign1;
 TOsign2  sign2;

 TPbitmap mybitmaps[2];


 if ( !pl_isvga() || !pl_is386())
  {  printf("386 and VGA card required\n") ;return;
  }
 if (coreleft() < 10000)
 { printf("Not enough memory \n"); return;
 }
 if (loadraw("sign1.bin",mybitmaps[0]))
 { printf("Could not load sign1.bin\n"); return;
 }
 if (loadraw("sign2.bin",mybitmaps[1]))
 { printf("Could not load sign2.bin\n"); return;
 }
 if (loadpal("sign.pal",&myworld))
 { printf("Could not load cloud.pal\n"); return;
 }


// Layers
 myworld.setdimensions(22,15);


 myworld.makelayer(LAY_BACK);
 // initialize the background layer
 for (i=0;i<22;i++) for (j=0;j<15;j++) myworld.setbackxy(i,j,0);
 myworld.makelayer(LAY_FORE);
 // another way to initialize a layer
 for (i=0;i<22*15;i++) myworld.Pforelayer[i]=0xffff;

// Tiles
 myworld.maketiles(1);
 memset(mytile,0,256);
 myworld.tileto4planes(mytile);
 myworld.settileno(0,mytile);

//////////// Bitmaps
 // make an array of 1 pointer to bitmaps
 myworld.makebitmaps(2);
 // try to store the bitmap in EMS memory

 for (i=0;i<2;i++)
 if (myworld.storebitmap(i,mybitmaps[i]))
  myworld.setbitmapi(i,mybitmaps[i]); // if error then keep the bitmap in conventional memory
 else
  free(mybitmaps[i]);

//////// frame descriptors
// make an array of 1 pointer to frame descriptors
 myworld.makeframesdescrs(2);
 // reserve memory for the first descriptor which consists of a single
 // bitmap (bitmap number 0: mybitmaps[0]) and request collision mask
 myworld.make1bitmapdescr(0,0,1);
 // reserve memory for the second descriptor which consists of a single
 // bitmap (bitmap number 1: mybitmaps[1]) and request no collision mask
 myworld.make1bitmapdescr(1,1,0);
 myworld.makeframecolidmask(1); // request collision mask now

/////////////objects
 sign1.init(1,85,115,mybitmaps[0]->len,mybitmaps[0]->hei,0);
 sign1.nextaction=sign1action;    // no action function
 myworld.addobj(0,&sign1,0);
////
 sign2.init(2,55,115,mybitmaps[1]->len,mybitmaps[1]->hei,1);
 sign2.nextaction=(TPaction) sign2action;
 sign2.psign1=myworld.getobjpointer(1);
 myworld.addobj(0,&sign2,0);

// exit object
 exitobj.init(0,0,0,0,0);
 exitobj.nextaction=exitaction;
 myworld.addobj(0,&exitobj,0);

 clrscr();
 printf("Example program 1. Copyright 1997 Liouros Thanasis\n");
 printf("--------------------------------------------------\n");
 printf("Use a,s,d,w to move the sign.\n");
 printf("Use the arrow keys to move the view window. ESC to exit.\n");
 printf("\nPress <ENTER> ...\n");
 getchar();


 pl_installkeys();
 mx320x200(44);
 myworld.jumpto(0,0);
 myworld.animate();

 pl_keysdone();
 pl_setvideomode(3);

}
