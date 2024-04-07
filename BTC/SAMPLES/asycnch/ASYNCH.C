/*
 * ASYCNCH.C A simple, low-level, interrupt driven comm driver for
 * Turbo/Borland C. No assembly required!
 *
 * (C) Copyright 1991 Steve Resnick, Asylum Software.
 * License granted for personal or educational use.
 * Commercial use licensed only through registration with the author.
 *
 * Steve Resnick - 530 Lawrence Expressway, Box 374, Sunnyvale, Ca 94086
 * resnicks@netcom.com, steve@apple!camphq,
 * steve.resnick@f105.n143.z1.FIDONET.ORG
*/
#include <stdio.h>
#include <dos.h>
#include "asynch.h"
/*
 * This table is used to translate a BPS rate to a baud rate divisor for the
 * UART
*/
struct _baud
{
	word	Rate, Divisor;
} BaudTable[] =
{
	110,1040,150,768,300,384,600,192,1200,96,2400,48,4800,24,9600,12,
	19200,6,		/* I have no idea if this works */
} ;
#if DEBUG
byte PICVals[2][2];		/* Mirror PIC values to these arrays */
byte DataTable[2][20];		/* Mirror UART values to these arrays */
byte portinb(word port);	/* Use our own port I/O functions */
void portoutb(word port, byte data);
#else
#define portinb(p) inportb(p)
#define portoutb(p,b) outportb(p,b)
#endif
uart cports[4] = {COM1_DEFAULTS,COM2_DEFAULTS};

