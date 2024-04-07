/*
**	litecomm.h
**		Communications support for Datalight (tm) C
**		Copyright (c) 1987, 1988 - Information Technology, Ltd.,
**							 All Rights Reserved
**  Revisions -
**		01		01/08/88	Modify for DLC 3.21
**							remove lc_sbrk define
**		2.70	02/25/88	abandon DLC support
**		5.00	02/10/89	change CCB definition for new txready
**							add define for lc_vport
*/

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#define ERR (-1)

#include <stddef.h>

#ifndef LITECOMM_H				/* if not already included		*/

#define COM1BASE	0x3f8		/* base for com port 1 */
#define COM2BASE	0x2f8		/* base for com port 2 */
#define IRQ1		0x10		/* int req mask for port 1 - irq4 */
#define IRQ2		0x08		/* int req mask for port 2 - irq3 */
#define COM1VEC		0x0c		/* vector for port 1 */
#define COM2VEC		0x0b		/* vector for port 2 */

#ifdef KRNL
unsigned COM3BASE = 0x3e8;		/* user changeable com port 3 */
unsigned COM4BASE = 0x2e8;		/* user changeable com port 4 */
char IRQ3 = IRQ1;				/* ditto for int levels */
char IRQ4 = IRQ2;
unsigned COM3VEC = 0x0c;
unsigned COM4VEC = 0x0b;
#else
extern unsigned COM3BASE;
extern unsigned COM4BASE;
extern char IRQ3;
extern char IRQ4;
extern unsigned COM3VEC;
extern unsigned COM4VEC;
#endif

/*
**	special control characters
*/
#define	XON		0x11
#define	XOFF	0x13

/*
**	8250 register defines (offsets from base)
*/
#define	THREG	0				/* transmit hold register */
#define RDREG	0				/* input data register */
#define LSBDIV	0				/* least sig. byte of baud rate divisor */
#define MSBDIV	1				/* most sig, only when line ctl bit 7 = 1 */
#define IEREG	1				/* interrupt enable register */
#define IIDREG	2				/* interrupt ID register */
#define LCREG	3				/* line control register */
#define MCREG	4				/* modem control register */
#define LSREG	5				/* line status register */
#define MSREG	6				/* modem status register */

/*
**	PARITY SETTINGS used in conjunction with above
*/
#define	NPARITY	0x00			/* no parity */
#define OPARITY	0x08			/* odd parity */
#define EPARITY	0x18			/* even parity */
#define	MPARITY	0x28			/* mark parity */
#define SPARITY	0x38			/* space parity */

/*
**	DATA and STOP BIT settings
*/
#define BIT5	0x00			/* 5 data bits */
#define BIT6	0x01			/* 6 data bits */
#define BIT7	0x02			/* 7 data bits */
#define BIT8	0x03			/* 8 data bits */
#define STOP1	0x00			/* 1 stop bit */
#define STOP2	0x04			/* 2 stop bit */

/*
**	Baud Rate Settings
*/
#define B110	110
#define	B300	300
#define	B600	600
#define B1200	1200
#define	B2400	2400
#define	B4800	4800
#define	B9600	9600
#define	B19200	19200

/*
** SPECIAL PURPOSE SETTINGS - internal use only
*/
#define	BREAK_ON 0x40			/* enable transmitter break */
#define	SETBAUD	0x80			/* enable access to baud rate divisor */

#define	DTR		0x01			/* turn on DTR modem signal */
#define RTS		0x02			/* turn on RTS modem signal */
#define	OUT2	0x08			/* enable OUT2 for interrupts */
#define	LOOPBACK 0x10			/* enable loopback mode */

#define	RDINT	0x01			/* enable receive data int */
#define	THREINT	0x02			/* enable transmit hold reg empty int */
#define	BRKINT	0x04			/* enable break/error int */
#define	MSTINT	0x08			/* enable modem change int */

#define NOPEND	0x01			/* no interrupts pending */
#define INTMST	0x00			/* modem change int */
#define	INTTHRE	0x02			/* THRE int */
#define	INTRD	0x04			/* receive data int */
#define	INTBRK	0x06			/* break/error int */

