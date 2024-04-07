

/* routines for handling 4-bit communications between 2 computers, using 
   the printer ports. For a port address P, we use the 8-bit output port 
   at P, and the 5-bit input port at P+1. The present routines implement
   polled transfers, not interrupt-driven. 
   
   Connections and bit assignments as follows
   
   Output Port P                         Input Port P+1
   Pin   Bit        Function                Pin   Bit
    2     0 ........ Data 0  ..............  15    3
    3     1 ........ Data 1  ..............  13    4
    4     2 ........ Data 2  ..............  12    5
    5     3 ........ Data 3  ..............  10    6
    6     4 ........ Synch   ..............  11    7
    
    The reverse connection is also made, of course, so each computer has
    4 data and one sync line in each direction. The cable should also have
    a straight through 25 to 25 connection for ground return.
    
    Note that bit 7 of the input port has negative logic, so it has to be
    inverted after reading.
    
   
    Protocol for polled transfers:
    
    Initialize:   All lines set to 0.

Low nybble:
    
    Sender:   Wait till incoming synch is low
              Put data on data lines then set synch high.
    Receiver: Poll for incoming Synch high, read data.
              Set outgoing synch high.

High nybble:
    Sender:   Poll for incoming sync high
              Put data on lines then set sync low
              Done.
    Receiver: Poll for incoming synch low, read data
              Set outgoing synch low 
              Done.
              
*/

#pragma inline

#include <dos.h>
#include "pport.h"

#define XT_CODE                  /* undefine to get better code for 286+ */

#if defined(__COMPACT__) || defined(__LARGE__) || defined(__HUGE__)
#define LARGE_DATA
#else
#undef LARGE_DATA
#endif


#define INPUT_SYNC_BIT  0x80  /* mask for input sync as read from port */
#define OUTPUT_SYNC_BIT 0x10


static unsigned Current_port;     /* address of last initialized port */



int pp_initialize(int port)      /* initialize port (arg is bios port 0-2) */   
                                 /* port 0 = LPT1, port 1 = LPT2, etc. */
{                                /* must be called before any i/o on the port */
   unsigned p;
   static unsigned int far *bios_ptr = MK_FP(0x40, 8);
    
   if (port > 2 || port < 0) return(NO_PORT);  
   p = *(port + bios_ptr);
   if (p == 0) return(NO_PORT);
   else
   {  outportb(p, 0);            /* initialize to zero output */
      Current_port = p;
      return(OK);
   }
}



void pp_wait_quiet(void)               /* wait till nothing more is coming */
{                                      /* in on current port. (For use after */
   unsigned i;                         /* unexpected timeouts, and other */
   unsigned char x, y, z;              /* crashes */
   
#define WAIT_COUNT   20000u            /* this should run the wait loop 10's */
                                       /* of ms on a 486, 100's on XT */

   x = inportb(Current_port + 1);
   for (i=0;  i<WAIT_COUNT;  i++)      /* first make sure it'd dead */
   {  y = inportb(Current_port +1);
      if (y != x)
      {  x = y;
         i = 0;
      }
   }
               /* arive here on no change in input in WAIT_COUNT tries */

   z = inportb(Current_port);          /* get current state of output sync */
   z &= OUTPUT_SYNC_BIT;               /* bit */
   
                                       /* now toggle output sync bit, and */   
                                       /* read input for change. Repeat till */
   while(1)                            /* timeout with no further changes */
   {  z ^= OUTPUT_SYNC_BIT;
      outportb(Current_port, z);       /* N.B. a hardware problem, like a */
      for (i=0;  i<WAIT_COUNT;  i++)   /* continuous noise source, could */
      {  y = inportb(Current_port + 1);/* make this loop forever. */
         if(y != x) break;
      }
      if (i >= WAIT_COUNT) break;      /* timeout */
      x = y;
   }
   pp_initialize(Current_port);        /* arrive here on really dead. */
}                                      /* reinitialize, for good luck */
      
   
   

int pp_send_byte(unsigned char data)     /* send 1 byte on current port */
{                                        /* all sync bits should be low */
   asm mov  bl, data                     /* to start, which they will be */
   asm mov  dx, Current_port             /* right after initialization on */
   asm inc  dx                           /* each end */
   asm xor  cx, cx
L0:
   asm in   al, dx                       /* wait sync lo ... high on bit 7 */
   asm test al, INPUT_SYNC_BIT
   asm jnz  L1
   asm loop L0
   
   return(START_TIMEOUT);
   
L1:
   asm dec  dx                           /* send the low nybble */ 
   asm mov  al, bl
   asm and  al, 0xf
   asm or   al, OUTPUT_SYNC_BIT
   asm out  dx, al

   asm inc  dx
   asm xor  cx, cx
L2:                                      /*wait for receiver to flip sync bit */ 
   asm in   al, dx
   asm test al, INPUT_SYNC_BIT
   asm jz   L3                   /* sync polarity is reversed for high nybble */
   asm loop L2
   
   return(SYNC_TIMEOUT);

L3:
   asm dec  dx                           /* send the high nybble */

#ifdef XT_CODE
   asm mov  cl, 4
   asm shr  bl, cl
#else
   asm shr  bl, 4
#endif   

   asm mov  al, bl
   asm out  dx, al 
   
   return(OK);
}



