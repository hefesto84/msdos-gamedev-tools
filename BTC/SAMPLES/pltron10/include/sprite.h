
/*
  sprite.h. Include file for the TOsprite and TOobject classes.

  Copyright 1997 Liouros Thanasis.

  This file is part of PLATFORMTRON game library. See the file license.txt
  for more details. If that file is missing then please contact me.

*/

#ifndef sprite_h
#define sprite_h

#include <alloc.h>



struct TOworld;
struct TOobject;   

typedef void (*TPaction) (TOobject *);

struct TOobject
  {
   friend TOworld;

   TOobject *Pnext;
   TOobject *Pprev;
   TOobject *Pnext2;
   TOobject *Pprev2;
   TOworld *owner;                  
   char framesbit;                  
   char visbit;     
   char activebit;                  
   char deadbit;                    
   char deletebit;                  
   char removebit;                  
   unsigned char type;
   unsigned short id;		    
   unsigned char priority;          
   unsigned short x,y;              
   signed char dx,dy;               
   signed char gx,gy;               
   unsigned short time;             
   TPaction nextaction;
   unsigned short slen, shei;	       

   void setvisbit(char bit)  { visbit = bit; }
   char getvisbit() { return visbit ;}
   void setdeadbit(char bit) { deadbit = (bit!=0); }
   char getdeadbit() { return deadbit; }
   void setnext(TOobject *nxt) { Pnext = nxt; }
   TOobject *getnext() { return Pnext; }
   void setpriority(unsigned char priority0);
   unsigned char getpriority() { return priority ; };
 


   void init(unsigned char id0,unsigned short x0,
	     unsigned short y0,unsigned short slen0,unsigned short shei0);
   ~TOobject(); 
    void callnext()
    {
      if (nextaction) nextaction(this);
    };


   private:

   char oldvisbit;                  
 };










struct TOsprite : public TOobject
 { friend TOworld;

   char staticbit;             
   unsigned short framebase;   
   signed short framenow;      
   void init(unsigned char id0,unsigned short x0,
	     unsigned short y0,unsigned short slen0,unsigned short shei0,
             signed short framenow0,unsigned short framebase0 = 0);

   private:

   unsigned short oldx,oldy,oldlen,oldhei;
   unsigned short clipx,clipy, cliplen, cliphei;    			       
			       
};



#include "world.h"

#endif
