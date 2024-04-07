#include <stdio.h>  /* OK */
#include <math.h>
#include <stdlib.h>


/*#pragma warn -stv*/



#include "anum.h"
#include "sysio.h"





COMPLEX bf(COMPLEX z)

{	COMPLEX res;

	res.x=cos(z.x)*(exp(-z.y)+exp(z.y)) /2 -z.x;
	res.y=sin(z.x)*(exp(-z.y)-exp(z.y)) /2 -z.y;
	return(res);
}



void results(COMPLEX guess, COMPLEX racine, COMPLEX yracine,
	     double tol,
	     int maxiter,
	     int iter,
	     int errcode)

{	printf("Initial approximation: "
	       "%lf +%lfi\n",guess.x,guess.y);
	printf("Tolerance : %lf\n",tol);
	printf("Maximum number of iterations: %d\n\n",maxiter);
        SYSMSG(errcode,stderr);

	printf("Iterations number : %d\n",iter);
	printf("Calculated root: %16.12lf + %16.12lfi\n",racine.x,racine.y);
	puts("Function value at this root : ");
	printf("(%16.12le) + (%16.12le) i\n\n",yracine.x,yracine.y);
}


void main()


{       COMPLEX z0={1,1};
	COMPLEX racine,yracine;
	double tol=1e-8;
	int maxiter=100;
	int errcode,iter;

	puts("Test program for function muller\n");
	muller(z0,tol,maxiter,
		&racine,&yracine,&iter,&errcode,bf);

	puts("\n\nResults :\n-------\n");

	results(z0,racine,yracine,tol,maxiter,iter,errcode);
}
