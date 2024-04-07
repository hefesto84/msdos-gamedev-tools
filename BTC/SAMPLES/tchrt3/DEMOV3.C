/*---------------------------------------------------------------------------
 |  Program DEMOV3.C                                                        |
 |                                                                          |
 |  This program demonstrates some of the capabilities of TCHRT V3          |
 |                                                                          |
 |  (c) 1989 Ryle Design, P.O. Box 22, Mt. Pleasant, Michigan 48804         |
 |                                                                          |
 |  V3.00  Turbo C Shareware Evaluation Version                             |
 ---------------------------------------------------------------------------*/

#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <process.h>

#include "pchrt.h"


void sin_funct(void)
/*---------------------------------------------------------------------------
 |  A simple function to time.  PCHRT will tell us how many times this      |
 |  function was called and how much time we spent here.                    |
 |                                                                          |
 |  Globals referenced: none                                                |
 |                                                                          |
 |  Arguments: void                                                         |
 |                                                                          |
 |  Returns: void                                                           |
 ---------------------------------------------------------------------------*/
{
    double  alpha;

	t_entry(4);                                             /* start timer 4 */

    alpha = sin(2.2734593);                                 /* do something */

	t_exit(4);                                              /* stop timer 4 */

} /* sin_funct */


void main(void)
{
    char            tstring[25];
    long unsigned   hits, elapsed;
	int             indx;

    t_request(5);                                           /* ask for 5 timers */
    if (t_start() != TRUE)                                  /* init TCHRT first thing */
    {
        printf("Insufficient heap for TCHRT operation.\n");
        exit(0);
    }

	t_entry(0);                                             /* we use timer 0 to time whole run */

    printf("TCHRT V3 Demonstration\n\n");

    printf("Press any key >> ");

	t_entry(2);                                             /* time getch() with timer 2 */
    getch();
	t_exit(2);

	t_ask_timer(2,&hits,&elapsed);                          /* get timer 2 results */

	printf("\nResponse time was %s\n",t_cvt_time(elapsed,tstring) );

    printf("\nCalling sin function with embedded timer 100 times ... ");
	for (indx=0; indx<100; indx++) sin_funct();
    printf("complete\n\n");

    printf("Press any key to generate timer reports >> ");
	getche();
	printf("\n");

    t_exit(0);                                              /* stop timer timing total run time */

    t_set_report(NONZERO);                                  /* specify report type */
    t_rname("NONZERO report type");                         /* report title        */
    t_name(0,"Total run time");                             /* give each timer a name */
	t_name(2,"getch() response");
	t_name(4,"sin() function");
    t_report(0);                                            /* do report - (0) goes to CRT */

    t_set_report(HIGHWATER);                                /* request different report type */
    t_rname("HIGHWATER report type");                       /* new name */
    t_report(0);                                            /* do it */

    t_stop();                                               /* shut down TCHRT and free heap */

    printf("TCHRT V3 Demo complete\n");                     /* all done ... */

} /* main */



