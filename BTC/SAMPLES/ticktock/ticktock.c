 
/*
     Name: ticktock.c
     Author: Michael Tighe
     System: IBM PC/XT/AT/PS2, MS-DOS 3.30
   Language: Turbo C Version 2.00
Description: Demo of the IBM-PC's high resolution clock
 
    INFO FOR TICKTOCK:
 
    The TICKTOCK programs demonstrate how to obtain accurate timing
information from the IBM PC/XT/AT family of computers. The next few
paragraphs should give you a basic idea of how the time is stored in these
computer systems.
 
    In the PC family, an internal clock runs at 1.193180 Mhz. This clock
is divided by 65536 to give 18.206482 clock pulses per second (.0549255
seconds per clock pulse). Therefore, the clock 'ticks' every .0549255
seconds.
 
    Two addresses in low memory are used to keep track of the tick count.
They are both 1 word (two bytes) in length. The first is at address
0000:046C. It is incremented 18.2 times a second. When it overflows, it is
reset to 0 and another word at address 0000:046E is incremented.
 
    It should be noted that the word at address 0000:046E is also the
current hour, in 24 hour format. The address at 0000:046C when divided by
18.2, is the current time past the hour, in seconds.
 
*/
# include <stdio.h>
 
# define TIMER_LO 0X46C
# define TIMER_HI 0X46E
 
void geticktock();
 
float tick, tock;
 
main()
{
  printf("[TICKTOCK Version 87.365]\n\n");
  getticktock();
  printf("tick value is %6.0f, tock value is %6.0f\n",tick,tock);
  printf("Sleeping for 5 seconds (~91 ticks)...\n"); sleep(5);
  getticktock();
  printf("tick value is %6.0f, tock value is %6.0f\n",tick,tock);
  return;
}
 
getticktock()
{
  unsigned char t1,t2;
 
  t1 = 0; t2 = 0;
  t1 = peekb(0,TIMER_LO); t2 = peekb(0,TIMER_LO+1);
  tick = (float) t1 + (float) t2 * 256;
  t1 = peekb(0,TIMER_HI); t2 = peekb(0,TIMER_HI+1);
  tock = (float) t1 + (float) t2 * 256;
  return;
}
 
