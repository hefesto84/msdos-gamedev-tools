/*
  Example program 4 with music. Copyright 1997 Liouros Thanasis.

  This file is part of PLATFORMTRON game library. See the file license.txt
  for more details. If that file is missing then please contact me.

  This is the example program 4 with music in the background.
  If you want to compile and run this program you must download yourself
  midas music library.

  Do not try to run this program under windows 95. The system will crash.
*/

#include <stdio.h>
#include <mem.h>
#include "world.h"
#include "plvga.h"
#include "plkeys.h"
#include "pltimer.h"
#define __BC16__
#include "midas.h"
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <ctype.h>

#define DXSCROLL    1
#define DYSCROLL    1
#define BITMAPSNO   22



struct TOele;

// the main character class

struct TOguy:public TOsprite
{
   void elevatorcheck(TOele *ele);  // checks if the guy is on an elevator
   char onelevator;                 // TRUE if the guy in on an elevator

   void init(unsigned char id0,unsigned short x0,
	     unsigned short y0,unsigned short slen0,unsigned short shei0,
	     signed short framenow0,TPaction nextaction0,
	     signed char dx0,signed char dy0)
  {
    TOsprite::init(id0,x0,y0,slen0,shei0,framenow0);
    nextaction=nextaction0;
    dx=dx0;
    dy=dy0;
    time=0;
    onelevator=0;
  }
};



// the scroll class
struct TOscroll:public TOobject
{
  TOguy *pguy;       // a pointer to our guy
  char xscroll;      // TRUE if the view window must scroll horizontally
  char yscroll;      // TRUE if the view window must scroll vertically

  void init(unsigned char id0,unsigned short x0,
	     unsigned short y0,unsigned short slen0,unsigned short shei0,
	     TPaction nextaction0, TOguy *pguy0)
  {
    TOobject::init(id0,x0,y0,slen0,shei0);
    nextaction=nextaction0;
    pguy=pguy0;
    xscroll=yscroll=0;
  }
};


// the (vertical) elevator class
struct TOele:public TOsprite
   {
      TOguy *pguy;               // a pointer to our guy
      char onelevator;           // TRUE if the guy is on this elevator
      unsigned short uplimit;
      unsigned short downlimit;  // y-coordinates that specify the limits
				 // in which the elevator moves.

   void init(unsigned char id0,unsigned short x0,
	     unsigned short y0,unsigned short slen0,unsigned short shei0,
	     signed short framenow0,TPaction nextaction0,
	     signed char dy0, TOguy *pguy0, unsigned short uplimit0,
	     unsigned short downlimit0)
   {
     TOsprite::init(id0,x0,y0,slen0,shei0,framenow0);
     dy=dy0;
     nextaction=nextaction0;
     uplimit=uplimit0;
     downlimit=downlimit0;
     pguy=pguy0;
     time=0;
     onelevator=0;
   }
};



// the cloud class
struct TOcloud:public TOsprite
{
    unsigned short leftlimit;
    unsigned short rightlimit;  // x-coordinates that specify the limits
				// in which the cloud moves
    unsigned short mintime;
    unsigned short maxtime;    // the cloud releases one thunder every T ticks
			       // of the timer, mintime <= T <= maxtime

   void init(unsigned char id0,unsigned short x0,
	     unsigned short y0,unsigned short slen0,unsigned short shei0,
	     signed short framenow0,TPaction nextaction0,
	     signed char dx0, unsigned short leftlimit0,
	     unsigned short rightlimit0, unsigned short mintime0,
	     unsigned short maxtime0)
   {
     TOsprite::init(id0,x0,y0,slen0,shei0,framenow0);
     nextaction=nextaction0;
     dx=dx0;
     leftlimit=leftlimit0;
     rightlimit=rightlimit0;
     mintime=mintime0;
     maxtime=maxtime0;
     time=0;
  }

};



