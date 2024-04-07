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



void results(int dim, int nbvp, double *mvectp)

{       int i;

	printf("\nEigen vectors : ");
	for (i=0; i<nbvp; i++)
	{	putchar('\n');
		affvect(dim, mvectp+dim*i);
	}
	printf("Core left : %ld bytes.\n",farcoreleft());
}






void main(void)


{	double matrix[4][4] ={3,2,1,0,4,6,3,1,2,3,7,2,1,3,5,7};
	double vvalp[4]={12.6904, 5.79129, 3.30958, 1.20871};
	double mvectp[4][4];
	int errcode;


	clrscr();
	printf("Test program for function eigen_vector\n");
	sleep(2);



	clrscr();
	printf("Matrix order : %d\n",4);
	affmat(4,matrix);
 	puts("Eigen values");
	affvect(4,vvalp);


	eigen_vect(4,matrix,3, vvalp,mvectp,&errcode);
	SYSMSG(errcode, stderr);

	results(4, 3, mvectp);


}



