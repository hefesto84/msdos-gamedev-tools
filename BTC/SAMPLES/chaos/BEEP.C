#define LINT_ARGS

#include <dos.h>
#include <conio.h>

#define  ONTONE  outp( 0x61, inp( 0x61 ) | 3 )
#define  OFFTONE outp( 0x61, inp( 0x61 ) & 0xFC )

static void setfreq( count )
unsigned int count;
{

    outp( 0x43, 0xB6 );
    outp( 0x42, (int) (count & 0xFF) );
    outp( 0x42, (int) (count >> 8) );
}

static unsigned long int getticks()
{
union   REGS regs;

    regs.h.ah = 0x00;
    int86( 0x1A, &regs, &regs );
    return( ((unsigned long int) regs.x.cx << 16) + regs.x.dx );
}

void     beep( pitch, nticks )
unsigned int pitch,nticks;
{
unsigned long int ticks,ticksa;

    setfreq( pitch );
    ticksa = getticks();
    while ( ( ticks = getticks() ) == ticksa )
        ;
    ONTONE;
    ticks = ticks + (unsigned long int) nticks;
    while ( getticks() < ticks )
        ;
    OFFTONE;
}

#ifdef DEBUG

#include <stdlib.h>
#include "beep.h"

void main()
{

    beep( 200, 1 );
    exit(0);
}

#endif
