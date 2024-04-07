/*
**  QBTTL is a complete, if somewhat limited, terminal emulation
**  program  designed to demonstrate the use of the LiteComm(tm)
**  ToolBox.  The executable version is included so that you can
**  try it out while viewing the code.  To successfully create a
**  new version of QBTTL, you must have the XMODEM and WXMODEM engines
**  which are provided as part of your registration package.  The
**  QUICKB functionality is derived from public domain code, and adapted
**  by Information Technology for use with LiteComm.
**  Also note that the windowing parts of the program are based upon
**  Vitamin-C by Creative Programming.  You must have this product and
**  be a registrant to successfully compile a new version of QBTTL.
**
**  While non-registered users cannot create a new version of QBTTL, as-is
**  we have provided the source as part of the distribution package to
**  help you in your understanding of the way in which the LiteComm
**  Communications ToolBox can be used.
**
**  QBTTL functions as a vidtex terminal when used in conjuction with
**  CompuServe.  In addition, it will run on any communications port from
**  1 thru 4, defaulting to port 2 (COM2).  To execute QBTTL on other than
**  COM2, specify
**    QBTTL n
**  where n is a number from 1 to 4.
**
**  Please note also that, to send a Ctrl-C, you must use the Alt-C keys
**
**  Information Technology, Ltd.
*/

#include "litecomm.h"
#include "litexm.h"
#include <vcstdio.h>

#ifdef __TURBOC__
#include <stdlib.h>
#include <dos.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <io.h>
#include <mem.h>
#endif

#ifdef M_I86
#include <signal.h>
#include <conio.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys\types.h>
#include <sys\stat.h>

#endif

#ifdef __TURBOC__
extern unsigned _stklen = 8192;
#endif

#define CTRLX 0x18

#define ESC 0x1b
#define ENQ 0x05

int nocbrk();
void moveleft();
void moveright();
void moveup();
void movedn();
void show_modem();
void simtab();
void setcursorpos();
void procesc();
void clreos();
void send(void);
void recv(void);
void xupload(void);
void xdnload(void);
void wupload(void);
void wdnload(void);

/*
**  Comm parameters - human-readable form
*/
int baud = 2400;
char parity = 'N';
int data = 8;
int stop = 1;

/*
**  Comm parameters - Litecomm form
*/
unsigned pbaud = 2400;
unsigned pparity = NPARITY;
unsigned pbits = BIT8;
unsigned pstop = STOP1;

unsigned port = 2;

int xmode = 0;                          /* xmodem type */
int yxmode = 0;							/* use small blocks */
int halfd = 0;                          /* half-duplex */
int hostm = 0;                          /* host mode */
int ctlc_hit = FALSE;

char MODEMSET0[] = "ATZ\r";
char MODEMSET1[] = "ATT E0\r";
char MODEMSET2[] = "ATC1 V0 X1 S0=1 M1\r";

COUNT mwin;                             /* for Vitamin-C */
COUNT swin;

COUNT mbd;
COUNT mbg;
COUNT msay;
COUNT mact;
COUNT mnact;
COUNT mtitle;
COUNT sbd;
COUNT sbg;
COUNT ssay;
COUNT sact;
COUNT snact;
COUNT stitle;
char    strbuf[80];                   /* for sprintf usage */

