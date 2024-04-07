/*     D‚finitions diverses pour les modules d'analyse num‚rique */


/*     Constants defines */

#ifndef __ANUM_H
#define __ANUM_H

#include <math.h>

#ifdef __TURBOC__
#ifdef __PASCAL__
#error Pascal calling conventions are not yet supported
#endif

#ifndef __LARGE__
#error Compilation should occur *ONLY* in LARGE Model
#endif
#endif



		/*     Constants defines */



                /*      Error codes returned by functions       */

#define ENOERROR	  0   /* No error 				*/
#define ETOLLE0          -1   /* Tolerance lower than or equal to 0	*/
#define EOVERMAXITER     -2   /* Attempted more iterations than allowed */
#define ENULLSLOPE       -3   /* Function derivative is 0		*/
#define ENOPARAB3        -4   /* No parabola fitting three given points	*/
#define WCPLXROOTS       -5   /* Possible complex roots			*/
#define EYSAMESIGN       -6   /* f(x1) and f(x2) same sign		*/
#define EMAXITERLT0      -7   /* Max nubr of allowed iterations l.e. 0	*/
#define ENOPXINTERSECT   -8   /* No parabola intersect with x axis	*/
#define EDIMLT0          -9   /* Array dimension lower than 0		*/
#define EMATSING       	-10   /* Singular matrix			*/
#define EDIMLE0        	-11   /* Array dimension lower than-equal to 0  */
#define ENOFITPARAB   	-12   /*            *** RESERVED ***            */
#define E0DIVIDE  	-13   /* Attempted 0 divide			*/
#define EDEGLT2		-14   /* Degree lower than or equal to 2	*/
#define ENOWCORE	-15   /* Not enough working core		*/
#define EDEGLE0		-16   /* Degree lower than or equal to 0	*/
#define ENUMINTLE0	-17   /* Number of intervals l.e. 0		*/
#define ELOGLE0		-18   /* Attempted compute log a negative real	*/
#define ENDPTSLT2	-19   /* One or less data points given (lsq)    */
#define ENTRMLT1	-20   /* Less than one term asked      		*/
#define ENDTPTLTNTRMS	-21   /* Less data points than terms asked (lsq)*/
#define ENOSOL		-22   /* No solution				*/
#define E0MATDIAG	-23   /* Matrix diagonal contains 0s		*/
#define WNODIAGDOM	-24   /* Not a diagonal dominant matrix		*/
#define EMATNOTSYM	-25   /* Not a symmetric matrix         	*/
#define ELBDGEUBD	-26   /* Lower bound g.e. than upper bound	*/
#define ETOLNR		-27   /* Tolerance not reached			*/
#define ENTRMGTNINT	-28   /* Less intervals than asked terms	*/
#define ENOTLINDIFEQ	-29   /* Not a linear differential equation	*/


/* Public types */


typedef struct complex COMPLEX;

typedef enum {expolsq, fourierlsq, loglsq, polylsq, powerlsq, xpowerlsq}
		lsqfit;

typedef enum {FALSE, TRUE} bool;


/*	Now the provided routines     */
/*	C-C++ compatibility hack      */

