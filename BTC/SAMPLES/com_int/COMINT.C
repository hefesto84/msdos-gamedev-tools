/*****************************************************************************
        Communication interrupt test program.

        Created on : September 5th, 1987
        By         :  Dale Loftis

        GENIE Mail :  D.LOFTIS

*****************************************************************************/
#include <stdio.h>
#include <dos.h>
/*****************************************************************************
	PROGRAM GLOBAL VARIABLES
*****************************************************************************/
#if __TURBOC__			/* FOR TURBO C only ! */
#define outp(portid,v)	outportb(portid,v)
#endif

#define	BAUD110		0x00	/* available BAUD rates in IBM PC/?? */
#define	BAUD150		0x20
#define	BAUD300		0x40
#define	BAUD600		0x60
#define	BAUD1200	0x80
#define	BAUD2400	0xA0
#define	BAUD4800	0xC0
#define	BAUD9600	0xE0

#define	NOPARITY	0x00	/* available PARITY options */
#define	ODDPARITY	0x08
#define	EVENPARITY	0x18

#define STOPBIT1	0x00	/* available STOP BIT options */
#define STOPBIT2	0x04

#define BITS7		0x02	/* available WORD LENGTH options */
#define BITS8		0x03

#define	MAXBUF		2048	/* input and output queue size   */
				/* If you change this be sure to */
				/* change INT?.ASM's also        */

unsigned char com1_output_queue[MAXBUF];/* COM 1 main Transmit buffer */
unsigned int  com1_out_queue_out;	/* removal pointer from output_queue */
unsigned int  com1_out_queue_ptr;	/* insertion pointer to output_queue */

unsigned char com1_input_queue[MAXBUF];	/* COM 1 main Receive buffer */
unsigned int  com1_in_queue_out;	/* removal pointer from input_queue */
unsigned int  com1_in_queue_ptr;	/* insertion pointer to input_queue */

unsigned char com2_output_queue[MAXBUF];/* COM 2 main Transmit buffer */
unsigned int  com2_out_queue_out;	/* removal pointer from output_queue */
unsigned int  com2_out_queue_ptr;	/* insertion pointer to output_queue */

unsigned char com2_input_queue[MAXBUF];	/* COM 2 main Receive buffer */
unsigned int  com2_in_queue_out;	/* removal pointer from input_queue */
unsigned int  com2_in_queue_ptr;	/* insertion pointer to input_queue */

unsigned int  com1_rs232_error;		/* current COM 1 error code */
unsigned int  com2_rs232_error;		/* current COM 2 error code */

unsigned int  com1_port_status;		/* current COM 1 pin status */
unsigned int  com2_port_status;		/* current COM 2 pin status */