void main(argc, argv)
int	argc;
char *argv[];
{
	COUNT    opt;

#ifdef M_I86
	signal(SIGINT, nocbrk);               /* set Ctrl-Break handler */
#endif

#ifdef __TURBOC__
    ctrlbrk(nocbrk);
#endif

/*
**  check for a port parameter
*/
	if (argc > 1)
	{
		port = atoi(argv[1]);
		if ((port < 1) || (port > 4))
		{
			puts("Invalid Port Specified\n");
			exit(4);
		}
	}

/*
** establish windowing environment
*/
	vcstart(SAVESCRN);
	VC_VIO = 1;
	if ((mwin=wxopen(0,0,23,79,NULL,
		 ACTIVE+CURSOR+SCROLL+COOKED+NOADJ+CENTER, 0, 0)) == -1)
		terror("mwin:Not Enough Memory");
	if ((swin=wxopen(24,0,24,79,NULL,
		 ACTIVE+COOKED+NOADJ, 0, 0)) == -1)
		terror("swin:Not Enough Memory");
	wselect(mwin);

/*
** get display attributes
*/
	wattr(mwin,&mbd, &mbg, &msay, &mact, &mnact, &mtitle, GET);
	wattr(swin,&sbd, &sbg, &ssay, &sact, &snact, &stitle, GET);

/*
** set-up the comm port
*/
	if (comm_opn(port,2400,NPARITY,BIT8,STOP1,2048,2048,FALSE) == -1)
	{
		urgentmsg("ERROR", "Can't open port");
		abort();
	}
	lc_setmdm(port, (DTR | RTS));

	while (1)
	{
		erase();
		at(0,0);
		vcputs("-- MAIN MENU --\r", msay);
		vcputs("T - enter Terminal mode\r", msay);
		vcputs("    Alt-X leaves terminal mode\r", msay);
		sprintf(strbuf, "H - toggle Host mode (now %s)\r",hostm ? "ON":"OFF");
		vcputs(strbuf, msay);
		sprintf(strbuf, "G - toGgle half-duplex mode (now %s)\r",halfd ? "ON":"OFF");
		vcputs(strbuf, msay);
		sprintf(strbuf, "C - change Comm parameters (now %d,%c,%d,%d)\r",
				baud,parity,data,stop);
		vcputs(strbuf, msay);
		sprintf(strbuf, "X - change Xmodem mode (now %s)\r",xmode ? "WIN":"NOR");
		vcputs(strbuf, msay);
		sprintf(strbuf, "Y - toggle Ymodem mode (now %s)\r",yxmode ? "ON":"OFF");
		vcputs(strbuf, msay);
		vcputs("S - Send a file\r", msay);
		vcputs("R - Receive a file\r", msay);
		vcputs("Q - Quit\r\r", msay);

		opt = getone();
		opt = toupper(opt);

		switch (opt)
		{
			case 'T':   terminal();
						break;
			case 'H':   if (hostm)
							hostm = 0;
						else
						{
							hostm = 1;
							halfd = 0;
						}
						break;
			case 'G':   if (halfd)
							halfd = 0;
						else
						{
							halfd = 1;
							hostm = 0;
						}
						break;
			case 'X':   if (xmode)
							xmode = 0;
						else
							xmode = 1;
						break;
			case 'Y':   if (yxmode)
						{
							ymodem = FALSE;
							yxmode = 0;
						}
						else
						{
							yxmode = 1;
							ymodem = TRUE;
						}
						break;
			case 'S':   send();
						break;
			case 'R':   recv();
						break;
			case 'C':   chgcomm();
						break;
		}
		if (opt == 'Q')
			break;                      /* shut down time */
	}                                   /* while (1) */

	comm_close(port,FALSE);
	vcend(CLOSE);
	exit(0);
}                                       /* main */

int nocbrk()
{
#ifdef M_I86
	signal(SIGINT, SIG_IGN);             /* set Ctrl-Break handler */
#endif
    ctlc_hit = TRUE;
    _abort_flag = TRUE;
#ifdef M_I86
	signal(SIGINT, nocbrk);             /* set Ctrl-Break handler */
#endif
    return(TRUE);
}

