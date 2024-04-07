/* ---------- clock.c ------------ */

#include <dos.h>

void interrupt (*oldtimer)();
void interrupt newtimer();
extern void interrupt (*ZeroDivVector)();
#define sizeprogram 375
unsigned intsp;
unsigned intss;
unsigned myss;
unsigned stack;
static struct REGS rg;
struct date dat;
struct time tim;
unsigned vseg;
int running = 0;
char bf[20];
unsigned v;
char tmsk [] = " %2d-%02d-%02d %02d:%02d:%02d ";
int ticker = 0;
static struct SREGS seg;

main()
{
	segread(&seg);
	/* ------ save stack of resident program -------- */
	myss = _SS;
	/* --------- prepare timer interrupt vectors -------- */
	oldtimer = getvect(0x1c);
	setvect(0x1c, newtimer);
	stack = (sizeprogram - (seg.ds - seg.cs)) * 16 - 300;
	vseg = vmode() == 7 ? 0xb000 : 0xb800;
	/* ---- get the current date and time ------ */
	gettime(&tim);
	getdate(&dat);
	/* ---- restore zero divide interrupt vector --- */
	setvect(0, ZeroDivVector);
	/* ----- terminate and stay resident ------- */
	rg.x.ax = 0x3100;
	rg.x.dx = sizeprogram;
	intdos(&rg, &rg);
}
/* ----- timer isr ------- */
void interrupt newtimer()
{
	(*oldtimer)();
	if (running == 0)	{
		running = 1;
		disable();		/* set resident stack */
		intsp = _SP;
		intss = _SS;
		_SP = stack;
		_SS = myss;
		enable();
		if (ticker == 0)	{	/*count ticks (18.2/second)*/
			ticker = (((tim.ti_sec % 5) == 0) ? 19 : 18);
			tim.ti_sec++;
			if (tim.ti_sec == 60)	{
				tim.ti_sec = 0;
				tim.ti_min++;
				if (tim.ti_min == 60)	{
					tim.ti_min = 0;
					tim.ti_hour++;
					if (tim.ti_hour == 24)
						tim.ti_hour = 0;
				}
			}
			/* ----- build the date/time display ------- */
			sprintf(bf,tmsk,dat.da_day,dat.da_mon,dat.da_year
				% 100,tim.ti_hour, tim.ti_min, tim.ti_sec);
		}
		--ticker;
		/* ----- display the date and time -------- */
		for (v = 0; v < 19; v++)
			vpoke(vseg, (60 + v) * 2, 0x7000 + bf[v]);
		disable();		/* reset the interrupted stack */
		_SP = intsp;
		_SS = intss;
		enable();
		running = 0;
	}
}

