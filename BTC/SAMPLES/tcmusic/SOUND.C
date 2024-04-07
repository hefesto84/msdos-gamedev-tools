/*
**  These routines demonstrate the use of TURBO C inline assembly.
**  The routines are almost verbatim copies of similarly named
**  assembler routines found in:
**
**      'THE BLUEBOOK of ASSEMBLY ROUTINES FOR THE PC & XT'
**         by Christopher L. Morgan
**
**  NOTE: I am not a micro assembler programmer!
**        Perhaps I'll get around to it one of
**        these days.
**
*/

extern  void    toneinit( void );
extern  void    toneset( unsigned period );
extern  void    toneon( void );

#include        <music.h>
#include        <stdio.h>
#pragma         inline

/*
**  TONEINIT: initialize speaker timer
**
**  This routine initializes the portion of the 8253 timer chip used by
**  the speaker system. In particular, it sets up channel 2 of this
**  timer as a square wave generator. This routine does not set the
**  frequency nor turn on the tone. Use TONESET to select the frequency,
**  TONEON to turn the tone on, and TONEOFF to turn it off.
**
*/

static  void    toneinit( void )
{
    asm     mov     al,10110110b;       /*  setup 8253 control word */
    asm     out     43h,al;             /*  send to timer-mode port */
}

/*
**  TONESET: set the tone on the speaker
**
**  This routine selects the frequency of the square wave tone to the
**  speaker. The input to this routine is an integer, n, which determines
**  the frequency f according to the formula:
**
**      f = F/n
**
**  where f is 1,193,182, the frequency of a clock signal which feeds the
**  timer. The value n is the number of cycles of the clock signal per
**  cycle of the resulting square wave. This routine does not actually
**  turn on the tone. Use TONEON to turn the tone on and TONEOFF to turn
**  it off. This routine assumes that the speaker timer has already been
**  properly initialized. This happens after during normal boot-up of the
**  computer, or you can use TONEINIT to initialize this timer.
**
**  COPIER'S NOTE: 1,193,182 won't work on machines not running
**                 at 4.77 Mhz !
**
*/

static  void    toneset( unsigned _CX )
{
    asm     mov     al,cl;              /*  move lower byte */
    asm     out     42h,al;             /*  out to frequency control port    */
    asm     mov     al,ch;              /*  move lower byte */
    asm     out     42h,al;             /*  out to frequency control port    */

}

/*
**  TONEON: turn on tone
**
**  Turns on the timer and speaker to produce a tone. The frequency
**  of the tone must have already been selected on the timer.
**
*/


static  void    toneon( void )
{
    asm     in      al,61h;             /*  get contents of speaker port    */
    asm     or      al,3;               /*  turn speaker and timer on       */
    asm     out     61h,al;             /*  send new values to speaker port */
}

/*
**  TONEOFF: turn off tone
**
**  This routine turns off the timer and speaker.
**
*/


void    toneoff( void )
{
    asm     in      al,61h;             /*  get contents of speaker port    */
    asm     and     al,11111100b;       /*  turn speaker and timer          */
    asm     out     61h,al;             /*  off                             */
}


/*
**  DELAY: delay for specified number of milliseconds
**
*/



void    delay( unsigned milliseconds )
{
    _CX = milliseconds;                 /*  get ready for 'loop'        */
delay1:
    asm     push    cx;
    asm     mov     cx,260;             /*  timing constant             */
delay2:
    asm     loop    delay2;             /*  small   loop                */
    asm     pop     cx;
    asm     loop    delay1;             /*  loop to count milliseconds  */
}

/*
**  FREQ: conversion from frequency to period
**
**  This routine converts from frequency to the number required by
**  TONESET to set the frequency. The routine performs the following
**  formula:
**      n = F / f
**
**  where f is the frequency input to this routine, n is the number
**  output by this routine, and F is 1,193,182. In other words this
**  routine divides the specified frequency f into the 1,193,182 hertz
**  clock frequency that drives the timer. Use this routine just
**  before TONESET.
**
**  COPIER'S NOTE: 1,193,182 won't work on machines not running
**                 at 4.77 Mhz !
**
*/

unsigned    freq( unsigned  frequency )
{
    asm     mov     dx,12h;             /*  upper part of numerator     */
    asm     mov     ax,34DEh;           /*  lower part of numerator     */
    asm     mov     cx,frequency;
    asm     div     cx;                 /*  divide by frequency         */
    asm     mov     cx,ax;              /*  move converted frequency    */
    return( _CX );
}

/*
**  TONE: make a tone
**
**  This routine makes a tone of a given frequency and given length.
**
*/

void    tone( unsigned frequency, unsigned length )
{
    unsigned    period;
    period = freq( frequency );
    toneset( period );
    toneon();
    delay( length );
    toneoff();
}

/*
**  PITCH: Convert from pitch to frequency
**
**  This routine converts from pitch number to the value required by
**  TONESET to set the frequency. The pitch numbers refer to an ext-
**  ended chromatic scale. The notes of this scale are numbered from
**  0 to 95 with 12 notes per octave. 0 corresponds to a C at 16.35
**  Hertz.
**
*/

