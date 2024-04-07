/* OK */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "anum.h"
#include "sysio.h"






void affpoly(double *p,int n)

{	int i;

	for (i=n;i>=0;i--) printf("Poly[%d] = %lf\n",i,*(p+i));
}




void results(int initdegree,
	     double *initpoly,
	     double initguess, double tol,
	     int maxiter, int degree, int numroots,
	     double *poly, double *root, double *imag,
	     double *value, double *deriv,
	     int *iter,
	     int errcode)



{	int i;

	puts("\n\n\nInitial polynome :\n");
	for (i=initdegree; i>=0; i--)
		printf("Poly[%d] : %lf\n",i,*(initpoly+i));
	printf("\nInitial approximation : %lf\n",initguess);
	printf("tolerance : %le\n",tol);
	printf("Maximum number of iterations : %d\n\n",maxiter);
        SYSMSG(errcode,stderr);
	if (errcode==EOVERMAXITER)
		SYSMSG(WCPLXROOTS,stderr);
	printf("\nNumber of calculated roots : %d\n", numroots);
	for (i=1; i<=numroots; i++)
	{	printf("\nRoot : %d\n",i);
		printf("Iterations number : %d\n",*(iter+i));
		printf("Root : %le",*(root+i));
		if (iseq0(*(imag+i)))
			putchar('\n');
		else
			printf(" + %lfi\n",*(imag+i));
		printf("Value of the function at this root : %le\n",
				*(value+i));
		printf("Value of the derivative at this root : %le\n\n",
				*(deriv+i));
	}

	if (errcode==EOVERMAXITER || errcode==WCPLXROOTS)
	{	puts("\nReduced polynome : ");
		for (i=degree; i>=0; i--) printf("RP[%d]=%lf\n",i,*(poly+i));
	}
}


void main()


{	double poly0[20];
	double x,tol,poly[20],racine[20],image[20],valeur[20],deriv[20];
	int maxiter,degre,nbrac,iter[20],errcode,degre0;

	FILE *fp;
	char s[80];
	int i;

	fp=fopen("toto.dat","rt");
	if (fp==NULL)
	{	errmsg(__FILE__,__DATE__,__TIME__,EFOPENR,
			"Can't read \"toto.dat\".",stderr);
		exit(EFOPENR);
	}
	fgets(s,80,fp);
	sscanf(s,"%d",&degre0);
	for (i=degre0; i>=0; i--)
	{	fgets(s,80,fp);
		sscanf(s,"%lf",&poly0[i]);
	}
	fclose(fp);

	maxiter=1000;
	x=1;
	tol=1e-8;


	puts("Polynome to solve using Newton-Horner algorithm :");
	affpoly(poly0,degre0);
	newton_horner(degre0,
		      poly0,
		      x,tol,
		      maxiter,
		      &degre,&nbrac,
		      poly,racine,image,valeur,deriv,
		      iter,&errcode);
	puts("\n\nResults :\n-------\n");

	results(degre0,poly0,x,tol,maxiter,
		degre,nbrac,poly,racine,image,valeur,deriv,iter,errcode);
}


