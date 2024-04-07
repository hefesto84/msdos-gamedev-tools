/*  Leica_u.c

    This is a version of cutils.c, hacked especially for the leica
    software.  */

#define   A          6378137.0    /* A and 1/F for WGS84 */
#define   ONE_F      298.257223563
#define   PI         3.1415926535898
#define   MAX_SATS   20
#define   MAXN 10     /*  Maximum size of vectors and arrays  */
/*  If this number needs to be made larger, a larger memory model
    should be used. Which one will depend on the details; use
    C's memory checking utilities to see if the memory model being
    used is adequate. The consequences of using too small a model
    include disappearing data and null pointer assignments.
*/


/*-----------------------------------------------------------------*/
/*   Global typedefs  */
typedef double vec[MAXN];     /*  Not really used any more.  */
typedef double mat[MAXN][MAXN];
typedef double (*daryp)[MAXN][MAXN];
typedef double (dary)[MAXN][MAXN];
/*-----------------------------------------------------------------*/
typedef struct {       /*  Generic vector definition          */
     int n;            /*  Size of vector                     */
     double a[MAXN];   /*  The elements of the vector         */
     int err;          /*  Error reporting: 0=ok, 1=error     */
		       /*  Note: provision has been made for error
			   reporting by including this element. However,
			   the program does not yet make full use of the
			   error reporting feature. (For future work ). The
			   same comments apply to the "err" element in the
			   matrix structure, below.                         */

} vector;
/*-----------------------------------*/
typedef struct {              /* Generic matrix definition    */
	int n;                /* Number of rows in matrix     */
	int m;                /* Number of columns in matrix  */
	double a[MAXN][MAXN]; /* Matrix of double-precision numbers.        */
	char message[80];     /* Any string you want - use for error report */
	int err;   /* Report existence of error condition: 0=ok, 1=error
		      Please see the comment for "err" in the definition of
		      the vector structure, above.                          */
} matrix;



matrix outmat;
double pi=3.1415926535898;


/*------------------------------------*/
/*  For returning a result from the one-variable nonlinear rootsolver. */
/*  This structure is used only for nsolve.                            */
typedef struct {
	double soln;     /*  The solution to the nonlinear function  */
	int iterations;  /*  How many iterations before the defined */
			 /*  precision (eps) was reached.           */
	int err;         /*  Error condition - 0 if OK, 1 if        */
			 /*  convergence did not occur within the   */
			 /*  maximum allowed iterations (imax)      */
} nroot;
/*-----------------------------------*/
/*  Simplified format for storing each rinex observation  */
typedef struct {
	double t;       /*  Time of observation, according to rinex file */
	double prs[4];  /*  4 "best" pseudoranges from an observation    */
} rinex;
/*-----------------------------------*/
typedef struct {  /*  Ephemeris structure definition   */
	int prn;        /* Satellite ID number.                             */
	int wntoe;      /* Week number from original GPS launch date.       */
	long int toe;	/* Time of ephemeris - t since week crossover       */
	double a;       /* Semi-major axis of sat orbit. (NB: Not SQRT(A)). */
	double e;       /* Eccentricity of sat orbit.                       */
	double m0;      /* Mean anomaly at reference time.                  */
	double omegadt; /* Rate of right ascension.                         */
	double omega0;  /* Longitude or asc node of orbit plane (wkly epoch)*/
	double sinw;    /* Sine of argument of perigee.                     */
	double cosw;    /* Cosine of argument of perigee.                   */
	double n;       /* Mean motion. (NB: Not dN, mean motion difference)*/
	double i0;      /* Inclination angle at reference time.             */
	double idot;    /* Rate of inclination angle.                       */
	double crc; 	/* Ampl of cos harmonic corrn term to orbit radius. */
	double crs;     /* Ampl of sin harmonic corrn term to orbit radius. */
	double cuc;	/* Ampl of cos harmonic corrn term to arg of lat.   */
	double cus;	/* Ampl of sin harmonic corrn term to arg of lat.   */
	double cic;	/* Ampl of cos harmonic corrn term to angle incl.   */
	double cis;	/* Ampl of sin harmonic corrn term to angle incl.   */
	double af0; 	/* Polynomial correction term a0 for sat clock.     */
	double af1;	/* Polynomial correction term a1 for sat clock.     */
	double af2;	/* Polynomial correction term a2 for sat clock.     */
	double toc; 	/* Time of capture (toe).                           */
	double ura;	/*   */
	int aode;	/* Age of ephemeris data (s). (Max 1 or 2 hours).   */

	} ephemeris;
