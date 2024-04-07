#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>

#include "anum.h"
#include "sysio.h"







/* Differential equation are defined as follows :


    x' = f(v)
     n

    with v = (t, x , x , ..., x , ..., x    )
		  1   2        n        nbeq

*/

double f1(double *v)

{	return(*(v+2));
}


double f2(double *v)

{	return(sin((*v)*5)*9.0/2.0 - 16*(*(v+1)));
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
	int nint,nret,neq,i,errcode;
	double (**tabfunc)(double *);

	clrscr();
	puts("Test program for function initcondsystem1");
	puts("-----------------------------------------\n");

	neq = 2;
	lb=3.0;
	ub=4.0;

	if ((initval=farcalloc(neq,sizeof(double)))==NULL)
	{	fputs("Not enough core", stderr);
		exit(-1);
	}

	tabfunc=farcalloc(2,sizeof( double (*)() ));

	if (tabfunc==NULL)
	{	fputs("Not enough core", stderr);
		exit(-1);
	}
	*tabfunc = *f1;
	*(tabfunc+1) = *f2;


	*initval     =  5.0;
	*(initval+1) =  6.0;
	nret=7;
	nint=100;

	printf("Lower bound            : %+10.5lf\n",lb);
	printf("Upper bound            : %+10.5lf\n",ub);

	for (i=0; i<neq; i++)
		printf("x[%d] at t = %+lf     : %+10.5lf\n",i+1,lb,
							*(initval+i));
	printf("Number of asked values : %3d\n",nret);
	printf("Number of intervals    : %3d\n\n",nint);

	if ((matsol=farcalloc((neq+1)*nret, sizeof(double)))==NULL)
	{	fputs("Not enough core", stderr);
		exit(-1);
	}


	initcondsystem1(neq, lb, ub, initval,
			nret, nint,
			matsol,
			&errcode, tabfunc);

	SYSMSG(errcode, stderr);

	results(neq, nret, matsol);

	farfree(initval);
	farfree(matsol);
	farfree(tabfunc);


}

