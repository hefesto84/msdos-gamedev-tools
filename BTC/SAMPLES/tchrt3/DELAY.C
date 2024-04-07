/*---------------------------------------------------------------------------
 |  Program DELAY.C                                                         |
 |                                                                          |
 |  This program demonstrates the high resolution delay functionality of    |
 |  TCHRT V3.0.                                                             |
 |                                                                          |
 |  (c) 1989 Ryle Design, P.O. Box 22, Mt. Pleasant, Michigan 48804         |
 |                                                                          |
 |  V3.00  Turbo C Shareware Evaluation Version                             |
 ---------------------------------------------------------------------------*/

#include <stdio.h>
#include <math.h>

#include "pchrt.h"

long unsigned       min_delay;
long unsigned       step_delay;
long unsigned       delay_request;
long unsigned       hits;
long unsigned       elapsed;

float               delay_error;
float               resolution;

tdelay_type         dp;


void demo_delay(long unsigned how_long)
/*--------------------------------------------------------------------------
 |  This function executes the requested high resolution delay, reports    |
 |  the deviation and percent error, calculates an additional delay        |
 |  optimization, and executes the delay again.                            |
 |                                                                         |
 |  Globals referenced: none                                               |
 |                                                                         |
 |  Arguments: (long unsigned) how_long - delay length in usec             |
 |                                                                         |
 |  Returns  : void                                                        |
 --------------------------------------------------------------------------*/
{
    long unsigned       hits, elapsed;
    float               delay_error, delay_ff;
    tdelay_type         dp;

    t_ask_delay(how_long, &dp);
    t_reset(1);

	if (delay_request < 54000L)
    {
        printf("Delay with interrupts disabled ... ");
        t_entry(1);
        t_do_delay(&dp);
        t_exit(1);
        printf("complete\n");
    }
    else
    {
        printf("Delay with interrupts enabled ... ");
        t_entry(1);
        t_do_delay_wints(&dp);
        t_exit(1);
        printf("complete\n");
    }

    t_ask_timer(1,&hits,&elapsed);
	delay_error = (float) (abs(elapsed - how_long) / (float) how_long) * 100.0;
    printf("Delay requested: %ld  Actual: %ld  Error: %5.3f%%\n\n",how_long,elapsed,delay_error);

	printf("Calculating additional delay optimization with t_calc_delay_ff ... ");
	if (how_long < 54000L)
		delay_ff = t_calc_delay_ff(how_long,NO_INTS_ON);
    else
		delay_ff = t_calc_delay_ff(how_long,INTS_ON);
    printf("%6.4f\n",delay_ff);

	t_ask_delay(how_long,&dp);
    t_reset(1);

	if (delay_request < 54000L)
    {
        printf("Delay with interrupts disabled ... ");
        t_entry(1);
        t_do_delay(&dp);
        t_exit(1);
        printf("complete\n");
    }
    else
    {
        printf("Delay with interrupts enabled ... ");
        t_entry(1);
        t_do_delay_wints(&dp);
        t_exit(1);
        printf("complete\n");
    }

    t_ask_timer(1,&hits,&elapsed);

    delay_error = (float) (abs(elapsed - how_long) / (float) how_long) * 100.0;
    printf("Delay requested: %ld  Actual: %ld  Error: %5.3f%%\n\n",how_long,elapsed,delay_error);

} /* demo_delay */


void main(void)
{
    printf("Delay Test - TCHRT V3 Demonstration Series\n\n");

    t_start();

	min_delay = t_min_delay();
	resolution = t_res_delay() / 10.0;

	printf("In this hardware environment, minimum delay possible is %ld usec.\n",min_delay);
	printf("Delay resolution is %3.1f usec.\n\n",resolution);

	printf("Enter delay in microseconds: %ld - 1000000, 0 to quit >> ",min_delay);
    scanf("%ld",&delay_request);

    while (delay_request != 0)
    {
        if (delay_request < min_delay)
            delay_request = min_delay;
		else if (delay_request > 1000000L)
			delay_request = 1000000L;

        demo_delay(delay_request);

		printf("Enter delay in microseconds: %ld - 1000000, 0 to quit >> ",min_delay);
        scanf("%ld",&delay_request);
    }

    t_stop();
    printf("\nDelay Test complete.\n");

} /* main */
