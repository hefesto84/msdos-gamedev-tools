#include            <stdio.h>
#include            <music.h>
#include            <video.h>
#include            <string.h>
#include            <process.h>


void    arg_error( void );

void    arg_error( void )
    {
        clrscrn();
        poscurs( 10, 10 );
        printf("\nUSAGE IS: music [1 or 2]\n");
        printf("\n1 for BUGLE call or  2 for HORN");
        exit(1);
    }

main( int argc, char * argv[] )

{


/*
**  play list for victory bugle call
*/

    static  unsigned    char    bugle[] =
                    {
                        'T',60,
                        'N',55,32,128,
                        'N',60,32,128,
                        'N',64,32,128,
                        'N',67,48,228,
                        'N',67,16,228,
                        'N',67,32,228,
                        'N',64,48,228,
                        'N',64,16,228,
                        'N',64,32,228,
                        'N',60,32,128,
                        'N',64,32,128,
                        'N',60,32,128,
                        'N',64,32,128,
                        'N',60,32,128,
                        'N',55,96,240,
                        'X','\0'

                    };

    static  unsigned    char    horn[] =
                    {

                        'T',28,
                        'N',58,24,192,
                        'N',62,8,192,
                        'N',65,96,192,
                        'N',62,24,192,
                        'N',67,8,192,
                        'N',65,32,192,
                        'N',62,24,192,
                        'N',58,8,192,
                        'N',53,32,192,
                        'N',58,24,192,
                        'N',62,8,192,
                        'N',60,32,192,
                        'N',58,24,192,
                        'N',53,8,192,
                        'N',50,32,192,
                        'N',46,24,192,
                        'N',55,8,192,
                        'N',53,64,192,
                        'N',41,64,192,
                        'X','\0'

                    };

    /*
    **  at least 1 argument must be entered
    */


    if( argc != 2 )
        arg_error();

    /*
    **  if not 1 or 2 show usage
    */

    if( strcmp( argv[1], "1" ) == 0 )
        play( bugle );
    else
    if( strcmp( argv[1], "2" ) == 0 )
        play( horn );
    else
        arg_error();
}
