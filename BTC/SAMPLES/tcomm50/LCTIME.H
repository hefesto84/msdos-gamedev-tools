/*
**	lctime.h - programmable timer interrupt handler for
**			   litecomm
**	Copyright (c) 1987 - Information Technology, Ltd.
*/

#ifndef LCTIME
#define LCTIME	1

extern volatile unsigned _secs_rem;

extern int lc_insclock(void);

extern void lc_setclock(unsigned);

extern void lc_clrclock(void);

#endif
