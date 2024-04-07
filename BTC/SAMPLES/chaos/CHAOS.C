#include <stdio.h>
#include <conio.h>

#define R_MIN 2.95
#define R_MAX 4.0

void play( int );

void clear_screen();
void draw_screen();
void hide_cursor();

void do_chaos( r, f )
double r;
void (*f)();
{
int i,t;
double x = .3;

        for ( i = 0; i < 250; i++ )
            x = r*x*(1.0-x);
        for ( ; ; )
            {
            x = r*x*(1.0-x);
            t = 95.0*x;
            (*f)( t );
            if ( kbhit() )
                break;
            }
}

void play_chaos()
{
double r = (R_MAX + R_MIN)/2.0;
double dr = (R_MAX - R_MIN)/500.0;
double bigdr = dr*10.0;
char outstr[40];
int done = 0;

    while ( !done )
        {
        sprintf( outstr, "r = %f\r", r );
        print_centered( outstr );
        do_chaos( r, play );
        switch( getch() )
            {
            case 27 :
                done = 1;
                break;

            case 0  :
                switch( getch() )
                    {
                    case 73 :
                        r -= bigdr;
                        if ( r < R_MIN )
                            r = R_MIN;
                        break;

                    case 81 :
                        r += bigdr;
                        if ( r > R_MAX )
                            r = R_MAX;
                        break;

                    case 72 :
                        r -= dr;
                        if ( r < R_MIN )
                            r = R_MIN;
                        break;

                    case 80 :
                        r += dr;
                        if ( r > R_MAX )
                            r = R_MAX;
                        break;

                    default :
                        break;

                    }
            default :
                break;
            }
        }
}

void main()
{

    clear_screen();
    draw_screen();
    play_chaos();
    clear_screen();
}
