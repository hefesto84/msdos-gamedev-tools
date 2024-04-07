/* OK m‚moire */

#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>

#include "anum.h"
#include "sysio.h"



double bf(double *val)

{	double res;

	res = -4 * val[1] * val[4];
	return(res);
}



void results(int order, int nret, double *matsol)

{	int i,j;

	for (i=0; i<order; i++)
	{	printf("\n       T          X[%d]       \n",i+1);
		  puts("------------------------------");
		  for (j=0; j<nret; j++)
			printf("% 10.5le  % 10.5le\n",
				*(matsol+j*(order+1)),
				*(matsol+j*(order+1)+i+1));
	}
}



void main(void)

{	double lb,ub,*initval,*matsol;
	int nint,nret,order,i,errcode;

	clrscr();
	puts("Test program for function initcond");
	puts("----------------------------------\n");

	order = 4;
	lb=5.0;
	ub=6.0;
	if ((initval=farcalloc(order,sizeof(double)))==NULL)
	{	fputs("Not enough core", stderr);
		exit(-1);
	}
	*initval     =  7.0;
	*(initval+1) =  8.0;
	*(initval+2) =  9.0;
	*(initval+3) = 10.0;
	nret=11;
	nint=100;

	printf("Lower bound            : %+10.5lf\n",lb);
	printf("Upper bound            : %+10.5lf\n",ub);

	for (i=0; i<order; i++)
		printf("x[%d] at t = %+lf     : %+10.5lf\n",i+1,lb,
							*(initval+i));
	printf("Number of asked values : %3d\n",nret);
	printf("Number of intervals    : %3d\n\n",nint);

	if ((matsol=farcalloc((order+1)*nret, sizeof(double)))==NULL)
	{	fputs("Not enough core", stderr);
		exit(-1);
	}


	initcond(order, lb, ub, initval,
		 nret, nint,
		 matsol,
		 &errcode, bf);
	SYSMSG(errcode, stderr);

	results(order, nret, matsol);

	farfree(initval);
	farfree(matsol);


}

