/*
** ppcevent.h -
**	Event Timers -
**	PPC Communication Services -
**	Copyright (c) 1988, Ralph B. Mace
**		-- All Rights Reserved --
*/

#ifndef _PPC_EV
#define _PPC_EV	1

#include <time.h>

#define online(x) (lc_mstat(x) & DCD)

#ifdef M_I86
#define setmem(d,c,v) memset(d,v,c)
#endif

/*
**	function prototypes
*/
int check_for_call(unsigned);
int get_modem_reply(unsigned);
void disconnect(unsigned);
int reset_modem(unsigned);
long new_event(unsigned);
int check_event(long);

#endif