/*---------------------------------------------------------------------*/
typedef struct {
   char rtype[40];  /*  The receiver type, eg. Leica, Garmin, Magnavox  */
   int  rtypen;     /*  The receiver type, expressed as a number. eg. Leica is 1  */
   int colums;      /*  Number of columns of data in the obs file   */
   int ntypes;      /*  Number of different types of data (could be less than
			the number of columns - eg. 4/6 in Leica).  */
   int gindex[10];  /*  Index numbers for the type of data (eg. C1, L1 etc) */
		    /*  The textual description of this data type can be
			retrieved simply by using this index in the globally
			defined text array *description[]   */
} infotype;
/*  The purpose of this structure is to assign one variable of infotype,
    say info1 and info2, to each obs file. This info for the obs file
    is then used later, to read the obs data correctly. The function
    datypes() fills info1 and info2 with data. Later, the structure can
    be expanded, along with datypes(), to give more information about
    the obs files.
*/
/*---------------------------------------------------------------------*/

vector avecvec(vector v1, vector v2);  /* Add 2 vectors                    */
vector svecvec(vector v1, vector v2);  /* Subtract 2 vectors               */
vector mvecvec(vector v1, vector v2);  /* Multiply elements of 2 vectors.  */
double vecvec(vector v1, vector v2);  /* Dot product of 2 vectors.         */
vector dvecvec(vector v1, vector v2);  /* Divide elements of 2 vectors     */
double mag(vector v1);  /*  Find the geometrical length of a vector        */
double frange(vector v1, vector v2); /*  Distance between 2 rect. co-ords  */
vector matvec(matrix inmat, vector v1); /* Multiply matrix & vector        */
matrix matmat(matrix *m1, matrix *m2); /* Multiply two matrices.           */
double angle(double x, double y);  /*  Find the angle of 2-d vector        */
matrix matinv(matrix inmat); /*  Invert a matrix                           */
void factor(daryp w, int n, double *d, int *ipivot, int *iflag);/* matinv's*/
void subst(daryp w, int *ipivot, double *b, int n, double *x);  /* matinv's*/
double amax(double x1, double x2);  /*  Used by matinv()                   */
double abs1(double x);               /* Used by matinv()                   */
void matdisp(matrix mm);  /*  Display contents of a matrix - for debugging */
nroot nsolve(double x0, double mk, double e); /* Nonlinear rootsolver      */
double nfunc(double x, double mk, double e);  /* Nsolve's function         */
int vecdisp(vector *anyv); /* Display contents of a vector - for debugging */
int substr(char *si, char *so, int start, int len); /*  Extract a substring*/

