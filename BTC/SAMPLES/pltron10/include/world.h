
/*
  world.h. Include file for the TOworld class.

  Copyright 1997 Liouros Thanasis.

  This file is part of PLATFORMTRON game library. See the file license.txt
  for more details. If that file is missing then please contact me.

*/



#ifndef world_h
#define world_h

#include <stdio.h>

#define DR_SOLID                  1
#define DR_THRU                   0

#define ERR_FOPEN                -1
#define ERR_FTELL                -10
#define ERR_FREAD		 -11
#define ERR_INVFILEFORMAT        -2
#define PRIORITIESNO              15
#define LISTSNO                   15

#define VIS_FULL                 0xf0
#define VIS_INVISIBLE            0x0f
#define VIS_PARTIAL              0xff

#define MAXLAYERSIZE             65534
#define ERR_LAYERTOBIG           -3
#define ERR_LAYERMADE           -5
#define ERR_LAYERNOTMADE        -6
#define ERR_TOOSMALLDIM          -19
#define ERR_LAYEREXISTS          -25

#define ERR_TILESMADE            -7
#define ERR_TILESNOTMADE         -8
#define ERR_NULLTILE             -26

#define ERR_BITMAPSNOTMADE       -12
#define ERR_BITMAPSMADE          -13
#define ERR_NOTEMSBITMAP         -24

#define ERR_DESCRSMADE           -14
#define ERR_DESCRSNOTMADE        -15

#define ERR_NULLDESCR            -16
#define ERR_SUBBITMAPRANGE       -17
#define ERR_NOTNULLDESCR         -18

#define ERR_OUTOFMEM             -4
#define ERR_ZEROMEMALLOC         -20
#define ERR_OUTOFRANGE           -9
#define ERR_NOERR                 0

#define ERR_NULLOBJ              -21
#define ERR_NULLBITMAP           -22
#define ERR_BITMAPNOTFIT         -23


#define MAXEMSHANDLES            256
#define PAGESPERHANDLE            6
#define ERR_EMSOUTOFMEM         -257
#define ERR_EMSOUTOFHANDLES     -258
#define ERR_ZEROSIZE            -260
#define ERR_NOEMS               -261

#define LAY_BACK                 0
#define LAY_FORE                 1
#define LAY_ATTR                 2



#define  MACROcmplace(x11,y11,x12,y12,  x21,y21,x22,y22, res) \
	       if ( (x11) >x21) x21= (x11);\
	       if ( (y11) >y21) y21= (y11);\
	       if ( (x12) <x22) x22= (x12);\
	       if ( (y12) <y22) y22= (y12);\
	       res=( (x21<=x22) && (y21<=y22) )

#define MACROcmplace2(x11,y11,x12,y12,  x21,y21,x22,y22, res,res2)\
	       (res2)=1;\
	       if ( (x11) >x21) { x21= (x11); (res2)=0; };\
	       if ( (y11) >y21) { y21= (y11); (res2)=0; };\
	       if ( (x12) <x22) { x22= (x12); (res2)=0; };\
	       if ( (y12) <y22) { y22= (y12); (res2)=0; };\
	       res=( (x21<=x22) && (y21<=y22) )


#include "sprite.h"




char pl_is386();

extern "C" void drawstile(char far *tile,unsigned char tilex,unsigned char tiley,unsigned short pageofs);
extern "C" void drawttile(char far *tile,unsigned char tilex,unsigned char tiley,unsigned short pageofs);
extern "C" void v2v(unsigned short pagefrom,unsigned short xfrom,unsigned short yfrom,
		      unsigned short pageto  ,unsigned short xto,  unsigned short yto,
		      unsigned short lenin4s, unsigned short hei);
extern "C" void clearforeasm(char far *forerefresh);
extern "C" void  M2Vsolidclipspr(char far *spr,unsigned short pagetar,unsigned short vx
			    ,unsigned short vy,unsigned short cliplen,unsigned short cliphei
			    ,unsigned short sprx,unsigned short spry
			    ,unsigned short totallen,unsigned short totalhei);

extern "C" void  M2Vclipspr(char far *spr,unsigned short pagetar,unsigned short vx
			     ,unsigned short vy,unsigned short cliplen,unsigned short cliphei
			     ,unsigned short sprx,unsigned short spry
			     ,unsigned short totallen);

extern "C" void drawscoltile(char far *tile,unsigned char tilex,unsigned char tiley,unsigned short pageofs,unsigned char colno);
extern "C" void drawsrowtile(char far *tile,unsigned char tilex,unsigned char tiley,unsigned short pageofs,unsigned char rowno);
extern "C" void drawtcoltile(char far *tile,unsigned char tilex,unsigned char tiley,unsigned short pageofs,unsigned char colno);
extern "C" void drawtrowtile(char far *tile,unsigned char tilex,unsigned char tiley,unsigned short pageofs,unsigned char colno);
extern "C" short howfarup(unsigned short x,unsigned short y,unsigned short len,unsigned short hei,unsigned short *lay,
			  unsigned short howmany,unsigned short wlen,unsigned short whei);
