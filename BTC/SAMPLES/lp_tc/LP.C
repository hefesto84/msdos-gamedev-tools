
#ifdef SCCSID
static sccsid[] = "@(#) lp.c 1.1 91/03/18  19:50:09";  
#endif
/*****************************************************************************
*	Hardware line printer driver.

	This module implements the functionality needed to drive a PC-style
	parallel port.  All function calls are non-blocking.  The function 
	passed to lpt_io() determine what action is taken.  The actions
	are defined in lp.h and again for reference below.  In general, to 
	use a port, the following procedure is used:

	Test for port presence  (lpt_io(IS_PRESENT))
	if so, then (lpt_io(INIT) to initialize the port

	For each character, test lpt_io(IS_BUSY)
	When port is not busy, lpt_io(OUT) with the character

	Optionally, one may retrieve the status of the printer port with
	lpt_io(STAT).

	Finally, if one desires to select or deselect a printer (assert or
	deassert the SELECT line on the interface), call lpt_io(SELECT) with
	the appropriate argument.


	Two test routines are provided in this module.  Defining TEST_1 
	includes a main() and creates a program that outputs 50,000 "*"
	characters to the port and displays transmission statistics.

	Defining TEST_2 creates a program that will output a file specified
	on the command line or redirected into it.


	Following is a table of function calls vs arguments vs returned value:


Function     		Port 			Byte 			Mode		Returned value
------------------------------------------------------------------------------
IN					LPT base port	N/A 			IN			Byte Read
OUT					LPT base port	byte to print 	OUT			Port Status
INIT				LPT base port 	N/A				INIT		Port Status
STAT				LPT base port 	N/A				STAT		Port Status
SELECT ASSERT		LPT base port	ASSERT			SELECT		Port Status
SELECT DEASSERT		LPT base port	DEASSERT		SELECT		Port Status
IS_BUSY				LPT base port 	N/A				IS_BUSY		0 if not busy
IS_ACK				LPT base port 	N/A				IS_ACK		0 if not ACK
IS_PRESENT			LPT base port 	N/A				IS_PRESENT	0 if not present

***************************************************************************/

/**************************************************************************/
/*&&&&&&&&&&&&&&&&&&&&&&*/
#define TEST_2
/*&&&&&&&&&&&&&&&&&&&&&&*/
/**************************************************************************/

#if defined(TEST_1) || defined(TEST_2)
#include <stdio.h>
#include <time.h>
#endif

#include <bios.h>
#include "lp.h"

/* function codes */
/********************************************************/
/* these are defined in lp.h and are here for reference 
#define IN 1
#define OUT 2
#define INIT 3
#define STAT 4
#define SELECT 5
#define IS_BUSY 6
#define IS_ACK 7
#define IS_PRESENT 8
***********************************************************/

/********************************************************/
/* subfunction codes for function SELECT
	Again, these are defined in lp.h
#define ASSERT	100
#define DEASSERT 101
****************************************************/

/***************************************************************************

 port architecture.

Each lpt port starts at a base address as defined below.  Status and 
control ports are defined off that base.

		          write									read
=============================================================================
Base	data to the printer is latched.			Read latched data

base+1	not defined                             Read printer status lines
												Bits are as follows:
												bit 7	busy		0x80
												bit 6 	ack			0x40
												bit 5	paper out	0x20
												bit 4	select in	0x10
												bit 3	error		0x08
												2,1,0	Not used

base+2	write control bits						read the same control bits
		Bits are defined as follows:			Normally reads the latched
		bit 0	*strobe			0x01			bits written to same port
		bit 1	*auto feed		0x02
		bit 2	init printer	0x04
		bit 3	*select out		0x08
		bit 4	turn on irq7 on ACK hi-2-lo toggle	0x10
		5,6,7	not used

******************************************************************************/
/********************************************/
/* defined in lp.h and are here for ref only
#define LPT1 0x3bc
#define LPT2 0x378
#define LPT3 0x278
**********************************************/

#ifdef TEST_1
main()
{
	unsigned status;
	unsigned lpt_io();
	unsigned int i;
	time_t start_time, end_time;

	status = lpt_io(LPT1, 0, INIT);
	printf("sending 50,000 chars\n");
	start_time = time(NULL);

	for (i=0;i<50000;i++) {
		while ( status=lpt_io(LPT1,0,IS_BUSY) ) /* spin while busy */
			;
		status = lpt_io(LPT1, '*', OUT);
		if (!(i%1000))
			printf("*");
	}
	end_time = time(NULL);
	printf("\n50,000 chars in %ld seconds or %ld chars/sec\n",
		end_time-start_time,
		50000L / (end_time-start_time) );

	exit(0);

}

