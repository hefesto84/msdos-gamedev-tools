/* OK */

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



void results(double det, int errcode)

{	printf("Error Code : %d\nDeterminant : %lf\n",errcode,det);
	printf("Core left : %ld bytes.\n",farcoreleft());
}




void main(void)


{	double *matrix;
	int dim,i,j,errcode;
	double det;


	puts("Test program for function determinant\n");
	for (dim=1;dim<=MAXDIM;dim++)
	{	matrix=farcalloc(dim*dim,sizeof(double));
		if (matrix==NULL)
		{	fputs("Not enough core", stderr);
			exit(ENOWCORE);
		}

		for (i=0;i<dim;i++)
			for(j=0;j<dim;j++)
				*(matrix+i*dim+j)=(double)(rand()%10)-5;

		printf("Matrix order : %d.\n",dim);
		affmat(dim,matrix);

		determinant(dim,matrix,&det,&errcode);
                SYSMSG(errcode,stderr);

		results(det,errcode);
		farfree(matrix);
	}
}




