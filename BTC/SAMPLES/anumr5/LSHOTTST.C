#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>

#include "anum.h"
#include "sysio.h"

#define LFLOATALLOC(p,n) p=farcalloc(n, sizeof(double));


double bf(double t, double x, double xprime)

{	double res;

	res=xprime / t - x / (t*t) + 1;
	return(res);
}




void results(int nret,
		double *tval, double *xval, double *xprimeval)

{	int i;

	puts("       T           X           X'");
	puts("----------------------------------------");
	for (i=0; i<nret; i++)
		printf("% 10.5le  % 10.5le % 10.5le\n",
				*(tval+i), *(xval+i), *(xprimeval+i));
}



void main(void)

{	double Tlb, Tub, lbXval, ubXval;
	double *tval, *xval, *xprimeval;
	int nint, nret, errcode;

	clrscr();
	puts("Test program for function linear_shooting");
	puts("-----------------------------------------\n");

	Tlb=1.0;
	Tub=2.0;
	lbXval=3.0;
	ubXval=4.0;
	nret=5;
	nint=6;



	printf("T lower bound          : %+10.5lf\n",Tlb);
	printf("T upper bound          : %+10.5lf\n",Tub);
	printf("X at T = %10.15lf      : %+10.5lf\n",Tlb, lbXval);
	printf("X at T = %10.15lf      : %+10.5lf\n",Tub, ubXval);


	printf("Number of asked values : %3d\n",nret);
	printf("Number of intervals    : %3d\n\n",nint);

	tval	  = farcalloc(nret, sizeof(double));
	xval	  = farcalloc(nret, sizeof(double));
	xprimeval = farcalloc(nret, sizeof(double));


	if (tval==NULL || xval==NULL || xprimeval==NULL)
	{	fputs("Not enough working core",stderr);
		exit(-1);
	}


	linear_shooting(Tlb, Tub, lbXval, ubXval,
			nret, nint,
			tval, xval, xprimeval,
			&errcode,
			bf);

	SYSMSG(errcode, stderr);

	results( nret, tval, xval, xprimeval);

	farfree(tval);
	farfree(xval);
	farfree(xprimeval);


}

