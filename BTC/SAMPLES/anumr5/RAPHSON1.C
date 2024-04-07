/* OK */

#include <math.h>
#include <conio.h>
#include <stdio.h>

#include "anum.h"
#include "sysio.h"



double f1(double x)

{	return(cos(x)-x);
}


double f2(double x)
{	return(-sin(x)-1);
}






void main(void)


{	double pt0,rac,tol,val,racderiv;
	int iter,maxiter,errcode;

	pt0=1e2;
	tol=1.e-10;
	maxiter=1000;


	puts("Essai de la procedure Newton-Raphson");
	printf("A partir de %lf et  avec une \n"
	       "tol‚rance de %le et un nb max d'it‚rations de %d\n",
	       pt0,tol,maxiter);




	newton_raphson(pt0,tol,maxiter,&rac,&val,&racderiv,&iter,
				&errcode,*f1,*f2);
        SYSMSG(errcode,stderr);

	printf("\n\nR‚sultats:\n----------\n"
	       "\n\nracine : %16.12lf\n valeur: %12.8le\n"
	       "deriv‚e : %16.12lf\niterations : %d\nerrcode : %d\n",
	       rac,val,racderiv,iter,errcode);


}
