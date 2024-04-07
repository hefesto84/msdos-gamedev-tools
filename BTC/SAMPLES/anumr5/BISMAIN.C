/* OK */

/* Test program for function bisect */

#include <math.h>
#include <stdio.h>
#include <time.h>

#include "anum.h"
#include "sysio.h"


double bf(double x)


{	return(log(x)+x);
}





void main(void)



{	double pp,dp,rac,tol,val;
	int iter,maxiter,errcode;


	pp=0.001;
	dp=1000;
	tol=1.e-10;
	maxiter=1000;




	puts("Test program of bisect function");
	printf("between the bounds %lf and %lf, with\n"
	       "%le as tolerance and %d as maximum number of iterations.\n",
	       pp,dp,tol,maxiter);



	bisect(pp,dp,tol,maxiter,&rac,&val,&iter,&errcode,bf);
	SYSMSG(errcode,stderr);
	printf("\n\nResults:\n--------\n"
	       "\n\nRoot : %16.12lf\nValue : %12.8le\n"
	       "Iterations : %d\nError code : %d\n",
	       rac,val,iter,errcode);


}