/*
 * SetPortParms sets up the UART line parameters and baud rate.
 * If Base, IRQ, and BreakLength are specified as 0, defaults are used.
 * (See ASYNCH.H)
 * This may only be called for an open port.
*/
int SetPortParms(int PortNo, int Baud, int Parity, int Data, int Stop,
		 int Base, int IRQ, int BreakLength)
{
	int i, b;
	if (PortNo > 3 || PortNo < 0)
		return EINVPORT;
	if (cports[PortNo].status & PS_ENABLED == 0)
		return EPORTNOTOPEN;
		
	if (Base)
		cports[PortNo].addr = Base;
	if (IRQ)
		cports[PortNo].irqline = IRQ;
	if (BreakLength)
		cports[PortNo].break_length = BreakLength;

	cports[PortNo].parms = 0;
	cports[PortNo].parms |= Data;
	cports[PortNo].parms |= Stop;
	cports[PortNo].parms |= Parity;
	if (Baud != 0)
	{
		for (i = 0, b = -1; i < dim(BaudTable); i++)
			if (BaudTable[i].Rate == Baud)
				b = i;
		if (b == -1)
			return EINVBAUDRATE;

		cports[PortNo].baud = BaudTable[b].Divisor;
		InitBaud(PortNo);
	}
	else if (cports[PortNo].baud == 0)
		return EINVBAUDRATE;
	portoutb(cports[PortNo].addr+LCR_REG,cports[PortNo].parms);
	return 0;
}
/*
 * copen opens a comm port spcified by PortNo. Input and Output buffers are
 * allocated according to the BufSiz parameter. 
 * If Base, IRQ, and BreakLength are specified as 0, defaults are used.
 * (See ASYNCH.H)
 *
 * When the port is opened, the following takes place:
 * Buffer memory is allocated
 * CommPort options are set
 * The interrupt vector is set based on IRQ+8
 * The 8259 is enabled to recieve interrupts.
 * The 8250 IE mask is set
 * The 8250 OUT2 is asserted, enabling the interrupt line from the UART to the
 * PIC.
*/
int copen(int PortNo, int BufSiz, int Baud, int Parity, int Data, int Stop,
		  int Base, int IRQ, int BreakLength)
{
	uart * Cp;
	int Rc;
	if (PortNo > 3 || PortNo < 0)
		return EINVPORT;
	if (cports[PortNo].status & PS_ENABLED)
		return EPORTALREADYOPEN;
	Cp = &cports[PortNo];
	if ((Cp->InBuffer = Qalloc(BufSiz)) == NULL)
		return EMEMALLOC;
	if ((Cp->OutBuffer = Qalloc(BufSiz)) == NULL)
	{
		Qfree(Cp->InBuffer);
		return EMEMALLOC;
	}
	Cp->status |= PS_ENABLED;
	Rc = SetPortParms(PortNo,Baud,Parity,Data,Stop,Base,IRQ,BreakLength);
	if (Rc)
	{
		Qfree(Cp->InBuffer);
		Qfree(Cp->OutBuffer);
		Cp->status = 0;
		return Rc;
	}
	SetIntVector(PortNo);
	disable();
	SetPIC(PortNo,1);
	SetUARTIe(PortNo);
	enable();
	ControlDTR(PortNo,ON);
	return 0;
}
/*
 * cclose closes a serial port by disabling interrupts on the UART, then on
 * the PIC and finally, returns the original vector to the IVT.
*/
int cclose(int PortNo)
{
	uart * Cp;
	int Rc;
	if (PortNo > 3 || PortNo < 0)
		return EINVPORT;
	if (cports[PortNo].status & PS_ENABLED == 0)
		return EPORTNOTOPEN;
	Cp = &cports[PortNo];
	disable();
	ClrIntVector(PortNo);
	SetPIC(PortNo,0);
	enable();
	Cp->status = 0;
	Qfree(Cp->InBuffer);
	Qfree(Cp->OutBuffer);
	portoutb(Cp->addr + IE_REG,0);
	portoutb(Cp->addr + MCR_REG,1);
	return 0;
}
/*
 * InitBaud sets the baudrate on the UART by setting the divisor latch access
 * bit (DLAB) then writing the divisor's low byte,then the divisors high byte.
 * For baudrates >= 600 BPS, the high byte should be 0. (See ASYNCH.H)
*/
int InitBaud(int PortNo)
{
	uart * Cp;
	byte bdata;
	if (PortNo > 3 || PortNo < 0)
		return EINVPORT;
	if (cports[PortNo].status & PS_ENABLED == 0)
		return EPORTNOTOPEN;
	Cp = &cports[PortNo];
	bdata = portinb(Cp->addr + LCR_REG);
	portoutb(Cp->addr + LCR_REG,bdata | DLAB);
	portoutb(Cp->addr + DIV_LOW,LOBYTE(Cp->baud));
	portoutb(Cp->addr + DIV_HI, HIBYTE(Cp->baud));
	portoutb(Cp->addr + LCR_REG,Cp->parms);
	return 0;
}
/*
 * SetPIC sets or clears the interrupt enable mask on the PIC for a
 * particular interrupt. This function will work for both the master
 * and the slave 8259A's in an AT.
*/
SetPIC(int PortNo, int State)
{
static	int States[] = {-1,-1,-1,-1};
	int PICAddr = PIC;
	byte mask;
	if (PortNo > 3 || PortNo < 0)
		return EINVPORT;
	if(State == 0 && States[PortNo] == -1)
		return -1;
	if (cports[PortNo].irqline > 7)
	{
		mask = ( 1 << (cports[PortNo].irqline - 8));
		PICAddr = 0xA0;
	}
	else
		mask = (1 << cports[PortNo].irqline);
 
	if (State == 0)
	{
		States[PortNo] = portinb(PIC+1);
		portoutb(PICAddr+1,(byte)States[PortNo]|mask);
		return 0;
	}
	mask = ~mask;	
	States[PortNo] = portinb(PICAddr+1);
	portoutb(PICAddr+1,States[PortNo] & mask);
	portinb(PICAddr+1);
	return 0;
}
/*
 * EOI Sends a non-specific end of interrupt to the 8259A specific to the
 * IRQ line of a com port
*/
int EOI(int PortNo)
{
	if (PortNo > 3 || PortNo < 0)
		return EINVPORT;
	if (cports[PortNo].status & PS_ENABLED == 0)
		return EPORTNOTOPEN;
	if (cports[PortNo].irqline > 7)
		portoutb(0xA0,0x20);
	else
		portoutb(0x20,0x20);
	return 0;
}
/*
 * This is the interrupt handler for ALL comm ports. Each ISR for a comm
 * port calls this routine passing the index into the UART definition array
 * so that values may be extracted for things like UART address, etc.
 *
 * This is a re-entrant function, although interrupts are disabled to ensure
 * that interrupts are handled one at a time for each UART. This may be
 * over-kill and may need to be changed in the future.
*/
void IsrHandler(int PortNo)
{
	uart * Cp;
	byte Idv, IIDvalue=0;
	disable();
	Cp = &cports[PortNo];
	IIDvalue = portinb(Cp->addr+ IID_REG);
	if ((IIDvalue & IID_PENDING) == 0)
	{
		IIDvalue &= IID_MASK ;
		Idv = IIDvalue >> 1;
		if (Idv & IID_DATA)
			QinsertChar(Cp->InBuffer,portinb(Cp->addr));

		if (Idv & IID_TEMPTY)
			DrainOutQueue(PortNo);

		if (Idv & IID_LSTAT)
			Cp->lstat = portinb(Cp->addr+ LSR_REG);

		if (Idv & IID_MSTAT)
			Cp->mstat = portinb(Cp->addr+ MSR_REG);
	
		portinb(Cp->addr+5);
		portinb(Cp->addr+6);
	} 
	enable();
	SetUartOUT2(PortNo);
	EOI(PortNo);
}
/*
 * Dummy ISR's
 * Each comm port needs to have it's own ISR, but we need only one real
 * handler. Since an ISR shared my multiple interrupts cannot determine
 * it's source, we set up these dummy ISR's to call IsrHandler specifying
 * the port which needs service.
*/
void interrupt com1isr()
{
	IsrHandler(0);
}
void interrupt com2isr()
{
	IsrHandler(1);
}
void interrupt com3isr()
{
	IsrHandler(2);
}
void interrupt com4isr()
{
	IsrHandler(3);
}
/*
 * SetIntVector sets the appropriate interrupt vector (correctly for both
 * (master and slaved IRQ's) based on the IRQ. The original vector is saved
 * in the UART structure so that it may be reset upon termination.
*/
SetIntVector(int PortNo)
{
	int NewInt;
static	void interrupt (*isrs[])() = {com1isr,com2isr,com3isr,com4isr};
	if (PortNo > 3 || PortNo < 0)
		return EINVPORT;
	if (cports[PortNo].status & PS_ENABLED == 0)
		return EPORTNOTOPEN;
	cports[PortNo].oldisr = getvect(IRQINT(cports[PortNo].irqline));
	NewInt = IRQINT(cports[PortNo].irqline);
	setvect(NewInt,isrs[PortNo]);
	return 0;
}
/*
 * ClrIntVector revectors interrupts back to the original vector before we
 * loaded up.
*/
ClrIntVector(int PortNo)
{
	if (PortNo > 3 || PortNo < 0)
		return EINVPORT;
	if (cports[PortNo].status & PS_ENABLED == 0)
		return EPORTNOTOPEN;
	setvect(IRQINT(cports[PortNo].irqline),cports[PortNo].oldisr);
	return 0;
}
/*
 * SetUARTIe sets the interrupt enable mask on the UART.
 * SetUartOUT2 is then called to unmask interrupts from the UART to the PIC
*/
SetUARTIe(int PortNo)
{
	int i;
	word IntFlags = IIV_LSTAT | IIV_RDATA | IIV_MSTAT | IIV_XMITE;
	if (PortNo > 3 || PortNo < 0)
		return EINVPORT;
	if (cports[PortNo].status & PS_ENABLED == 0)
		return EPORTNOTOPEN;
	for (i = 5; i < 8; i++)
		inportb(cports[PortNo].addr+i);

	portoutb(cports[PortNo].addr + IE_REG, IntFlags);
	SetUartOUT2(PortNo);
	return 0;
}
/*
 * SetUARTOut2 set's the OUT2 line on the UART high, allowing interrupts
 * to pass from the UART to the PIC (thanks, IBM)
*/
SetUartOUT2(int PortNo)
{
	byte IoData;	
	IoData = portinb(cports[PortNo].addr + MCR_REG);
	portoutb(cports[PortNo].addr + MCR_REG,IoData | UARTIEMASK);
	return 0;
}
/*
 * DrainOutQueue reads characters from the output queue to the UART, as
 * long as the UART is ready for data. If it is not ready, we return
 * immediately.
*/
DrainOutQueue(int PortNo)
{
	byte IoData;
	char Ch;
	if (PortNo > 3 || PortNo < 0)
		return EINVPORT;
	if (cports[PortNo].status & PS_ENABLED == 0)
		return EPORTNOTOPEN;

	while(1)
	{
		IoData = portinb(cports[PortNo].addr + LSR_REG);
		if (IoData & 0x20 || IoData & 0x40)
		{
			Ch = QreadChar(cports[PortNo].OutBuffer);
			if (Ch == -1)
				break ;
			portoutb(cports[PortNo].addr,Ch);
			continue ;
		}
		break ;
	}
	return 0;
}
/*****************************************************************************
 ****************************** User Functions ******************************/
