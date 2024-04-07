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

}


	

void main(void)


{	double *mat_A, *vect_B, *vect_X, *mat_P, *mat_D;
	int dim, i, j, errcode, nvect;


	puts("Test program for matrix solving using LU algorithm");
	puts("--------------------------------------------------\n");

	for (dim=2;dim<=MAXDIM;dim++)
	{	mat_A=farcalloc(dim*dim,sizeof(double));
		mat_D=farcalloc(dim*dim,sizeof(double));
		mat_P=farcalloc(dim*dim,sizeof(double));

		vect_B=farcalloc(dim,sizeof(double));
		vect_X=farcalloc(dim,sizeof(double));

		for (i=0; i<dim; i++)
			for(j=0;j<dim;j++)
				*(mat_A+i*dim+j)=(double)(rand()%10)-5;

		printf("\nMatrix order %d\n",dim);
		puts("Matrix A");
		affmat(dim,mat_A);

		lu_decompose(dim, mat_A, mat_D, mat_P, &errcode);
		SYSMSG(errcode,stderr);

		if (errcode==ENOERROR)
			for (nvect=1; nvect<=dim; nvect++)
			{	for (i=0; i<dim; i++)
					*(vect_B+i)=(double)(rand()%10)-5;
     
				puts("\nVector B");
				affvect(dim, vect_B);

				lu_solve(dim, mat_D, vect_B, mat_P,
						vect_X, &errcode);
				SYSMSG(errcode,stderr);

				results(dim,vect_X);
			}

		farfree(mat_A);
		farfree(mat_P);
		farfree(mat_D);
		farfree(vect_X);
		farfree(vect_B);
		printf("Core left : %ld bytes.\n",farcoreleft());
		delay(2000);
	}
}