unsigned    pitch(unsigned pitch_no )
{
    static      int     notes[] =
        {
            4186,       /*  C                   */
            4435,       /*  C sharp/ D flat     */
            4699,       /*  D                   */
            4978,       /*  D sharp/ E flat     */
            5274,       /*  E                   */
            5588,       /*  F                   */
            5920,       /*  F sharp/ G flat     */
            6272,       /*  G                   */
            6645,       /*  G sharp/ A flat     */
            7040,       /*  A                   */
            7459,       /*  A sharp/ B flat     */
            7902        /*  B                   */
        };

    asm     mov     ax,pitch_no;
    asm     mov     ah,0;
    asm     mov     cl,12;          /*  divisor of 12 notes per octave      */
    asm     div     cl;
    asm     mov     dl,al;          /*  move quotient octave                */
    asm     mov     al,ah;          /*  move remainder pitch                */
    asm     cbw;                    /*  convert to word                     */
    asm     mov     bx,ax;          /*  setup index into note table         */
    _CX = notes[ _BX ];             /*  get a note                          */

    asm     push    dx;             /*  save dx                             */
    _CX = freq( _CX );              /*  convert to frequency                */
    asm     pop     dx;             /*  restore dx                          */

    asm     xchg    cx,dx;          /*  octave in cl, period in dx          */
    asm     neg     cl;             /*  8 - octave = shift count            */
    asm     add     cl,8;
    asm     sal     dx,cl;

    return( _DX );

}

/*
**  PLAY: Play music
**
**  This routine plays music. It reads a 'play list' which contains
**  instructions to make the tune. This list consists of a series of
**  music instructions. In this particular implementation, there are
**  four instructions: Tempo, Note, Rest, and End. The syntax is given
**  in MUSIC.DOC.
**
*/


void    play( char *play_list )
{
    unsigned            whole;
    unsigned            acount;
    unsigned            rcount;

    asm     mov     whole,2000;             /*  2 seconds for a whole note  */
    asm     cld;                            /*  move forward                */
    asm     mov     si,word ptr play_list;  /*  load play_list adr          */

/*
**  Main loop
*/

play1:

    asm     lodsb;                  /*  get char from command line          */

    asm     cmp     al,'X';         /*  END command ?                       */
    asm     jne     chktempo;       /*  no. check for tempo                 */
    asm     jmp     playexit;       /*  yes, get out.                       */

    /*
    **  check for Tempo command
    */

chktempo:
    asm     cmp     al,'T';         /*  TEMPO command ?                     */
    asm     jne     chkpitch;       /*  no, check for pitch                 */

/*
**  process tempo command
*/

    asm     lodsb;                  /*  get tempo                           */
    asm     mov     cl,al;          /*  setup as divisor                    */
    asm     mov     ch,0;           /*  clear hiorder of divisor            */
    asm     mov     ax,60000;       /*  number of milliseconds/minute       */
    asm     mov     dx,0;           /*  clear hiorder part of dividend      */
    asm     div     cx;             /*  divide into time                    */
    asm     mov     whole,ax;       /*  number of milliseconds/whole note   */
    asm     jmp     play1;          /*  go process more commands            */

/*
**  check for Pitch
*/

chkpitch:

    asm     cmp     al,'N';         /*  Pitch command ?                     */
    asm     jne     chkrest;        /*  no. check for rest                  */

/*
**  process pitch command
*/

    asm     lodsb;                  /*  get the pitch                       */

    _CX = pitch( _AX );             /*  convert pitch number                */

    toneset( _CX );                 /*  set frequency                       */
    toneon();                       /*  turn tone on                        */

    asm     mov     cx,whole;       /*  number of milliseconds/whole note   */
    asm     lodsb;                  /*  get duration                        */
    asm     mov     ah,al;          /*  setup duration as multiplier        */
    asm     mov     al,0;           /*  clear                               */
    asm     sal     cx,1;           /*  scale factor 1                      */
    asm     mul     cx;
    asm     mov     cx,dx;          /*  total count  for the note           */
    asm     lodsb;                  /*  get style                           */
    asm     mov     ah,al;          /*  setup style as multiplier           */
    asm     mov     al,0;           /*  clear rest of multiplier            */
    asm     mul     cx;             /*  multiply by style                   */
    asm     mov     acount,dx;      /*  store count for note                */
    asm     sub     cx,dx;          /*  calculate count for rest            */
    asm     mov     rcount,cx;      /*  save count for rest                 */

    delay( acount );                /*  audible count of note               */
    toneoff();                      /*  turn tone off                       */
    delay( rcount );                /*  inaudible part of note              */

    asm     jmp     play1;          /*  process more commands               */

chkrest:

    asm     cmp     al,'R';         /*  REST command ?                      */
    asm     jne     playexit;       /*  nope... get out                     */

/*
**  process rest command
*/

    asm     mov     cx,whole;       /*  number of milliseconds/whole note   */
    asm     lodsb;                  /*  get duration                        */
    asm     mov     ah,al;          /*  setup duration as multiplier        */
    asm     mov     al,0;           /*  clear rest of multiplier            */
    asm     sal     cx,1;           /*  scale factor of 1                   */
    asm     mul     cx;

    delay( _DX );

    asm     jmp     play1;          /*  back for more...                    */

playexit:

    return;
}