// ----------------------- action function of the thunder -------------------
void thundergo(TOsprite *o)
{
 // check if we can go down, if not, die in the next frame
 if (o->owner->howfarD(o,o->dy) < o->dy) o->deletebit=1;
 else // go down
  o->y+=o->dy;
}

// --------------------------------------------------------------------------



// ------------------- cloud action function ----------------------------

void cloudgo(TOcloud *o)
{
 unsigned short t;
 TOsprite *thunder;

 t=pl_dtime[0];
 if (o->time>t) o->time-=t; else
 {
  // time counter has reached zero , release a thunder
  o->time=random(o->maxtime-o->mintime)+o->mintime;
  thunder = new TOsprite ;
  thunder->init(0,o->x+10,o->y+10,o->owner->bitmaps[19]->len,o->owner->bitmaps[19]->hei,0);
  thunder->nextaction=(TPaction) thundergo;
  thunder->framenow=20;
  thunder->dy=random(2)+1;
  o->owner->addobj(0,thunder,6);
 }

 if ( o->x > o->rightlimit || o->x < o->leftlimit) o->dx=-o->dx;

 o->x+=o->dx;
}

// --------------------------------------------------------------------------


void guyrun(TOguy *obj);
void guystartjump(TOguy *obj);
void guystartfall(TOguy *obj);
void guyjump(TOguy *obj);
void guyfall(TOguy *obj);
void guystand(TOguy *obj);


// this functions checks if the guy is on elevator ele or has just left
// elevator ele.

void TOguy::elevatorcheck(TOele *ele)
{
 if (ele->onelevator)
 {
  if ( (x<ele->x-13) || (x>ele->x+ele->slen-15) || (y+shei != ele->y) )
  {
    ele->onelevator=0;
    onelevator=0;
  }
 }
 else
 {
   if (
	(x>=ele->x-13) &&
	(x<=ele->x+ele->slen-15) &&
	(y+shei<=ele->y) &&
	(y+dy+shei >=ele->y)
       )
     {
      nextaction=(TPaction) guystand;
      ele->onelevator=1;
      onelevator=1;
      y=ele->y-shei;
     }
  }

}








// this action tries to keep the main character in the view by scrolling the
// view window every time the main character tries to go out of the view.

void scrollaction(TOscroll *o)
{
 TOworld  *w=o->owner;

  if (!o->xscroll)
  {
	if ( o->pguy->x < (w->x + 100) ) o->xscroll=-DXSCROLL;
   else if ( o->pguy->x > (w->x + 200) ) o->xscroll=DXSCROLL;
  }
  else
  {
    w->scrollR(o->xscroll);
    if (  (o->pguy->x <= w->x + 160) && (o->pguy->x >= w->x+140) ) o->xscroll=0;
  }

  if (!o->yscroll)
  {
	if ( o->pguy->y < (w->y + 20) ) o->yscroll=-DYSCROLL;
   else if ( o->pguy->y > (w->y + 145) ) o->yscroll=DYSCROLL;
  }
  else
  {
    w->scrollD(o->yscroll);
    if (  (o->pguy->y <= w->y + 100) && (o->pguy->y >= w->y+60) ) o->yscroll=0;
  }
}





// -------------------- action functions of our guy -------------------------
// --------------------------------------------------------------------------

void guystand(TOguy *obj)
{
  obj->dx=0;
  obj->dy=0;
  obj->framenow=0;

  if ( pl_testkey(mcNUM6) )     // right arrow
  {
   obj->framebase=0;
   obj->framenow=1;
   obj->time=0;
   obj->nextaction=(TPaction) guyrun;
   obj->dx=1;
  }
  else
  if (pl_testkey(mcNUM4) )   // left arrow
  {
   obj->framebase=9;
   obj->framenow=1;
   obj->time=0;
   obj->nextaction=(TPaction)guyrun;
   obj->dx=-1;
  }
  else
  if ( pl_testkey(mcNUM8) )
   obj->nextaction=(TPaction)guystartjump;
}



