/* OK */

#include <math.h>
#include <stdio.h>

#include "anum.h"
#include "sysio.h"



double f1(double x)

{	return(cos(x)-x);
}








void main(void)


{	double pt0,rac,tol,val;
	int iter,maxiter,errcode;

	pt0=1e6;
	tol=1.e-10;
	maxiter=1000;


	puts("Test program for function stefensen");
	printf("Initial approximation : %lf\n"
	       "Tolerance : %le\n"
	       "Maximum number of iterations : %d\n",
	       pt0,tol,maxiter);




	steffensen(pt0,tol,maxiter,&rac,&val,&iter,
				&errcode,f1);
        SYSMSG(errcode,stderr);

	printf("\n\nResults:\n--------\n"
	       "\n\nRoot : %16.12lf\nValue : %12.8le\n"
	       "Iteration number : %d\nError code : %d\n",
	       rac,val,iter,errcode);


}
