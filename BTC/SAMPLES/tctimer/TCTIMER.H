
void cardinal(long l, double *result);


void elapsedtime(long start, long stop, double *result);
/*Calculate time elapsed (in milliseconds) between Start and Stop*/

void initializetimer(void);
/*Reprogram the timer chip to allow 1 microsecond resolution*/

void restoretimer(void);
/*Restore the timer chip to its normal state*/

long readtimer(void);
/*Read the timer with 1 microsecond resolution*/