void guyrun(TOguy *obj)
{
 TOworld *w=obj->owner;

 if ( w->howfarD(obj,1) && (!obj->onelevator))
  obj->nextaction=(TPaction)guystartfall;
 else
 if ( pl_testkey(mcNUM6) )
 {
  obj->framebase=0;
  obj->dx= w->howfarR(obj,abs(obj->dx));
  obj->x+=obj->dx;
 }
 else
 if ( pl_testkey(mcNUM4) )
 {
  obj->framebase=9;
  obj->dx=-w->howfarL(obj,abs(obj->dx));
  obj->x+=obj->dx;
 }
 else
 obj->nextaction=(TPaction)guystand;

 if ( pl_testkey(mcNUM8) )
   obj->nextaction=(TPaction)guystartjump;

  obj->time+=pl_dtime[0];
  if (obj->time > 3)
   {
      obj->framenow++;
      obj->time=0;
      if (obj->framenow >8) obj->framenow=1;
    }

}


void guystartjump(TOguy *obj)
{
 obj->time=0;
 if ( pl_testkey(mcNUM6) || pl_testkey(mcNUM4) ) obj->gx=5;
 obj->dy=-3;
 obj->nextaction=(TPaction)guyjump;
}




void guyjump(TOguy *obj)
{
unsigned short wd;
TOworld *w=obj->owner;

 obj->time+=pl_dtime[0];

 if ( obj->time > 7 )
 {
   obj->time=0;
   obj->dy++;
 };


  if ( pl_testkey(mcNUM6) )
  {
   obj->framenow=1;
   obj->framebase=0;
   obj->dx=1;
   if (obj->gx > 0 ) obj->dx++;
   obj->dx=w->howfarR(obj,obj->dx);
   obj->x+=obj->dx;
  }
  else
  if ( pl_testkey(mcNUM4) )
  {
   obj->framenow=1;
   obj->framebase=9;
   obj->dx=-1;
   if (obj->gx > 0 ) obj->dx--;
   obj->dx=-w->howfarL(obj,-obj->dx);
   obj->x+=obj->dx;
  }


  obj->dy=-w->howfarU(obj,-obj->dy);
  obj->y+=obj->dy;
  obj->gx--;

  if ( obj->dy >= 0 ) obj->nextaction=(TPaction)guystartfall;
};


void guystartfall(TOguy *obj)
{
 obj->dy=1;
 if ( pl_testkey(mcNUM6) || pl_testkey(mcNUM4) ) obj->gx=10;
 obj->time=0;
 obj->nextaction=(TPaction)guyfall;
}



void guyfall(TOguy *obj)
{
 TOworld *w=obj->owner;
 short wd;

  obj->time+=pl_dtime[0];

  if ( pl_testkey(mcNUM6) )
  {
   obj->framebase=0;
   obj->framenow=1;
   obj->dx=abs(obj->dx);
   obj->dx=w->howfarR(obj,obj->dx);
   obj->x+=obj->dx;
  }
  else if ( pl_testkey(mcNUM4) )
  {
   obj->framenow=1;
   obj->framebase=9;
   obj->dx=-abs(obj->dx);
   obj->dx=-w->howfarL(obj,-obj->dx);
   obj->x+=obj->dx;
  }

  obj->gx--;
  wd=w->howfarD(obj,obj->dy);

  if (wd!=obj->dy)
  {
   obj->dy=obj->dx=0;
   obj->y+=wd;
   obj->nextaction=(TPaction)guystand;
  }
  else
  {
   obj->y+=obj->dy;
   if ( (obj->time > 7) && (obj->dy<4) )
   {
    obj->dy++;
    obj->time=0;
   }
  };

}

// --------------------------------------------------------------------------
// --------------------------------------------------------------------------



// loads a bin file from the disk into a Tbitmap structure