terminal()
{
	COUNT ch;
	COUNT row, col;
	COUNT twin;

	erase();
	lc_xoff(port,TRUE);

	while (1)
	{
		if ((ch = lc_get(port)) != -1)
			switch (ch & 0x7f)
			{
				case 0x08:             /* BS */
    				moveleft();
					vcputc(' ', msay);
					moveleft();
					break;
				case '\t':				/* HT */
                    simtab();
					break;
				case '\r':              /* CR */
					wcurspos(mwin, &row, &col);
					at(row,0);
					break;
				case DLE:
					twin=wxopen(6,20,13,60,"TRANSFER A FILE",
						BORDER+BD1+ACTIVE+CURSOR+SCROLL+COOKED+NOADJ+CENTER, 0, 0);
					wselect(twin);
					bp_DLE();
					wclose(twin);
					wselect(mwin);
					break;
				case ENQ:
					bp_ENQ();
					break;
				case ESC:
					procesc();
					break;
				default:
					if (pparity != NPARITY)
					    ch &= 0x7f;				/* strip parity */
					vcputc(ch, msay);
					if (hostm)
					{
						lc_put(port,ch);            /* echo back */
						if (ch == '\r')             /* was it return */
						{
							lc_put(port,'\n');      /* echo lf as well */
							vcputc('\n', msay);
						}
					}
			}
		show_modem (lc_mstat(port));

		if (keyrdy())                       /* anything typed ? */
		{
			ch = getone();                  /* get input */
			if (ch == ALT_X)
				return(0);
			if (ch == ALT_C)
				ch = 0x03;
			lc_put(port,ch);                /* xmit the char */
			if (hostm || halfd)             /* local echo needed ? */
			{
				vcputc(ch, msay);
				if (ch == '\r')             /* was it CR */
				{
					vcputc('\n', msay);     /* add LF */
					if (hostm)
						lc_put(port, '\n'); /* and send LF in host mode */
				}
			}
		}
	}                                       /* while */
}

void procesc()
{
	int ch;


	ch = lc_getw(port);					/* wait for actual command */

	switch (ch & 0x7f)
	{
		case 'A':
			moveup();
			break;
		case 'B':
			movedn();
			break;
		case 'C':
			moveright();
			break;
		case 'D':
			moveleft();
			break;
		case 'H':
			at(0,0);
			break;
		case 'I':
			bp_ESC_I();
			break;
		case 'J':
			clreos();
			break;
		case 'K':
			xeraeol(vc.dflt);
			break;
		case 'j':
			erase();
			break;
		case 'Y':
			setcursorpos();
			break;
	}
}										/* procesc */

void show_modem(mdmstat)
int mdmstat;
{
	COUNT lwin;

    if ((mdmstat & 0x0f))			/* any status change ? */
	{
		lwin = wselect(swin);
		erase();
		at(0,2);
		if (mdmstat & CTS)
			vcputs("CTS", ssay);
		at(0,8);
		if (mdmstat & DSR)
			vcputs("DSR", ssay);
		at(0,14);
		if (mdmstat & DCD)
			vcputs("DCD", ssay);
		at(0,20);
		if (mdmstat & RI)
			vcputs("RI", ssay);
		wselect(lwin);
	}
}           							/* show_modem */

void clreos()
{
	xeraeos(vc.dflt);
}                                        /* clreos */

void setcursorpos()
{
	int	ch;
	int	x;
	int	y;

	while((ch = lc_get(port)) == -1)
	;
	y = (ch & 0x7f) - 32;

	while((ch = lc_get(port)) == -1)
	;
	x = (ch & 0x7f) - 32;
	at(y, x);
}

struct text_info
{
	COUNT   wintop,
			winbottom,
			winleft,
			winright,
			curx,
			cury;
};

void gettextinfo(r)
struct text_info *r;
{
	wcoord(mwin, &(r->wintop), &(r->winleft), &(r->winbottom), &(r->winright));
	wcurspos(mwin, &(r->cury), &(r->curx));
}

void moveleft()
{
	int scurx,
		scury;
	struct text_info r;

	gettextinfo(&r);
	scurx = r.curx;
	scury = r.cury;

	scurx--;                             /* decrement x position */
	if (scurx < r.winleft)
	{
	    scury--;
		scurx = r.winright;
		if (scury < r.wintop);
			scury = r.winbottom;
	}
	at(scury, scurx);
}                                        /* moveleft */

