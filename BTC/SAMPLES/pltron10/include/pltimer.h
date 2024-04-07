
/*
  pltimer.h. Include file for the timer handlers.

  Copyright 1997 Liouros Thanasis.

  This file is part of PLATFORMTRON game library. See the file license.txt
  for more details. If that file is missing then please contact me.

*/

#ifndef pltimer_h
#define pltimer_h

#include <stdio.h>

#define MINHz         18.206497192
#define MAXHz         1001
#define PITHz         1193181
#define RTCMINRATE    1
#define RTCMAXRATE    11

extern volatile unsigned long pl_dtime[];


short pl_inittimer(float rate, void (*userroutine)() = NULL); 
short pl_timerdone();	
float pl_getActualHz();
long pl_getTimerCount();

short pl_RTCinittimer(char rate, void (*userroutine)() = NULL);
short pl_RTCtimerdone();

#endif
