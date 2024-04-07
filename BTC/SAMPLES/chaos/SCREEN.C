#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>

void clear_screen()
{
union REGS regs;

	/* first clear the screen */
    regs.h.ah = 6;
    regs.h.al = 0;
    regs.h.bh = 7;
    regs.h.ch = 0;
    regs.h.cl = 0;
    regs.h.dh = 24;
    regs.h.dl = 79;
    int86( 0x10, &regs, &regs );

	/* then put the cursor in the top left corner */
    regs.h.ah = 2;
    regs.h.dh = 0;
    regs.h.dl = 0;
    regs.h.bh = 0;
    int86( 0x10, &regs, &regs );
}

void print_centered( s )
char *s;
{
int i;

    for ( i = 0; i < (80 - strlen( s ))/2; i++ )
        putch( ' ' );
    cputs( s );
}

void draw_screen()
{

    printf( "\n\n\n" );
    print_centered( "Music from Chaos" );
    printf( "\n\n" );
    print_centered( "by" );
    printf( "\n\n" );
    print_centered( "Peter J. Becker" );
    printf( "\n\n\n" );
    print_centered( "\xDA\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xBF" );
    printf( "\n" );
    print_centered( "\xB3  \x18 to decrease r, small step   \xB3" );
    printf( "\n" );
    print_centered( "\xB3  \x19 to increase r, small step   \xB3" );
    printf( "\n" );
    print_centered( "\xB3                                \xB3" );
    printf( "\n" );
    print_centered( "\xB3 PgUp to decrease r, large step \xB3" );
    printf( "\n" );
    print_centered( "\xB3 PgDn to increase r, large step \xB3" );
    printf( "\n" );
    print_centered( "\xB3                                \xB3" );
    printf( "\n" );
    print_centered( "\xB3          ESC to exit           \xB3" );
    printf( "\n" );
    print_centered( "\xC0\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xD9" );
    printf( "\n\n" );
}
