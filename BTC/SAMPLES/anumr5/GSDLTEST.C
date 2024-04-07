/* OK */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mem.h>
#include <alloc.h>
#include <dos.h>
#include <conio.h>


#include "anum.h"
#include "sysio.h"

#define MAXDIM 5


void affmat(int dim, double *matrix)


{	int i,j;

	for (i=0;i<dim;i++)
	{	for (j=0;j<dim;j++) printf("%8.4g ",*(matrix+i*dim+j));
		putchar('\n');
	}
}


void affvect(int dim, double *vect)


{	int i;

	for (i=0;i<dim;)
		printf("[%d] = %8.4g\n",++i,*(vect+i));

}



void results(int dim, double *vsol)

{	puts("Solution of A.X=B :");
	affvect(dim,vsol);
	printf("Core left : %ld bytes.\n",farcoreleft());
}






void main(void)


{	double *mat_A, *vect_B, *vect_X;
	double tol=1.e-07;
	int maxiter=12500;
	int iter;
	int dim,i,j,errcode;


	ena_m_beep();
        clrscr();
	puts("Test program for function gauss_seidel");
	puts("--------------------------------------\n");

	for (dim=1;dim<=MAXDIM;dim++)
	{	mat_A=farcalloc(dim*dim,sizeof(double));
		vect_B=farcalloc(dim,sizeof(double));
		vect_X=farcalloc(dim,sizeof(double));

		for (i=0;i<dim;i++)
		{	*(vect_B+i)=(double)(rand()%(10*(1+dim / 4))
							-5 * (1+dim/4));
			*(vect_X+i)=(double)(rand()%(10*(1+dim/4))
							-5*(1+dim/4));
			for(j=0;j<dim;j++)
				*(mat_A+i*dim+j)=(double)(rand()%10*dim
							-5*dim);
		}

		printf("\nMatrix order %d\n",dim);
		puts("Matrix A");
		affmat(dim,mat_A);
		puts("\nVector B");
		affvect(dim, vect_B);

		gauss_seidel(dim, mat_A, vect_B, tol, maxiter, vect_X,
				 &iter , &errcode);
		SYSMSG(errcode,stderr);

		results(dim,vect_X);
		farfree(mat_A);
		farfree(vect_X);
		farfree(vect_B);
	}
}




