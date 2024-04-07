#if !defined(ASYNCH_INCLUDED)	/* Prevent multiple includes */
#define ASYNCH_INCLUDED
#include "circleq.h"
typedef	unsigned char	byte;
typedef unsigned short  word;
typedef unsigned long	dword;
typedef struct _lcr
{
	word	databits	: 2;
	word	stopbits	: 1;
	word    parity		: 2;
	word	stuckparity	: 1;
	word	breakenable 	: 1;
	word	dlab 		: 1;
} lcr;
typedef struct _uart
{
	word 	addr;
	byte 	irqline;
	word 	baud;
	byte 	lstat;

	byte 	mstat;
	byte 	status;
	byte	break_length;
	byte	parms;
	CircleQ *InBuffer;
	CircleQ *OutBuffer;
	void interrupt (*oldisr)();
} uart;
#define LineStatus(n) ((n >= 0 && n <4)  ? cports[n].lstat : EINVPORT)
#define ModemStatus(n) ((n >= 0 && n <4)  ? cports[n].mstat : EINVPORT)
 
extern uart cports[4];
#define COM1_DEFAULTS\
	{0x3F8,4,0,0,0,0,250,\
	DATA8|STOP1|NONE,\
	NULL,NULL}

#define COM2_DEFAULTS \
	{0x2F8,3,0,0,0,0,250,\
	DATA8|STOP1|NONE,\
	NULL,NULL}

#define PIC	0x20			/* 8259A PIC Address */
#define IRQINT(n) ((n < 9) ? n+8 : (n-9)+0x70)
					/* Make int value from IRQ */
#define IID(n)	(n & 7) 		/* Get Interrupt ID from reg val */
/*
 * Define Interrupt ID values from the interrupt ID register
*/
#define IID_PENDING	0x01
#define IID_LSTAT	0x00			/* Line status change */
#define IID_DATA	0x02			/* Data is available from 8250 */
#define IID_TEMPTY	0x04			/* Transmitter buffer empty */
#define IID_MSTAT	0x06			/* New modem status */
#define IID_MASK	0x07

#define IIV_LSTAT	0x01
#define IIV_XMITE	0x02
#define IIV_RDATA	0x04
#define IIV_MSTAT	0x08
/*
 * Define some offsets from the UART base address for various registers
*/
#define XMT_REG		0x00		/* Transmitter holding buffer */
#define RCV_REG		0x00		/* Reciever holding buffer */
#define DIV_LOW		0x00		/* Divisor low byte when DLAB=1 */
#define DIV_HI		0x01		/* Devisor high byte when DLAB=1 */

#define IE_REG		0x01		/* Interrupt enable register */
#define IID_REG		0x02		/* Interrupt ID register */
#define LCR_REG		0x03		/* line control register */
#define MCR_REG		0x04		/* Modem control register */
#define LSR_REG		0x05		/* line status register */
#define MSR_REG		0x06		/* modem status register */

/*
 *	Stuff for the line control register
*/
#define NONE	0x00			/* No Parity */
#define ODD	0x08			/* Odd Parity */
#define EVEN	0x18			/* Even Parity */
#define STOP1	0x00			/* Need I explain these? */
#define STOP2	0x04
#define DATA5	0x00
#define DATA6	0x01
#define DATA7	0x02
#define DATA8	0x03
#define DLAB	0x80

#define UARTIEMASK	0x08		/* Mask used to enable interrupts on
					   the 8250 OUT2 line */
#define PS_ENABLED	0x01
#define PS_INTERRUPT	0x02		/* Not used */

enum errors {EPORTNOTOPEN=1,EPORTALREADYOPEN,EINVBAUDRATE,EINVPORT,EMEMALLOC};
#define COM1	0
#define COM2	1
#define ON	1
#define OFF	0
#define HIBYTE(n) ((n >> 8) & 0xFF)
#define LOBYTE(n) (n & 0xFF)

#if !defined(dim)
#define dim(x) (sizeof(x)/sizeof(x[0]))
#endif
int SetPortParms(int PortNo, int Baud, int Parity, int Data, int Stop,
		  int Base, int IRQ, int BreakLength);
int copen(int PortNo, int BufSiz, int Baud, int Parity, int Data, int Stop,
		  int Base, int IRQ, int BreakLength);
int cclose(int PortNo);
int InitBaud(int PortNo);
SetPIC(int PortNo, int State);
cgetc(int PortNo);
cputc(int PortNo, char Ch);

#endif

