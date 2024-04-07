#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>

#include "anum.h"
#include "sysio.h"




/* Differential equation are defined as follows :


    x" = f(v, v')
     n

    with v = (t, x , x , ..., x , ..., x    )
		  1   2        n        nbeq

    and  v'= (1, x', x', ..., x', ..., x'   )
		  1   2        n        nbeq

*/


double bf1(double *v, double *vprime)

{	double t, res;

	t = *v;
	res = -9.8 * (*(v+1)) / 0.6125 - 16.0 * (*(v+1) - *(v+2));
	return(res);
}



double bf2(double *vX, double *vXprime)

{	double t,res;

	t = *vX;
	res = -9.8 * (*(vX+2)) / 0.6125 + 16.0 * (*(vX+1) - *(vX+2));
	return(res);
}





void results(int neq, int nret, double *matsol)

{	int i,j;

	for (i=0; i<neq; i++)
	{	printf("\n       T          Value       \n");
		  puts("------------------------------");
		  for (j=0; j<nret; j++)
			printf("% 10.5le  % 10.5le\n",
				*(matsol+j*(neq+1)),
				*(matsol+j*(neq+1)+i+1));
	}
}



void main(void)

{	double lb,ub,*initvalX, *initvalXprime, *matsolX, *matsolXprime;
	int nint,nret,neq,i,errcode;
	double (**tabfunc)(double *, double *);

	clrscr();
	puts("Test program for function initcondsystem2");
	puts("-----------------------------------------\n");

	neq = 2;
	lb=3.0;
	ub=4.0;

	if ((initvalX=farcalloc(neq,sizeof(double)))==NULL)
	{	fputs("Not enough core", stderr);
		exit(-1);
	}

	if ((initvalXprime=farcalloc(neq,sizeof(double)))==NULL)
	{	fputs("Not enough core", stderr);
		exit(-1);
	}

	tabfunc=farcalloc(neq,sizeof( double (*)() ));

	if (tabfunc==NULL)
	{	fputs("Not enough core", stderr);
		exit(-1);
	}
	*tabfunc = *bf1;
	*(tabfunc+1) = *bf2;


	*initvalX     =  5.0;
	*(initvalX+1) =  7.0;
	*initvalXprime     =  6.0;
	*(initvalXprime+1) =  8.0;


	nret=9;
	nint=100;

	printf("Lower bound            : %+10.5lf\n",lb);
	printf("Upper bound            : %+10.5lf\n",ub);

	for (i=0; i<neq; i++)
		printf("x[%d] at t = %+lf     : %+10.5lf\n",i+1,lb,
							*(initvalX+i));
	printf("Number of asked values : %3d\n",nret);
	printf("Number of intervals    : %3d\n\n",nint);

	if ((matsolX=farcalloc((neq+1)*nret, sizeof(double)))==NULL)
	{	fputs("Not enough core", stderr);
		exit(-1);
	}

	if ((matsolXprime=farcalloc((neq+1)*nret, sizeof(double)))==NULL)
	{	fputs("Not enough core", stderr);
		exit(-1);
	}

	initcondsystem2(neq, lb, ub, initvalX, initvalXprime,
			nret, nint,
			matsolX, matsolXprime,
			&errcode, tabfunc);

	SYSMSG(errcode, stderr);

	puts("Computed X(T) is as follows:\n");
	results(neq, nret, matsolX);
	puts("\n\nComputed X'(T) is as follows:\n");
	results(neq, nret, matsolXprime);

	farfree(initvalX);
	farfree(matsolX);
	farfree(initvalXprime);
	farfree(matsolXprime);
	farfree(tabfunc);

}

