#include <stdio.h>
#include <dos.h>
#include "tctimer.h"
void main()
{
	long start_time, stop_time;
	double *time_ptr;

	time_ptr=(double *)malloc(sizeof(double));
	initializetimer();
	delay(100);

	start_time=readtimer();
	delay(2);
	stop_time=readtimer();
	elapsedtime(start_time, stop_time, time_ptr);

	printf("\nelapsed time = %f\n", *time_ptr);

	printf("Press any key to stop timer");

	start_time=readtimer();
	while(!kbhit());
	stop_time=readtimer();
	getch();
	elapsedtime(start_time, stop_time, time_ptr);

	printf("\nelapsed time = %f\n", *time_ptr);

	restoretimer();
}