char loadbitmap(char *fname,TPbitmap &frame)
{
 FILE *f;
 unsigned short l,h;

 if ( (f=fopen(fname,"rb")) == NULL ) return 0;
 fread(&l,2,1,f);
 fread(&h,2,1,f);
 if ( (frame = (TPbitmap)malloc(l*h+10)) == NULL )
  {
   fclose(f);
   return 0;
  }
 frame->len = l;
 frame->hei = h;
 frame->drawmethod = 0;
 fread(frame->data,l*h,1,f);
 fclose(f);
 return 1;
}



// --------------- elevator's action function -------------------------------

void elevatorgo(TOele *o)
{
 o->pguy->elevatorcheck(o);
 if ( (o->y > o->downlimit) || (o->y < o->uplimit)  )  o->dy=-o->dy;

 if (o->onelevator)
 // i know it is not very "object oriented" to change the y coordinate of the
 // guy from inside this function.
  o->pguy->y+=o->dy;

 o->y+=o->dy;

}

// --------------------------------------------------------------------------


// ---------------------- exit object's action function ---------------------
void exitaction(TOobject *s)
{
 TOworld *w=s->owner;

 // if ESCAPE was hit then finish.
 if ( pl_testkey(mcESC) ) w->exitbit=1;

 // if P was hit then pause until P is hit again
 if (pl_testkey(mcP) ) {
   for (;pl_testkey(mcP););
   for (;!pl_testkey(mcP););
   for (;pl_testkey(mcP););
  }

 // reset timer variable to zero. This variable counts the ticks between 2
 // consecutive frames

 pl_dtime[0]=0;
};

//----------------------------------------------------------------------

//----------------- action function of the C 1997 logo -----------------

void stable(TOsprite *s)
{
 s->x=s->owner->x+274;
 s->y=s->owner->y+3;
};

//----------------------------------------------------------------------


 TOworld world;
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
int framecount=0,oldframecount=0;
char nextframeflag = 1;
char frameready=1;

mpModule    *mymodule;
int          errorvar, configok;

void preVR()
{
 if (!frameready) return;
 framecount++;
 world.pageswap();
 world.setsadr();
}

void immVR()
{
 if (!frameready) return;
 world.sethpel();
}

void inVR()
{
 if (!frameready) return;
 world.refreshpal();
 frameready=0;
}


