#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>

#include "anum.h"
#include "sysio.h"

#define LFLOATALLOC(p,n) p=farcalloc(n, sizeof(double));

/*

 f(t, x, x') = x"

*/

double f(double t, double x, double xprime)

{	double res;

	res=x/xprime;
	res*=res*192;
	return(res);
}




void results(int nret, int iter,
		double *tval, double *xval, double *xprimeval)

{	int i;

	printf("\nResults after %d iteration%c :\n\n",
		iter, (iter >=2 ? 's' : ' '));
	puts("       T           X           X'");
	puts("----------------------------------------");
	for (i=0; i<nret; i++)
		printf("% 10.5le  % 10.5le % 10.5le\n",
				*(tval+i), *(xval+i), *(xprimeval+i));
}



void main(void)

{	double Tlb,Tub,lbXval,ubXval,slope0,tol;
	int iter, maxiter;
	double *tval, *xval, *xprimeval;
	int nint,nret, errcode;

	clrscr();
	puts("Test program for function shooting");
	puts("----------------------------------\n");

	Tlb=1.0;
	Tub=2.0;
	lbXval=3.0;
	ubXval=4.0;
	slope0=5.0;
	nret=6;
	nint=7;
	tol=1.e-8;
	maxiter=100;



	printf("T lower bound          : %+10.5lf\n",Tlb);
	printf("T upper bound          : %+10.5lf\n",Tub);
	printf("X at T = %10.15lf      : %+10.5lf\n",lbXval);
	printf("X at T = %10.15lf      : %+10.5lf\n",ubXval);
	printf("Slope at T = %10.15lf  : %+10.5lf\n",slope0);


	printf("Number of asked values : %3d\n",nret);
	printf("Number of intervals    : %3d\n\n",nint);

	tval	  = farcalloc(nret, sizeof(double));
	xval	  = farcalloc(nret, sizeof(double));
	xprimeval = farcalloc(nret, sizeof(double));


	if (tval==NULL || xval==NULL || xprimeval==NULL)
	{	fputs("Not enough working core",stderr);
		exit(-1);
	}


	shooting(Tlb, Tub, lbXval, ubXval, slope0,
		 nret,
		 tol, maxiter, nint,
		 &iter,
		 tval, xval, xprimeval,
		 &errcode,
		 f);

	SYSMSG(errcode, stderr);

	results( nret, iter, tval, xval, xprimeval);

	farfree(tval);
	farfree(xval);
	farfree(xprimeval);


}

