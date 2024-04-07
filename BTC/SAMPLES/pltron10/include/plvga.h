
/*
  plvga.h. Include file for the various vga and raw buffer routines.

  Copyright 1997 Liouros Thanasis.

  This file is part of PLATFORMTRON game library. See the file license.txt
  for more details. If that file is missing then please contact me.

*/

#ifndef plvga_h
#define plvga_h

char pl_iswindows();
char pl_isvga();
void mx320x200(unsigned char linesize);
char *rawto4planes(char *inmap,char *outmap,unsigned short len,unsigned short hei);
extern "C" void  pl_pixel(short x,short y,char col,char *buf,
			     unsigned short buflen);
extern "C" void  pl_horline(short x,short y,short len,char color,void *buf,
			     unsigned short buflen);
extern "C" void  pl_verline(short x,short y,short hei,char color,void *buf,
			     unsigned short buflen);
void pl_line(short x1,short y1,short x2, short y2,
			     char  color ,char *buf,unsigned short buflen);
extern "C" void pl_thruclip(char *fromp,unsigned short l,unsigned short clipl,
			     unsigned short cliph,unsigned short sx,
			     unsigned short sy, unsigned short tox,
			     unsigned short toy, char *buf,
			     unsigned short buflen);
extern "C" void pl_solidclip(char *fromp,unsigned short l,unsigned short clipl,
			     unsigned short cliph,unsigned short sx,
			     unsigned short sy, unsigned short tox,
			     unsigned short toy, char *buf,
			     unsigned short buflen);


void pl_setpal(char far *PPalette);
void pl_setvideomode(unsigned char mode);
void pl_setstartadr(unsigned short adrs);



extern "C" void pl_retrace();
extern "C" void pl_sethorpel(unsigned char value);
extern "C" void pl_setRGBcolor(unsigned char color,char R,char G,char B);
void pl_getRGBcolor(unsigned char i,char &R,char &G,char &B);

#endif
