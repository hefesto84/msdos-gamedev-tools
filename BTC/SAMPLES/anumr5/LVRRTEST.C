

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mem.h>
#include <alloc.h>


#include "anum.h"
#include "sysio.h"

#define MAXDIM 5


void affmat(int dim, double *matrix)


{	int i,j;

	for (i=0;i<dim;i++)
	{	for (j=0;j<dim;j++) printf("%6g ",*(matrix+i*dim+j));
		putchar('\n');
	}
}



void results(int dim, double *poly)

{       int i;

	puts("Characteristic polynomial");
	for (i=dim; i>=0; i--)
	{	printf("% 10lg ", *(poly+i));
		if (i>=1)
		{	printf("*X%c",i>1 ? '^' : ' ');
			if (i>1) printf("%d", i);
			puts(" +");

		}
	}
	printf("\n Core left : %ld bytes.\n",farcoreleft());
}




void main(void)


{	double *matrix, *poly;
	int dim,i,j,errcode;
	double ;


	puts("Test program for function le_verrier\n");
	for (dim=1;dim<=MAXDIM;dim++)
	{	matrix=farcalloc(dim*dim,sizeof(double));
		if (matrix==NULL)
		{	fputs("Not enough core", stderr);
			exit(ENOWCORE);
		}

		poly=farcalloc(dim+1, sizeof(double));
		if (poly==NULL)
		{       farfree(poly);
			fputs("Not enough core", stderr);
			exit(ENOWCORE);
		}



		for (i=0;i<dim;i++)
			for(j=0;j<dim;j++)
				*(matrix+i*dim+j)=(double)(rand()%10)-5;

		printf("Matrix order : %d.\n",dim);
		affmat(dim,matrix);

		le_verrier(dim, matrix, poly, &errcode);
		SYSMSG(errcode,stderr);

		results(dim, poly);
		farfree(matrix);
		farfree(poly);
	}
}




