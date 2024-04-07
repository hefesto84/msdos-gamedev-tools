#define TASYNC
#include <dos.h>
#include"tasync.h"
/*
   a set  of Turbo-C functions to support
   interrupt driven serial i/o.
   input and output are buffered line and modem status drivers are
   also interrupt driven. tested upto 9600 baud all code is written
   in "C" thanks to Borland Low level hooks that are available in
   Turbo-C.
   thanks to Glenn F. Marshall for his Pascal code and to
   Curt Klinsing for his assembler routines
*/

#define false   0
#define true    !false
#define COM0 1          /* either com0 or com1 */

#ifdef COM0
#define comport  0      /*  Com port # */
#define base     0x03f8 /* base for serial board  */
#define comint   0xc    /*  Int Vector used by port */
#define enblirq  0xef   /* enable communications  */
#define maskirq  0x10   /* bit to disable comm interrupt */
int pused = 0;
#endif

#ifdef COM1
#define comport  1      /*  Com Port */
#define base     0x02f8 /* base for serial board */
#define comint   0xb    /*  Int Vector used by port */
#define enblirq  0xf7   /* enable communications */
#define maskirq  0x8    /* bit to disable comm interrupt */
int pused = 1;
#endif

/* 8250 registers */
#define mdmbd0   base    /* lsb baud rate register */
#define dataport base    /* transmit/receive data port */
#define mdmbd1   base+1  /* msb baud rate register */
#define ier      base+1  /*  interrup enable register */
#define irr      base+2  /* reason for interrupt */
#define lcr      base+3  /*  line control register */
#define mcr      base+4  /* modem control register */
#define mdmsta   base+5  /* line status register */
#define mdmmsr   base+6  /* modem status register */

/* 8250 values */
#define mdmcd   0x80     /* mask for carrier dectect */
#define mdmtbe  0x20     /* 8250 tbe flag */
#define dlab    0x80     /* enable divisor latch */

/* 8250 interrupt enable values */
#define enbldrdy 1       /*  enable 'data-ready' interrupt bit */
#define enbltrdy 2       /*  enable 'xmit-empty' interrupt bit */
#define enbllrdy 4       /*  enable 'line-change' interrupt bit */
#define enblmrdy 8       /*  enable 'modem-change' interrupt bit */

/* 8250 interrupt causes */
#define intms    0       /* int caused by modem status */
#define inttx    2       /* int caused by td */
#define intrd    4       /* int caused by dr */
#define intls    6       /* int caused by line status */

/* 8259 ports and values */
#define intctlr 0x21     /* ocw 1 for 8259 controller */
#define rs8259  0x20     /* ocw 3 for 8259 */
#define rstint  0x20     /* specific eoi for comm interrupt */

/*        miscellaneous equates */
#define xoff    0x13
#define xon     0x11
#define bufsiz  4096     /* max number of chars */

char in_c_buf[bufsiz];   /* allow 512 maximum buffered characters */
char ou_c_buf[bufsiz];
volatile char xon_sent,linstat,modstat;

#define ou_c_top &ou_c_buf[bufsiz-1]
#define in_c_top &in_c_buf[bufsiz-1]

volatile char *in_c_in;   /* in_c_buf pointer to last char. placed in buffer */
volatile char *in_c_cur;  /* in_c_buf pointer to next char. to be retrieved */
volatile int  in_c_ct;    /* count of characters used in buffer */
volatile char *ou_c_in;   /* ou_c_buf pointer to last char. placed in buffer */
volatile char *ou_c_cur;  /* ou_c_buf pointer to next char. to be transmitted */
volatile int  ou_c_ct;    /* count of characters used in buffer */

int carrier(void)
{
   return (inport(mdmmsr) & 0x80);
}

void setport(int prot,int baud)
{
    disable();
    outport(lcr,dlab);           /* enable buad sender */
    outport(mdmbd0,baud & 0xFF); /* send lsb */
    outport(mdmbd1,baud / 256);  /* send msb */
    outport(lcr,prot);           /* set protocol */
    enable();
}

/*  int getport();
15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
   T  T  B  F  P   O  D  D  R  D  C  D  T  D  D
   S  H  I  E  E   R  R  C  I  S  T  D  E  D  C
   R  R                  D     R  S  C  R  S  T
   E  E                              D  I  R  S
Data Ready = DR, Overrun Error = OR, Parity Error = PE, Framing Error = FE,
Break Interrupt = BI, Transmitter Holding Register = THRE,
Transmitter Empty = TSRE, Delta Clear to Send = DCTS, Clear to Send = CTS,
Delta Data Set Ready = DDSR, Data Set Ready = DSR, Ring Indicator = RI,
Trailing Edge Ring Indicator = TERI, Delta Carrier Detect = DCD,
Carrier Detect = CD */
int getport(void)
{
    return(256 * linstat + modstat);
}

