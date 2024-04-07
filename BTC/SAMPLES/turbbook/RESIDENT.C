/* -------- resident.c --------- */

#include <dos.h>
#include <stdio.h>

static union REGS rg;
static struct SREGS seg;
static unsigned mcbseg;
static unsigned dosseg;
static unsigned dosbusy;
static unsigned enddos;
char far *intdta;
static unsigned intsp;
static unsigned intss;
static char far *mydta;
static unsigned myss;
static unsigned stack;
static unsigned ctrl_break;
static unsigned mypsp;
static unsigned intpsp;
static unsigned pids[2];
static int pidctr = 0;
static int pp;
static void interrupt (*oldtimer)();
static void interrupt (*old28)();
static void interrupt (*oldkb)();
static void interrupt (*olddisk)();
static void interrupt (*oldcrit)();
extern void interrupt (*ZeroDivVector)();
void interrupt newtimer();
void interrupt new28();
void interrupt newkb();
void interrupt newdisk();
void interrupt newcrit();
extern unsigned sizeprogram;
extern unsigned scancode;
extern unsigned keymask;
static int resoff = 0;
static int running = 0;
static int popflg = 0;
static int diskflag = 0;
static int kbval;
static int cflag;

void dores(), pidaddr();

/* -------- establish & declare residency --------- */
void resinit()
{
	segread(&seg);
	myss = seg.ss;
	/* ------ get address of DOS busy flag ---- */
	rg.h.ah = 0x34;
	intdos(&rg, &rg);
	dosseg = _ES;
	dosbusy = rg.x.bx;
	/* ----- get address of resident program's dta ----- */
	mydta = getdta();
	/* -------- get addresses of PID in DOS ------- */
	pidaddr();
	/* ----- get original interrupt vectors ----- */
	oldtimer = getvect(0x1c);
	old28 = getvect(0x28);
	oldkb = getvect(9);
	olddisk = getvect(0x13);
	/* ----- attach vectors to resident program ----- */
	setvect(0x1c, newtimer);
	setvect(9, newkb);
	setvect(0x28, new28);
	setvect(0x13, newdisk);
	/* ------ compute stack pointer ------- */
	stack = (sizeprogram - (seg.ds - seg.cs)) * 16 - 300;
	/* ---- restore zero divide interrupt vector --- */
	setvect(0, ZeroDivVector);
	/* ----- terminate and stay resident ------- */
	rg.x.ax = 0x3100;
	rg.x.dx = sizeprogram;
	intdos(&rg, &rg);
}

/* ------ BIOS disk functions ISR ------- */
void interrupt newdisk(bp,di,si,ds,es,dx,cx,bx,ax,ip,cs,flgs)
{
	diskflag++;
	(*olddisk)();
	ax = _AX;		/* for the ax return */
	newcrit();		/* to get current flags register */
	flgs = cflag;
	--diskflag;
}

/* -------- critical error ISR ---------- */
void interrupt newcrit(bp,di,si,ds,es,dx,cx,bx,ax,ip,cs,flgs)
{
	ax = 0;
	cflag = flgs;	/* for newdisk */
}

/* ----- keyboard ISR ------ */
void interrupt newkb()
{
	if (inportb(0x60) == scancode)	{
		kbval = peekb(0, 0x417);
		if (!resoff && ((kbval & keymask) ^ keymask) == 0)	{
			/* --- reset the keyboard ---- */
			kbval = inportb(0x61);
			outportb(0x61, kbval | 0x80);
			outportb(0x61, kbval);
			disable();
			outportb(0x20, 0x20);
			enable();
			/* ---- set hotkey indicator ---- */
			if (!running)
				popflg = 1;
			return;
		}
	}
	(*oldkb)();
}

/* ----- timer ISR ------- */
void interrupt newtimer()
{
	(*oldtimer)();
	if (popflg && peekb(dosseg, dosbusy) == 0)
		if (diskflag == 0)	{
			outportb(0x20, 0x20);
			popflg = 0;
			dores();
		}
}

/* ----- DOSOK ISR -------- */
void interrupt new28()
{
	(*old28)();
	if (popflg && peekb(dosseg, dosbusy) != 0)	{
		popflg = 0;
		dores();
	}
}

/* ------ switch psp context from interrupted to TSR ----- */
resident_psp()
{
	/* ------ save interrupted program's psp ----- */
	intpsp = peek(dosseg, *pids);
	/* ----- set resident program's psp ----- */
	for (pp = 0; pp < pidctr; pp++)
		poke(dosseg, pids [pp], mypsp);
}

/* ---- switch psp context from TSR to interrupted ---- */
interrupted_psp()
{
	/* ----- reset interrupted program's psp ----- */
	for (pp = 0; pp < pidctr; pp++)
		poke(dosseg, pids [pp], intpsp);
}

