/* OK */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mem.h>
#include <alloc.h>
#include <conio.h>
#include <dos.h>


#include "anum.h"
#include "sysio.h"

#define MAXDIM 8



void affmat(int dim, double *matrix)


{	register int i,j;

	for (i=0;i<dim;i++)
	{	for (j=0;j<dim;j++) printf("% 6lg ",*(matrix+i*dim+j));
		putchar('\n');
	}
}


void affvect(int dim, double *v)

{	register int i;

	for (i=0; i<dim; i++)
	{	printf("% lf ",*(v+i));
		if ((i+1)%6==0) putchar('\n');
	}
	putchar('\n');
}



void results(int dim, double valp, double *vectp, int errcode, int iter)

{       gotoxy(1,11);
	SYSMSG(errcode,stderr);
	gotoxy(1,18);
	printf("\nEigen value : %lf\n",valp);
	puts("Eigen vector :");
	affvect(dim,vectp);
	printf("Iteration number : %d\n",iter);
	printf("Core left : %ld bytes.\n",farcoreleft());
}




void main(void)


{	double *matrix,*vect0,tol,valp,*vectp;
	int dim,i,j,errcode,maxiter,iter;

	tol=1e-10;
	maxiter=200;

	clrscr();
	printf("Test program for function eigen_power\n");
	sleep(2);
	for (dim=1;dim<=MAXDIM;dim++)
	{	matrix=farcalloc(dim*dim,sizeof(double));
		vect0=farcalloc(dim,sizeof(double));
		vectp=farcalloc(dim,sizeof(double));

		for (i=0;i<dim;i++)
			for(j=0;j<dim;j++)
				*(matrix+i*dim+j)=(double)(rand()%100)-50;

		for (i=0;i<dim;i++) *(vectp+i)=1.0/((double)(rand()%1000+1));

		clrscr();
		printf("Matrix order : %d\n",dim);
		affmat(dim,matrix);


		eigen_power(dim,matrix,vect0,maxiter,tol,&valp,
			vectp,&iter,&errcode);

		results(dim,valp,vectp,errcode,iter);
		delay(4000);
		farfree(matrix);
		farfree(vect0);
		farfree(vectp);
	}
}



