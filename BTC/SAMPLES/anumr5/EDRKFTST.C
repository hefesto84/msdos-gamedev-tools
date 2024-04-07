/* OK m‚moire */


#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>

#include "anum.h"
#include "sysio.h"



double bf(double t, double x)

{	double res;

	res = x/t + t - 1;
	return(res);
}



void results(int nret, double *xval, double *tval)

{	int i;

	puts("       T          X");
	puts("---------------------------");
	for (i=0; i<nret; i++)
		printf("% 10.5le  % 10.5le\n", *(tval+i), *(xval+i));
}



void main(void)

{	double lb,ub,x0,tol,*xval,*tval;
	int nret,errcode;

	clrscr();
	puts("Test program for function RKF");
	puts("-----------------------------\n");

	lb=2.0;
	ub=3.0;
	x0=4.0;
	nret=5;
	tol=1.0e-6;

	printf("Lower bound            : %+10.5lf\n",lb);
	printf("Upper bound            : %+10.5lf\n",ub);
	printf("x at t = %+lf     : %+10.5lf\n\n",lb,x0);
	printf("Number of asked values : %3d\n",nret);
	printf("Tolerance in X/T units : %10.5le\n\n",tol);

	xval=farcalloc(nret,sizeof(double));
	tval=farcalloc(nret,sizeof(double));
	if (xval==NULL || tval==NULL)
	{	fputs("Not enough core", stderr);
		exit(-1);
	}

	RKF(lb, ub, x0, tol, nret,tval, xval, &errcode, bf);
	SYSMSG(errcode, stderr);

	results(nret, xval, tval);

	farfree(tval);
	farfree(xval);

}