unsigned int  com1_interrupt_status;	/* current COM 1 interrupt status */
unsigned int  com2_interrupt_status;	/* current COM 2 interrupt status */
/*****************************************************************************/
main()
{
	while(kbhit())			/* empty keyboard buffer */
	   getche();			/* throw away the keys without echoing */

	com1_set_interrupt(BAUD1200 |	/* initialize com1 port and interrupt */
			   NOPARITY |
			   STOPBIT1 |
			   BITS8);

	com2_set_interrupt(BAUD1200 |	/* initialize com2 port and interrupt */
			   NOPARITY |
			   STOPBIT1 |
			   BITS8);

	com1_output_queue_insert("This is how to send out COM 1");
	com2_output_queue_insert("This is how to send out COM 2");

	scr_clr();			/* clear screen */

	while(!kbhit())			/* go until a key is hit */
	{
		queue_delete(1);	/* check for incoming COM 1 characters */
		queue_delete(2);	/* check for incoming COM 2 characters */

		show_com_status();	/* show current com1 and com2 status */
	}

	com1_restore_interrupt();	/* replace interrupt vectors before */
	com2_restore_interrupt();	/* leaving otherwise its crash city */
}
/*****************************************************************************

	This function displays the status of the buffer pointers, pin status
	 and interrupt status.

	This is primarily for troubleshooting problems with the interrupts
	and it slows things down to much for continual usage.

*****************************************************************************/
show_com_status()
{
int row,col,temp;

	temp = read_cursor_position();	/* find where cursor currently sits */

	row  = temp >> 8;		/* row is high byte */
	col  = temp & 0xff;		/* column is low byte */

	scr_rowcol(24,0);		/* on the bottom line */

	printf(
"      %04d - %04d   %04d - %04d  %02xh %02xh %04d - %04d  %04d - %04d  %02xh %02xh  ",
		com1_in_queue_ptr,com1_in_queue_out,
		com1_out_queue_ptr,com1_out_queue_out,
		com1_interrupt_status,com1_port_status,

		com2_in_queue_ptr,com2_in_queue_out,
		com2_out_queue_ptr,com2_out_queue_out,
		com2_interrupt_status,com2_port_status
		);
	if(row != 24)
		scr_rowcol(row,col);	/* put cursor back where it was */
	else
	{
		scroll(1,1,80,24,1);	/* scroll screen up             */
		scr_rowcol(23,0);	/* and start new line           */
	}
}
/*****************************************************************************
	This function looks at the COM ? input and output queue pointers
	and displays the characters if they are not equal.
*****************************************************************************/
queue_delete(port)
int port;
{
int row,temp;

	switch(port)
	{
	case 1:		/* COM 1 reception queue check */

		while(com1_in_queue_ptr != com1_in_queue_out)	/* test for presence of char */
		{
			if(com1_in_queue_out == MAXBUF+1)/* are you at the buffers end? */
				com1_in_queue_out = 0;	/* YES! then reset to beginning */

		     if(com1_input_queue[com1_in_queue_out] < 0x20)
			printf("%x",com1_input_queue[com1_in_queue_out++]);
		     else
			printf("%c",com1_input_queue[com1_in_queue_out++]);
			

			if(com1_rs232_error & 0x1e)	/* mask off non-error conditions */
			{
			temp = read_cursor_position();	/* find where cursor currently sits */

			row  = temp >> 8;		/* row is high byte */
			if(row == 24)
			{
				scroll(1,1,80,24,1);	/* scroll screen up             */
				scr_rowcol(23,0);	/* and start new line           */
			}

				if(com1_rs232_error & 0x10)
					printf("Break Detect");

				if(com1_rs232_error &0x08)
					printf("Framing Error");

				if(com1_rs232_error & 0x04)
					printf("Parity Error");

				if(com1_rs232_error & 0x02)
					printf("Overrun Error");

				com1_rs232_error = 0;	/* reset to no errors */
			}
		}
		break;
	case 2:			/* COM 2 reception queue check */
		while(com2_in_queue_ptr != com2_in_queue_out)	/* test for presence of char */
		{
			if(com2_in_queue_out == MAXBUF+1)/* are you at the buffers end? */
				com2_in_queue_out = 0;	/* YES! then reset to beginning */
	
		     if(com2_input_queue[com2_in_queue_out] < 0x20)
			printf("%x",com2_input_queue[com2_in_queue_out++]);
		     else
			printf("%c",com2_input_queue[com2_in_queue_out++]);
	
			if(com2_rs232_error & 0x1e)	/* mask off non-error conditions */
			{
			temp = read_cursor_position();	/* find where cursor currently sits */

			row  = temp >> 8;		/* row is high byte */
			if(row == 24)
			{
				scroll(1,1,80,24,1);	/* scroll screen up             */
				scr_rowcol(23,0);	/* and start new line           */
			}
				if(com2_rs232_error & 0x10)
					printf("Break Detect");
	
				if(com2_rs232_error &0x08)
					printf("Framing Error");

				if(com2_rs232_error & 0x04)
					printf("Parity Error");

				if(com2_rs232_error & 0x02)
					printf("Overrun Error");

				com2_rs232_error = 0;	/* reset to no errors */
			}
		}
		break;
	}
}
/****************************************************************************
       This function places characters into the COM 1 output queue.

       Call with:
          com1_output_queue_insert("Send this out COM 1");

      Characters will begin sending as soon as the interrupts are enabled.
*****************************************************************************/
com1_output_queue_insert(text)
char *text;
{
	while(*text)			/* go until end of string */
	{
		while( (com1_out_queue_ptr + 1) % MAXBUF ==
			com1_out_queue_out      % MAXBUF)
		{
			outp(0x3f9,0x0f);		/* enable rs232 xmit interrupts */
			printf("COM1 XMIT FULL");
		}

		com1_output_queue[com1_out_queue_ptr++] =
					*text++;     /* install char to xmit */
		outp(0x3f9,0x0f);		     /* enable rs232 xmit interrupts */

		if(com1_out_queue_ptr == MAXBUF +1)
			com1_out_queue_ptr = 0;	/* reset pointer if wrap-around */

	}
}
/****************************************************************************
       This function places characters into the COM 2 output queue.

       Call with:
          com2_output_queue_insert("Send this out COM 2");

      Characters will begin sending as soon as the interrupts are enabled.
*****************************************************************************/
com2_output_queue_insert(text)
char *text;
{
	while(*text)			/* go until end of string */
	{
		while( (com2_out_queue_ptr + 1) % MAXBUF ==
			com2_out_queue_out      % MAXBUF)
		{
			outp(0x2f9,0x0f);		/* enable rs232 xmit interrupts */
			printf("COM2 XMIT FULL");
		}

		com2_output_queue[com2_out_queue_ptr++] =
					*text++;     /* install char to xmit */
		outp(0x2f9,0x0f);		     /* enable rs232 xmit interrupts */

		if(com2_out_queue_ptr == MAXBUF +1)
			com2_out_queue_ptr = 0;	/* reset pointer if wrap-around */

	}
}
/***************************************************************************
	This function returns the current cursor position.

	call with:
		variable = read_cursor_position();

	returns int with  row in high byte
		       column in low  byte
***************************************************************************/
int read_cursor_position()
{
union REGS regs;
int temp;

	regs.h.ah = 3;			/* func. 3 is read cursor */
	regs.h.bh = 0;			/* video page zero        */
	int86(0x10,&regs,&regs);	/* int 10h                */

	temp  = regs.h.dh * 256;	/* row in High byte       */
	temp |= regs.h.dl;		/* col in Low  byte       */

	return(temp);			/* pass value back */
}
/***************************************************************************
	This function positions the cursor at the specified row and column

	call with:
		scr_rowcol(row,col);

	where row is an integer from 0 to 24.
	and   col is an integer from 0 to 80.

***************************************************************************/
scr_rowcol(row, col)
int row,col;
{
union REGS regs;
		regs.h.ah = 2;		/* display function 2 */
		regs.h.dh = row;	/* assign row         */
		regs.h.dl = col;	/* assign column      */
		regs.h.bh = 0;		/* video page zero    */
		int86(0x10,&regs,&regs);/* int 10h            */
}
/****************************************************************************/
scr_clr()
{
union REGS regs;

	regs.h.ah = 6;		/* video function 6        */
	regs.h.al = 0;		/* 0 means clear screen    */
	regs.h.bh = 0x1f;	/* set video attributes    */
	regs.x.cx = 0;		/* top left row and column */
	regs.h.dh = 24;		/* bottom right row        */
	regs.h.dl = 79;		/* bottom right column     */
	int86(0x10,&regs,&regs);/* int 10h                 */
}
/****************************************************************************/
scroll(lcol,trow,rcol,brow,lines)   /* scroll a screen area up */
   int lcol, trow, rcol, brow, lines;
{
   union REGS regs;
   regs.h.bh = 0;			/* video page 0 */
   regs.h.dl = --lcol;
   regs.h.dh = --trow;
   regs.h.ah = 2;			/* set cursor position */
   int86(0x10,&regs,&regs);

   regs.h.bh = 0x1f;			/* video attributes */
   regs.h.cl = lcol;
   regs.h.ch = trow;
   regs.h.dl = --rcol;
   regs.h.dh = --brow;
   regs.h.al = lines;
   regs.h.ah = 6;			/* do the scroll */
   int86(0x10,&regs,&regs);
}