void moveright()
{
	int scurx,
		scury;
	struct text_info r;

	gettextinfo(&r);
	scurx = r.curx;
	scury = r.cury;

	scurx++;                             /* decrement x position */
	if (scurx > r.winright)
	{
	    scury++;
		scurx = r.winleft;
		if (scury > r.winbottom);
			scury = r.wintop;
	}
	at(scury, scurx);
}                                        /* moveright */

void moveup()
{
	int scury;

	struct text_info r;

	gettextinfo(&r);
	scury = r.cury;

	scury = r.cury - 1;
	if (scury < r.wintop)
		scury = r.winbottom;
	at(scury, r.curx);
}        								/* moveup */

void movedn()
{
	int scury;

	struct text_info r;

	gettextinfo(&r);
	scury = r.cury;

	scury = r.cury + 1;
	if (scury > r.winbottom)
		scury = r.wintop;
	at(scury, r.curx);
}

void simtab()
{
	int scurx,
		scury;
	struct text_info r;

	gettextinfo(&r);
	scurx = r.curx;
	scury = r.cury;

    do
		scurx++;                         /* bump x position */
	while (scurx % 9);

	if (scurx > r.winright)
	{
	    scury++;
		scurx = r.winleft;
		if (scury > r.winbottom);
		{
			at(r.winbottom, r.winright);
			vcputc('\n', msay);
			return;
		}
	}
	at(scury, scurx);
}                                        /* simtab */


chgcomm()
{
	COUNT opt;
	unsigned sbaud;
	unsigned sparity;
	unsigned sbits;
	unsigned sstop;
	int hbaud;
	char hparity;
	int hdata;
	int hstop;

/*
** get current default settings
*/
	sbaud = pbaud;
	sparity = pparity;
	sbits = pbits;
	sstop = pstop;
	hbaud = baud;
	hparity = parity;
	hdata = data;
	hstop = stop;


	while (1)
	{
		erase();
		vcputs("-- COMM PARAMETERS --\r", msay);
		sprintf(strbuf,"  (Presently %d,%c,%d,%d)\r",
				hbaud,hparity,hdata,hstop);
		vcputs(strbuf, msay);
		vcputs("B - change Baud Rate\r", msay);
		vcputs("P - change Parity\r", msay);
		vcputs("D - change Data bits\r", msay);
		vcputs("S - change Stop bits\r\r", msay);
		vcputs("A - Abandon Changes\r", msay);
		vcputs("Q - Quit to Main Menu\r\r", msay);

		opt = getone();
		opt = toupper(opt);

		switch (opt)
		{
			case 'A':
				return;
			case 'Q':
				pbaud = sbaud;          /* reset the globals */
				pparity = sparity;
				pbits = sbits;
				pstop = sstop;
				baud = hbaud;
				parity = hparity;
				data = hdata;
				stop = hstop;
				comm_setup(port,pbaud,pparity,pbits,pstop);
				return;
			case 'B':
				vcputs("1 - 110, 2 - 300, 3 - 600, 4 - 1200, 5 - 2400\r", msay);
				vcputs("6 - 4800, 7 - 9600, 8 - 19200\r", msay);
				opt = getone();
				switch (opt)
				{
					case '1': hbaud = 110;
							  sbaud = 110;
							  break;
					case '2': hbaud = 300;
							  sbaud = 300;
							  break;
					case '3': hbaud = 600;
							  sbaud = 600;
							  break;
					case '4': hbaud = 1200;
							  sbaud = 1200;
							  break;
					case '5': hbaud = 2400;
							  sbaud = 2400;
							  break;
					case '6': hbaud = 4800;
							  sbaud = 4800;
							  break;
					case '7': hbaud = 9600;
							  sbaud = 9600;
							  break;
					case '8': hbaud = 19200;
							  sbaud = 19200;
							  break;
				}
				break;
			case 'P':
				vcputs("1 - NONE, 2 - EVEN, 3 - ODD, 4 - MARK, 5 - SPACE\r", msay);
				opt = getone();
				switch (opt)
				{
					case '1': hparity = 'N';
							  sparity = NPARITY;
							  break;
					case '2': hparity = 'E';
							  sparity = EPARITY;
							  break;
					case '3': hparity = 'O';
							  sparity = OPARITY;
							  break;
					case '4': hparity = 'M';
							  sparity = MPARITY;
							  break;
					case '5': hparity = 'S';
							  sparity = SPARITY;
							  break;
				}
				break;
			case 'D':
				vcputs("Number of Data Bits (5, 6, 7, 8)\r", msay);
				opt = getone();
				switch (opt)
				{
					case '5': hdata = 5;
							  sbits = BIT5;
							  break;
					case '6': hdata = 6;
							  sbits = BIT6;
							  break;
					case '7': hdata = 7;
							  sbits = BIT7;
							  break;
					case '8': hdata = 8;
							  sbits = BIT8;
							  break;
				}
				break;
			case 'S':
				vcputs("Number of Stop Bits (1, 2)\r", msay);
				opt = getone();
				switch (opt)
				{
					case '1': hstop = 1;
							  sstop = STOP1;
							  break;
					case '2': hstop = 2;
							  sstop = STOP2;
							  break;
				}
				break;
		}
	}
}