int pp_send_n_bytes(int count, unsigned char *input)  /* return no. sent */
{                                      /* this packages the above routine */
   asm push si                         /* in a loop, so that you don't have */
   asm push di                         /* to take subroutine call overhead on */
                                       /* each byte */
#ifdef LARGE_DATA
   asm les  si, input
#else
   asm mov  si, input
#endif

   asm mov  di, count
   asm mov  bh, INPUT_SYNC_BIT
   asm mov  dx, Current_port

MAIN_LOOP:

#ifdef LARGE_DATA
   asm mov  bl, es:[si]
#else
   asm mov  bl, [si]
#endif
   
   asm inc  dx
   asm xor  cx, cx
L0:
   asm in   al, dx               /* wait sync lo ... high on bit 7 */
   asm test al, bh
   asm jnz  L1
   asm loop L0
   
   asm jmp  BREAK_LOOP
   
L1:
   asm dec  dx
   asm mov  al, bl
   asm and  al, 0xf
   asm or   al, OUTPUT_SYNC_BIT
   asm out  dx, al

   asm inc  dx
   asm xor  cx, cx
L2:
   asm in   al, dx
   asm test al, bh
   asm jz   L3                   /* sync polarity is reversed in high nybble */
   asm loop L2
   
   asm jmp  BREAK_LOOP

L3:
   asm dec  dx

#ifdef XT_CODE
   asm mov  cl, 4
   asm shr  bl, cl
#else
   asm shr  bl, 4
#endif   

   asm mov  al, bl
   asm out  dx, al 
   
   asm inc  si                   /* pointer to next datum */
   asm dec  di
   asm jnz  MAIN_LOOP
   
   asm inc  dx
L6:                        /* wait for receiver to drop sync, so it */
                           /* will be safe to turn the line around for */
	asm in   al, dx         /* transmission in opposite direction */
	asm test al, bh
	asm jz  L6              /* -ve logic! low sync is high on bit 7 */


BREAK_LOOP:
   asm mov  ax, count
   asm sub  ax, di               /* compute bytes sent*/
   asm pop  di
   asm pop  si   
   return(_AX);

}



int pp_read_byte(unsigned char *output)   /* read 1 byte on current port */
{
   asm mov  dx, Current_port
   asm xor  cx, cx
   asm inc  dx

L0:                                       /* wait for input sync bit */
   asm in   al, dx
   asm test al, INPUT_SYNC_BIT
   asm jz   L1
   asm loop L0
   
   return(START_TIMEOUT);
                                    /* read the port a second time because */
L1:asm in   al, dx                  /* sync might settle before data */

   asm mov  ah, al                  /* low nybble in al */
   asm dec  dx
   asm mov  al, OUTPUT_SYNC_BIT
   asm out  dx, al                  /* toggle output sync bit */

#ifdef XT_CODE
   asm shr  ah, 1
   asm shr  ah, 1
   asm shr  ah, 1
#else
   asm shr  ah, 3
#endif
   
   asm xor  cx, cx
   asm inc  dx                      /* wait for input sync to change */
L2:
   asm in   al, dx
   asm test al, INPUT_SYNC_BIT
   asm jnz  L3
   asm loop L2
   
   return(SYNC_TIMEOUT);

L3:asm in   al, dx               /* 2nd read, again. High nybble */
   asm shl  al, 1
   asm and  al, 0xf0
   asm or   ah, al               /* assemble byte in ah */
   
   asm dec  dx
   asm xor  al, al
   asm out  dx, al               /* toggle output sync bit */
   
#ifdef LARGE_DATA                /* save byte to output */
   asm les  bx, output
   asm mov  es:[bx], ah
#else
   asm mov  bx, output
   asm mov  [bx], ah
#endif

   return(OK);
}