/*
 * cgetc reads a character from the input queue and returns it, or -1 if no
 * character is available.
*/ 
cgetc(int PortNo)
{
	if (PortNo > 3 || PortNo < 0)
		return -10 - EINVPORT;
	if (cports[PortNo].status & PS_ENABLED == 0)
		return -10 - EPORTNOTOPEN;
	return QreadChar(cports[PortNo].InBuffer);
}
/*
 * cputc writes a character to the output queue
*/
cputc(int PortNo, char Ch)
{
	if (PortNo > 3 || PortNo < 0)
		return -10 - EINVPORT;
	if (cports[PortNo].status & PS_ENABLED == 0)
		return -10 - EPORTNOTOPEN;

	QinsertChar(cports[PortNo].OutBuffer,Ch);
	DrainOutQueue(PortNo);
	return 0;
}
/*
 * SendBreak sets a break condition on the serial line and holds it
 * for break_lenght miliseconds
*/
int SendBreak(int PortNo)
{
	byte IoData;
	if (PortNo > 3 || PortNo < 0)
		return EINVPORT;
	if (cports[PortNo].status & PS_ENABLED == 0)
		return EPORTNOTOPEN;
	IoData = portinb(cports[PortNo].addr + LCR_REG);
	portoutb(cports[PortNo].addr + LCR_REG,IoData | 0x40);
	delay(cports[PortNo].break_length);
	portoutb(cports[PortNo].addr + LCR_REG,IoData);
	return 0;	
}
/*
 * ControlDTR raises or lowers DTR on a specific port
*/
int ControlDTR(int PortNo, int State)
{
	byte IoData;
	if (PortNo > 3 || PortNo < 0)
		return EINVPORT;
	if (cports[PortNo].status & PS_ENABLED == 0)
		return EPORTNOTOPEN;
	IoData = portinb(cports[PortNo].addr + MCR_REG);
	if (State == ON)
		portoutb(cports[PortNo].addr + MCR_REG,(byte)(IoData | 1));
	else 
		portoutb(cports[PortNo].addr + MCR_REG,(byte)(IoData & ~1)&0xFF);
	return 0;
}
/******************************************************************************
****************************** Debugging Functions ***************************/
#if DEBUG
void portoutb(word port, byte data)
{
	int Idx = port - 0x3f8;
	if (Idx > -1 && Idx < 20)
		DataTable[0][Idx] = data;
	if (port >0x1F && port < 0x22)
	{
		Idx = port - 0x20;
		PICVals[0][Idx] = data;
	}
	outportb(port,data);
}
byte portinb(word port)
{
	int Idx = port - 0x3F8;
	byte Data;
	Data = inportb(port);
	if (port >0x1F && port < 0x22)
	{
		Idx = port - 0x20;
		PICVals[1][Idx] = Data;
	}
	else if (Idx > -1 && Idx < 20)
		DataTable[1][Idx] = Data;
	return Data;
}
#endif

	