#endif

#ifdef TEST_2
/* this version outputs a file to lpt1 */
main(argc, argv)
int argc;
char **argv;
{
	unsigned status;
	unsigned lpt_io();
	long int i=0L;
	time_t start_time, end_time;
	int character;
	int busy_flag=0;

	status = lpt_io(LPT1, 0, INIT);
	start_time = time(NULL);

	if (argc > 1) {
		if (freopen(argv[1], "rb", stdin) == (FILE *) NULL) {
			cprintf("Error, file %s open failed\n", argv[1]);
			exit(1);
		}
	}

	while ( (character = fgetchar()) != EOF) {

		while ( status=lpt_io(LPT1,0,IS_BUSY) ){ /* spin while busy */
			if (!busy_flag) {
				gotoxy(70,25);
				cputs("BUSY    ");
				busy_flag=1;
			}
		}
		if (busy_flag) {
			gotoxy(70,25);
			cputs("PRINTING");
			busy_flag=0;
		}
		status = lpt_io(LPT1, character, OUT);
		i++;
	}
	end_time = time(NULL);

	gotoxy(70,25);cputs("        ");
	gotoxy(1,24);
	cprintf("%ld chars in %ld seconds or %ld chars/sec",
		i,
		end_time-start_time,
		i / (end_time-start_time) );

	exit(0);

}

#endif

/*
*	The meaning of life and the bits returned in the status byte
*	NOTE:  Important - the sense of all bits are flipped such that
*	if the bit is set, the condition is asserted.
*
*Bits----------------------------
*   7   6   5   4   3   2   1   0
*   |   |   |   |   |   |   |   +-- unused
*   |   |   |   |   |   |   +------ unused
*   |   |   |   |   |   +---------- unused
*   |   |   |   |   +-------------- 1 = i/o error
*   |   |   |   +------------------ 1 = selected
*   |   |   +---------------------- 1 = out of paper
*   |   +-------------------------- 1 = acknowledge
*   +------------------------------ 1 = not busy
*
*/


unsigned int
lpt_io(port,byte,mode)
	unsigned port;
	unsigned byte;
	int mode;
{
	unsigned i,j,status;
	long unsigned otime;


	switch (mode) {  /* test for valid commands */

	case OUT:
		outportb(port,byte);	/* send the character to the port latch */

		outportb(port+2, 0x0d); /* set strobe high */
		outportb(port+2, 0x0d); /* do it again to kill some time */
		outportb(port+2, 0x0c); /* set strobe low */
		inportb(port+1);  /* pre-charge the line if +busy is floating*/
		status = (inportb(port+1) & 0x00f8) ^ 0x48;
		return(status);

	case  IN:
		return(inportb(port));

	case IS_BUSY:	/* this checks the busy line */
		return ( (inportb(port+1) & 0x80) ^ 0x80 );	/* zero if not busy */
		/* note that we flip the sense of this bit because it is inverted
			on the port */

	case IS_ACK:	/* this checks the ack line */
		return ( inportb(port+1) & 0x60 );	/* zero if ACK not asserted */

	case SELECT:

		switch (byte) {

		case ASSERT:
			i = inportb(port+2); 		/* get the control bits */
			outportb(port+2, i | 0x8);	/* mask bit 3 ON and output */
			return ( (inportb(port+1) & 0xf8) ^ 0x48 );

		case DEASSERT:
			i = inportb(port+2); 		/* get the control bits */
			outportb(port+2, i & ~0x8);	/* mask bit 3 OFF and output */
			return ( (inportb(port+1) & 0xf8) ^ 0x48 );

		default:
			return(~0); 	/* error */
		}

	case INIT:
		otime = biostime(0,0L); 	/* get the timer ticks */
		outport(port+2, 0x08); 		/* set init line low */

		/* wait for the next timer transition */
		while ( otime + 1 > biostime(0,0L)) ;
		outportb(port+2, 0x0c); 	/* set init line high */
									/* and select printer */
		/* fall thru */
	case STAT:
		return( ((inportb(port+1) & 0xf8) ^ 0x48) );

	case IS_PRESENT:	/* test to see if the port is present */
	outportb(port,0x55);
		if (inportb(port) == 0x55) {
			return(~0);
		}
		return(0);

	default:
		return(~0);		/* error, all bits set */
	}
}

/************** end of file ******************/
