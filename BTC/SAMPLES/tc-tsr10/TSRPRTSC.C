/*--------------------------------------------------------------------------*
 | TsrPrtSc.C                                                               |
 |   contains main, interrupt handling, and DOS interface routines          |
 |  TsrPrtSc.c is supplied as is with no warranty, expressed or implied.    |
 |  It is not copyrighted, either.                                          |
 *--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*
 | Author:                                                                  |
 | Sherif El-Kassas        .       :.                                       |
 | EB dept           \_____o__/ __________                                  |
 | Eindhoven U of Tec       .. /                                            |
 | The Netherlands            /             Email: elkassas@eb.ele.tue.nl   |
 *--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*
 | The author shall not be liable to the user for any direct, indirect      |
 | or consequential loss arising from the use of, or inability to use,      |
 | any program or file howsoever caused.  No warranty is given that the     |
 | programs will work under all circumstances.                              |
 *--------------------------------------------------------------------------*/


#include <dos.h>
#include "prtsc.h"

/*------------------------------------------------------------------------*
 |                           GLOBAL VARIABLES                             |
 *------------------------------------------------------------------------*/

void interrupt (* old_intr_0x09)();      /* pointer to old interrupt 0x9  */
void interrupt (* old_intr_0x28)();      /* pointer to old interrupt 0x28 */

unsigned _heaplen = 1;                   /* set minimum heap size         */

unsigned c_ss, c_sp;        /* c_ss, c_sp are used to save PRTSC'S stack  */
unsigned save_ss, save_sp;  /* save_ss and save_sp are used to save the   */
                            /* active stack                               */

byte active = FALSE;  /* program active flag (to prevent recursion)       */
byte wanted = FALSE;  /* wanted is set to TRUE if the Hot key was pressed */
                      /* but the program could not be activated           */

byte far * dos_active;                        /* pointer to DOS busy flag */
byte far * shift_byte = (byte far *) 0x00400017L; /* pointer to keyboard  */
                                                  /* status byte          */

/*------------------------------------------------------------------------*
 |                       Keyboard interrupt handler                       |
 *------------------------------------------------------------------------*/

void kbd_reset(void){    /* Reset Keyboard and programable interrupt      */
                         /* controller (PIC)                              */
     register char x;
     x = inp(0x61);
     outp(0x61, (x | 0x80));
     outp(0x61, x);
     cli();
     outp(0x20, 0x20);
     sti();
}/* kbd_reset */


void interrupt intr_0x09(){  /* interrupt 9 handler (whenever a key is    */
                             /* pressed control arrives hear)             */
     register char x;

     sti();
     x = inp(0x60);                   /* read keyboard data port          */
     if ( ( !active )        &&       /* if the program is not active and */
          ( x == SCAN_CODE ) &&       /* x == hot key scan code and     */
          ( ((*shift_byte) & ALT) != 0) /* the ALT key is pressed       */
     )
     {
       active = TRUE;        /* set active to prevent recursion           */
       kbd_reset();          /* reset keyboard controller and PIC         */
       if (!(*dos_active)){  /* if DOS is not active then do main task    */
         wanted = FALSE;
         do_main_task();
       }
       else wanted = TRUE;   /* else set wanted flag to be processed via  */
                             /* interrupt 0x28                            */
       active = FALSE;
     }
     else (* old_intr_0x09)(); /* call old interrupt 9 handler            */

} /* intr_0x09 */


/*------------------------------------------------------------------------*
 |                       Interrupt 0x28 handler                           |
 *------------------------------------------------------------------------*/
void interrupt intr_0x28(){

     (* old_intr_0x28)(); /* call old interrupt 0x28(to give other memory */
                          /* resident programs a chance to pop-up !)      */

     if (wanted){         /* if the wanted flag is set then do main task  */
       wanted = FALSE;
       active = TRUE;
       sti();
       do_main_task();
       cli();
       active = FALSE;
     }

} /* intr_0x28 */


/*------------------------------------------------------------------------*
 |                       do_main_task                                     |
 *------------------------------------------------------------------------*/
void do_main_task(void){

     cli();                        /* disable interrupts                  */
     save_ss = _SS; save_sp = _SP; /* save the current stack              */
     _SS = c_ss; _SP = c_sp;       /* restore PrtSc's stack               */
     sti();                        /* enable interrupts                   */
     main_task();                  /* call the print screen routines      */
     cli();                        /* disable interrupts                  */
     _SS = save_ss; _SP = save_sp; /* restore stack                       */
     sti();                        /* enable interrupts                   */

} /* do_main_task() */


/*------------------------------------------------------------------------*
 |                       get_dos_flag                                     |
 *------------------------------------------------------------------------*/

byte far * get_dos_flag(void){   /* get a pointer to DOS busy flag        */
     union  REGS  reg;
     struct SREGS s_reg;

     reg.x.ax = 0x3400;                /* function 0x34 get DOS busy flag */
     intdosx(&reg, &reg, &s_reg);      /* call DOS (interrupt 0x21)       */
     return( MK_FP(s_reg.es, reg.x.bx) ); /* return far pointer ES:BX     */

}/* get_dos_flag() */


/*------------------------------------------------------------------------*
 |                         program_size                                   |
 *------------------------------------------------------------------------*/

unsigned program_size(void){   /* return the size of the current memory   */
                           /* control block (in paragraphs)               */

    return(* ((unsigned far *) (MK_FP(_psp-1, 3))) );

}/* program_size() */


/*------------------------------------------------------------------------*
 |                         main function                                  |
 *------------------------------------------------------------------------*/
main(){

  initialize_video();             /* get video mode assume page 0         */

  c_ss = _SS; c_sp = _SP;         /* save PrtSC's stack                   */

  dos_active = get_dos_flag();    /* get a pointer to DOS busy flag       */

  old_intr_0x28 = getvect(0x28);  /* save old interrupt 0x28 vector       */
  setvect( 0x28, intr_0x28 ); /* set interrupt 0x28 vector to 'intr_0x28' */

  old_intr_0x09 = getvect(0x09);  /* save old interrupt 0x09 vector       */
  setvect( 0x09, intr_0x09 ); /* set interrupt 0x09 vector to 'intr_0x09' */

  keep(0, program_size());    /* terminate and stay resident              */

} /* main() */
