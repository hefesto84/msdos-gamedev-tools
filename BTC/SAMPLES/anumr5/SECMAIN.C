/* OK */

#include <stdio.h>
#include <conio.h>
#include <math.h>

#include "anum.h"
#include "sysio.h"


double bf(double x)


{	return(cos(x)-x);
}





void main(void)



{	double pp,dp,rac,tol,val;
	int iter,maxiter,errcode;


	pp=-10000;
	dp=10000;
	tol=1.e-10;
	maxiter=1000;


	puts("Test program for function secant");
	printf("with bounds %lf and %lf, and %le as tolerance\n"
	       "and %d as maximum number of iterations\n",
	       pp,dp,tol,maxiter);




	secant(pp,dp,tol,maxiter,&rac,&val,&iter,&errcode,bf);
	SYSMSG(errcode,stderr);

	printf("\n\nResults :\n--------\n"
	       "\n\Root : %16.12lf\nValue : %12.8le\n"
	       "Iteration number : %d\nError code : %d\n",
	       rac,val,iter,errcode);


}