void send()
{
	COUNT twin;

	twin=wxopen(6,25,10,50,"SEND A FILE",
		 BORDER+BD1+ACTIVE+CURSOR+SCROLL+COOKED+NOADJ+CENTER, 0, 0);
	wselect(twin);
	if (xmode)
		wupload();
	else
		xupload();
	wclose(twin);
	wselect(mwin);
}

void recv()
{
	COUNT twin;

	twin=wxopen(6,25,10,50,"RECEIVE A FILE",
		 BORDER+BD1+ACTIVE+CURSOR+SCROLL+COOKED+NOADJ+CENTER, 0, 0);
	wselect(twin);
	if (xmode)
		wdnload();
	else
		xdnload();
	wclose(twin);
	wselect(mwin);
}

void getfname(fname)
char *fname;
{
	erase();
	atsay(0,0,"File Name: ");
	empty(fname, 13);
	get(fname,NULL);
	readgets();
	cputs("\r");
}

void xupload(void)
{
	char fname[13];
    int  fd;
    unsigned char buf[1024];
    unsigned char *bpos;
    int toread;
    int tosend;
    unsigned char hdshk;
    int hmode;
    int result;

	getfname(fname);

    ctlc_hit = FALSE;
    if (ctlc_hit == TRUE)
    {
        ctlc_hit = FALSE;
        return;
    }

	if (isblank(fname))
        return;
	trim(fname);

	if ((fd = open(fname, (O_RDONLY|O_BINARY))) == -1)
    {
		urgentmsg("WARNING", "Unable to open the file");
        return;
    }
/*
** the file has been opened successfully...now switch the
** comport mode to 8 bits for xmodem protocol
*/
    if (comm_setup(port,pbaud,NPARITY,BIT8,pstop) == ERR)
    {
		urgentmsg("ERROR","Unable to switch line mode");
        return;
    }
/*
** begin file transmission to receiver -
** in receiver's specified mode
*/
    if (yxmode)
    {
    	ymodem = TRUE;
		toread = 1024;
	}
	else
	{
		ymodem = FALSE;
		toread = 128;
	}

    while (TRUE)
    {
		memset(buf, 0x1a, sizeof(buf));     /* clear buff, short rec */
        if ((tosend = read(fd, buf, sizeof(buf))) < 1)  /* EOF or Error */
        {
            lcxteot(port);            /* send end of file */
			urgentmsg("SUCCESS","End of Transmission");
            break;
        }
		bpos = buf;
		while (TRUE)
		{
			if (tosend <= 0)
				break;						/* block sent completely */
      		if (yxmode)
        		if (tosend != toread)	/* short block */
				{
          			toread = 128;       /* sending short */
          			ymodem = FALSE;
				}
        	result = lcxtrec(port, bpos);
        	switch(result)           /* what action to take ? */
        	{
            	case SUCCESS:
					sprintf(strbuf,"Sent record: %d",(rec-1));
					atsay(1,1,strbuf);
					eraeol();
            		tosend -= toread;
            		bpos += toread;
    				break;
            	case CAN:
					urgentmsg("WARNING","Cancel Received");
                	break;
            	case RETRIES:
					sprintf(strbuf, "Too Many tries...Rec %d",(rec-1));
					urgentmsg("ERROR", strbuf);
                	break;
            	default:
					urgentmsg("ERROR", "Fatal transmission error");
                	break;
        	}
        	if (result != SUCCESS)
            	break;
		}									/* inner while */
		if (result != SUCCESS)
			break;
	}                                          /* while TRUE */
    comm_setup(port,pbaud,pparity,pbits,pstop);
    close(fd);                                 /* close down the file */
}

