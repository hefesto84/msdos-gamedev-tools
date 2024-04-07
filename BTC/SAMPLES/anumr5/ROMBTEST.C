#include <math.h>
#include <stdio.h>


#include "anum.h"
#include "sysio.h"



double bf(double x)

{	return exp(3.0*x) + x*x/3.0; }


void main(void)

{       double integ;

	double lb=1.0;
	double ub=10.0;
	double tol=1.e-08;
	int maxiter=30;
	int errcode, iter;

	puts("Test for romberg function\n"
	     "-------------------------");
	printf("Lower bound                  : %lf\n",lb);
	printf("Upper bound                  : %lf\n",ub);
	printf("Maximum number of iterations : %d\n",maxiter);

	romberg(lb,ub,
		tol, maxiter,
		&integ,
		&iter,
		&errcode,
		bf);
        SYSMSG(errcode,stderr);

	printf("Value                        : %le\n",integ);
	printf("Ran through                  : %d iterations\n",iter);
}
