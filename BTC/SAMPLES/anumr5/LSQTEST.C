
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <alloc.h>

#include "anum.h"
#include "sysio.h"


void allocate(double **ptr, int n)

{	*ptr=farcalloc(n, sizeof(double));
	if (*ptr==NULL)
	{	fputs("Not enough core", stderr);
		exit(-1);
	}
}


void getdata(int *n, int *m, double **x, double **y, lsqfit *lsq)

{       int i;

	do
	{	printf("Number of points ( >=2 ) ?");
		while (scanf("%d", n)!=1) ;
	}	while ( *n<2 || *n>20);

	allocate(x, *n);
	allocate(y, *n);

	for (i=0; i<*n; i++)
	{	do
			printf("XPt[%d] ?\n",i+1);
		while (scanf("%lf",*x+i)!=1);
		do
			printf("YPt[%d] ?\n",i+1);
		while (scanf("%lf",*y+i)!=1);
	}


	printf("Number of terms ? (2-%d)\n",*n);
	do
	{	while(scanf("%d",m)!=1) ;
	} while (*m<2 || *m>*n);


	puts("Choose method");
	puts("_____________");
	puts("1) exponantial least square fit");
	puts("2) fourier least square fit");
	puts("3) logarithm least square fit");
	puts("4) polynomial least square fit");
	puts("5) power least square fit");
	puts("6) powers of x least square fit");
	do
	{	while(scanf("%d",&i)!=1) ;
	}     while(i<1 || i>6);

	switch(i)
	{	case 1	: *lsq=expolsq;
				  break;

		case 2	: *lsq=fourierlsq;
				  break;

		case 3	: *lsq=loglsq;
				  break;

		case 4	: *lsq=polylsq;
				  break;

		case 5  : *lsq=powerlsq;
				  break;

		case 6	: *lsq=xpowerlsq;
				  break;
	}
}



void disp_results(int nbpoints, int nbtermes,
		  double *xdata, double *ydata,
		  double *vectsol, double *yfit, double *residus,
		  double ecrtyp, double variance,
		  lsqfit fit, int errcode)

{	int i;

	clrscr();
	SYSMSG(errcode, stderr);
	puts("Results");
	puts("-------");
	puts("The data points:");
	puts("  N             X            Y");
	for (i=0; i<nbpoints; i++)
		printf("%3d        % 10le   % 10le\n", i+1, *(xdata+i),
					*(ydata+i));
	putchar('\n');
	puts(lsqname(fit));
	putchar('\n');

	puts("Coefficients in least squares fit");
	for (i=0; i<nbtermes; i++)
		printf("Coeff[%3d] = % 10le\n",i, *(vectsol+i));
	putchar('\n');

	puts("     X          LSQ Fit       Residual");
	for (i=0; i<nbpoints; i++)
		printf("% 10le  % 10le  % 10le\n", *(xdata+i), *(yfit+i),
							*(residus+i));
	putchar('\n');
	printf("Standard deviation : % 10le\n",ecrtyp);
	printf("Variance           : % 10le\n",variance);

}




void main(void)

{	int nbpoints, nbtermes;
	double *xdata, *ydata, *vectsol, *yfit, *residus,
	       ecrtyp,variance;
	int errcode;
	lsqfit fit;

	ena_m_beep();
	clrscr();
	getdata(&nbpoints, &nbtermes, &xdata, &ydata, &fit);

	allocate(&vectsol, nbpoints);
	allocate(&yfit,    nbpoints);
	allocate(&residus, nbpoints);

	lsq(nbpoints,
		xdata, ydata,
		&nbtermes,
		vectsol, yfit, residus,
		&ecrtyp, &variance,
		&errcode,
		fit);


	disp_results(nbpoints, nbtermes, xdata, ydata,
		vectsol, yfit, residus,
		ecrtyp, variance,
		fit, errcode);

}