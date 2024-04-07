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



void results(int dim, double *valp, double *vectp, int errcode, int iter)

{       int i;

	SYSMSG(errcode,stderr);
	puts("\nEigen values :");
	affvect(dim, valp);
	puts("Eigen vectors :");
	for (i=0; i<dim; i++)
		affvect(dim,vectp+i*dim);
	printf("Iteration number : %d\n",iter);
	printf("Core left : %ld bytes.\n",farcoreleft());
}




void main(void)


{	double *matrix,tol,*valp,*mvectp;
	int dim,i,j,errcode,maxiter,iter;

	tol=1e-10;
	maxiter=200;

	clrscr();
	printf("Test program for function jacobi\n");
	sleep(2);
	for (dim=1;dim<=MAXDIM;dim++)
	{	matrix=farcalloc(dim*dim,sizeof(double));
		mvectp=farcalloc(dim*dim,sizeof(double));
		valp=farcalloc(dim,sizeof(double));

		for (i=0;i<dim;i++)
			for(j=i; j<dim; j++)
			{	*(matrix+i*dim+j)=(double)(rand()%100)-50;
				*(matrix+j*dim+i)=*(matrix+i*dim+j);
			}

		clrscr();
		printf("Matrix order : %d\n",dim);
		affmat(dim,matrix);


		jacobi(dim,matrix,maxiter,tol,valp,
			mvectp,&iter,&errcode);

		results(dim,valp,mvectp,errcode,iter);
		farfree(matrix);
		farfree(valp);
		farfree(mvectp);
		delay(4000);
	}
}



