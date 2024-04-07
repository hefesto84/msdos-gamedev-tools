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


void affvect(int nbvp, double *v)

{	register int i;

	for (i=0; i<nbvp; i++)
	{	printf("% lf ",*(v+i));
		if ((i+1)%6==0) putchar('\n');
	}
	putchar('\n');
}



void results(int nbvp,double *vvalp, int errcode)

{       SYSMSG(errcode,stderr);
	printf("\n\nNumber of asked eigenvalues : %d\n",nbvp);
	puts("\nEigen values :");
	affvect(nbvp, vvalp);
	printf("Core left : %ld bytes.\n",farcoreleft());
}




void main(void)


{	double *matrix,*vvalp;
	int dim,i,j,errcode,nbvp;


	clrscr();
	printf("Test program for function householder_givens\n");
	sleep(2);
	for (dim=1;dim<=MAXDIM;dim++)
	{	matrix=farcalloc(dim*dim,sizeof(double));
		vvalp=farcalloc(dim,sizeof(double));
		nbvp=dim;
		for (i=0;i<dim;i++)
			for(j=i; j<dim; j++)
			{	*(matrix+i*dim+j)=(double)(rand()%100)-50;
				*(matrix+j*dim+i)=*(matrix+i*dim+j);
			}

		clrscr();
		printf("Matrix order : %d\n",dim);
		affmat(dim,matrix);


		householder_givens(dim,matrix,nbvp,
			vvalp,&errcode);

		results(nbvp,vvalp,errcode);
		farfree(matrix);
		farfree(vvalp);
		delay(4000);
	}
}



