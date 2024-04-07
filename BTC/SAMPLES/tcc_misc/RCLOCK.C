/*
 * RCLOCK.C : A resident clock program ported from Microsoft C 4.00 to
 *            Turbo C 1.00.  This program shows off some of the advanced
 *            features of Turbo C such as interrupt handling and such.
 *            It was alot (I mean ALOT) easier to write this in Turbo C
 *            than Microsoft...
 *
 *  Written by Dean D. McCrory
 *  For Turbo C 1.00
 *  May 12, 1987     (I got my RT clock fixed <grin>)
 *
 *  Compile with:
 *    tcc -N- rclock.c
 *
 *  The -N- switch turns stack checking off... Exemod is no longer needed
 *  for this version of the clock.
 */

#define LINT_ARGS

#include <dos.h>

/* function prototypes */
int main (void);
unsigned prgsize (void);
void exit (int);
void interrupt display_clock (void);
void sc_putsa (int, int, char *, int);

/* Suppress some library functions to conserve space */
_setargv () {}
_setenvp () {}

#define IRET      0xcf     /* for iret in int d0 just in case */
#define INTR      0xd0     /* interrupt for install checking */
#define TIMER     0x1c     /* timer interrupt number */
#define ON        1        /* value for active flag ... clock is on */
#define CONSTANT  2        /* value for active flag ... clock is constant */
#define OK        0        /* errorlevel value, installed ok */
#define NOTOK     1        /* errorlevel value, not installed ok */
#define SCREEN    (char far *) 0xb8000000L

/*
 * Structure definition for the clock configuration... This could be modified
 * by another program to modify the operation of this isr.
 */
struct s_clock_config
   {
   char iret;           /* filled with IRET instruction */
   long far * sign_str; /* pointer to long containing 'clok' */
   int active_fl;       /* is rclock active - default yes */
   int old_second;      /* last last second in timer tick isr */
   int row;             /* row of clock display */
   int col;             /* column of clock display */
   int attr;            /* attribute of clock display */
   };

/*
 * This is the default structure of the current clock configuration this
 * structure will be pointed at by interrupt D0 so this program can
 * determine if it is already loaded.

 * Note the insertion of an IRET instruction right before the rest of the
 * structure.  If someone should call interrupt D0 it will just do a return
 * from interrupt which is better than having the processor execute the
 * data inside this structure.  Also, the reason this info is put in a
 * structure and tied to an unused (hopefully) interrupt is so that another
 * program such as a clock control program could find this structure and
 * change the row, column, attribute, etc.
 */
struct s_clock_config clock_config =
   {
   IRET, (long far *) "clok", ON, 99, 0, 68, 7
   };

/*
 * Below is a far pointer initialized to the BIOS clock tick count
 */
long far * clock_count = (long far *) 0x0000046cL;

/*
 * Pointer to old timer interrupt service routine
 */
void interrupt (*old_timer) ();

/*  main ()
 *
 *  Check to see if our code is already installed and if not install it.
 *  A message saying that the code is already installed is not printed for
 *  memory space reasons.  As far as I know there is no way to only make
 *  part of a program resident and dump the install code (There is in
 *  assembler but not in higher level languages).  It does return a code in
 *  errorlevel indicating the success or failure of the install.
 */
int main ()
{
   struct s_clock_config far * config_ptr;

   /*
    * set a temporary pointer to be used to access structure more easily
    */
   config_ptr = (struct s_clock_config far *) getvect (INTR);

   /*
    * check to see if the code is already resident by checking interrupt
    */
   if (*config_ptr->sign_str != *clock_config.sign_str)
      {
      /* Not resident, install resident code, and set interrupt D0 so we can
       * tell that rclock is resident.
       */
      old_timer = getvect (TIMER);      /* save old vector */
      setvect (TIMER, display_clock);   /* set to ours */
      setvect (INTR, (void interrupt (*) ()) &clock_config);
      keep (OK, prgsize ());            /* terminate and stay resident */
      }

   return (NOTOK);                     /* already installed */
}

/* prgsize ()
 *
 * Calculates the program size by looking at __brklvl which is set to
 * the end of initialized and uninitialized data whithin the data segment
 * at program startup.  __brklvl is then changed as memory space is
 * malloc'd.  __brklvl is decremented as malloc'd areas are free'd.
 *
 *   ** This function should work in Tiny, Small, and Meduim models **
 */

unsigned prgsize ()
{
   extern unsigned __brklvl;     /* current top of heap == sbrk (0) */
   extern unsigned _psp;         /* lowest segment address occupied */

   return (_DS + (__brklvl + 15) / 16 - _psp);
}

/* exit ()
 *
 * Rewrite exit for memory conservation.  This exit () does not close files
 * or flush buffers, which is fine in this case because we have no open
 * files or buffers which need to be flushed.
 *
 */
void exit (status)
   int status;
{
   _exit (status);
}

/* display_clock ()
 *
 * Timer ISR, displays the clock on screen if the second has changed and
 * the clock active flag is 1 or the clock active flag is 2 (constant mode).
 */
void interrupt display_clock ()
{
   int hour;               /* temporary for hour */
   int minute;             /* temporary for minute */
   int second;             /* temporary for second */
   long remain;            /* temporary for remainder during divides */
   static int in_fl = 0;   /* are we already in this ISR */
   static char time_string[9] = "  :    m";

   /* Chain to the old timer routine */
   (*old_timer) ();
   
   /* Make sure we do not enter our code recursively */
   if (in_fl)
      return;

   in_fl = 1;

   /* The following formula was taken from Peter Nortons Programmer's Guide
      to the IBM-PC... there is a bug in it which momentarily (half second)
      assigns the minute to 60 just before changing hours */
   remain = *clock_count;
   hour = remain / 65543L;
   remain %= 65543L;
   minute = remain / 1092;
   remain %= 1092;
   second = remain * 100 / 1821;

   if (clock_config.active_fl && second != clock_config.old_second)
      {
      /* The clock is active and the second is different, rebuild the
         clock string */
      time_string[2] = time_string[2] == ':' ? ' ' : ':'; 
      time_string[6] = hour >= 12 ? 'p' : 'a';

      /* convert the hour into normal 12 hour system */
      if (hour == 0)
         hour = 12;
      else
         if (hour > 12)
            hour -= 12;

      *time_string = hour / 10 + '0';
      *(time_string+1) = hour % 10 + '0';
      *(time_string+3) = minute / 10 + '0';
      *(time_string+4) = minute % 10 + '0';
      }

   if ((clock_config.active_fl == ON && second != clock_config.old_second) ||
      clock_config.active_fl == CONSTANT)
      {
      /*
       * Save the old second and write the clock string to the correct
       * screen position.
       */
      clock_config.old_second = second;
      sc_putsa (clock_config.row, clock_config.col, time_string,
         clock_config.attr);
      }

   in_fl = 0;     /* we are now ready to exit our isr so indicate so */
}

/* sc_putsa ()
 *
 * Print a string with the given attribute and row and column directly to
 * color screen RAM.  Note that this will produce "snow" on CGA systems.
 */
void sc_putsa (row, col, string, attr)
   int row;
   int col;
   register char * string;
   int attr;
{
   register char far * ptr;         /* will be pointer into screen RAM */

   /* calculate pointer to screen RAM */
   ptr = SCREEN + row * 160 + col * 2;

   /* write each of the characters in string to the screen RAM */
   while (*string)
      {
      *ptr++ = *string++;           /* write the character */
      *ptr++ = attr;                /* write the attribute */
      }
}