/* ------ execute the resident program ------- */
void dores()
{
	running = 1;
	disable();
	intsp = _SP;
	intss = _SS;
	_SP = stack;
	_SS = myss;
	enable();
	oldcrit = getvect(0x24);/* redirect critical error     */
	setvect(0x24, newcrit);
	rg.x.ax = 0x3300;		/* get ctrl break setting      */
	intdos(&rg, &rg);
	ctrl_break = rg.h.dl;
	rg.x.ax = 0x3301;		/* turn off ctrl break logic   */
	rg.h.dl = 0;
	intdos(&rg, &rg);
	intdta = getdta();		/* get interrupted dta         */
	setdta(mydta);			/* set resident dta            */
	resident_psp();			/* swap psps                   */
	popup();				/* execute resident program    */
	interrupted_psp();		/* reset interrupted psp       */
	setdta(intdta);			/* reset interrupted dta       */
	setvect(0x24, oldcrit);	/* reset critical error        */
	rg.x.ax = 0x3301;		/* reset ctrl break            */
	rg.h.dl = ctrl_break;
	intdos(&rg, &rg);
	disable();				/* reset interrupted stack     */
	_SP = intsp;
	_SS = intss;
	enable();
	running = 0;
}
/*page*/
static int avec = 0;

/* ------- test to see if the program is already resident
      if not, attach to an available interrupt ---------- */
unsigned resident(signature, ifunc)
char *signature;
void interrupt (*ifunc)();
{
	char *sg;
	unsigned df;
	int vec;

	segread(&seg);
	df = seg.ds-seg.cs;
	for (vec = 0x60; vec < 0x68; vec++)	{
		if (getvect(vec) == NULL)	{
			if (!avec)
				avec = vec;
			continue;
		}
		for (sg = signature; *sg; sg++)
			if (*sg!=peekb(peek(0,2+vec*4)+df,(unsigned)sg))
				break;
		if (!*sg)
			return vec;
	}
	if (avec)
		setvect(avec, ifunc);
	return 0;
}
/*page*/
/* -------- find address of PID ---------- */
static void pidaddr()
{
	unsigned adr = 0;

	/* ------- get the current pid --------- */
	rg.h.ah = 0x51;
	intdos(&rg, &rg);
	mypsp = rg.x.bx;
	/* ----- find the end of the DOS segment ------- */
	rg.h.ah = 0x52;
	intdos(&rg, &rg);
	enddos = _ES;
	enddos = peek(enddos, rg.x.bx-2);
	/* ---- search for matches on the pid in dos ---- */
	while (pidctr < 2 &&
			(unsigned)((dosseg<<4) + adr) < (enddos<<4))	{
		if (peek(dosseg, adr) == mypsp)	{
			rg.h.ah = 0x50;
			rg.x.bx = mypsp + 1;
			intdos(&rg, &rg);
			if (peek(dosseg, adr) == mypsp+1)
				pids[pidctr++] = adr;
			/* ---- reset the original pid ------ */
			rg.h.ah = 0x50;
			rg.x.bx = mypsp;
			intdos(&rg, &rg);
		}
		adr++;
	}
}
/*page*/
#undef peekb	/* to overcome TC 1.0 bug */
/* ------- terminate function ----------- */
static resterm()
{
	closefiles();	/*  close TSR files */
	/* ----- restore the interrupt vectors ----- */
	setvect(0x1c, oldtimer);
	setvect(9, oldkb);
	setvect(0x28, old28);
	setvect(0x13, olddisk);
	setvect(avec, (void interrupt (*)()) 0);
	/* ---- get the seg addr of 1st DOS MCB ---- */
	rg.h.ah = 0x52;
	intdos(&rg, &rg);
	mcbseg = _ES;
	mcbseg = peek(mcbseg, rg.x.bx-2);
	/* ---- walk thru mcb chain & release memory ----- */
	segread(&seg);
	while (peekb(mcbseg, 0) == 0x4d)	{
		if (peek(mcbseg, 1) == mypsp)	{
			rg.h.ah = 0x49;
			seg.es = mcbseg+1;
			intdosx(&rg, &rg, &seg);
		}
		mcbseg += peek(mcbseg, 3) + 1;
	}
}
/* --------- terminate the resident program --------- */
terminate()
{
	if (getvect(0x13) == (void interrupt (*)()) newdisk)
		if (getvect(9) == newkb)
			if (getvect(0x28) == new28)
				if (getvect(0x1c) == newtimer)	{
					resterm();
					return;
				}
	resoff = 1;	/* another TSR is above us, merely suspend */
}

/* ------------- restart the resident program --------- */
restart()
{
	resoff = 0;
}

/* ------- put the program on hold -------- */
wait()
{
	resoff = 1;
}

