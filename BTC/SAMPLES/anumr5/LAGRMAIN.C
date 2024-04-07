
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "anum.h"
#include "sysio.h"






void affpoly(COMPLEX *p,int n)

{	int i;

	for (i=n;i>=0;i--)
		printf("Poly[%d] = %10lf+%10lfi\n",i,(p+i)->x, (p+i)->y);
}




void results(int initdegree,
	     COMPLEX *initpoly,
	     COMPLEX initguess, double tol,
	     int maxiter, int numroots,
	     COMPLEX *root, COMPLEX *value,
	     int *iter,
	     int errcode)



{	int i;

	puts("\n\n\nInitial polynome :\n");
	for (i=initdegree; i>=0; i--)
		printf("Poly[%d] : %10lf+%10lfi\n",i,(initpoly+i)->x,
					(initpoly+i)->y);
	printf("\nInitial approximation : %lf+%lfi\n",initguess.x,
					initguess.y);
	printf("tolerance : %le\n",tol);
	printf("Maximum number of iterations : %d\n\n",maxiter);
        SYSMSG(errcode,stderr);

	for (i=1; i<=numroots; i++)
	{	printf("\nRoot %d\n",i);
		printf("Number of iterations : %d\n",*(iter+i));
		printf("Root value : %le+%lei",(root+i)->x,
						(root+i)->y);
		printf("\nFunction value at this root :"
		       " %le+%lei\n",(value+i)->x,(value+i)->y);
	}

}



void laguerre(int degre,
	      COMPLEX *poly,
	      COMPLEX z0,
	      double tol,
	      int maxiter,
	      int *nbrac,
	      COMPLEX *racines, COMPLEX *yracines,
	      int *iter, int *errcode);

void main()


{	COMPLEX poly0[20];
	double tol;
	COMPLEX z;
	COMPLEX racines[20],valeurs[20];
	int maxiter,nbrac,iter[20],errcode,degre0;

	FILE *fp;
	char s[80];
	int i;

	fp=fopen("toto.dat","rt");
	if (fp==NULL)
	{	errmsg(__FILE__,__DATE__,__TIME__,EFOPENR,
		"Can't read file \"toto.dat\".",stderr);
		exit(EFOPENR);
	}
	fgets(s,80,fp);
	sscanf(s,"%d",&degre0);
	degre0 /= 2;
	for (i=degre0; i>=0; i--)
	{	fgets(s,80,fp);
		sscanf(s,"%lf",&(poly0[i].x));
		fgets(s,80,fp);
		sscanf(s,"%lf",&(poly0[i].y));
	}
	fclose(fp);

	maxiter=1000;
	rassign(1,&z);
	tol=1e-8;


	puts("Polynome to solve using Laguerre algorithm :");
	affpoly(poly0,degre0);
	laguerre(degre0,
		 poly0,
		 z,
		 tol,
		 maxiter,
		 &nbrac,
		 racines,valeurs,
		 iter,&errcode);
	puts("\n\nResults:\n-------\n");

	results(degre0,poly0,z,tol,maxiter,
		nbrac,racines,valeurs,iter,errcode);
}

