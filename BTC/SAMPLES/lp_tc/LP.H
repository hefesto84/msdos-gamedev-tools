
# @(#) lp.h 1.1 91/03/18  19:50:17

/* lp.h include file */
/* function codes */
#define IN 1
#define OUT 2
#define INIT 3
#define STAT 4
#define SELECT 5
#define IS_BUSY 6
#define IS_ACK 7
#define IS_PRESENT 8

/* SELECT subfunction calls */
#define ASSERT 100
#define DEASSERT 101


/* common lpt ports on PCs.  NOTE that the PC ROM bios does not bind these */
/* addresses to LPTs according to any rule.  Instead it scans this set of */
/* addresses and assigns LPTs in sequence.  The subtlty does not matter */
/* except when dealing with some "smart" lpt cards that try to out-guess */
/* what the BIOS is doing and adjusts its address accordingly.  For the */
/* Nidget, we keep things a bunch simpler.  */



#define LPT1 0x3bc
#define LPT2 0x378
#define LPT3 0x278

extern unsigned int
lpt_print( unsigned int, unsigned char, int);