/*
**	line status register values
*/
#define	RDRDY	0x01			/* receive data ready */
#define ORUNERR	0x02			/* over-run error */
#define	PARERR	0x04			/* parity error */
#define FRMERR	0x08			/* framing error */
#define	BRKDET	0x10			/* break detect */
#define THREMP	0x20			/* transmit hold reg empty */
#define TSREMP	0x40			/* transmit shift reg empty */
#define TIMEOUT	0x80			/* timeout */

/*
**	modem status register values
**		Bits 0 - 3 define which signal(s) have changed
**				   with a value of 1 indicating a change has occurred
**		Bits 4 - 7 indicate the current state of each of the 4 signals
*/
#define	CTSCHG	0x01			/* delta CTS */
#define DSRCHG	0x02			/* delta DSR */
#define	RICHG	0x04			/* delta RI */
#define	DCDCHG	0x08			/* delta DCD */
#define	CTS		0x10			/* Clear To Send */
#define DSR		0x20			/* Data Set Ready */
#define RI		0x40			/* Ring Indicator */
#define DCD		0x80			/* Data Carrier Detect */

/*
** macros for litecomm
*/

/*
** reduce lc_vport to a macro to speed up code
*/

#define lc_vport(x) (ports[(x-1)&0x0f])

/*
** defines to resolve differences between MSC and TURBOC
** semantics
*/
#ifdef M_I86
#define IVPTR(x) void (interrupt far *x)()
#define getvect(x) _dos_getvect(x)
#define setvect(x,y) _dos_setvect(x,y)
#define enable _enable
#define disable _disable
#define inportb(x) inp(x)
#define outportb(x,y) outp(x,y)
#define MK_FP(seg,ofs)	((void far *) \
			   (((unsigned long)(seg) << 16) | (unsigned)(ofs)))
#endif

#ifdef __TURBOC__
#define IVPTR(x) void interrupt (*x)()
#endif

#ifdef M_I86
#pragma pack(1)
#endif
typedef	struct
{
    IVPTR(orgvect);             /* original vector */
	unsigned	orgirq;			/* original irq settings */
	unsigned	baseport;		/* baseline port */
	unsigned	pvector;		/* int vector for port */
	unsigned	pirq;			/* port int req mask */
	volatile unsigned mdmstat;	/* current modem status */
	unsigned	mdmctlr;		/* current modem control values */
	volatile unsigned lasterr;	/* last error return */
	volatile unsigned brkrecd;  /* break was detected */
	volatile unsigned txready;	/* TRUE if tranmitter available */
	unsigned	intmask;		/* current active ints */
	char		*inbuff;		/* input ring buffer */
	char		*inbuffend;		/* internal calcs */
	int 		inbuflen;		/* size of buffer */
	volatile int inbufcnt;		/* chars in buffer now */
	int			inbufmrk;		/* trigger point for xoff */
	volatile char *inhead;		/* pointer to insert */
	volatile char *intail;		/* pointer to remove */
	int			xstate;			/* TRUE if XON/XOFF active */
	volatile int inxrecd;		/* TRUE if XOFF rec'd */
	volatile int outxsent;		/* TRUE if XOFF sent */
	char		*outbuff;		/* output ring buffer */
	char		*outbuffend;	/* internal calcs */
	int			outbuflen;		/* size of buffer */
	volatile int outbufcnt;		/* chars in buffer now */
	volatile char *outhead;		/* pointer to insert */
	volatile char *outtail;		/* pointer to remove */
	int		rch;		/* port's input char buffer */
	char	far *	oldstk;		/* save area for old stack */
	char	far *	newstk;		/* pointer to handler int stack */
	char	far *	stkbase;	/* pointer to base of int stack */
} COMM;

#ifdef M_I86
#pragma pack()
#endif

#ifdef KRNL
COMM *ports[16] =	{
					NULL,NULL,NULL,NULL,
					NULL,NULL,NULL,NULL,
					NULL,NULL,NULL,NULL,
					NULL,NULL,NULL,NULL
					};
#else
extern COMM *ports[16];
#endif

#undef USEHUGE

#ifdef M_I86
#ifdef M_I86CM
#define USEHUGE
#endif

#ifdef M_I86LM
#define USEHUGE
#endif
#endif

#ifdef __TURBOC__
#ifdef __COMPACT__
#define USEHUGE
#endif

#ifdef __LARGE__
#define USEHUGE
#endif
#endif

#include "litecomm.fns"

#define LITECOMM_H	1

#endif