int pp_read_n_bytes(int count, unsigned char *output)  /* return # read */
{                                   /* byte input packaged in loop */
   asm push si
   asm push di

#ifdef LARGE_DATA
   asm les  di, output
#else
   asm mov  di, output
#endif
   
   asm mov  si, count
   asm mov  bh, INPUT_SYNC_BIT
   asm mov  dx, Current_port

MAIN_LOOP:
   asm xor  cx, cx
   asm inc  dx

L0:
   asm in   al, dx
   asm test al, bh
   asm jz   L1
   asm loop L0
   
   asm jmp  BREAK_LOOP
   
L1:asm in   al, dx               /* because sync might settle before data */
   asm mov  ah, al
   asm dec  dx
   asm mov  al, OUTPUT_SYNC_BIT
   asm out  dx, al

#ifdef XT_CODE
   asm shr  ah, 1
   asm shr  ah, 1
   asm shr  ah, 1
#else
   asm shr  ah, 3
#endif
   
   asm xor  cx, cx
   asm inc  dx
L2:
   asm in   al, dx
   asm test al, bh
   asm jnz  L3
   asm loop L2
   
   asm jmp  BREAK_LOOP

L3:asm in   al, dx               /* because sync might settle before data */
   asm shl  al, 1
   asm and  al, 0xf0
   asm or   ah, al
   
   asm dec  dx
   asm xor  al, al
   asm out  dx, al
   
#ifdef LARGE_DATA
   asm mov  es:[di], ah
#else
   asm mov  [di], ah
#endif

   asm inc  di
   asm dec  si
   asm jnz  MAIN_LOOP

BREAK_LOOP:
   pp_delay();                /* so sender can detect lo sync before */
                              /* any line turnaround */
   asm mov  ax, count
   asm sub  ax, si            /* compute # received */

   asm pop  di
   asm pop  si
   return(_AX);
}



#define NDEL   16

void pp_delay(void)
{
   int i;
   
   i = 0;
   while(i < NDEL) i++;
}

/* higher level routines that transfer data blocks. A block consists of a
   16-bit count of data bytes, sent twice, followed by the data, followed 
   by a 16-bit checksum formed by adding  data  as unsigned ints. (If the
   number of bytes is odd, the last one is not included in the sum)
   This is not a very stringent error test, but it's fast, and errors
   seem to be extremely rare, based on my tests with a 6ft cable. Haven't
   yet had one in over 25,000,000 bytes transmitted.
*/

int pp_send_data_block(int count, unsigned char *input) /* returns err. code */
{
   unsigned csum;
   unsigned char *p, *pend;
   int c2[2];
   int j;
   
   asm mov  dx, 0             /* checksum calcs in assembler for speed */
   asm mov  cx, count
   asm shr  cx, 1             /* count of words */
/*   asm mov  ah, 0 */
   asm push ds
#ifdef LARGE_DATA
   asm lds  si, input
#else
   asm mov  si, input
#endif
   asm cld
L1:
   asm lodsw
   asm add  dx, ax
   asm loop L1
   asm pop  ds

   asm mov  csum, dx

   c2[0] = c2[1] = count;
   j = pp_send_n_bytes(4, (char *)c2);
   if (j == 0) return(CHAR1_TIMEOUT);
   if (j != 4) return(GEN_TIMEOUT);

   j = pp_send_n_bytes(count, input);
   if (j != count) return(GEN_TIMEOUT);
   
   j = pp_send_n_bytes(2, (char *)&csum);
   if (j != 2) return(GEN_TIMEOUT);
   
   return(OK);
}



int pp_read_data_block(int *count, unsigned char *output)
                            /* returns error code or OK */
                            /* size of the incoming block is returned in */
                            /* count. output must point to enough space */
                            /* to handle anything that may come. (32k is */
                            /* guaranteed to be enough, since output */
                            /* routine won't send more) */
{
   int  k, i;
   unsigned csum_in, csum_cal;
   int c[2];
   
   k = pp_read_n_bytes(4, (unsigned char *)c);
#ifdef DEBUG
      printf("%04x", k);
      printf(" %04x %04x\n", c[0], c[1]);
#endif
   if (k == 0) return(CHAR1_TIMEOUT);
   if (k != 4) return(GEN_TIMEOUT);

   if (c[1] != c[0]) return(COUNT_FAIL);
   
   k = pp_read_n_bytes(c[1], output);
#ifdef DEBUG
      printf("%04x", k);
      for (i=0;  i<k;  i++) printf(" %02x", output[i]);
      printf("\n");
#endif
   if (k != c[0]) return(GEN_TIMEOUT);
   
   k = pp_read_n_bytes(2, (unsigned char *)&csum_in);
#ifdef DEBUG
      printf("%04x", k);
      printf(" %04x\n", csum_in);
#endif
   if (k != 2) return(GEN_TIMEOUT);
   
   asm mov  dx, 0                   /* compute checksum of incoming data */
   asm mov  cx, c[0]
   asm shr  cx, 1 
/*   asm mov  ah, 0 */
   asm push ds
#ifdef LARGE_DATA
   asm lds  si, output
#else
   asm mov  si, output
#endif
   asm cld
L1:
   asm lodsw
   asm add  dx, ax
   asm loop L1
   asm pop  ds

   asm mov  csum_cal, dx

   if (csum_in != csum_cal) return(CSUM_FAIL);
   
   *count = c[0];
   return(OK);
}