void xdnload(void)
{
	char fname[13];
    int  fd;
    unsigned char buf[1024];
    unsigned char hdshk;
    int bsize;
    int hmode;
    int result;

    ctlc_hit = FALSE;

	getfname(fname);
    if (ctlc_hit == TRUE)
    {
        ctlc_hit = FALSE;
        return;
    }
	if (isblank(fname))
		return;
	trim(fname);

	if ((fd = open(fname,
         (O_WRONLY|O_CREAT|O_TRUNC|O_BINARY),(S_IREAD|S_IWRITE))) == -1)
    {
		urgentmsg("WARNING", "Unable to open the file");
        return;
    }
/*
** the file has been opened successfully...now switch the
** comport mode to 8 bits for xmodem protocol
*/
    if (comm_setup(port,pbaud,NPARITY,BIT8,pstop) == ERR)
    {
		urgentmsg("ERROR", "Unable to switch line mode");
        return;
    }
/*
** setup conditions for XMODEM receive
*/
    hdshk = CRC;                 /* use crc method */
    hmode = RELAXED;   		     /* normal time delay used */
    while (TRUE)
    {
        while ((result = lcxrrec(port, buf, &bsize, hmode, &hdshk)) == SUCCESS)
        {
            write(fd, buf, bsize);  /* dump the block */
			sprintf(strbuf,"Received record: %d",(rec-1));
			atsay(1,1,strbuf);
			eraeol();
		}
        switch(result)           /* unusual conditions */
        {
            case DUPSEQ:
				atsay(1,1,"Duplicate record seq");
				eraeol();
                break;
            case CAN:
				urgentmsg("WARNING", "Cancelled by host");
                break;
            case EOT:
				urgentmsg("SUCCESS", "Normal Termination");
                break;
            case TOUT:
				urgentmsg("ERROR", "SOH timeout");
                break;
            case RETRIES:
				sprintf(strbuf, "Too Many tries...Rec %d",(rec-1));
				urgentmsg("ERROR", strbuf);
                break;
            default:
				urgentmsg("ERROR", "Fatal transmission error");
                break;
        }
        if ((result != SUCCESS) && (result != DUPSEQ))
            break;
    }                                          /* while TRUE */

    comm_setup(port,pbaud,pparity,pbits,pstop);
    close(fd);                                 /* close down the file */
}