short mykernel(TOworld *w)
{
 for (;framecount==oldframecount;);
 oldframecount=framecount;
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

void main(int argc,char *argv[])
{
 int i,j;
 char name[54];
 TPbitmap pbitmaps[22];   // 22 pointers to bitmaps
 TOobject tim;

 TOguy myguy;
 TOobject exitobj;
 TOscroll scrollobj;
 TOele elevator[5];
 TOcloud clouds[5];
 TOsprite c1997;

 if (argc!=2)
 {
  printf("Syntax: ex4mus modulefile\n");
  exit(1);
 }

 if ( !pl_isvga() || !pl_is386())
  {  printf("386 and VGA card required\n") ;return;
  }
 // check the memory left to avoid checking what individual routines return
 // i tried to guess how much memory this program needs, 10000 bytes should
 // be enough
 if (coreleft() < 10000)
 { printf("Not enough memory \n"); return;
 }

// load the bitmaps into memory
for (i=1;i<=BITMAPSNO;i++)
{
  sprintf(name,"bin\\steal%3d.bin",i);
  for (j=9;name[j]==' ';name[j]='0',j++);

  if (!loadbitmap(name,pbitmaps[i-1]))
  {
    printf("Could not load bitmap %s\n",name);
    exit(1);
  }
}

// load the level
 world.loadlevel("level0.dat");

 // tell the world object what bitmaps to use.
 // and make 1-bitmap frame descriptors
 world.makebitmaps(BITMAPSNO);
 world.makeframesdescrs(BITMAPSNO);
 for (i=0;i<BITMAPSNO;i++)
 {  world.setbitmapi(i,pbitmaps[i]);
    world.make1bitmapdescr(i,i,0);
 }


 // set the prioirities of the back and fore layers
 world.setbackprior(6);
 world.setforeprior(6);

 // add the main character to the world
 myguy.init(1,80,509,pbitmaps[0]->len,pbitmaps[0]->hei,0,(TPaction)guystand,1,1);
 world.addobj(0,&myguy,0);

 // add the scroll object to the world
 scrollobj.init(3,0,0,0,0,(TPaction) scrollaction,&myguy);
 world.addobj(1,&scrollobj,0);


 c1997.init(13,600,600,pbitmaps[21]->len,pbitmaps[21]->hei,21);
 c1997.nextaction=(TPaction)stable;
 world.addobj(0,&c1997,9);


 // add the elevators to the world
 elevator[0].init(6,500,245,pbitmaps[18]->len,pbitmaps[18]->hei,18,
		  (TPaction) elevatorgo,1,&myguy,40,460);
 world.addobj(0,&elevator[0],0);

 elevator[1].init(7,300,100,pbitmaps[18]->len,pbitmaps[18]->hei,18,
		  (TPaction) elevatorgo,1,&myguy,40,560);
 world.addobj(0,&elevator[1],0);

 elevator[2].init(8,400,90,pbitmaps[18]->len,pbitmaps[18]->hei,18,
		  (TPaction) elevatorgo,1,&myguy,50,145);
 world.addobj(0,&elevator[2],0);


 // add the clouds to the world
 clouds[0].init(7,20,320,pbitmaps[19]->len,pbitmaps[19]->hei,19,
		(TPaction) cloudgo, 1, 10,200,123,177);
 world.addobj(0,&clouds[0],6);

 clouds[1].init(8,70,120,pbitmaps[19]->len,pbitmaps[19]->hei,19,
		(TPaction) cloudgo, 1, 60,600,123,177);
 world.addobj(0,&clouds[1],6);

 clouds[2].init(7,20,20,pbitmaps[19]->len,pbitmaps[19]->hei,19,
		(TPaction) cloudgo, 1, 10,600,123,177);
 world.addobj(0,&clouds[2],6);


// add the exitobj to the world
 exitobj.init(0,0,0,0,0);
 exitobj.nextaction=exitaction;
 world.addobj(1,&exitobj,7);



//---------------------------------------------------------------
  if ( (errorvar = fileExists("MIDAS.CFG", &configok)) != OK )
  midasError(errorvar);
  if ( !configok )
  {
   puts("Error reading configuration file MIDAS.CFG. Please run MSETUP.EXE");
   exit(EXIT_FAILURE);
  }

  midasSetDefaults();
  midasLoadConfig("MIDAS.CFG");
//---------------------------------------------------------------



 // set modex
 mx320x200(44);

// -------------
 unsigned short tt;
 tmrGetScrSync(&tt);

 asm push di   // midas version 0.40 is buggy (midasInit() alters di)
 midasInit();
 asm pop di

 tmrSyncScr(tt,preVR,immVR,inVR);
 mymodule = midasLoadModule(argv[1], &mpMOD, NULL);  // load the module
 midasPlayModule(mymodule, 0);            // start playing

 world.Pframeready=&frameready;
 world.Pkernelproc=mykernel;

//---------------

 // initialize the keyboard handler
 pl_installkeys();
 // initialize the timer handler
 pl_RTCinittimer(6); // init RTC handler at 64Hz (2^6)

 // jump to position 0 ,0
 world.jumpto( 0,336);
 // and go
 world.animate();

 // set text mode
 pl_setvideomode(03);

 // restore the original keyboard handler
 pl_keysdone();
 // restore the original timer handler
 pl_RTCtimerdone();

//--------------------------------------------------------------
  midasStopModule(mymodule);
  midasFreeModule(mymodule);
  midasClose();
  tmrStopScrSync();
//--------------------------------------------------------------

}