/*-------------------------------------------------------------------------*/
vector avecvec(vector v1, vector v2)
/* Add two vectors */
{

int i;
vector vx;

	vx.err=0;
	if ((v1.n)!=(v2.n)) vx.err=1; /* Make sure vectors are same length. */
	if ((v1.n>MAXN)||(v1.n<0)) vx.err=1;  /* Check bounds.              */
	if (vx.err==1) { vx.n=0; return(vx); } /* Quit if dimensions wrong. */
	vx.n=v1.n; /* Set output vector length = input vector length.       */
	for (i=0;i<vx.n;i++) /* Subtract over the length of the input vecs. */
		vx.a[i]=v1.a[i]+v2.a[i]; /* Perform the addition.           */

	return(vx);
}
/*-------------------------------------------------------------------*/
vector svecvec(vector v1, vector v2)
/* Subtract two vectors.  */
{
int i;
vector vx;  /* Set up vx to return the result.  */

	vx.err=0;
	if ((v1.n)!=(v2.n)) vx.err=1; /* Make sure vectors are same length. */
	if ((v1.n>MAXN)||(v1.n<0)) vx.err=1; /*  Check bounds.              */
	if (vx.err==1) { vx.n=0; return(vx); } /* Quit if dimensions wrong. */
	vx.n=v1.n; /* Set output vector length = input vector length.       */
	for (i=0;i<vx.n;i++) /* Subtract over the length of the input vecs. */
		vx.a[i]=v1.a[i]-v2.a[i]; /* Perform the subtraction.        */

	return(vx);
}
/*-------------------------------------------------------------------*/
vector mvecvec(vector v1, vector v2)
/* Multiply two vectors (element-by-element multiplication).   */
{
int i;
vector vx;   /*  Set up vx to return the result. */

	vx.err=0;
	if ((v1.n)!=(v2.n)) vx.err=1; /* Make sure vectors are same length. */
	if ((v1.n>MAXN)||(v1.n<0)) vx.err=1; /* Check bounds.               */
	if (vx.err==1) { vx.n=0; return(vx); } /* Quit if dimensions wrong. */

	vx.n=v1.n; /* Set length of output = length of input vectors.       */
	for (i=0;i<vx.n;i++) /* Perform the calculation over length of vecs.*/
		vx.a[i]=v1.a[i]*v2.a[i]; /*  Multiply the two elements.     */

	return(vx);
}
/*-------------------------------------------------------------------*/
double vecvec(vector v1, vector v2)
/* Multiply two vectors (find their dot product).   */
{
int i, N, err;
double x;   /*  Set up x to return the result. */

	err=0;
	if ((v1.n)!=(v2.n)) err=1; /* Make sure vectors are same length. */
	if ((v1.n>MAXN)||(v1.n<0)) err=1; /* Check bounds.               */
	if (err==1) { return(0); } /* Quit if dimensions wrong. */

/*  Error reporting is a bit dodgey here, because the returned value
    is a double, not a vector. At the moment, the only way of telling
    that an error ocurred is when the returned value is 0.  */

        N=v1.n;
	x=0.0;
	for (i=0;i<N;i++) /* Perform the calculation over length of vecs.*/
		x += v1.a[i]*v2.a[i]; /*  Multiply the two elements.     */

	return(x);
}
/*-------------------------------------------------------------------*/
/*  Could put cross-product, xvecvec() in here. */
/*-------------------------------------------------------------------*/
vector dvecvec(vector v1, vector v2)
/* Divide vector length MAXN by vector length MAXN  */
/*  (element-by-element division)   */
{
int i;
vector vx;  /*  Set up vx to return the result.  */

	vx.err=0;
	if ((v1.n)!=(v2.n)) vx.err=1; /*  Check that vectors are same size. */
	if ((v1.n>MAXN)||(v1.n<0)) vx.err=1;   /* Do bounds check.          */
	if (vx.err==1) { vx.n=0; return(vx); } /* Quit if dimensions wrong  */

	vx.n=v1.n; /* Set length of output vector = length of input vector. */
	for (i=0;i<vx.n;i++) {
		if (v2.a[i]==0.0) {  /*  If any component of the divisor    */
			vx.n=0;      /*  is zero, then quit, with the error */
			vx.err = 1;  /*  flag set.                          */
			return(vx);
		}
		vx.a[i]=v1.a[i]/v2.a[i];  /* Perform the division. */
	}
	return(vx);
}
/*-------------------------------------------------------------------*/
double mag(vector v1)
/*  Find the geometrical length of a vector, according to:
      len = sqrt(a0^2 + a1^2 + a3^2 + ... + aN^2)
*/
{
int i;
double x;  /*  Set up x to return the result.  */

	x=0;    /*  Reset the sum of squared components.  */
	for (i=0;i<v1.n;i++) {           /*  Sum over size of vector.       */
		x += v1.a[i]*v1.a[i];    /*  Could have used mvecvec here.  */
	}
	x = sqrt(x);
	return(x);

/*  pow(v1.a[i],2.0); could be used to find the square of a number,
    but the base could be negative - so use pow for non-integer exponents.
*/

}
/*-------------------------------------------------------------------*/
double frange(vector v1, vector v2)
/*  Finds the geometrical distance between two vectors v1 and v2. The
    vectors have to be in rectangular cartesian co-ordinates.          */
{
vector vx;  /*  Set up a vector to store result of the vector subtraction. */
double x;   /*  Set up x to return the result.   */

	vx=svecvec(v1,v2);  /*  Subtract the 2 vectors or coordinates.  */
	x=mag(vx);  /*  Calculate the size of the difference vector.    */
	return(x);  /*  Return the size as the "range" result.          */

/*  svecvec will do the size check: vx.err=1 if wrong. But need to create
    a type (dsoln ?) to return the mag and the error report ? - (see nroot)
*/

}
/*-------------------------------------------------------------------*/
vector matvec(matrix inmat, vector v1)
/*  Multiplies a matrix by a vector. The number of rows in the vector
    must equal the number of columns in the matrix. The returned value
    is a vector of length equal to the number of rows in the matrix.
*/
{
int i,j;
vector vx;  /*  Setup vx, which will be used to return the result.  */

	/*  Assign all zeroes to the elements of vx.            */
	for (i=0;i<MAXN;i++)
		vx.a[i]=0.0;
	vx.err=0;   /*  Start with no error condition in the return vector. */
	     /*  Check the bounds of the matrix:  */
	if ((inmat.n>MAXN)||(inmat.n<0)||(inmat.m>MAXN)||(inmat.m<0)) vx.err=1;
	     /*  Check the bounds of the vector:  */
	if ((v1.n>MAXN)||(v1.n<0)) vx.err=1;
	     /*  Check that matrix and vector dimensions agree:  */
	if ((v1.n)!=(inmat.m)) vx.err=1;
	if (vx.err==1) { vx.n=0; return(vx); } /*  Quit if dimensions wrong */
	vx.n=inmat.n;  /* Size of return vector = no. of rows in matrix.    */
	for (i=0;i<inmat.n;i++) {     /*  This is the calculation proper:  */
		for (j=0;j<inmat.m;j++)
			vx.a[i]=vx.a[i]+inmat.a[i][j]*v1.a[j];
	}
	return(vx);
}
/*-------------------------------------------------------------------*/
matrix matmat(matrix *m1, matrix *m2)
/*  23/2/93.  Multiplies two matrices, m1 * m2. The number of columns
    in m1 must equal the number of rows in m2.      */
{
int i,j,k,mcount;

   outmat.err=0;

   /*  Check the bounds of both matrices:  */
   if ((m1->n>MAXN)||(m1->n<0)||(m1->m>MAXN)||(m1->m<0)) outmat.err=1;
   if ((m2->n>MAXN)||(m2->n<0)||(m2->m>MAXN)||(m2->m<0)) outmat.err=1;
   /*  Check agreement between matrices, on the critical dimensions:  */
   if  ((m1->m)!=(m2->n))  outmat.err=1;
   mcount=m1->m;
   if (outmat.err==1) {
      strcpy(outmat.message,"Something went wrong.");
      outmat.n=0;
      outmat.m=0;
      return(outmat);  /*  Quit if something went wrong.   */
   }

   /*  Define the bounds of outmat:  */
   outmat.n=m1->n;
   outmat.m=m2->m;

   /*  Assign all zeroes to the elements of outmat.    */
   for (i=0;i<outmat.n;i++)  {
      for (j=0;j<outmat.m;j++)
	outmat.a[i][j]=0.0;
   }

   /*  The calculation proper:    */
   for (i=0;i<outmat.n;i++) {
      for (j=0;j<outmat.m;j++) {
	 for (k=0;k<mcount;k++) {
	    outmat.a[i][j] += m1->a[i][k] * m2->a[k][j] ;
	 }
      }
   }
   return(outmat);
}
/*-------------------------------------------------------------------*/
double angle(double x, double y)
{
/*  Finds the angle of the vector specified by (x,y), in the correct
    quadrant. The output is in radians.

    In C++, the complex.h module and arg can be used to provide this
    function. In C, the atan2 function can be used to give the angle
    in the correct quadrant. The only reasons for creating this function
    are:
	(i)  Better readability ('angle' is less obscure than 'atan2')
	(ii) To swap x and y: in finding theta, the expression
	     angle(x,y) is conventional - ordinate first.
*/
double theta;

	theta = atan2(y,x);
	return(theta);
}
/*--------------------------------------------------------------------*/
matrix matinv(matrix inmat)
{
/*  Matrix inversion routine.
    The credit for this routine and the ones below (subst and factor) go
    to:
		Samuel D Conte & Carl de Boor
	   "Elementary Numerical Analysis, an algorithmic approach"
	   Third Edition, McGraw Hill, 1981, ISBN 0-07-066228-2
		     pg. 164 - 167.


    Notes:

    (1)  Matrix dimensions have been made 1 bigger than they should be,
    for the benefit of the Fortran program.

    (2)  Functions "factor" and "subst" belong to the "matinv" matrix
    inversion function. All of these functions have been translated
    directly from the original Fortran program by Conte and de Boor (see
    comments in "matinv" above). The sparse commenting of the original
    program is reflected in these functions.

*/

int i, ibeg, *iflag, ipivot[MAXN], j, n;
double (*a)[MAXN][MAXN], (*ainv)[MAXN][MAXN], b[MAXN];
matrix mx;

/*  When fixed up, use ^QA to replace outmat with mx, in
    this function only.
*/

	n=inmat.n;

	outmat.n = n;  /*  Set up matrix for return - dimensions */
	outmat.m = n;
	outmat.err=0;

	iflag = malloc(sizeof(int));
	a = (dary *)malloc(sizeof(dary));
	ainv = (dary *)malloc(sizeof(dary));


/*  Pointers necessary so that the called functions can modify these
    data.                                                              */


	if ((inmat.n==!inmat.m)) {
	  strcpy(outmat.message,"Matrix has to be square to be invertible.");
	  outmat.err=1;
	  free(iflag);
	  free(a);
	  free(ainv);
	  return(outmat);
	}
	if ((n<1)||(n>(MAXN-1))) {
	   strcpy(outmat.message,"Matrix size is out of acceptable bounds.");
	   outmat.err=1;
	   free(iflag);
	   free(a);
	   free(ainv);
	   return(outmat);
	}
	for (i=0;i<n;i++) {    /* Convert to format of FORTRAN program */
		for (j=0;j<n;j++) {
			(*a)[i+1][j+1]=inmat.a[i][j];
		}
	}

	factor(a,n,&b[0],&ipivot[0],iflag);

	if (*iflag==0) {
		strcpy(outmat.message,"Matrix is singular");
		outmat.err=1;
		free(iflag);
		free(a);
		free(ainv);
		return(outmat);
	}
	for (i=1;i<n+1;i++)
		b[i]=0.0;
	ibeg=1;
	for (j=1;j<n+1;j++) {
		b[j]=1.0;
		subst(a,&ipivot[0],&b[0],n,&(*ainv)[j][0]);
		b[j]=0.0;
		ibeg=ibeg+n;
	}
	for (i=0;i<n;i++) {    /* Convert from format of FORTRAN program */
		for (j=0;j<n;j++) {
			outmat.a[j][i]=(*ainv)[i+1][j+1];
		}
	}
	strcpy(outmat.message,"Matrix inversion successful.");

	/* Free all pointer allocations before leaving a function -
	   to prevent memory crash  */

	free(iflag);
	free(a);
	free(ainv);
	return(outmat);
/* Calc condition number here and return it ? (See de Boor, pg. 175..177) */
/*  ^^^  */
}
/*-------------------------------------------------------------------*/
/* Outputs: w, ipivot, iflag */
void factor(daryp w, int n, double *d, int *ipivot, int *iflag)
/*  See comments in function "matinv", above.                       */
{
double rowmax, colmax, awikod, temp, ratio;
int i,j,k,istar;

	*iflag=1;
	for (i=1;i<(n+1);i++){
		ipivot[i]=i;
		rowmax=0.0;
		for (j=1;j<(n+1);j++){
			rowmax=amax(rowmax,abs1((*w)[i][j]));
		}
		if (rowmax==0.0) {
			*iflag=0;
			rowmax=1;
		}
		d[i]=rowmax;
	}
	if (n<=1) return;
	for (k=1;k<n;k++) {      /* This n here is correct (1 to n-1) */
		colmax=abs1((*w)[k][k])/d[k];
		istar=k;
		for (i=k+1;i<n+1;i++) {
			awikod=abs1((*w)[i][k])/d[i];
			if (awikod>colmax) {
				colmax=awikod;
				istar=i;
			}
		}
		if (colmax==0.0) {
			*iflag=0;
		}
		else {
			if (istar>k) {
				*iflag=-*iflag;
				i=ipivot[istar];
				ipivot[istar]=ipivot[k];
				ipivot[k]=i;
				temp=d[istar];
				d[istar]=d[k];
				d[k]=temp;
				for (j=1;j<n+1;j++) {
					temp=(*w)[istar][j];
					(*w)[istar][j]=(*w)[k][j];
					(*w)[k][j]=temp;
				}
			}
			for (i=k+1;i<n+1;i++) {
				(*w)[i][k]=(*w)[i][k]/(*w)[k][k];
				ratio=(*w)[i][k];
				for (j=k+1;j<n+1;j++)
					(*w)[i][j]=(*w)[i][j]-ratio*(*w)[k][j];
			}
		}
	}
	if ((*w)[n][n]==0.0)
		*iflag=0;
	return;
}
/*-------------------------------------------------------------------*/
/*  Outputs: x   */
void subst(daryp w, int *ipivot, double *b, int n, double *x)
/*  See comments in function "matinv", above.                         */
{
int ip, i, j;
double sum;

	if (n<= 1)
	{
		x[1]=b[1]/(*w)[1][1];
		return;
	}
	ip=ipivot[1];
	x[1]=b[ip];
	for (i=2;i<n+1;i++)
	{
		sum=0.0;
		for (j=1;j<i;j++)
			sum=(*w)[i][j]*x[j]+sum;
		ip=ipivot[i];
		x[i]=b[ip]-sum;
	}
	x[n]=x[n]/(*w)[n][n];
	for (i=n-1;i>0;i--)
	{
		sum=0.0;
		for (j=i+1;j<n+1;j++)
			sum=(*w)[i][j]*x[j]+sum;
		x[i]=(x[i]-sum)/(*w)[i][i];
	}
}
/*-------------------------------------------------------------------*/
double amax(double x1, double x2)
/*  Finds and returns the larger of two double-precision numbers x1 and x2 */
{
	if (x1>x2)
		return(x1);
	else
		return(x2);
}
/*-------------------------------------------------------------------*/
double abs1(double x)
/* Returns the absolute value of a double-precision number. (C's
   abs function operates on integers only).    */
{
	if (x<0)
		return(-x);
	else
		return(x);
}
/*-------------------------------------------------------------------*/
void matdisp(matrix mm)
/*  For the purpose of debugging only. Displays a matrix.
*/
{
int i,j;

	printf("\n\nMatrix has number of rows = %d",mm.n);
	printf("\n\nMatrix has number of columns = %d",mm.m);
	for (i=0;i<mm.n;i++) {
		for (j=0;j<mm.m;j++)
			printf("\nmatrix.a[%d][%d] = %f",i,j,mm.a[i][j]);
	}
}
/*-----------------------------------------------------------------------*/
nroot nsolve(double x0, double mk, double e)
/*  Solves a nonlinear function of one variable specified in nfunc,
    using Newton's method for one unknown.
    The nonlinear function to be solved (of the form f(x) = 0) is
    inserted into the nfunc space: input variable is x and nfunc must
    return f(x). Then, to solve the equation, nsolve is called with
    an initial approximation x0. Convergence is checked
    (precision specified by eps , below) and the function is terminated
    after sufficient precision, or a specified maximum number
    of iterations (imax) is reached. The returned value is of the type
    nroot (see typedef for nroot), containing the solution, number
    of iterations that occurred, and an error flag.

    mk and e are additional parameters passed, specifically for the
    solution of kepler's equation in the gps project.
*/
{
nroot root;   /*  See typedef of nroot - root is used to return solution   */
int i,j;
double x1,fx0,dfdx0,aeps;   /*  x1 is the output of one iteration (ie. an
				improved estimate. fx0 is f(x0).
				dfdx0 is d(f(x0))/dx and aeps is the
				absolute error for testing convergence.    */

int imax=20;       /*  Maximum number of iterations allowed    */
double eps=1e-13;  /*  Roughly, the exponent is the number of  */
		   /*  significant figures achieved. ie. 13, in this case. */

	root.err=1;
	aeps=x0*eps/10;    /*  Determine the absolute acceptable error  */
	for (i=0;i<imax;i++) {
		fx0=nfunc(x0,mk,e);                    /*  Find f(x0)   */
		dfdx0=(nfunc(x0+aeps,mk,e)-fx0)/aeps;  /*  Find f'(x0)  */
		x1=x0-fx0/dfdx0;   /*  Better approx. = x0-f(x0)/f'(x0) */
		if (abs1(x1-x0)<10*aeps) {    /*  Check for convergence */
			root.err=0; /* Convergence occurred within imax */
			break;   /*  Stop looping if accurate enough  */
		}
		x0=x1;
	}
	root.iterations=i+1;  /*  Return the number of iterations taken. */
	root.soln=x1;         /*  Return the solution.  */
	return(root);
}
/*-------------------------------------------------------------------*/
double nfunc(double x, double mk, double e)
/*  For use only by nsolve, the nonlinear rootsolver.
    The nonlinear function of one variable to be solved for is
    inserted here: the solution will be the solution to:
	  nfunc(x)  =  f(x)  =  0

    Example of a function for which method doesn't converge (it
    has no real solution): return(sin(x)+2);  */
{

return(x-e*sin(x)-mk);        /*  Kepler's equation  */

}
/*-------------------------------------------------------------------*/
int vecdisp(vector *anyv)
/*  Displays the contents of the vector *anyv, for debugging purposes. */
/*  Done 21/11/92  */
{
int i;

    printf("\n\nThe error term for vector is: ");
    if (anyv->err==0) printf("OK"); else printf("ERROR");
    if (anyv->n==0) {
	printf("\nVector contains NO elements."); }
    else {
	printf("\nElements of vector are:");
	for (i=0;i<anyv->n;i++) printf("\n    %f",anyv->a[i]);
    }
    return(1);
}
/*-----------------------------------------------------------------------*/
double trace(matrix *anym)
/*  23/2/93.   Computes the trace of a matrix. (Sum of the diagonal
    elements). The matrix should be square. The matrix dimensions
    should previously have been defined (in elements n and m).

*/
{
int i,j;
double result;

   result=0;
   anym->err=0;
   if ((anym->n)!=(anym->m)) anym->err=1;  /* Make sure its square */
   if ((anym->n>MAXN)||(anym->n<0)) anym->err=1;  /* Check bounds.              */
   if (anym->err==1) { return(0); } /* Quit if something's wrong. */
   for (i=0;i<anym->n;i++) {
      result += anym->a[i][i];  /*  Add diagonal elements.  */
   }
   return(result);
}
/*-----------------------------------------------------------------------*/
int transpose(matrix *dest, matrix *src)
/*  23/2/93.  Returns the transpose of matrix *src in matrix *dest.
    Returns 0 if something went wrong.
*/
{
int i,j;

   dest->err=0;
   if ((src->n>MAXN)||(src->n<0)) dest->err=1;  /* Check bounds.    */
   if ((src->m>MAXN)||(src->m<0)) dest->err=1;  /* Check bounds.    */
   if (dest->err==1) {
      strcpy(dest->message, "Define the size of your matrix, stupid.");
      return(0);
   } /* Quit if something's wrong. */
   strcpy(dest->message,"Done.");
   dest->n=src->m;
   dest->m=src->n;
   for (i=0;i<src->n;i++) {
      for (j=0;j<src->m;j++) {
	 dest->a[j][i]=src->a[i][j];
      }
   }
   return(1);
}
/*-----------------------------------------------------------------------*/
int substr(char *si, char *so, int start, int len)
/*   16/5/93.  Written because the stupid string library doen't
     have a substring function in it.

      *so must be initialised with a string of length greater than
      that which will be written to it.
*/
{
int i,j;
   for (i=start;i<(start+len);i++) {
      if ((i >= 0)&&(i <= strlen(si))&&((i-start) <= (strlen(so)-1)))
	 so[i-start]=si[i];
   }
   so[len]='\0';
   return(1);
}
/*-----------------------------------------------------------------------*/