#ifdef __cplusplus
extern "C" {
#endif


/*
 **********************************************************************
 **
 **
 **	Reserved functions
 **	******************
 **
 **
 **
  */


  bool cdecl ctestroot(COMPLEX, COMPLEX, COMPLEX, double);

  void cdecl checkslope(double, int *);

  bool cdecl iseq0(double);

  bool cdecl isinfinite(double);

  void cdecl rowmultadd(int ,
		       double *,
		       double ,
		       int , int );

  bool cdecl rtestroot(double, double, double, double);

  bool cdecl islt0(double);

  bool cdecl isle0(double);

  bool cdecl isgt0(double);

  bool cdecl isge0(double);




/*
 **********************************************************************
 **
 **
 **	Functions in module FCOMPLEX
 **	****************************
 **
 **
 **
  */




/*	void conjugue(COMPLEX z1, COMPLEX *z2)
 *
 *	This function calculates a-ib if z=a+ib
 *
 *	Input parameters:
 *	----------------
 *	z1 : a + ib
 *
 *	Output parameters:
 *	-----------------
 *	z2 : a - ib
 */
  void cdecl conjugue(COMPLEX, COMPLEX *);






/*	double modulus(COMPLEX z)
 *
 *	This function calculates the modulus of a complex number.
 *
 *	Input parameters:
 *	----------------
 *	z : complex operand.
 *
 *	Returns: value of the modulus
 */
  double cdecl modulus(COMPLEX);





/*	double carg(COMPLEX z)
 *
 *
 *	This function  returns  the  characteristic  angle  of  a
 *	complex number.
 *
 *	Input parameters:
 *	----------------
 *	z : complex number
 *
 *
 *	Returns : the value of the angle.
 *
 *
 * 	Notes : carg(0) = PI/2 (convention)
 */
  double cdecl carg(COMPLEX);



/*
 *	void cadd(COMPLEX z1, COMPLEX z2, COMPLEX *z3)
 *
 *	This function performs the addition of two complex numbers.
 *
 *	Input parameters:
 *	----------------
 *	z1 : First operand.
 *	z2 : Second operand.
 *
 *	Output parameters:
 *	-----------------
 *	z3 : = z1+z2
 */
  void cdecl cadd(COMPLEX, COMPLEX, COMPLEX *);




/*	void csub(COMPLEX z1, COMPLEX z2, COMPLEX *z3)
 *
 *	This function performs complex substraction.
 *
 *	Input parameters:
 *	----------------
 *	z1 : First complex operand.
 *	z2 : Second complex operand.
 *
 *	Output parameters:
 *	-----------------
 *	z3 : = z1 - z3.
 */
  void cdecl csub(COMPLEX, COMPLEX, COMPLEX *);



/*	void cmult(COMPLEX z1, COMPLEX z2,
 *		COMPLEX *z3)
 *
 *	This function performs complex multiplication.
 *
 *	Input parameters:
 *	----------------
 *	z1 : First complex operand.
 *	z2 : Second complex operand.
 *
 *	Output parameters:
 *	-----------------
 *	z3 : = z2 x z3
 */
  void cdecl cmult(COMPLEX, COMPLEX, COMPLEX *);




/*	void rmult(double lambda, COMPLEX *z)
 *
 *	This function performs multiplication. of a complex number
 *	by a real.
 *
 *	Input parameters:
 *	----------------
 *	lambda	: Real multiplier.
 *	z	: complex operand.
 *
 *	Output parameters:
 *	-----------------
 *	z	: = lambda * z
 */
  void cdecl rmult(double, COMPLEX *);





/*	void cdiv(COMPLEX z1, COMPLEX z2,
 *		  COMPLEX *z3, int *errcode)
 *
 *	This functions performs complex division.
 *
 *	Input parameters:
 *	----------------
 *	z1 : complex number to be divided.
 *	z2 : complex number to divide by.
 *
 *	Output parameters:
 *	-----------------
 *	z3	: = z1/z3.
 *	errcode : possible error code.
 *
 *	Possible error codes:
 *	--------------------
 *	E0DIVIDE
 */
  void cdecl cdiv(COMPLEX, COMPLEX, COMPLEX *, int *);





/*  void cpow(COMPLEX z1, int n, COMPLEX *z2)
 *
 *	This function performs a complex power evelation.
 *
 *	Input parameters:
 *	----------------
 *	z1 : complex to elevate
 *	n : value of the power
 *
 *	Output parameters:
 *	-----------------
 *	z2 : z1**n
 */
  void cdecl cpow(COMPLEX, int, COMPLEX *);




/*	void csqrt(COMPLEX z1, COMPLEX *z2)
 *
 *	This function calculates a complex square root.
 *
 *	Input Parameters:
 *	----------------
 *	z1 : complex operand
 *
 *	Output parameters:
 *	-----------------
 *	z2 : = z1**(1/2)
 */
  void cdecl csqrt(COMPLEX, COMPLEX *);




/*	void rassign(double lambda, COMPLEX *z)
 *
 *	This function assigns a real value to a complex number.
 *
 *	Input Parameters:
 *	----------------
 *	lambda	: value to assign.
 *
 *	Output parameters:
 *	-----------------
 *	z	: = labda + 0i.
 */
  void cdecl rassign(double, COMPLEX *);




/*	void cpoly(int degree,
 *		   COMPLEX *poly,
 *		   COMPLEX z,
 *		   COMPLEX *y, *yderiv, COMPLEX *yderiv2,
 *		   int *errcode)
 *
 *	This  function   computes  the   values  of  the  complex
 *	polynomial poly, and of its first and second derivatives at a
 *	given point.
 *
 *	Input parameters:
 *	----------------
 *	degree 	: degree of the polynomial
 *	poly 	: pointer to the coefficients of the polynomial
 *	z 	: value of the point at which omputation must be performed
 *
 *	Output parameters:
 *	-----------------
 *	y 	: = poly(z)
 *	yderiv 	: = poly'(z)
 *	yderiv2 : = poly"(z)
 *	errcode : possible error code.
 *
 *	Possible error codes:
 *	--------------------
 *	ENOWCORE
 */
  void cpoly(int,
	     COMPLEX *,
	     COMPLEX,
	     COMPLEX *, COMPLEX *, COMPLEX *,
	     int *);



/*	void cpoly2_solve(COMPLEX *poly,
 *		COMPLEX *root1, COMPLEX *root2)
 *
 *	This function  solves a  second degree complex polynomial
 *	equation.
 *
 *	Input parameters:
 *	----------------
 *	poly : complex second degree polynome.
 *
 *	Output parameters:
 *	-----------------
 *	root1 : first root.
 *	root2 : second root.
 */
  void cpoly2_solve(COMPLEX *,
		COMPLEX *, COMPLEX *);




/*	void cpoly_one_root(int degree,
 *		COMPLEX *poly,
 *		COMPLEX z0,
 *		double tol,
 *		int maxiter,
 *		COMPLEX *zroot, COMPLEX *yroot,
 *		int *iter, int *errcode)
 *
 *	This  function   approximates  a   root  of   a   complex
 *	polynomial.
 *
 *	Input parameters:
 *	----------------
 *	degree 	: degree of the polynomial
 *	poly 	: pointer to the coefficients of the polynomial
 *	z0 	: value of the root guess
 *	tol 	: tolerance used for convergence test.
 *	maxiter : maximum number of allowed iterations
 *
 *	Output parameters:
 *	-----------------
 *	zroot 	: value of the root if found
 *	yroot 	: = poly(zroot)
 *	iter    : actual  number  of  iterations  the  algorithm  went
 *		  through.
 *	errcode : possible error code.
 *
 *	Possible error codes:
 *	--------------------
 *	ENOWCORE
 *	E0DIVIDE
 *	EOVERMAXITER
 */
  void cpoly_one_root(int,
		COMPLEX *,
		COMPLEX,
		double,
		int,
		COMPLEX *, COMPLEX *,
		int *, int *);




/*  void cpoly_reduce(int *degree, COMPLEX *poly, COMPLEX zroot,
 *	int *errcode)
 *
 *
 *	This function  reduces the degree of a complex polynomial
 *	by factoring out one of its given root.
 *
 *	Input parameters:
 *	----------------
 *	degree : degree of the polynomial
 *	poly   : pointer to the coefficients of the polynomial
 *	zroot  : value of the root
 *
 *	Output parameters:
 *	-----------------
 *	errcode : possible error code.
 *
 *	Possible error codes:
 *	--------------------
 *	ENOWCORE
 */
  void cpoly_reduce(int *,
	COMPLEX *,
	COMPLEX,
	int *);











/*
 **********************************************************************
 **
 **
 **
 **	Equation solving functions
 **	==========================
 **
 **
 **
 */


/*	 void newton_raphson(double x0, double tol,
 *		int maxiter,
 *		double *root, double *yroot, double *deriv,
 *		int *iter, int *errcode,
 *		double f(double t), double fprime(double t))
 *
 *	This routine  solves a  real equation  f(x)=0  using  the
 *	general Newton algorithm (more often called Newton-Raphson).
 *
 *	Input parameters:
 *	----------------
 *	x0 	: Approximate value of the solution.
 *	tol 	: Tolerance used for convergence tests.
 *	maxiter : Maximum number of allowed iterations.
 *	f	: function defining the equation.
 *	fprime	: first derivate of f.
 *
 *	Output parameters:
 *	-----------------
 *	roots 	: Value of the root, if found.
 *	yroot	: = f(root)
 *	deriv 	: = fprime(root).
 *	iter	:  Number of  iterations the  algorithm  actually  went
 *		   through.
 *	errcode : Error code.
 *
 *	Possible error codes:
 *	--------------------
 *	ETOLLE0
 *	EMAXITERLT0
 *	EOVERMAXITER
 *	ENULLSLOPE
 */
  void cdecl newton_raphson(double, double,
			   int,
			   double *, double *, double *,
			   int *, int *,
			   double (*)(double), double (*)(double));




/*
 *	void bisect(double lb, double ub, double tol,
 *		int maxiter,
 *		double *root, double *value,
 *		int *iter, int *errcode,
 *		double f(double))
 *
 *	This function uses a dichotomic algorithm to find one root
 *	of the equation f(x)=0 on a given interval.
 *
 *	Input parameters:
 *	----------------
 *	lb : lower bound of the search interval
 *	ub : upper bound of the search interval
 *	f  : function of the equation.
 *
 *	Output parameters:
 *	-----------------
 *	root    : value of the root, if found.
 *	value   : = f(root)
 *	iter    : number of iterations taken the algorithm
 *		  actually went through.
 *	errcode : possible error code.
 *
 *	Possible error codes:
 *	--------------------
 *	EYSAMESIGN
 *	ETOLLE0
 *	EMAXITERLT0
 *	EOVERMAXITER
 */
  void cdecl bisect(double, double, double,
		   int,
		   double *, double *,
		   int    *, int *,
		   double (*)(double));




/*	void secant(double x01, double x02, double tol,
 *		int maxiter,
 *		double *root, double *y,
 *		int *iter, int *errcode,
 *		double f(double t))
 *
 *	This function  solves a  real equation  f(x)=0 using  the
 *	secant algorithm.
 *
 *	Input parameters:
 *	----------------
 *	x01,x02	: Approximate values of the root.
 *	tol	: Tolerance used for convergence tests.
 *	maxiter : Maximum number of allowed iterations.
 *	f	: function defining the equation.
 *
 *	Output parameters:
 *	-----------------
 *	root	: Found root, if any.
 *	iter	: Actual number of iterations the algorithm went through.
 *	y	: = f(root).
 *	errcode	: Error code
 *
 *	Possible error codes:
 *	--------------------
 *	EYSAMESIGN
 *	EMAXITERLT0
 *	EOVERMAXITER
 *	ETOLLE0
 */
  void cdecl secant(double, double, double,
		   int,
		   double *, double *,
		   int *, int *,
		   double (*)(double));



/*	void newton_horner(int degree0,
 *		double *poly0,
 *		double x0, double tol,
 *		int maxiter,
 *		int *degree, int *nbroots,
 *		double *poly, double *roots, double *image,
 *		double *value, double *deriv,
 *		int *iter, int *errcode)
 *
 *	This function solves a real polynomial equation using the
 *	Newton-Horner algoritm.
 *
 *	Input parameters:
 *	----------------
 *	degree0 : Degree of the polynomial equation.
 *	poly0 : Polynome defining the equation.
 *	x0 : Approximate value of the solution.
 *	tol : Tolerance used for convergence tests.
 *	maxiter : Maximum number of allowed iterations.
 *
 *	Output parameters:
 *	-----------------
 *	degree 	: Degree of the resulting polynome after treatment.
 *	nbroots : Number of found roots
 *	poly  	: Resulting deflated polynome.
 *	roots 	: Array containing the values of the real parts of the
 *		  roots found so far.
 *	image 	: Array containing  the values of the imaginary parts
 *		  of the roots found so far.
 *	value 	: Array containing  the value  of poly0  at the found
 *		  roots.
 *	deriv 	: Array containing the value of the first derivate of
 *		  poly0 at the found roots.
 *	iter  	: Array  containing  the  number  of  iterations  the
 *		  algorithm actually went through for each found root.
 *	errcode : Error code.
 *
 *	Possible error codes:
 *	--------------------
 *	ETOLLE0
 *	EMAXITERLT0
 *	EDEGLE0
 *	EOVERMAXITER
 *	ENULLSLOPE
 */
  void cdecl newton_horner(int,
			  double *,
			  double, double,
			  int,
			  int *, int *,
			  double *, double *, double *,
			  double *, double *,
			  int *, int *);


/*	void muller(COMPLEX z0,
 *		double tol,
 *		int maxiter,
 *		COMPLEX *root, COMPLEX *yroot,
 *		int *iter, int *errcode,COMPLEX f(COMPLEX z))
 *
 *	This function  solves a  general complex  equation, using
 *	the Mller's algorithm.
 *
 *	Input parameters:
 *	----------------
 *	z0 	: initial approximate value of the root.
 *	tol 	: tolerance used for convergence tests.
 *	maxiter : maximum number of allowed iterations.
 *	f	: function to solve.
 *
 *	Output parameters:
 *	-----------------
 *	root	: value of the complex root, if found.
 *	yroot 	: = f(root).
 *	errcode : error code.
 *
 *	Possible error codes:
 *	--------------------
 *	EOVERMAXITER
 *	ETOLLE0
 *	EMAXITERLT0
 *	ENOINTERSECT
 *	ENOFITPARAB
 */
  void cdecl muller(COMPLEX ,
		   double ,
		   int ,
		   COMPLEX *, COMPLEX *,
		   int *, int *,COMPLEX (*)(COMPLEX));



/*	void laguerre (int degree,
 *		COMPLEX *poly,
 *		COMPLEX z0,
 *		double tol,
 *		int maxiter,
 *		int *nbroot,
 *		COMPLEX *roots,
 *		COMPLEX *yroots,
 *		int *errcode)
 *
 *	This function solves a complex polynome.
 *
 *	Input parameters:
 *	----------------
 *	degree 	: degree of the polynome.
 *	poly 	: polynome to solve.
 *	z0	: initial guess of an actual root.
 *	tol	: tolerance to use.
 *	maxiter : maximum number of allowed iterations.
 *
 *	Output parameters:
 *	-----------------
 *	nbroot	: number of found roots.
 *	roots	: array containing the found roots.
 *	yroots	: array containing  the value of the polynome at the
 *			found roots.
 *	errcode : error code.
 *
 *	Possible error codes:
 *	--------------------
 *	ENOWCORE
 *	EDEGLT2
 *	EMAXITERLT0
 *	EOVERMAXITER
 */
  void cdecl laguerre(int ,
		     COMPLEX *,
		     COMPLEX ,
		     double ,
		     int ,
		     int *,
		     COMPLEX *, COMPLEX *,
		     int *, int *);

/*	void steffensen(double x0, double tol,
 *		int maxiter,
 *		double *root, double *value,
 *		int *iter, int *errcode,
 *		double f(double t))
 *
 *	This  function   finds  the  root  of  f(x)=0  using  the
 *	steffensen algorithm.
 *
 *	Input parameters:
 *	----------------
 *	x0	: Approximate value of the root.
 *	tol	: tolerance to be used as stp criteria.
 *	f	: function to solve.
 *
 *	Output parameters:
 *	-----------------
 *	root	: value of the found root.
 *	value	: =f(root).
 *	iter	:  number of  iterations  the  algorith  actually  went
 *			through.
 *	errcode : error code.
 *
 *	Possible error codes:
 *	--------------------
 *	E0DIVIDE
 *	EOVERMAXITER
 */
  void cdecl steffensen(double, double ,
		       int ,
		       double *, double *,
		       int *, int *,
		       double (*)(double));




/*
 **********************************************************************
 **
 **
 **
 **	Elementary matrix routines
 **	==========================
 **
 **
 **
 */


/*	void swap_rows(int dim, double *mat, int nrow1, int nrow2,
 *	int *errcode)
 *
 *	This routine exchanges two rows of a square matrix.
 *
 *	Input parameters:
 *	----------------
 *	dim 		: Order of the matrix.
 *	mat 		: Matrix to be treated.
 *	nrow1, nrow2 	: indexes of the given rows (1 to n)
 *
 *	Output parameters:
 *	-----------------
 *	mat 	: treated matrix.
 *	errcode : Error code.
 *
 *	Possible error codes:
 *	--------------------
 *	ENOWCORE
 */
  void cdecl swap_rows(int,
		      double *,
		      int, int ,
		      int *);



/*	void row_mat_div(int dim,
 *		double *mat,
 *		double lambda,
 *		int nrow,
 *		int *errcode)
 *	This function divides a given row of a square matrix by a
 *	given constant.
 *
 *	Input parameters:
 *	----------------
 *	dim	: Order of the matrix.
 *	mat	: Points to the matrix.
 *	lambda	: The real constant.
 *	nrow	: Row index (1 to dim).
 *
 *	Output parameters:
 *	-----------------
 *	errcode : Error code.
 *
 *	Possible error codes:
 *	--------------------
 *	E0DIVIDE
 */
  void cdecl row_mat_div(int,
			double *,
			double,
			int,
			int *);




/*	void slargest_vect(int dim, double *vect, double *res)
 *
 *	This function finds the largest element in a vector.
 *
 *	Input parameters:
 *	----------------
 *	dim : dimension of the vector.
 *	vect : vector.
 *
 *	Output parameters:
 *	-----------------
 *	res : result.
 */
  void cdecl slargest_vect(int,
			  double *,
			  double *);





/*	void rdiv_vect(int dim,
 *		double *vect,
 *		double lambda,
 *		int *errcode)
 *
 *	This  function  allows  to  divide  all  the  coordinates
 *	composing a vector by a real constant
 *
 *	Input parameters:
 *	----------------
 *	dim	: vector dimension.
 *	vect	: pointer to an array containing the vector.
 *	lambda	: divider.
 *
 *	Output parameters:
 *	-----------------
 *	vect	: resulting vector.
 *	errcode	: error code.
 *
 *	Possible error codes:
 *	--------------------
 *	E0DIVIDE
 *	EDIMLE0
 */
  void cdecl rdiv_vect(int,
		      double *,
		      double ,
		      int *);




/*	void mult_mat_vect(int dim, double *mat,
 *		double *vect, double *res, int *errcode)
 *
 *	This function multiplies a square matrix by vector.
 *
 *	Input parameters:
 *	----------------
 *	dim	: Order of the matrix (= vector dimension)
 *	mat	: Matrix.
 *	vect	: Vector.
 *
 *	Output parameters:
 *	-----------------
 *	res	: Resulting vector.
 *	errcode : Error code.
 *
 *	Possible error codes:
 *	--------------------
 *	EDIMLE0
 */
  void cdecl mult_mat_vect(int ,
			  double *, double *, double *,
			  int *);






/*	double mult_vect_vect(int dim,
 *		      double *v1, double *v2)
 *
 *	This routine computes the scalar product of two vectors.
 *
 *	Input parameters:
 *	----------------
 *	dim	: Order of the vectors.
 *	v1,v2	: Pointers to arrays containing the vectors.
 *
 *	Returns: the scalar product.
 */

  double cdecl mult_vect_vect(int ,
			     double *, double *);





/*	void trans_size_system(int dim1, double *m1, int dim2,
 *		double *m2)
 *
 *	This function  converts a  matrix of  a  given  order  to
 *	another one.
 *
 *	Input parameters:
 *	----------------
 *	dim1	: order of the initial matrix.
 *	m1	: initial Matrix to be treated.
 *	dim2	: order of the target matrix
 *
 *	Output parameters:
 *	-----------------
 *	m2	: target matrix
 *
 *	Note:
 *	----
 *	If dim2>dim1 the rest of the m2 will be completed by zeroes,
 *	If dim2<dim1 the  dim1-dim2th columns  and rows  of  the
 *		matrix m1 will be lost in m2.
 */
  void cdecl trans_size_system(int,
			      double *,
			      int,
			      double *);




/*	bool ismatsym(int dim,
 *		double *mat,
 *		int *errcode)
 *
 *
 *
 *	This function  determinates if  a matrix  is symmetric or
 *	not.
 *
 *	Input parameters:
 *	----------------
 *	dim	: Order of the matrix.
 *	mat	: matrix to examine.
 *
 *	Output parameters:
 *	-----------------
 *	errcode : error code
 *
 *	Returns : TRUE or FALSE
 *
 *	Possible error codes :
 *	EDIMLE0
 */
  bool cdecl ismatsym(int,
		     double *,
		     int *);









/*
 **********************************************************************
 **
 **
 **	Linear algebra functions
 **	========================
 **
 **
 **
  */

/*	void determinant(int dim,
 *		double *mat0,double *det,
 *		int *errcode)
 *
 *	This function  calculates the  determinant  of  a  square
 *	matrix.
 *
 *	Input parameters:
 *	----------------
 *	dim 	: order of the matrix.
 *	mat0 	: matrix the determinant of which will be calculated.
 *
 *	Output parameters:
 *	-----------------
 *	det 	: value of the calculated determinant.
 *	errcode : error code.
 *
 *	Possible error codes:
 *	--------------------
 *	EDIMLE0
 *	ENOWCORE
 */
  void cdecl determinant(int,
			double *, double *,
			int *);




/*	void inverse(int dim,
 *		double *mat0,
 *		double *invmat,
 *		int *errcode)
 *
 *	This routine inverts a square matrix
 *
 *	Input parameters:
 *	----------------
 *	dim	: order of the matrix to invert.
 *	mat0	: matrix to invert
 *
 *	Output parameters:
 *	-----------------
 *	invmat	: inverted matrix.
 *	errcode : error code.
 *
 *	Possible error codes:
 *	--------------------
 *	EDIMLE0
 *	EMATSING
 *	ENOWCORE
 */
  void cdecl inverse(int,
		    double *,
		    double *,
		    int *);


/*	void gauss_elimination(int dim,
 *		double *mat_A,
 *		double *vect_B,
 *		double *vect_X,
 *		int *errcode)
 *
 *	This  routine   solves  a  linear  system  (A*X=B)  using
 *	gaussian elimination.
 *
 *	Input parameters:
 *	----------------
 *	dim	: order of the matrix.
 *	mat_A	: matrix A
 *	vect_B	: second member vector B
 *
 *	Output parameters:
 *	-----------------
 *	vect_X	: solution vector X.
 *	errcode : error code.
 *
 *	Possible error codes:
 *	--------------------
 *	EDIMLE0
 *	ENOSOL
 *	EMATSING
 */
  void cdecl gauss_elimination(int ,
			      double *,
			      double *, double *,
			      int *);




/*	void partial_pivot(int dim,
 *		double *mat_A,
 *		double *vect_B,
 *		double *vect_X,
 *		int *errcode)
 *
 *	This function  solves a  linear system  (AxX=B) using the
 *	partial pivoting algorithm.
 *
 *	Input parameters:
 *	----------------
 *	dim	: order of matrix A.
 *	mat_A	: matrix A.
 *	vect_B	: second member vector B.
 *
 *	Output parameters:
 *	-----------------
 *	vect_X	: solution vector X.
 *	errcode	: Error code.
 *
 *	Possible error codes:
 *	--------------------
 *	EDIMLE0
 *	EMATSING
 *	ENOWCORE
 */
  void cdecl partial_pivot(int,
			  double *,
			  double *, double *,
			  int *);




/*	void lu_decompose(int dim,
 *		double *mat_A,
 *		double *decomp, double *matperm,
 *		int *errcode)
 *
 *	This function performs LU decomposition in order to solve
 *	several  linear  systems  having  the  same  matrix  (AxX=B,
 *	AxX'=B', AxX"=B", ...) with the lu_solve function.
 *
 *	Input parameters:
 *	----------------
 *	dim	: order of the matrix.
 *	mat_A	: matrix A.
 *
 *	Output parameters:
 *	-----------------
 *	decomp, matperm : matrices  to be  passed as  arguments  to
 *				lu_solve..
 *	errcode		: error code.
 *
 *	Possible error codes:
 *	--------------------
 *	EDIMLE0
 *	ENOWCORE
 *	EMATSING
 */
  void cdecl lu_decompose(int ,
			 double *, double *, double *,
			 int *);




/*	void lu_solve(int dim,
 *		double *decomp,
 *		double *vect_B,
 *		double *matperm,
 *		double *vect_X,
 *		int *errcode)
 *
 *	This function  solves a  linear system (AxX=B) the matrix
 *	of which has been previously processed with lu_decompose.
 *
 *	Input parameters:
 *	----------------
 *	dim	: order of the matrix A.
 *	decomp	: matrix resulting from the call to lu_decompose.
 *	vect_B	: Second member vector B.
 *	matperm : matrix resulting from the call to lu_decompose.
 *
 *	Output parameters:
 *	-----------------
 *	vect_X	: solution vector X.
 *	errcode : error code.
 *
 *	Possible error codes:
 *	--------------------
 *	EDIMLE0
 *	ENOWCORE
 */
  void cdecl lu_solve(int ,
		     double *, double *, double *, double *,
		     int *);




/*	void gauss_seidel(int dim,
 *		double *mat_A,
 *		double *vect_B,
 *		double tol,
 *		int maxiter,
 *		double *vect_X,
 *		int *iter, int *errcode)
 *
 *	This routine  solves a  linear system  (A*X=B) using  the
 *	Gauss Seidel iterative algorithm.
 *
 *	Input parameters:
 *	----------------
 *	dim 	: order of the matrix to invert.
 *	mat_A 	: matrix A
 *	vect_B 	: second member vector B
 *	tol 	: tolerance used for convergence test
 *	maxiter : maximum number of allowed iterations
 *
 *	Output parameters:
 *	-----------------
 *	vect_X 	: solution vector X.
 *	iter 	: actual number of performed iterations
 *	errcode : error code.
 *
 *	Possible error codes:
 *	--------------------
 *	EDIMLE0
 *	ENOWCORE
 *	EOVERMAXITER
 *	WNODIAGDOM
 *	ETOLLE0
 *	EDIMLE0
 *	EMAXITERLE0
 *	E0MATDIAG
 *	ENOSOL
 *	EMATSING
 */
  void cdecl gauss_seidel(int ,
			 double *, double *,
			 double ,
			 int ,
			 double *,
			 int *, int *);







/*
 **********************************************************************
 **
 **
 **	Eigen values and vectors routines set
 **	=====================================
 **
 **
 **
  */



/*	void eigen_power(int dim,
 *		double *mat,
 *		double *vect0,
 *		int maxiter,
 *		double tol,
 *		double *eigenval, double *eigenvect,
 *		int *iter, int *errcode)
 *
 *	This function  computes  the  dominant  eigenvalue  of  a
 *	square matrix.
 *
 *	Input parameters:
 *	----------------
 *	dim : Order of the matrix
 *	mat : Matrix
 *	vect0 : Approximate value of the eigenvector.
 *	maxiter : Maximum number of iterations to perform.
 *	tol : tolerance used for convergence test.
 *
 *	Output parameters:
 *	-----------------
 *	eigenval 	: Computed eigenvalue.
 *	eigenvect 	: Computed eigenvector.
 *	iter  		: Actual  number  of  iterations  the  algorith  went
 *			  through.
 *	errcode : Error code.
 *
 *	Possible error codes:
 *	--------------------
 *	EDIMLE0
 *	ETOLLE0
 *	EMAXITERLT0
 *	E0DIVIDE
 *	ENOWCORE
 *
 *	Note:
 *	----
 *	This algoritm  will not converge if vect0 happens
 *	to be orthogonal to the actual result.
 */
  void cdecl eigen_power(int,
			double *,
			double *,
			int,
			double,
			double*, double *,
			int *, int *);

			/* This algorithm will not converge *
			 * if the approximate eigenvalue    *
			 * is orthogonal to the actual      *
			 * dominant one  		    */



/*	void jacobi(int dim,
 *		double *mat,
 *		int maxiter,
 *		double tol,
 *		double *eval, double *evect,
 *		int *iter, int *errcode)
 *
 *	This function  computes the  eigen vectors  and the eigen
 *	values of  a symmetric  matrix using  the Jacobi's rotations
 *	algorithm.
 *
 *	Input parameters:
 *	----------------
 *	dim 	: Order of the matrix.
 *	mat 	: Matrix.
 *	maxiter : maximum number of allowed iterations.
 *	tol 	: tolerance used in convergence tests.
 *
 *	Output parameters:
 *	-----------------
 *	eval 	: computed eigen values.
 *	evect 	: computed eigen vectors.
 *	errcode : error code.
 *
 *	Possible error codes:
 *	--------------------
 *	EDIMLE0
 *	ETOLLE0
 *	EMAXITERLT0
 *	EOVERMAXITER
 *	EMATNOTSYM
 */
  void cdecl jacobi(int,
		   double *,
		   int ,
		   double,
		   double *, double *,
		   int *, int *);




/*	void le_verrier(int dim,
 *		double *mat,
 *		double *poly,
 *		int *errcode)
 *
 *	This function compute the characteristic polynome of a
 *	square matrix.
 *
 *	Input parameters:
 *	----------------
 *	dim	: Order of the matrix.
 *	mat	: matrix
 *
 *	Output parameters:
 *	-----------------
 *	poly	: characteristic polynomial.
 *	errcode	: error code.
 *
 *	Possible error codes:
 *	--------------------
 *	ENOWCORE
 */
  void cdecl le_verrier(int,
		       double *,
		       double *,
		       int *);



/*	void eigen_vect(int dim,
 *		double *mat,
 *		int nbeval,
 *		double *eval,
 *		double *evect,
 *		int *errcode)
 *
 *	This function  computes  the  eigenvectors  of  a  square
 *	matrix, the corresponding eigen values of which are known.
 *
 *	Input parameters:
 *	----------------
 *	dim	: Order of the matrix
 *	mat	: Matrix
 *	nbeval	: Number of given eigen values.
 *	eval	: Pointer to an array containing known eigen values
 *
 *	Output parameters:
 *	-----------------
 *	evect	: Computed eigenvectors.
 *	errcode : Error code.
 *
 *	Possible error codes:
 *	--------------------
 *	EDIMLE0
 *	E0DIVIDE
 *	ENOWCORE
 *
 *	Note:
 *	-----
 *	This algoritm  will not converge if vect0 happens
 *	to be orthogonal to the actual result.
 */
  void cdecl eigen_vect(int,
		       double *,
		       int,
		       double *,
		       double *,
		       int *);



/*	void householder_givens(int dim,
 *		double *mat,
 *		int nbeval,
 *		double *eval,
 *		int *errcode)
 *
 *	This routine computes a given number of eigen values of a
 *	symmetric matrix using the the Householder Givens algorithm.
 *	(It is  an adaptation of the version written by J. Ortega in
 *	the 1960's for the IBM 7094).
 *
 *	Input parameters:
 *	----------------
 *	dim 	: order of the matrix.
 *	mat 	: matrix
 *	nbeval 	: number of eigen values to be computed.
 *
 *	Output parameters:
 *	-----------------
 *	eval 	: computed eigen values
 *	errcode : error code.
 *
 *	Possible error codes:
 *	--------------------
 *	EDIMLE0
 *	ENOWCORE
 *	EMATNOTSYM
 *
 *	Note:
 *	----
 *	If nbeval  is lower  than or  equal to  0 then no computation
 *		is performed. If it is greater than dim then all
 *		eigenvalues are computed.
 */
  void cdecl householder_givens(int ,
			       double *,
			       int,
			       double *,
			       int *);


/*
 **********************************************************************
 **
 **
 **
 **	Elementary real polynomial functions
 **	====================================
 **
 **
 **
 */





/*	void rpoly2_solve(double a, double b, double c,
 *		double *root1r, double *root1im,
 *		double *root2r, double *root2im)
 *
 *	This  function   solves  the   real  polynomial  equation
 *	ax**2+bx+c=0.
 *
 *	Input parameters:
 *	----------------
 *	a, b, c : coefficient of the polynome.
 *
 *	Output parameters:
 *	-----------------
 *	root1r  : Real part of the first root.
 *	root1im : Imaginary part of the first root.
 *	root2r  : Real part of the second root.
 *	root2im : Imaginary part of the second root.
 */
  void cdecl rpoly2_solve(double , double , double ,
			 double *, double *,
			 double *, double *);





/*	void rpoly_value_deriv(int degree,
 *		double *poly,
 *		double x,
 *		double *y, double *deriv)
 *
 *	This routines  computes the  value of  a polynome and the
 *	one of its first derivate at a given point.
 *
 *	Input parameters:
 *	----------------
 *	degree	: Degree of the polynome.
 *	poly	: Polynome.
 *	x	: Real value.
 *
 *	Output parameters:
 *	-----------------
 *	y	: = poly(x).
 *	deriv	: = poly'(x).
 */
  void cdecl rpoly_value_deriv(int ,
			      double *,
			      double,
			      double *, double *);


/*	void rpoly_one_root(int degree
 *		double *poly,
 *		double x, double tol,
 *		int maxiter,
 *		double *root, double *value, double *deriv,
 *		int *iter, int *errcode)
 *
 *	This function  finds  one  real  root  of  a  given  real
 *	polynome.
 *
 *	Input parameters:
 *	----------------
 *	degree : Degree of the polynome.
 *	poly : Polnome, one root of which must be found.
 *	x : Initial approximation of the root.
 *	tol : Tolerance used for convergence tests.
 *	maxiter : Maximum number of iterations allowed.
 *
 *	Output parameters:
 *	-----------------
 *	root	: Found root, if any.
 *	value	: = poly(root)
 *	deriv	: value of the first derivate of poly for x = root.
 *	iter  	: Actual  number  of  iteration  the  algorithm  went
 *		  through.
 *	errcode : Error Code.
 *
 *
 *	Possible error codes:
 *	--------------------
 *	EDEGLE0
 *	EOVERMAXITER
 *	ENULLSLOPE
 */
  void cdecl rpoly_one_root(int,
			   double *,
			   double , double ,
			   int ,
			   double *, double *, double *,
			   int *, int *);






/*	void rpoly_reduce(int *degree,
 *		double *poly,
 *		double root,
 *		int *errcode)
 *
 *	This functions  reduce the  degree of a given polynome, a
 *	root of which is known.
 *
 *	Input parameters:
 *	----------------
 *	degree	: Degree of the polynome.
 *	poly	: Polynome.
 *	root	: Known root.
 *
 *	Output parameters:
 *	-----------------
 *	poly	: Reduced polynomial.
 *	errcode : Error code.
 *
 *	Possible error codes:
 *	--------------------
 *	EDEGLE0
 *	ENOWCORE
 */
  void cdecl rpoly_reduce(int *,
			 double *,
			 double ,
			 int *);



/*
 **********************************************************************
 **
 **
 **	Integration functions
 **	=====================
 **
 **
 **
 */


/*	void simpson(double lb, double ub,
 *		int nbinter,
 *		double *I,
 *		int *errcode,
 *		double f(double))
 *
 *	This function computes the integration of a real function
 *	on an interval using the Simpson method.
 *
 *	Input parameters:
 *	----------------
 *	lb 	: lower bound of the interval.
 *	ub 	: upper bound of the interval.
 *	nbinter : number of sub-intervals used for the computation.
 *	I 	: value of the computed integral.
 *
 *	Output parameters:
 *	-----------------
 *	errcode : Error code.
 *	f 	: function to integrate
 *
 *	Possible error codes:
 *	--------------------
 *	ENUMINTLE0
 */
  void cdecl simpson(double , double ,
		    int ,
		    double *,
		    int *,
		    double (*)(double));






/*	void romberg(double lb, double ub, double tol,
 *		int maxiter,
 *		double *I,
 *		int *iter,
 *		int *errcode,
 *		double f(double))
 *
 *	This function computes the integration of a real function
 *	on an interval using the Romberg algorithm.
 *
 *	Input parameters:
 *	----------------
 *	lb : lower bound of the interval.
 *	ub : upper bound of the interval.
 *	tol : tolerance used for convergence test.
 *	maxiter : maximum number of allowed iterations.
 *
 *	Output parameters:
 *	-----------------
 *	I 	: value of the computed integral.
 *	iter 	: actual  number  of  iterations  the  algorithm  went
 *		  through.
 *	errcode : Error code.
 *	f	: function to integrate
 *
 *	Possible error codes:
 *	--------------------
 *	ENOWCORE
 *	EOVERMAXITER
 *	EMAXITERLT0
 *	ETOLLE0
 */
  void cdecl romberg(double, double, double,
		    int,
		    double *,
		    int *, int *,
		    double (*)(double));








/*
 **********************************************************************
 **
 **
 **
 **	Differential equations functions
 **	================================
 **
 **
 **
 */





/*
 *	void adams(double lb, double ub, double x0,
 *		int nret, int nint,
 *		double *tval, double *xval,
 *		int *errcode,
 *		double f(double, double))
 *
 *	This function solves a first order differential equation
 *	with a specified initial condition.
 *
 *	Input Parameters:
 *	----------------
 *	lb   : Lower bound of t interval.
 *	ub   : Upper bound of t interval.
 *	x0   : x value for t = lb.
 *	tol  : Tolerance used for convergence tests.
 *	nret : Number of values to be returned.
 *	nint : Number of sub-intervals used for computation
 *	f    : f(t, x) = dx/dt
 *
 *	Output parameters:
 *	-----------------
 *	tval    : Pointer to an array containing nret values of t
 *		  scattered between lb and ub.
 *	xval    : Pointer to an array containing the nret
 *		  corresponding values of x.
 *	errcode : error code.
 *
 *	Possible error codes:
 *	--------------------
 *	ENTRMGTNINT
 *	ENTRMLT1
 *	ELBDGEUBD
 *	ENOWCORE
 */
  void cdecl adams(double, double, double,
	     int, int,
	     double *, double *,
	     int *,
	     double (*)(double, double));






/*	void RKF(double lb, double ub, double x0, double tol,
 *		int nret,
 *		double *tval, double *xval,
 *		int *errcode,
 *		double f(double, double))
 *
 *	This function  solves a first order ordinary differential
 *	equation with a specified initial condition.
 *
 *	Input parameters:
 *	----------------
 *	lb	: Lower bound of t interval.
 *	ub	: Upper bound of t interval.
 *	x0	: x value for t = lb.
 *	tol	: Tolerance used for convergence tests.
 *	nret	: Number of values to be returned.
 *	f 	: f(t, x) = dx/dt
 *
 *	Output parameters:
 *	-----------------
 *	tval	: Pointer to  an array  containing nret  values  of  t
 *		  scattered between lb and ub.
 *	xval	: Pointer to an array containing the nret corresponding
 *		  values of x.
 *	errcode : error code.
 *
 *	Possible error codes:
 *	--------------------
 *	ENTRMGTNINT
 *	ENTRMLT1
 *	ELBDGEUBD
 *	ENOWCORE
 *	ETOLNR
 */
  void cdecl RKF(double, double, double, double,
	   int,
	   double *, double *,
	   int *,
	   double (*)(double, double));



/*	void initcond1storder(double lb, double ub, double x0,
 *		int nret, int nint,
 *		double *tval, double *xval,
 *		int *errcode,
 *		double f(double, double))
 *
 *	This finction  solves a first order ordinary differential
 *	equation with initial condition specified).
 *
 *	Input parameters:
 *	----------------
 *	lb	: lower bound value of t
 *	ub	: upper bound value of t
 *	x0	: value of x for t = lb
 *	nret	: number of values to be returned
 *	nint	: number of sub-intervals used for computation.
 *	f	: dx/dt = f(t,x)
 *
 *	Output parameters:
 *	-----------------
 *	tval	: pointer to an array containint nret values of t
 *	xval	: pointer to an array containint nret values of x
 *	errcode : error codes
 *
 *	Possible error codes:
 *	--------------------
 *	ENTRMLT1
 *	ELBDGEUBD
 *	ENOWCORE
 */
  void cdecl initcond1storder(double , double , double ,
			     int , int ,
			     double *, double *,
			     int *,
			     double (*)(double, double));




/*	void initcond2ndorder(double lb, double ub, double x0,
 *		double xprime0,
 *		int nret, int nint,
 *		double *tval, double *xval, double *xprimeval,
 *		int *errcode,
 *		double f(double, double, double))
 *
 *	This function  solves a  2nd order  ordinary differential
 *	equation with specified initial conditions.
 *
 *	Input parameters:
 *	----------------
 *	lb	: lower bound value of t
 *	ub	: upper bound value of t
 *	x0	: values of x for t = lb
 *	xprime0 : values of dx/dt for t = lb
 *	nret	: number of values to be returned
 *	nint	: number of sub-intervals used for computation.
 *	f	: dx2/dt2 = f(t, x, dx/dt)
 *
 *	Output parameters:
 *	-----------------
 *	tval		: pointer to an array containint nret values of t.
 *	xval		: pointer to an array containint nret values of x.
 *	xprimeval	:  pointer to  an array  containint nret values of
 *				dx/dt.
 *	errcode 	: Error code.
 *
 *	Possible error codes:
 *	--------------------
 *	ENTRMLT1
 *	ENTRMGTNINT
 *	ELBDGEUBD
 *	ENOWCORE
 */
  void cdecl initcond2ndorder(double, double, double, double,
		      int, int,
		      double *, double *, double *,
		      int *,
		      double (*)(double, double, double));




/*	void initcond(int order,
 *		double lb, double ub,
 *		double *initval,
 *		int nret, int nint,
 *		double *matsol,
 *		int *errcode,
 *		double f(double *))
 *
 *	This routine solves an  n-order ordinary  differential
 *	equation with  specified initial conditions.
 *
 *	Input parameters:
 *	----------------
 *	n	: order of the equation to be solved.
 *	lb 	: lower bound value of t
 *	ub 	: upper bound value of t
 *	initval : pointer to  an array containing the values values
 *		  of x and its "order" successive derivates for t=lb.
 *	nret	: number of values to be returned
 *	nint	: number of sub-intervals used for computation.
 *	f	: d(n)x/dtn = f(t, x, dx/dt, ....,d(n-1)x/dt(n-1))
 *
 *	Output parameters:
 *	-----------------
 *	matsol 	: bi dimensional  array containing  the solution  :;
 *		  matsol[i][j]
 *	errcode	: error codes
 *
 *	Possible error codes:
 *	--------------------
 *	ENTRMLT1
 *	ENTRMGTNINT
 *	ELBDGEUBD
 *	ENOWCORE
 *	EDIMLE0
 */
  void cdecl initcond(int,
	      double, double,
	      double *,
	      int, int,
	      double *,
	      int *,
	      double (*)(double *));






/*	void initcondsystem1(int neq,
 *		double lb, double ub,
 *		double *initval,
 *		int nret, int nint,
 *		double *matsol, int *errcode,
 *		double (**tabfunc)(double *))
 *
 *	This procedure solves a system of 1st-order differential
 *	equation with specified initial conditions.
 *
 *	Input parameters:
 *	----------------
 *	neq	: number of equations.
 *	lb, ub	: lower and upper bounds of the study interval.
 *      initval : values of x1, x2, ... xneq at t
 *	nret	: numbers of values of t that computed results must
 *			be returned for.
 *	nint 	: numbers of sub intervals of [lb, ub] used for computation.
 *      tabfunc : array of functions defining the system to be solved.
 *		  (Each function is as x' = f(t, x , x , ..., x , ..., x   )
 *                                       i        1   2        j        neq
 *	Output parameters:
 *	-----------------
 *	matsol	: matrix containing the results.
 *	errcode : error code.
 *
 *	Possible error codes:
 *	--------------------
 *	ENTRMLT1
 *	ENTRMGTNINT
 *	ELBDGEUBD
 *	ENOWCORE
 *	EDIMLE0
 *
 *	Note:
 *	----
 *	the result (matsol) is organized the following way
 *	matsol is a neq+1 columns nret lines matrix
 *	Column 0 contains the succesive values of t
 *	Column 1 contains the succesive values of x1
 *	etc ...
 *	(i.e. each line contains a value of t and the corresponding
 *	x1, x2, ..., xneq.
 *
 */
  void cdecl initcondsystem1(int,
		    double, double,
		    double *,
		    int, int,
		    double *, int *,
		    double (**)(double *));




/*	void initcondsystem2(int neq,
 *		double lb, double ub,
 *		double *initval,
 *		int nret, int nint,
 *		double *matsol, int *errcode,
 *		double (**tabfunc)(double *))
 *
 *	This procedure solves a system of 2nd-order differential
 *	equation with specified initial conditions.
 *
 *	Input parameters:
 *	----------------
 *	neq	: number of equations.
 *	lb, ub	: lower and upper bounds of the study interval.
 *      initval : values of x1, x2, ... xneq at t
 *	nret	: numbers of values of t that computed results must
 *			be returned for.
 *	nint 	: numbers of sub intervals of [lb, ub] used for computation.
 *      tabfunc : array of functions defining the system to be solved.
 *		  (Each function is as x" = f(v, w)
 *					 i
 *		   with v = (t, x , x , ..., x , ..., x   )
 *                               1   2        j        neq
 *                 and
 *			w = (t, x', x', ..., x', ..., x'  )
 *                               1   2        j        neq
 *	Output parameters:
 *	-----------------
 *	matsol	: matrix containing the results.
 *	errcode : error code.
 *
 *	Possible error codes:
 *	--------------------
 *	ENTRMLT1
 *	ENTRMGTNINT
 *	ELBDGEUBD
 *	ENOWCORE
 *	EDIMLE0
 *
 *	Note:
 *	----
 *	the result (matsol) is organized the following way
 *	matsol is a neq+1 columns nret lines matrix
 *	Column 0 contains the succesive values of t
 *	Column 1 contains the succesive values of x1
 *	etc ...
 *	(i.e. each line contains a value of t and the corresponding
 *	x1, x2, ..., xneq.
 *
 */
  void cdecl initcondsystem2(int,
		     double, double,
		     double *, double *,
		     int, int,
		     double *, double *,
		     int *,
		     double (**)(double *, double *));




/*	void shooting(double lb, double ub, double lb0, double ub0,
 *			double lbprime,
 *	      int nret,
 *	      double tol,
 *	      int maxiter, int nint,
 *	      int *iter,
 *	      double *tval, double *xval, double *xprimeval,
 *	      int *errcode,
 *	      double (*f)(double, double, double))
 *
 *	This routine solves a possibly non-linear 2nd order ordinary
 *	differential equation with specified initial and final
 *	conditions.
 *
 *
 *	Input parameters:
 *	----------------
 *	lb, ub	: lower and upper bounds of the study interval.
 *      lb0,ub0	: values of x t=lb and t=ub.
 *	lbprime	: values of x' at t=ub.
 *	nret	: numbers of values of t that computed results must
 *			be returned for.
 *	tol	: tolerance used for algorithm convergence tests.
 *      maxiter : maximum number of allowed iterations.
 *	nint 	: numbers of sub intervals of [lb, ub] used for computation.
 *	f	: differential equation (x" = f(t, x, x'))
 *
 *	Output parameters:
 *	-----------------
 *	iter		: actual number of iterations the algorithm went through.
 *	tval		: values of t  between lb and ub.
 *	xval		: values of x  between lb and ub.
 *	xprimeval	: values of x' between lb and ub.
 *	errcode 	: error code.
 *
 *	Possible error codes:
 *	--------------------
 *	ENTRMLT1
 *	ENTRMGTNINT
 *	ELBDGEUBD
 *	ENOWCORE
 *	EOVERMAXITER
 *	ENOSOL
 *	EMAXITERLT0
 *	ETOLLE0
 */
  void cdecl shooting(double, double, double, double,
	      double,
	      int,
	      double,
	      int, int,
	      int *,
	      double *, double*, double *,
	      int *,
	      double (*)(double, double, double));



/*	void linear_shooting(double tlb, double tub, double xlb, double xub,
 *		int nret, int nint,
 *		double *tval, double *xval, double *xprimeval,
 *		int *errcode,
 *		double (*f)(double, double, double))
 *
 *
 *	This routine solves a linear 2nd order ordinary
 *	differential equation with specified initial and final
 *	conditions.
 *
 *	Input parameters:
 *	----------------
 *	tlb, tub	: lower and upper bounds of the study interval.
 *      xlb, xub	: values of x t=lb and t=ub.
 *	nret		: numbers of values of t that computed results must
 *				be returned for.
 *	nint 	: numbers of sub intervals of [lb, ub] used for computation.
 *	f	: differential equation (x" = f(t, x, x'))
 *
 *	Output parameters:
 *	-----------------
 *	tval		: values of t  between lb and ub.
 *	xval		: values of x  between lb and ub.
 *	xprimeval	: values of x' between lb and ub.
 *	errcode 	: error code.
 *
 *	Possible error codes:
 *	--------------------
 *	ENTRMLT1
 *	ENTRMGTNINT
 *	ELBDGEUBD
 *	ENOWCORE
 *	ENOTLINDIFEQ
 */
  void cdecl linear_shooting(double, double, double, double,
		     int, int,
		     double *, double *, double *,
		     int *,
		     double (*)(double, double, double));








/*
 **********************************************************************
 **
 **
 **
 **	Least Squares routine
 **	=====================
 **
 **
 **
 */





/* 	void lsq(int nbpoints,
 *		double *xdata, double *ydata,
 *		int *nbtermes,
 *		double *vectsol, double *yfit, double *residus,
 *		double *stddev, double *variance,
 *		int *errcode,
 *		lsqfit fit)
 *
 *	This function computes least square fit of points.
 *
 *	Input parameters:
 *	----------------
 *	nbpoints : number of data points.
 *	xdata 	 : x of each point.
 *	ydata 	 : y of each point.
 *	nbterms  : number of terms in the least square fit.
 *	lsq 	 : least square fit type
 *
 *	Output parameters:
 *	-----------------
 *	vectsol  : computed LS coefficients.
 *	yfit  	 : values of each point.
 *	stddev 	 : standard deviation.
 *	variance : variance.
 *	errcode  : error code.
 *
 *	Possible error codes:
 *	--------------------
 *	ENOWCORE
 *	E0DIVIDE
 *	ELOGLE0
 *	ENDPTSLT2
 *	ENTRMLT1
 *	ENDTPTLTLTRMS
 *	ENOSOL
 */
  void cdecl lsq(int ,
		double *, double *,
		int *,
		double *, double *, double *,
		double *, double *,
		int *,
		lsqfit );





/*	char *lsqname(lsqfit fit)
 *
 *	This function  returns the  name associated  to  a  least
 *	square method.
 *
 *	Input parameters:
 *	----------------
 *	lsq : least square fit type
 *
 *	Returns:
 *	-------
 *	the name of the least square type fit
 */
  char * cdecl lsqname(lsqfit);

#ifdef __cplusplus
}
#endif


#endif