/*
   a set  of Turbo-C functions to support
   interrupt driven serial i/o.
   Written by Joe DeRosa 07/87 and released in the Public Domain
   input and output are buffered line and modem status drivers are also
   interrupt driven. tested upto 9600 baud all code is written in "C".
   thanks to Borland Low level hooks that are available in Turbo-C.
   thanks to Glenn F. Marshall for his Pascal code.
   thanks to Curt Klinsing for his assembler routines.
*/

/* return count of characters in input buffer */
#define inp_cnt() in_c_ct

/* word size */
#define BIT7 2
#define BIT8 3
/* stop bits */
#define STOP1 0
#define STOP2 4
/* parity */
#define NONE 0
#define ODD 8
#define EVEN 24
/* baud rate */
#define B300 384
#define B1200 96
#define B2400 48
#define B4800 24
#define B9600 12
#define B19200 6

#ifndef TASYNC
extern int in_c_ct,modstat;        /* for count and carrier macros */
void setport(int prot,int baud);   /* setup the serial port */
/* prot is word | stop | parity    ex: setport(BIT8|STOP1|NONE,B1200) */
int carrier(void);
int getport(void);                 /* returns modem & line status
                                      bits same as in ibm tech manual */
void flush_port(void);             /* flushes input and output buffers */
void uninit(void);                 /* remove interrupt */
void scrstat(void);                /* modem lights (don't call to often)*/
void comout(char c);               /* que character in buffer */
char inp_char(void);               /* get one char from buffer, */
void init_com(void);               /* initialize the comm port, */
#endif
