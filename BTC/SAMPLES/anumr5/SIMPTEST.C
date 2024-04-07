#include <math.h>
#include <stdio.h>


#include "anum.h"
#include "sysio.h"



double bf(double x)

{	return exp(3.0*x) + x*x/3.0; }


void main(void)

{       double integ;

	int n=50;
	double lb=1.0;
	double ub=10.0;
	int errcode;

	puts("Test for Simpson function\n"
	     "-------------------------\n");
	printf("Lower bound : %lf\n",lb);
	printf("Upper bound : %lf\n",ub);
	printf("Number of intervals : %d\n",n);

	simpson(lb,ub,
		n,
		&integ,
		&errcode,
		bf);
        SYSMSG(errcode,stderr);

	printf("Value               : %lf\n",integ);
}