#define inp_cnt() in_c_ct

/* flush_port()    flush the buffers */
void flush_port(void)
{
    disable();
    in_c_in=in_c_cur=&in_c_buf[0];
    ou_c_in=ou_c_cur=&ou_c_buf[0];
    ou_c_ct=xon_sent=in_c_ct=0;
    enable();
}

/* uninit()          removes the interrupt structure */

void interrupt (*oldhold)();

void uninit(void)     /* remove initialization, */
{
    disable();
    setvect(comint,oldhold); /* restore old vector */
    outportb(intctlr,inport(intctlr) | maskirq); /*  disable irq on 8259 */
    outport(ier,0); /* clear all interrupt enables */
    outport(lcr,0); /* clear the protocol */
    outport(mcr,0); /* disable out2 on 8250 */
    enable();
}

volatile char rd,tx;  /* last recieved and transmitted character */

void scrstat(void) /* will cause loss of characters if called too often */
{
    char s[10];
    int i,n,j;
    n=(modstat & 0xF0) | ((linstat>>1) & 0x0F) ;
    for(i=1,j=2;i<256;i*=2,j++)
        if(n & i)
            s[j]=7;
        else
            s[j]=250;
    s[0]=rd;
    s[1]=tx;
    for(i=0,j=0;i<10;i++,j+=2)
        pokeb(0xb800,140+j,s[i]);
}

void comout(char c) /* que character in buffer */
{
    if((inportb(mdmsta) & mdmtbe) && !ou_c_ct)
        outportb(dataport,tx=c); /* if status is clear then just send */
    else {                    /* load it in the buffer */
        disable();
        if(ou_c_ct<bufsiz) {  /* if buffer is full ignore character */
            *ou_c_in=c;
            ou_c_ct++;
            ou_c_in = ou_c_in==ou_c_top ? ou_c_buf : (char *)ou_c_in+1;
        }
        enable();

    }
}


/* char inp_char()        return a character from the input */
/*                        buffer. assumes you have called */
/* inp_cnt() to see if theres any characters to get. */

char inp_char(void)                /* get one char from buffer, */
{
    char cin;
    disable();
    cin=*in_c_cur;
    if(inp_cnt()) {
        in_c_ct--;
        in_c_cur = in_c_cur==in_c_top ? in_c_buf : (char *)in_c_cur+1;
    }
    enable();
    if(xon_sent && (in_c_ct<128)) {
        comout(xon);
        xon_sent=false;
    }
    return cin;
}

/* receive interrupt handler (changed to place characters in in_c_buf */
void interrupt inthdlr(void)
{
    disable();
    switch(inportb(irr)) {
    case intrd: /* recieve data */
        if(in_c_ct<bufsiz) { /* if buffer is full ignore character */
            *in_c_in=inportb(dataport);
            rd=*in_c_in;
            in_c_ct++;
            in_c_in = in_c_in==in_c_top ? in_c_buf : (char *)in_c_in+1;
        }
        break;
    case intms:
        modstat=inportb(mdmmsr);
        break; /* modem status */
    case intls:
        linstat=inportb(mdmsta);
        break; /* line status */
    case inttx: /* transmitter empty */
        if(ou_c_ct>0) { /* if there is a character */
            outportb(dataport,*ou_c_cur);
            tx=*ou_c_cur;
            ou_c_ct--;         /* decrement in_c_buf count */
            ou_c_cur = ou_c_cur==ou_c_top ? ou_c_buf : (char *)ou_c_cur+1;
        }
    } /* switch */
    outportb(rs8259,rstint);
    /*   scrstat(); */
    if(in_c_ct>(bufsiz*3/4)) {
/*        pokeb(0xB800,0,7); */ /* if xon xoff screen report is needed */
        comout(xoff);
        xon_sent=true;
    }
    enable();
}

/*  --------- init ----------------------------------- */
/*  program initialization: */
/*    --  set up vector for rs232 interrupt */
/*    --  enbl irq */
/*    --  enbl rs232 interrupt on dr,tx,ms,ls */

/*  --------------------------------------------------- */

void init_com(void)       /* initialize the comm port, */
{
    disable();
    flush_port();
    oldhold=getvect(comint);
    setvect(comint,inthdlr);
    outportb(intctlr,inportb(intctlr) & enblirq);
    outport(lcr,inportb(lcr) & 0x7f); /* reset dlab for ier access */
    outportb(ier,enbldrdy+enbllrdy+enblmrdy+enbltrdy);
    outportb(mcr,0xf); /*  modem control register enable out2 out1 dtr rts */
    linstat=inportb(mdmsta);
    modstat=inportb(mdmmsr);
    enable();
}

void comok(void)
{
    outportb(mcr,0xf); /* enable modem */
}