extern "C" short howfardown(unsigned short x,unsigned short y,unsigned short len,unsigned short hei,unsigned short *lay,
			  unsigned short howmany,unsigned short wlen,unsigned short whei);
extern "C" short howfarleft(unsigned short x,unsigned short y,unsigned short len,unsigned short hei,unsigned short *lay,
			  unsigned short howmany,unsigned short wlen,unsigned short whei);
extern "C" short howfarright(unsigned short x,unsigned short y,unsigned short len,unsigned short hei,unsigned short *lay,
			  unsigned short howmany,unsigned short wlen,unsigned short whei);

void makecolidmask(char *pdata,short sprlen, short sprhei,char *mask);


struct Tbitmap
       {
	  unsigned short len;  
	  unsigned short hei;  
	  unsigned char drawmethod;  
	  unsigned char emsbit; 
	  unsigned long emsptr; 
          char data[65000];   
       };

typedef Tbitmap *TPbitmap;  


struct Tframedescr
       {
	 unsigned short len; 
	 unsigned short hei; 
	 unsigned char xdispl; 
	 unsigned char ydispl; 
	 unsigned short bitmapsno; 
	 char  *colidemask;     
	 char data[16000]; 
      };

typedef Tframedescr *TPframedescr; 

struct TOworld
  {
    unsigned short length,height;   
    unsigned short x,y;             
    unsigned short tilesno;       
    char **Ptiles;                
    unsigned short  *Pbacklayer;   
    unsigned short  *Pforelayer;   
    unsigned short  *Pbackattr;    
    char windowsbit; 
    char attrbit;  
    char exitbit;        
    char palettechanged;    
    TPbitmap *bitmaps;      
    unsigned short bitmapsno;  
    TOobject *objs[LISTSNO];        
    TOobject *priorities[PRIORITIESNO]; 
    char palette[768];   
    unsigned char xspace,yspace; 
    unsigned char backprior,foreprior; 
    TPframedescr  *framesdescr; 
    unsigned short framedescrno; 
    short (*Pkernelproc)(TOworld *w);
    char *Pframeready;    


   
   
    inline void cleartilerefresh(void);
    inline void clearforerefresh(void);
    short destroybitmaps();
    TPbitmap setbitmapi(unsigned short i,TPbitmap pbitmap);
    short makebitmaps(unsigned short bitmapsno0);
    short bitmapto4planes(TPbitmap map);
    long storebitmap(unsigned short bitmapno,TPbitmap bitmap);
    long getbitmapdataptr(unsigned short bitmapno,char * &bdata);
    void freeemsbitmaps(); 
    short makeframesdescrs(unsigned short framesdescrno0); 
    short destroyframesdescrs(); 
    short setsubbitmap(unsigned short descrno,unsigned short descrsubbitmapno,unsigned short bitmapno,
		       unsigned short bitx, unsigned short bity );
    short freedescrmem(unsigned short descrno);
    short getdescrmem(unsigned short descrno,unsigned short len0,unsigned short hei0,
	              unsigned short subbitmapsno,unsigned char xdisplacement=0,
                      unsigned char ydisplacement=0);
    short make1bitmapdescr(unsigned short descrno, unsigned short bitmapno, char colidmask,
                           unsigned char xdispl=0, unsigned char ydispl=0);
    short makeframecolidmask(unsigned short descrno);
    short sprintfdescr(unsigned short descrno,long basebitmapno,char *st,
	               unsigned char xdispl=0,unsigned char ydispl=0);
    short setframedisplacement(unsigned short frameno,unsigned char dx,unsigned char dy);

    short setbackprior(unsigned char pno)
    {
     if (pno>=PRIORITIESNO) return ERR_OUTOFRANGE;
     backprior=pno;
     if (foreprior<pno) foreprior=pno;
     return ERR_NOERR;
    };

    short setforeprior(unsigned char pno)
    {
      if ((pno>=PRIORITIESNO) ) return ERR_OUTOFRANGE;
      foreprior=pno;
      if (pno<backprior) backprior=pno;
      return ERR_NOERR;
    };

    short addobj(unsigned char listno,TOobject *obj,unsigned char priority);
    TOobject *removelistobj(unsigned char listno,unsigned short id0);
    TOobject *removeobjid(unsigned short id0);
    short removeobj(TOobject *obj);
    TOobject *getlistobjpointer(unsigned char listno,unsigned short id0);
    TOobject *getobjpointer(unsigned short id0);
    char collision(TOobject *spr1,TOobject *spr2);

    short howfarU(TOobject *spr,unsigned short howmany)
    { return howfarup(spr->x, spr->y, spr->slen, spr->shei, Pbackattr, howmany,length,height);}
    short howfarD(TOobject *spr,unsigned short howmany)
    { return howfardown(spr->x, spr->y, spr->slen, spr->shei, Pbackattr, howmany,length,height);}
    short howfarL(TOobject *spr,unsigned short howmany)
    { return howfarleft(spr->x, spr->y, spr->slen, spr->shei, Pbackattr, howmany,length,height);}
    short howfarR(TOobject *spr,unsigned short howmany)
    { return howfarright(spr->x, spr->y, spr->slen, spr->shei, Pbackattr, howmany,length,height);}
    short makelayer(unsigned char layerid);
    short destroylayer(unsigned char layerid);
    short setdimensions(unsigned short len0,unsigned short hei0);
    short setlayer(unsigned short layerid,unsigned short *newlayer);
    short loadlevel(char *filename);
    short maketiles(unsigned short tilesno0);
    short destroytiles();
    short tileto4planes(char *tile);
    char *settileno(unsigned short tileno0,char *tiledata);
    void rgbset(unsigned char i,unsigned char r,unsigned char g,unsigned char b)
    { char *t=&palette[i*3]; t[0]=r; t[1]=g; t[2]=b; palettechanged=1; }
    inline void M2Vthru(char *data,unsigned char vx, unsigned char vy,
			unsigned short cliplen,unsigned short cliphei,
			unsigned short startx, unsigned short starty,
			unsigned short totallen);
    inline void M2Vsolid(char *data,unsigned char vx, unsigned char vy,
	    		 unsigned short cliplen,unsigned short cliphei,
			 unsigned short startx, unsigned short starty,
			 unsigned short totallen,unsigned short totalhei);
    TOworld();            
    ~TOworld();               
    short jumpto(unsigned short newx,unsigned short newy);
    inline void refresh();
    unsigned short getbackxy(unsigned short bx,unsigned short by)
     { return Pbacklayer[by*length+bx] ; }
    unsigned short getforexy(unsigned short fx,unsigned short fy)
     { return Pforelayer[fy*length+fx] ; }
    unsigned short getattrxy(unsigned short ax,unsigned short ay)
     { return Pbackattr[ay*length+ax] ; }
    void setbackxy(unsigned short bx,unsigned short by,unsigned short newvalue)
     { Pbacklayer[by*length+bx] = newvalue; }
    void setforexy(unsigned short fx,unsigned short fy,unsigned short newvalue)
     { Pforelayer[fy*length+fx] = newvalue; }
    void setattrxy(unsigned short ax,unsigned short ay,unsigned short newvalue)
     { if (attrbit) Pbackattr[ay*length+ax] = newvalue; }
    void putbackxy(unsigned short bx,unsigned short by,unsigned short newvalue);
    void putforexy(unsigned short bx,unsigned short by,unsigned short newvalue);
    inline void pageswap();
    inline void sethpel();
    inline void setsadr();
    inline void refreshpal();
    short animate();
    short scrollR(signed char dx);
    short scrollD(signed char dy);






    private:

    unsigned short wx,wy;   
                     
    char layersmade;                
    char tilesmade;		    
    char internaltilesloader;       
    TOobject  *wordlist;       
    char objdeleted;           
    char refreshbit;      
    unsigned short showofs;   
    unsigned short drawofs;   
    unsigned short backofs;   
    unsigned char hpel;
    signed char vadr;         
    signed char hadr;         
    signed char huplim,hdownlim; 
    signed char vuplim,vdownlim; 
    short scrollrow,scrollcol;   
    unsigned short sadr;      
    char verupdate;           
    char horupdate;           
    unsigned char forerefresh[15][22];
    unsigned char tilerefresh[17][24];
    signed long wx0,wy0; 
    unsigned char tilechanged;
    char internalbitmapsloader;  
    char bitmapsptrmade;         
    unsigned short emsh[MAXEMSHANDLES];
    unsigned short map[4]; 
    unsigned short curpage;    
    unsigned short curpos;     
    unsigned short handlesno;      
    char emsexists;         
    char descrsmade;             
    char internallays[3];


   
    void emsfreeall(); 
    long emsstore(char *data,unsigned short size,unsigned long *emsptr);
    long emsgetptr(unsigned long emsptr,unsigned short size,char **data);
    char *setcolidemask(unsigned short descrno,char *mask);
    void callactionfunctions();
    void drawsprites(unsigned char low,unsigned char high);
    void erasesprites();
    void drawforeground();
    void drawbackground();
    void undoll(FILE *f);   
    void refreshtiles1();
    void refreshtiles2();
    void  drawall(unsigned short pageofs); 
    void  drawcol(unsigned char col,unsigned short pageofs); 
    void  drawrow(unsigned char row, unsigned short pageofs); 

};



#endif
