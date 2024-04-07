
   /********************************************************************/
   /*                                                                  */
   /*  Demonstration program for  SERIOUS  version 2.21                */
   /*  by Norman J. Goldstein                                          */
   /*                                                                  */
   /********************************************************************/

/* This program illustrates how to program with  SERIOUS  by implementing
   a simple terminal.  The file  serface.c  must be compiled with the
   command line version of Turbo C, as it contains inline code.       */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "serface.h"

/*- - - - - - - - Ports and IRQ's - - - - - - - - - - - - - - - - - - - -
  Here are some standard hardware configurations.

  DOS name    Port number       IRQ
    COM1         3f8             4
    COM2         2f8             3
    COM3         3e8             4
    COM4         2e8             3

  The DOS name is not important for using SERIOUS .  If these options do
  not work in a particular system, the hardware should be checked/adjusted
  for the desired settings.  The SERIOUS driver is designed to be the only
  hardware handler for any one IRQ -- the driver does not chain to any
  previously installed handlers.  This was a design decision for the
  sake of security.  I do beleive that, under SERIOUS, different serial
  ports can share the same IRQ .  Please contact me if this is a configuraion
  that you would like to have.
  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


/*- - - - - - -  DispInBuff - - - - - - - - - - - - - -
 * Transfers the input buffer to the screen.
 *- - - - - - - - - - - - - - - - - - - - - - - - - -
 */
void DispInBuff(void)
{
 int ci;

 while( (ci = S_RecvChar()) != -1 ) putchar( ci );
}/* DispInbuff */


/* These are auxilliary bytes used in the routine  comm . */
#define AltQ  16
#define Alt0 129

/*- - - - -  comm - - - - - - - - - - - - - - - - - - - - - - -
 * This is the dumb terminal
 * Input: vptr -- This is the routine to display the input buffer.
 *                In this file, it is called with  vptr = DispInBuff .
 *- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 */
void comm( void (*vptr)(void) )
{
 int ci;       /* To hold characters */
 int stay = 1; /* A flag to control the main loop */
 char *cp;

 putchar('\n');    /* Set up the screen */
 fflush( stdout );

 while(stay)  /* main loop ----------*/
 {
  if( kbhit() )
  {
   if( (ci = getch()) != 0 )
                      {
                       if( S_SendChar(ci) == ERR_UARTsleeps )
                       /* This message bears bad tidings. */
                       printf("\n!!! UART sleeps !!!\n");
                      }

   else /* ci == 0  indicates that a special character was entered. */
   switch( ci = getch() )
   {
    case AltQ:  stay = 0;          /* to leave  comm  */
                break;

    case Alt0:  S_SendChar('\0');  /* to send the null character */
                break;

    default:    /* Throw away other special characters. */
                break;
   }/*switch*/
  }/* kbhit */

  /* Display the input buffer. */
  (*vptr)();
 }/* while              --------------*/
} /* comm */


/*- - - - - - ChangeMode - - - - - - - - - -
 * Set the interrupt mask for the UART .
 * The mask bits are 0 -- Received data.
 *                   1 -- Transmitter ready.
 *                   2 -- Line error.
 *                   3 -- Modem status change.
 *- - - - - - - - - - - - - - - - - - - - -
 */
void ChangeMode(void)
{
 unsigned mask;

 printf("\nEnter interrupt mask MLTR: ");
 scanf("%x" , &mask );
 S_SetMode( (char) mask );
}/*changeMode*/


/*- - - - - -  param  - - - - - - - - - - - - - -
 * Allows the transmission parameters within the UART to be set.
 *- - - - - - - - - - - - - - - - - - - - - - - -
 */
void param(void)
{
 unsigned baud , u;
 int data , stop , parity;

 printf("\nEnter baud , data , stop , parity(0:none 1:odd 3:even):\n");
 scanf("%u %d %d %d", &baud, &data, &stop, &parity);

 if( (u=S_SetParms(baud , data , stop , parity)) == 0 )
 printf("\nbaud  data  stop  parity= %u %d %d %d\n", baud, data, stop, parity);
 else printf("Parameters not set. S_SetParms= %x .\n", u);
}/* param */


/*- - - - - - - menu - - - - - - - - -
 * The main menu.  The options are:
 * c -- Become a dumb terminal.
 * p -- Set the transmission paramaters.
 * m -- Set the interrupt mask.
 * o -- Exit this program without closing SERIOUS .
 *      WARNING: Be sure to be using the SERIOUS internal buffer, as this
 *               programs's user supplied buffer is deallocated at exit!
 * q -- Close SERIOUS and exit this program.
 *- - - - - - - - - - - - - - - - - -
 */
static void menu(void)
{
 char c;

 do
 {
  printf("\nEnter <p>arameters, <m>ode ,  <c>omm , <q>uit , <o>s: ");
  fflush( stdout );

  switch(  c = getche() )
  {
   case 'c':
             comm( DispInBuff );
             break;

   case 'm':
             ChangeMode();
             break;

   case 'o':
             printf("\nSerial port active!\n");
             exit(0);
             break;
   case 'p':
             param();
             break;

  }/* switch*/
 } while( c != 'q' );
} /*menu*/

/*- - - The user supplied buffer. - - - - - - -
  The order is at most  16 . */
#define InBuffOrd 10
#define InBuffLength (1L << (InBuffOrd))
static char InBuff[ InBuffLength ];

int main(int argc , char **argv)
{
 unsigned u , ord = InBuffOrd , port = 1,  /* Initially not 0 . */
                                irq;

 if( argc < 2 )
 {
  printf("\nUsage: sertest <device-name> [char]");
  printf("\n       If  char  is omitted, use the user supplied buffer.");
  printf("\n       char = '!' -- Assumes SERIOUS is already open.");
  printf("\n       char = anything else -- Use internal SERIOUS buffer.");
  exit(1);
 }

 /* Check for a 2nd parameter. */
 if( argc >= 3 )
 {
  if( *argv[2] == '!' ) port = 0;
  else ord = 0;
 }

 if(port) /* Driver is not yet opened. */
 {
  printf("\nEnter hex  port  and  irq: ");
  scanf("%x%x", &port , &irq );
 }


 /* port == 0 : Assumes driver is open, just sets the entry address.
    ord == 0 : Specify driver's default buffer to be used.
  */

   u = S_Open( argv[1] , port , irq , InBuff , ord );
   printf("\n S_Open= %X\n", u );
   if(u) exit(0);

 menu();

 printf("\n S_Close= %X\n", S_Close() );

 return 0;
}/*main*/