void wupload(void)
{
	char fname[13];
    int  fd;
    unsigned char buf[128];
    unsigned char hdshk;
    int hmode;
    int result;
    int nrec;                           /* returned record */

    ctlc_hit = FALSE;
	getfname(fname);
    if (ctlc_hit == TRUE)
    {
        ctlc_hit = FALSE;
        return;
    }
	if (isblank(fname))
		return;
	trim(fname);

	if ((fd = open(fname,(O_RDONLY|O_BINARY))) == -1)
    {
		urgentmsg("ERROR", "Unable to open the file");
        return;
    }
/*
** the file has been opened successfully...now switch the
** comport mode to 8 bits for xmodem protocol
*/
    if (comm_setup(port,pbaud,NPARITY,BIT8,pstop) == ERR)
    {
		urgentmsg("ERROR", "Unable to switch line mode");
        return;
    }
    lc_xoff(port,TRUE);                    /* turn on auto xon-xoff */

/*
** begin file transmission to receiver -
** in receiver's specified mode
*/
    while (TRUE)
    {
        memset(buf, 0x1a, 128);     /* clear buff, short rec */
        if (read(fd, buf, sizeof(buf)) < 1)  /* EOF or Error */
            nrec = -1;              /* defines End of File */
        else
            nrec = 0;

        result = lwxtrec(port, buf, &nrec);

        switch(result)           /* what action to take ? */
        {
            case SUCCESS:
                if (nrec == -1)  /* EOF confirmed */
					urgentmsg("SUCCESS","End of Transmission");
				else
				{
					sprintf(strbuf, "Sent record: %d",rec);
					atsay(1,1,strbuf);
					eraeol();
				}
                break;
            case CAN:
				urgentmsg("ERROR", "Cancel Received");
                break;
            case RETRIES:
				sprintf(strbuf, "Too Many tries...Rec %d",rec);
				urgentmsg("ERROR", strbuf);
                break;
            case RESEND:                /* must reset file position */
                lseek(fd, (long)(nrec * sizeof(buf)), SEEK_SET);
                break;
            default:
				urgentmsg("ERROR", "Fatal transmission error");
                break;
        }
        if (result == RESEND)
            continue;
        if (result == SUCCESS)
            if (nrec == -1)             /* was EOF signalled */
                break;
            else
                continue;
        break;                          /* some other error */
    }                                          /* while TRUE */
    lc_xoff(port, FALSE);                         /* turn off auto xon/xoff */
    comm_setup(port,pbaud,pparity,pbits,pstop);
    close(fd);                                 /* close down the file */
}

void wdnload(void)
{
	char fname[13];
    int  fd;
    unsigned char buf[128];
    unsigned char hdshk;
    int hmode;
    int result;

    ctlc_hit = FALSE;

	getfname(fname);
	if (ctlc_hit == TRUE)
    {
        ctlc_hit = FALSE;
        return;
    }
	if (isblank(fname))
		return;
	trim(fname);

	if ((fd = open(fname,
         (O_WRONLY|O_CREAT|O_TRUNC|O_BINARY),(S_IREAD|S_IWRITE))) == -1)
    {
		urgentmsg("ERROR", "Unable to open the file");
        return;
    }
/*
** the file has been opened successfully...now switch the
** comport mode to 8 bits for xmodem protocol
*/
    if (comm_setup(port,pbaud,NPARITY,BIT8,pstop) == ERR)
    {
		urgentmsg("ERROR", "Unable to switch line mode");
        return;
    }
/*
** setup conditions for XMODEM receive
*/
    while (TRUE)
    {
        while ((result = lwxrrec(port, buf)) == SUCCESS)
        {
            write(fd, buf, sizeof(buf));  /* dump the block */
			sprintf(strbuf, "Received record: %d", rec-1);
			atsay(1,1,strbuf);
			eraeol();
        }
        switch(result)           /* unusual conditions */
        {
            case DUPSEQ:
				atsay(1,1,"Duplicate record seq");
				eraeol();
                break;
            case CAN:
				urgentmsg("ERROR", "Cancelled by host");
                break;
            case EOT:
				urgentmsg("SUCCESS","Normal Termination");
                break;
            case TOUT:
				urgentmsg("ERROR", "SOH timeout");
                break;
            case RETRIES:
				sprintf(strbuf, "Too Many tries...Rec %d",(rec-1));
				urgentmsg("ERROR", strbuf);
                break;
            default:
				urgentmsg("ERROR", "Fatal transmission error");
                break;
        }
        if ((result != SUCCESS) && (result != DUPSEQ))
            break;
    }                                          /* while TRUE */

    comm_setup(port,pbaud,pparity,pbits,pstop);
    close(fd);                                 /* close down the file */
}
