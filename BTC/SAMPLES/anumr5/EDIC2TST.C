/* OK m‚moire */

#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>

#include "anum.h"
#include "sysio.h"


/* f(t, x, x') = x"  */


double f(double t, double x, double xprime)

{	double res;

	res = 9.0 / 2.0 * sin(5 * t) - 16 * x;
	return(res);
}



void results(int nret, double *xval, double *tval, double *xprimeval)

{	int i;

	puts("       T          X         X'");
	puts("---------------------------------------");
	for (i=0; i<nret; i++)
		printf("% 10.5le  % 10.5le   % 10.5le\n",
				*(tval+i), *(xval+i), *(xprimeval+i));
}



void main(void)

{	double lb,ub,x0,xprime0,*xval,*tval, *xprimeval;
	int nint,nret,errcode;

	clrscr();
	puts("Test program for function initcond2ndorder");
	puts("------------------------------------------\n");

	lb=2.0;
	ub=3.0;
	x0=4.0;
	xprime0=5.0;
	nret=5;
	nint=100;

	printf("Lower bound            : %+10.5lf\n",lb);
	printf("Upper bound            : %+10.5lf\n",ub);
	printf("x at t = %+lf     : %+10.5lf\n",lb,x0);
	printf("x'at t = %+lf     : %+10.5lf\n\n",lb,xprime0);
	printf("Number of asked values : %3d\n",nret);
	printf("Number of intervals    : %3d\n\n",nint);


	xval=farcalloc(nret,sizeof(double));
	xprimeval=farcalloc(nret,sizeof(double));
	tval=farcalloc(nret,sizeof(double));
	if (xval==NULL || tval==NULL || xprimeval==NULL)
	{	fputs("Not enough core", stderr);
		exit(-1);
	}

	initcond2ndorder(lb, ub, x0, xprime0,
			 nret, nint,
			 tval, xval, xprimeval,
			 &errcode, f);
	SYSMSG(errcode, stderr);

	results(nret, xval, tval, xprimeval);

	farfree(tval);
	farfree(xval);
	farfree(xprimeval);


}

