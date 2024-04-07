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
	{	for (j=0;j<dim;j++) printf("%10.4g ",*(matrix+i*dim+j));
		putchar('\n');
	}
}



void results(int dim, double *invmat, int errcode)

{	printf("Error code : %d\nInverted matrix :\n",errcode);
	affmat(dim,invmat);
	printf("Core left : %ld bytes.\n",farcoreleft());
}




void main(void)


{	double *matrix, *invmat, *product;
	int dim, i, j, k, errcode;


	puts("Test program for function inverse");
	puts("---------------------------------\n");
	for (dim=1;dim<=MAXDIM;dim++)
	{	matrix=farcalloc(dim*dim,sizeof(double));
		invmat=farcalloc(dim*dim,sizeof(double));
		product=farcalloc(dim*dim,sizeof(double));

		for (i=0;i<dim;i++)
			for(j=0;j<dim;j++)
				*(matrix+i*dim+j)=(double)(rand()%10)-5;

		printf("\nMatrix order %d\n",dim);
		affmat(dim,matrix);

		inverse(dim,matrix,invmat,&errcode);
                SYSMSG(errcode,stderr);

		results(dim,invmat,errcode);

		if (errcode==ENOERROR)
		{	for (i=0; i<dim; i++)
				for(j=0; j<dim; j++)
				{	*(product+i*dim+j)=0.0;
					for (k=0; k<dim; k++)
						*(product+i*dim+j) +=
						  (*(matrix+i*dim+k))*
							(*(invmat+k*dim+j));
				}
			affmat(dim,product);
		}

		farfree(product);
		farfree(matrix);
		farfree(invmat);
	}
}




