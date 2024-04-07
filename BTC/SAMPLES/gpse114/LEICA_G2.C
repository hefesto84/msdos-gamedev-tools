/*  30/11/92.   Leica_g2.c

    This module is included by leica_g1.c - it contains GPS - specific
    code which is likely to be changed only occasionally. Leica_g1
    is the "working" code - ie. the more likely to change as the software
    develops. leica_u.c contains c utilities which are completely non-GPS
    specific (except for some typedefs), and shouldn't be changed at all
    over the course of the development.

*/

vector outvec;             /*  Global return vector            */
vector nulvec;             /*  Null vector, for global use     */
double outdoub, nuldoub;   /*  Same, except for generic double */
matrix nulmat;             /*  Null matrix, for global use     */
matrix outmat;             /*  Global return matrix            */

int edisp(matrix *anym); /* Display contents of matrix containing EPH data.*/
int odisp(matrix *anym); /* Display the contents of matrix with OBS data. */
vector findk(matrix s, vector prs, vector x0); /* Receiver-pos algorithm   */
vector eph2uvw(ephemeris eph, double t);  /* Satellite ephemeris algorithm */
void ginit();  /*  Set up global variables                                 */
int findrec(double *xdop, vector *xx, ephemeris alleph[4], rinex obs);
/* The main function for calculating receiver position        */
matrix fsats(ephemeris alleph[4], rinex obs);  /*  Find 4 satellite pos.   */
int ephdisp(ephemeris *eph);  /* Display an ephemeris record (Magellan!) */
int uvw3geo(double u, double v, double w, double *Po, double *Lo, double *ho);
/*  Convert from earth-fixed rectangular coordinates to polar, WGS84 coords.*/
double gdop(matrix s, vector x0); /*  Find dilution of precision.  */
double elevation(vector *s, vector *p); /*  Find elevation of a satellite. */
int onesat(vector *s, ephemeris oneph, double t); /* Find pos of 1 sat.  */
int maskselect();  /*  Reject all sats with bad (low) elevation angle. */
int maskselect12(); /*  Same as above, but for the differencing case. */
int dopselect12(); /* Select the 4 satellites which give the best dop.  */
int dopselect(); /*  Same as above, but for non-diff case.  */
int firstselect12(); /* Simply grab the first 4 of all available sats.  */
int firstselect();  /* Same as above, but for non-diff case.  */
int header(); /*  Displays output format for data. */
/*-------------------------------------------------------------------*/
int edisp(matrix *anym) /* Display contents of matrix containing EPH data.*/
/*  Utility especially for displaying ephemeris matrices - for debugging
    purposes only. This one is specifically for the Leica Rinex
    format, number structure per block: 10 4 4 4 4 4 4 1

    Done 21/11/92
*/
{
int   i, j;

printf("\n1st row: %f %f %f %f %f", anym->a[0][0], anym->a[0][1], anym->a[0][2], anym->a[0][3], anym->a[0][4]);
printf("\n1st row: %f %f %f %f %f", anym->a[0][5], anym->a[0][6], anym->a[0][7], anym->a[0][8], anym->a[0][9]);
for (i=0;i<6;i++) {
 printf("\n %f %f %f %f ", anym->a[i+1][0], anym->a[i+1][1], anym->a[i+1][2], anym->a[i+1][3]);
}
printf("\n %f \n\n",anym->a[7][0]);
wait();
return(0);
}
/*-------------------------------------------------------------------*/
int odisp(matrix *anym)
/*  Utility especially for displaying observation matrices - for debugging
    purposes only. This one is specifically for the Leica Rinex
    format, number structure per block: 8; (nsats+1);  6 (nsats of)

    Done 25/11/92
*/
{
int   i, j, nsats;

nsats=(int)anym->a[1][0];
printf("\n");
for (i=0;i<8;i++) printf("%f ",anym->a[0][i]);
printf("\n");
for (i=0;i<(nsats+1);i++) printf("%f ",anym->a[1][i]);
for (j=0;j<nsats;j++) {
   printf("\n");
   for (i=0;i<6;i++) printf("%f ",anym->a[j+2][i]);
}
wait();
return(0);
}
/*-------------------------------------------------------------------*/



/*==================---Dodgey boundary--------------==================*/

/*--------------------------------------------------------------------*/
vector findk(matrix s, vector prs, vector x0)
/*
    This is the algorithm which uses the satellite positions and
    pseudoranges corrected for satellite clock errors, to find the
    receiver position. It is an iterative procedure because the system
    of equations for the receiver position is nonlinear. The
    iterations are applied to a linearised version of the nonlinear eqs.
    findk performs one "iteration" of the procedure - ie. the function
    calling findk must perform the iteration.

    Given satellite positions s, pseudo-ranges prs and approximate
    initial earth point x0, findk finds a better approximation, x1,
    to the receiver position. Only a few (4 or 5) such iterations
    are generally required to give the receiver position to the
    required precision: the algorithm converges very quickly.

*/
{
vector ii, x1, r0, xa, xb, xd;/* ii is the misclosure vector.
				 x1 is the improved receiver pos (returned).
				 r0 are the ranges from receiver to sat.
				 xa is used for storing sat positions.
				 xb is used for storing approx receiver pos.
				 xd is the correction from matrix equation. */

matrix m1, m2;  /*  m1 is the matrix for the linearised system.
		    m2 is the inverted matrix.                         */
int i,j;

   xa.n=3;  /*  Each coordinate is a vector of length 3 only           */
   xb.n=3;  /*  This is to the approx receiver position                */
   r0.n=4;  /*  There are 4 approximate pseudoranges- receiver to sat. */

   for (j=0;j<3;j++)
	xb.a[j]=x0.a[j]; /*  Assign the first 3 coords of x0 (which  */
			 /*  is length 4), to xb, to get vector      */
			 /*  of length 3 - for receiver position.    */


   for (i=0;i<4;i++) {  /*  Find each of the approx. pseudoranges  */
	for (j=0;j<3;j++)
	     xa.a[j]=s.a[i][j];  /*  Assign i-th satellite position to xa  */
				 /*  -have to do this to pass a vector of  */
				 /*  length 3.                             */

	r0.a[i]=frange(xa,xb);  /*  Find the approx. pseudo-r for sat. i */
   }

   x0.a[3]=0;  /*  Has to be zeroed because the receiver clock error  */
	       /*  is re-calculated each time.                       */

   m1.n=4;  /*  The matrix always has dimensions 4X4             */
   m1.m=4;

   for (i=0;i<4;i++) {     /*  Find the matrix for the linearised system */
       for (j=0;j<3;j++)
	   m1.a[i][j]=(x0.a[j]-s.a[i][j])/r0.a[i];
   }
   for (i=0;i<4;i++)     /*  4th column of the matrix is all -c  */
       m1.a[i][3] = -c;

   ii = svecvec(prs,r0);       /*  Find the misclosure vector  */
			       /*  prs and r0 both have length 4  */

   m2 = matinv(m1);      /*  Find the matrix inverse                      */
   xd = matvec(m2,ii);   /*  Calculate correction for earth position x    */
   x1 = avecvec(x0,xd);  /*  New, corrected earth position                */
   return(x1);      /*  length is 4 - 4th element is receiver clock error */
}
/*--------------------------------------------------------------------*/
vector eph2uvw(ephemeris eph, double t)
/*  Takes ephemeris data for one satellite and a fully corrected time
    for that satellite, and calculates corresponding satellite
    position. 'Fully corrected' means corrections for transit time,
    satellite clock error, etc. The time t is the absolute time, in
    seconds, since the last reference epoch, and is corrected for
    end-of-week crossovers. The time received from satellites is GPS
    time, which is leap-seconds out from UTC. No correction for this
    is necessary, because all parameters are referenced to GPS time,
    which receivers can obtain to within milliseconds (and thereafter,
    microseconds). The returned data (in vector type) is the fixed,
    earth-referenced cartesian position (u,v,w) of the satellite.

    The reason that time corrections are not performed in eph2uvw is
    that the pseudoranges are needed to correct for transit-time:
    to do all the corrections in eph2uvw would therefore required
    the pseudoranges to be passed to the function. Satellite clock
    corrections could be performed in eph2uvw (only the eph data is
    required to do satellite clock corrections), but it was considered
    preferable to make all the corrections in one place - in fsats, below.
*/
{
vector v0;  /*  Vector which will be used to return the satellite position  */
nroot keplers; /*  Structure which will receive the solution to kepler's eq.*/
long int toe;         /*  See the ephemeris data structure definition for   */
/* int prn, wntoe, aode;*/  /*  a description of all these variables.             */
double a, e, m0, omegadt, omega0, sinw, cosw, n, dn, i0, idot;
double crc, crs, cuc, cus, cic, cis;
/* double af0, af1, af2, toc, ura;  */

double mu, oedt, n0, tk, mk, ek, cosfk, sinfk, w, fk, pk;   /* Additional  */
double duk, drk, dik, uk, rk, ik, xkd, ykd, ok, xk, yk, zk; /* Variables   */


/*  Somewhat alleviate the pain of not having a 'with' construct in C:  */

/*  Variables which have been commented out have not been used, but DONT
    delete these comments - they may be useful in the future, ura for eg. */

/*  prn =eph.prn;  */        /*  Satellite ID number  */
/*  wntoe =eph.wntoe; */    /*  Week number from original GPS launch date */
toe =eph.toe;        /* Time of ephemeris, eph data taken & ref. to  */
a =eph.a;            /* Square root of semi-major axis. (***)  */
e =eph.e;            /* Eccentricity  */
m0 =eph.m0;          /* Mean anomaly at reference time  */
omegadt =eph.omegadt;/* Rate of right ascension  */
omega0 =eph.omega0;  /* Longitude or asc node of orbit plane (wkly epoch)  */
sinw =eph.sinw;      /* Sine of argument of perigree (***) */
cosw =eph.cosw;      /* Cosine of argument of perigree (***) */
dn =eph.n;            /* Mean motion difference (***) */
i0 =eph.i0;          /* Inclination angle at reference time  */
idot =eph.idot;      /* Rate of inclination angle  */
crc =eph.crc;        /* Ampl of cos harmonic corrn term to orbit radius  */
crs =eph.crs;        /* Ampl of sin harmonic corrn term to orbit radius  */
cuc =eph.cuc;        /* Ampl of cos harmonic corrn term to arg of lat  */
cus =eph.cus;        /* Ampl of sin harmonic corrn term to arg of lat  */
cic =eph.cic;        /* Ampl of cos harmonic corrn term to angle incl  */
cis =eph.cis;        /* Ampl of sin harmonic corrn term to angle incl  */
/* af0 =eph.af0;   */      /* Poly correction term a0 for satellite clock.  */
/* af1 =eph.af1;   */      /* Poly correction term a1 for satellite clock.  */
/* af2 =eph.af2;   */      /* Poly correction term a2 for satellite clock.  */
/* toc =eph.toc;   */      /* Time of capture (equals TOE)  */
/* ura =eph.ura;   */      /* User range accuracy - see ICD. pg. 67  */
/* aode =eph.aode; */      /* Age of ephemeris data  */


mu =3.986005E+14;       /* WGS84 value of earth's universal grav. parameter */
oedt =7.2921151467E-05;  /* (rad/s) WGS84 value of the earth's rotation rate */
/*  There is some disagreement in the value of oedt here - this value is
    from the ICD - see "The Global Positioning System" book, pg. 62 - this
    is presumably where the mistake is - the value quoted there has the
    digit `4' missing.
*/
a = a*a;                /* Semi-major axis    --  ~ 2.656 E+07  */
n0 = sqrt(mu/pow(a,3.0));    /* Computed mean motion - (rad/s).  */

tk = t-(double)toe;             /* Time from ephemeris reference epoch. */
if (tk>302400.0) tk-=604800.0;  /*  Correct for possible end-of         */
if (tk<-302400.0) tk+=604800.0; /*  GPS-week crossovers.                */

n = n0+dn;               /*  Corrected mean motion.  */
mk = m0+n*tk;            /* Mean anomaly   */
keplers=nsolve(1,mk,e);  /* Solve Kepler's equation for  ek   */
ek=keplers.soln;

cosfk = (cos(ek)-e)/(1-e*cos(ek));         /* True anomaly  */
sinfk = sqrt(1-e*e)*sin(ek)/(1-e*cos(ek));

w = angle(cosw,sinw);      /*  Use cos() and sin() to get the angle in  */
fk = angle(cosfk,sinfk);   /*  the correct quadrant.                    */
pk = fk + w ;              /*  Argument of latitude                     */

duk = cus*sin(2*pk)+cuc*cos(2*pk);     /* Argument of latitude correction  */
drk = crc*cos(2*pk)+crs*sin(2*pk);     /* Radius correction  */
dik = cic*cos(2*pk)+cis*sin(2*pk);     /* Correction to inclination  */

uk = pk+duk;                /* Corrected argument of latitude  */
rk = a*(1-e*cos(ek))+drk;   /* Corrected radius                */
ik = i0+dik+idot*tk;        /* Corrected inclination           */

xkd = rk*cos(uk);           /* Positions in orbital plane  */
ykd = rk*sin(uk);           /*   */

ok = omega0+(omegadt-oedt)*tk-oedt*((double)toe);
/* Corrected longitude of ascending node */

xk = xkd*cos(ok)-ykd*cos(ik)*sin(ok);       /* Earth fixed co-ordinates */
yk = xkd*sin(ok)+ykd*cos(ik)*cos(ok);       /*    */
zk = ykd*sin(ik);                           /*    */

v0.n=3;   /*  Dimension of the output vector   */
v0.err=0;
v0.a[0] = xk;
v0.a[1] = yk;
v0.a[2] = zk;

return(v0);  /*  Return the satellite position in earth-fixed rectangular
		 co-ordinates.      */
}
/*-----------------------------------------------------------------------*/
void ginit()
/*  Initialises some global variables here, to avoid cluttering
    main. The variables outmat and nulmat here are used by the
    matinv function.
*/
{
int i,j;


/*-- Global null & return scalar --*/

nuldoub=0.0;
outdoub=nuldoub;

/*--- Global null & return vector -----*/

nulvec.n=0;
nulvec.err=0;
for (i=0;i<MAXN;i++)
	nulvec.a[i]=0.0;

outvec=nulvec;

/*-----  Global null & return matrix ------ */

nulmat.n=0;  /*  MAXN ?  */
nulmat.m=0;
strcpy(nulmat.message," ");
nulmat.err=0;
for (i=0;i<MAXN;i++){
	for (j=0;j<MAXN;j++) {
		nulmat.a[i][j] = 0.0;
	}
}

outmat = nulmat;              /*  Zero contents of outmat */
}
/*-----------------------------------------------------------------------*/
int findrec(double *xdop, vector *xx, ephemeris alleph[4], rinex obs)
/*  Beware: this was one of the earlier functions, and some of the
    here may be old and inappropriate:

    Findrec is a procedure which processes
    GPS (global positioning system) data to produce receiver positions.
    The input data should be the same as that produced by the LEICA
    GPS unit, and comprises 4 sets of ephemeris data and
    one rinex observation. See the "ephemeris" and "rinex" structure
    definitions below for details on how the data should be passed to
    findrec. Findrec assumes that the best 4 satellites for any observation
    have been selected by its calling routine, and that necessary checks on
    data validity, time, etc, have already been done. Findrec operates on a
    "per-observation" basis, ie. it accepts one rinex observation (4
    pseudoranges and one time), and produces one receiver position.

    Prerequisites for the use of findrec are re-iterated more concisely
    in the following:
    Findrec receives 4 sets of ephemeris data and one rinex observation
    record, containing 4 pseudoranges and a time. From these data
    are calculated the receiver position. Data entering this function
    must satisfy the following requirements:
      (i) That the necessary checks have been done, and that data is valid.
	  ie. - time in ephemeris and rinex data are close enough (within
	  an hour or so) and that pseudorange data is valid.
     (ii) That the SV-numbers have been checked in the eph and rinex
	  files to make sure that there is eph data for all the SVs in
	  the pseudorange data.
    (iii) That the best of 4 pseudorange observations have been selected
	  from the rinex file, and that the corresponding 4 eph data
	  records have been selected from the ephemeris file.
     (iv) That the order of the satellite eph data entering this function
	  is the same as the order of the pseudoranges.
    From this one set of observational data, one receiver position
    is returned in vector format. The position is in earth-fixed
    rectangular cartesion coordinates, conforming to the WGS84 model.
    The dilution of precision is returned in *xdop.

    Two data items are presently passed implicitly: cobs and robs. These
    are used in pr differencing. A future improvement could be to pass the
    explicitly (through the argument list).
*/
{
double a;  /*  Wench double. For checking convergence.  */
double eps=1e-12;  /*  Threshold for convergence test (no. of sig. digits) */
int i,j;
// double c = 2.99792458E+08;    /*  Speed of light. - got it elsewhere  */
vector rright; /* approximate receiver pos - starting point for iteration   */
//double t;    /* time of observation according to rinex data                 */
//	     /* ^^^ not used at present: obs.t is used directly             */
vector tk;   /* The tk parameter for each satellite, which is (t-toe),
		corrected for satellite clock errors                        */
rinex obsc;  /* The input pseudoranges obs, corrected for sat clock errors. */
vector x0;   /* Dummy vector, starting value for findk - to make sure
		findk is converging, even with a bad starting point         */
vector x1;   /* Will hold the returned value for findrec - receiver pos.    */
matrix sats; /* The 4 satellite positions, calculated by fsats, to be
		used by findk.                                              */
vector prs;  /* Defined only to match data types - need to pass 4 pseudo-
		ranges in obsc. to a vector prs in findk. Notice later
		on that prs values get assigned to obsc - could've left
		out obsc and put values directly into prs. Did it this way
		so that obsc, a complete, corrected rinex variable, can
		be passed to somewhere else if necessary.                   */
vector xp,xc;/* Vectors for doing the convergence check.        */
int noconv;  /* Integer signifying that convergence has taken place (if =0) */

prs.n=4;     /*  Dimension prs as a vector of length 4                      */
/*  Enter constant data into approx receiver position vector:               */
rright.n = 3;  /*  size of vector (3 coordintaes)                           */
rright.a[0] = 5247000;   /*  5060019;      all in metres  */
rright.a[1] = 2508000;   /*  2706341;    */
rright.a[2] =-2611000;   /*  -2778761;   */

tk.n=4;    /*  Make the vector hold tk for the 4 satellites in the 4  */
tk.err=0;  /*  ephemeris records                                      */

/*  Create initial position which is in error by a smallish amount:      */

x0=xjohannesburg;

//printf("\nx0.n = %d",x0.n);
//printf("\nx0.a[0] = %f",x0.a[0]);
//printf("\nx0.a[1] = %f",x0.a[1]);
//printf("\nx0.a[2] = %f",x0.a[2]);
//printf("\nx0.a[3] = %f",x0.a[3]);
//wait();

   x0.n=4;
   x0.a[0]=  5247000;
   x0.a[1]=  2508000;
   x0.a[2]= -2611000;
   x0.a[3]=0;


/*  Position which is close to Johannesburg. The vector xjohannesburg
    is defined in the main c file. Using a vector close to the correct
    values ensures quick convergence and the least probability of
    divergence, for bad DOPs. For use elsewhere, define co-ords in main
    also, and simply assign them here, eg. x0=xbrisbane;
*/


/*  A small problem has appeared here - under the conditions listed below,
    the program (in fact, the whole PC) hangs thoroughly.

    data used: in zz.92n and zz.92o (the offending point is 6:52:30, or
    t = 456 750)

    over here, initial value of x0.a[] are:  1, 1, 1, 0

    don't set xx (returned) to 0 after the convergence check fails - try to
    pass back the bad values (which are huge).

    The hanging appears to be caused by attempting to pass back the
    bad values - (eg. it goes away when fixed, well-behaved values are
    passed back) - but WHY should this hang the program? My feeling
    is that its a number representation problem (check typecasting in
    this function, and findk, etc) or otherwise a similar, or
    variable-passing problem inside findk - with its matinv, and passing
    matrices, etc.
*/

//x0.a[0]= 1;
//x0.a[1]= 1;
//x0.a[2]= 1;

x1.n=4;

xp.n=4;
xc.n=4;

sats.n=4; /*  4 rows for 4 satellites  */
sats.m=3; /*  3 co-ordinates per satellite. These m and n values are
	      re-assigned when fsats is called, anyway */

obsc.t=obs.t;
//t=obs.t;  /*  Not currently used  */

/* Satellite clock corrections: this is what CM refers to as
   pseudo-range correction for satellite clock errors (note (b)).
   This is a major correction for satellite clock offset and drift,
   and accounts for the difference between each satellite's time and
   true GPS time. The correction is applied to the pseudorange because
   this is the parameter in which the satellite clock error shows up.

   The reason this is a major correction is that the time
   errors multiply by the *speed of light* to give range errors -
   so the range errors are large, of the order of hundreds of
   kilometres. (see below for smaller errors).
*/

/*  Conceptual: pr = pr + ( af0 + (t-toe)*af1 + (t-toe)^2*af2 )*c  */

for (i=0;i<4;i++) {
    tk.a[i]=obs.t-(double)alleph[i].toe;       /*  Find tk for each satellite  */
    if (tk.a[i]>302400.0) tk.a[i]-=604800.0;  /*  Correct for possible end-of */
    if (tk.a[i]<-302400.0) tk.a[i]+=604800.0;  /*  GPS-week crossovers         */
      /* Calculate pseudoranges corrected for the satellite clock errors:  */
    obsc.prs[i] = obs.prs[i]+(alleph[i].af0+tk.a[i]*alleph[i].af1)*c;
    obsc.prs[i] += tk.a[i]*tk.a[i]*alleph[i].af2*c;
     /* Above line may be redundant, because af2=0 in most data. */
     /*  "obsc", derived here, is for use in findk()        */
}


for (i=0;i<4;i++)
	prs.a[i]=obsc.prs[i];  /*  Assign the corrected pseudoranges
				   to a variable called prs, for input
				   to the findk algorithm. This is done
				   because findk requires its prs input
				   to be of type vector.                  */

sats = fsats(alleph, obs);  /*  Given the same ephemeris data and rinex
				observation that was passed to findrec,
				calculate the 4 satellite positions. -
				fsats() applies its own corrections.  */


/*  Set up the variables cobs and csats, which will be passed out of this
    function implicitly, for use in the pseudo-range differencing
    calculations (if applicable):  see comments in main program.   */

csats=sats;  /*  Make this assignment specifically for the stationary
		 GPS unit in the pseudorange differential mode. - so that
		 the "correct" range from stat to sats can be found, in
		 the main program.        */
cobs=obsc;   /*  Assign the pseudoranges for the stat unit, corrected for
		 satellite clock errors. (See comment above, for csats1,
		 and also see the description of the pr-diff method in the
		 main program.)           */

//cobs.t=obs.t;
//for (i=0;i<4;i++) {
//	prs.a[i]=prs.a[i]+x1.a[3]*c;
//	cobs.prs[i]=prs.a[i];
//}
//for (i=0;i<4;i++) {
//   for (j=0;j<3;j++) csats[i][j] = sats[i][j];
//}

/*  Comments on the for loop below:
    You get 1 to 2 decimal places added accuracy per iteration. To be
    safe, iterate about 8 to 10 times here. The algorithm appears to converge
    quicker near the end. A future improvement could be to check for
    convergence. See comments in the "findk" function for more detail.
*/
for (i=0;i<10;i++) {    /*  10 iterations - keep this here to limit no. its. */
			/*  (was 8)     */
	x1 = findk(sats,prs,x0);  /*  Solve the linearised system          */
	xp=x0;
	x0=x1; /* Feed previous result back to the input, for the next loop */
// printf("\nGot here!!!!!!!!: %f %f %f",xp.a[0],xp.a[1],xp.a[2]);
//wait();
/*  Could improve things a bit here by testing for convergence - instead of
    having a fixed 8 loops. Convergence to acceptable precision usually
    takes place within about 5 iterations, so some time could be saved
    by building the convergence test below, into this loop.      */
/*  To Wit:   */

	a = abs1(x1.a[0]/xp.a[0]-1);
	a += abs1(x1.a[1]/xp.a[1]-1);
	a += abs1(x1.a[2]/xp.a[2]-1);
	if (a < eps) {
//	   printf("\nNo. of loops to conv = %d", i);
	   break;
	}

}
*xdop=gdop(sats, x0);  /* Calculate and return the dilution of precision. */
// printf("\nGDOP = %f", *xdop);
//*xdop=2.3;

/*  Convergence check: check only the 3 co-ords. */
xc=dvecvec(x1,xp);  /*  Divide elements of the vectors containing the
			last 2 steps in the iterations.       */
noconv=0;

if (abs1((xc.a[0]-1))>(1E-10)) noconv=1;  /*  If the last 2 iterates don't      */
if (abs1((xc.a[1]-1))>(1E-10)) noconv=1;  /*  agree to 10 significant digits    */
if (abs1((xc.a[2]-1))>(1E-10)) noconv=1;  /*  in all 3 elements, then consider  */
					  /*  the process unsuccessful, and ... */

//   if (i>8) noconv=1;       /*  Better ? .. fits in with earlier conv chk */


/* .................................... */
//  /*  The code in this section (between the dotted lines) is NOT
//      being used, and is unlikely to be used in the future - the
//      reasoning given in the paragraph below (marked ***),
//      fortunately does not hold in practise: a correction
//      to the pr for the uncorrected receiver clock error is NOT
//      necessary, because the receiver clock error is built into
//      t, the time at which the observation was made. The problem is
//      therefore 'self-correcting'. If you want positive proof of this,
//      just include this code in the compile. Then run it on real data -
//      watch the result when the algorithm switches satellites, the
//      error will be huge, evidence that the ephemeris is not correct
//      with this code switched in.       */
//
// if (!noconv) {
///*  *** Correct the pseudoranges by the amount of the receiver clock offset.
//    This is for the benefit of stupid units such as the Garmin, which
//    do not correct their clocks, thereby causing hassles such as this one.
//    The Garmin clock offset can be 35000km or more, which is 100ms or more.
//    In this amount of time, a satellite can move (600m/s)*(0.1s) = 60m
//    or more. Thus, the clock offset of the Garmin as described above, can
//    translate to errors of 60m or more in the range. To correct for this,
//    the code between these two half-ruled divisions has been added: it
//    corrects the pseudoranges prs.a[i] for the satellite clock error
//    x1.a[3] and then re-calculates the satellite positions sats. The receiver
//    position is then re-calculated. (The whole thing is very iterative).
//*/
//
//cobs.t=obs.t;
//
//for (i=0;i<4;i++) {
//	prs.a[i]=prs.a[i]+x1.a[3]*c;
//	cobs.prs[i]=prs.a[i];
//// printf("\nCorrected pr is %f ",cobs.prs[i]);
//}
//
//csats = fsats(alleph, cobs);
//
// for (i=0;i<8;i++) {    /*  8 iterations  */
//	x1 = findk(csats,prs,x0);  /*  Find receiver position again,      */
//	xp=x0;                     /*  for improved satellite positions.  */
//	x0=x1;
//
//	 /*  Do a convergence check: if the sum of the errors in the
//	     3 co-ordinates is better than our threshold, the exit
//	     the loop.      */
//	a = abs1(x1.a[0]/xp.a[0]-1);
//	a += abs1(x1.a[1]/xp.a[1]-1);
//	a += abs1(x1.a[2]/xp.a[2]-1);
//	if (a < eps) {
////	   printf("\nNo. of loops to conv = %d", i);
//	   break;
//	}
// }
//}
///* ................................... */
if (noconv) {  /*  Set all the returned values to 0 to signify        */
   x1.a[0]=0;  /*  that the process failed to converge. In main       */
   x1.a[1]=0;  /*  the value returned by the function should be       */
   x1.a[2]=0;  /*  checked, and if convergence failed, this data      */
   x1.a[3]=0;  /*  should be discarded.                               */
}

//x1.a[0]=1;
//x1.a[1]=2;
//x1.a[2]=3;
//x1.a[3]=4;

//printf("\nReturning the following: %f %f %f",x1.a[0],x1.a[1],x1.a[2]);
//wait();

xx->a[0] = x1.a[0];    /*   Assign the returned values  */
xx->a[1] = x1.a[1];    /*   x1 at this point is the final estimate  */
xx->a[2] = x1.a[2];    /*   of the receiver position.               */
xx->a[3] = x1.a[3];

return(!noconv);  /*  Return 1 for convergence,  0 for no convergence  */
}
/*------------------------------------------------------------------------*/
matrix fsats(ephemeris alleph[4], rinex obs)
/*  This function takes a set of 4 ephemeris records and a rinex
    observation, and returns the positions of the 4 satellites for
    the time t contained in the rinex observation. The data is
    returned in a 4X3 matrix structure. The reason the rinex data
    is required (specifically, the pseudoranges), is that to
    calculate the satellite position correctly, the time of
    *transmission* for each pseudorange is needed. This can be
    deduced only from the pseudorange itself, the correction to t
    to find the time of transmission is  t := t-(pseudorange)/c. This
    correction is referred to as the "correction for signal transit time".
    Further correction terms from the eph data (see below) are required,
    before t can be used in the ephemeris algorithm "eph2uvw()".

    Satellite clock corrections have a small effect on the calculated
    satellite position. They are done in the function findrec() which
    calls fsats(), and are *repeated* here so that any other function
    can call fsats() with eph and raw (plain, as in straight from rinex
    file) pseudoranges "obs", and get correct satellite positions. It
    would have been possible to write this function so that it makes
    use of the clock corrections already done in findrec(), by passing
    it the corrected pseudoranges in "obsc". t1..4 for each position
    would then simply have been
				t1 = t - (obs.prs[1])/c;  etc.,
    where obsc in findrec() is passed directly to obs in this routine.
    However, it was felt that the advantages of putting the extra
    computing into fsats far outweighs the overheads.
*/
{


/*  More notes on the nature of the clock correction:
    This is what CM refers to as correction TO the satellite
    clock time (note (a)). I'll call it 'clock correction for SV position.'
    This is a minor correction which takes account of transit time of
    the signal and satellite clock errors, to get a better estimate of
    the satellite position: the SV position has to be calculated for the
    GPS time at which the signal was transmitted, not the time at which
    it was received. This is a minor correction because the time difference
    multiplies the satellite speed (not the speed of light). Transit
    times are of the order of a hundred milliseconds, and satellite
    clock offsets are kept to within a millisecond of GPS time, so the total
    correction to the time is of the order of a hundred milliseconds.
    This would translate to around 100m in the range.

    Note that in this step, both transit time and satellite clock
    corrections are made.
*/


matrix mx;      /*  A  4X3 matrix in which the data is returned - 4
		    satellite positions, 3 co-ordinates each.               */
vector spos[4]; /*  An array of 4 vectors, holding the positions (u,v,w)
		    of the 4 satellites in ephemeris set alleph             */
rinex obsc;     /*  As in findrec, the pseudoranges after correction
		    for satellite clock errors                              */
vector tk;      /*  The tk parameter for each satellite, which is (t-toe),
		    corrected for satellite clock errors. tk, as derived
		    here, is used only to do transit time and sat clk
		    corrections for satellite positional corrections.       */

int i,j,k;
double t0, t1, t2, t3;  /*  The 4 corrected times which will be used
			    in the ephemeris algorithm, to calculate
			    satellite positions.  */

tk.n=4;   /*  Dimension the tk vector - 4 time values */
tk.err=0; /*  Just to keep things sweet  */

obsc.t=obs.t;  /*  Keep obsc complete by giving it the time value, as well */
mx.n=4;  /* Dimension the matrix which will return the satellite positions */
mx.m=3;  /*   4X3 dimension  */
mx.err=0;

/*  Conceptual:  tx = t - pr/c + af0 + (t - toe)*af1 + (t - toe)^2*af2  */


/*  Correct pseudoranges for sat clock errors:  (only so that we can
    use it indirectly - see below - to calculate sat times for ephemeris
    algorithm).  */

for (i=0;i<4;i++) {
    tk.a[i]=obs.t-(double)alleph[i].toe;   /*  Find tk for each satellite  */
    if (tk.a[i]>302400.0) tk.a[i]-=604800.0;  /*  Correct for possible end-of */
    if (tk.a[i]<-302400.0) tk.a[i]+=604800.0;  /*  GPS-week crossovers         */
      /* Get corrected pseudoranges - for the satellite clock errors:  */
    obsc.prs[i] = obs.prs[i]+(alleph[i].af0+tk.a[i]*alleph[i].af1)*c;
    obsc.prs[i] += tk.a[i]*tk.a[i]*alleph[i].af2*c;
     /* Above line may be redundant, because af2=0 in most data. */
}
/*  The correction of tk for end-of-week crossovers here needs to be
    repeated again in eph2uvw - because it is done here only for
    calculating pseudorange corrections, and tk is not passed on
    to eph2uvw. tk is re-calculated in eph2uvw.  */

t0 = obs.t - (obsc.prs[0]/c);  /* Find the exact time to be used in eph.   */
t1 = obs.t - (obsc.prs[1]/c);  /* algorithm, for each satellite. The time  */
t2 = obs.t - (obsc.prs[2]/c);  /* is corrected for sat clk error and       */
t3 = obs.t - (obsc.prs[3]/c);  /* signal transit time.                     */

/*  No need to correct for sat clock errors in eph2uvw - the above
    step does it - t0, t1, t2, t3 are the corrected values  */

spos[0]=eph2uvw(alleph[0],t0);  /*  Earth-ref'd position uvw of satellite 1 */
spos[1]=eph2uvw(alleph[1],t1);  /*  Position of satellite 2, etc.    */
spos[2]=eph2uvw(alleph[2],t2);
spos[3]=eph2uvw(alleph[3],t3);


/*  Return satellite positions in a matrix  */
for (i=0;i<4;i++) {
	for (j=0;j<3;j++)
		mx.a[i][j]=spos[i].a[j];  /* Assign coords to a matrix */
}
return(mx);
}
/*-----------------------------------------------------------------------*/
int ephdisp(ephemeris *eph)
/* 12/1/93. Display, in an easy-to-read manner, the contents of any data of
    type ephemeris. For development work.    */
{
printf("\n        Contents of ephemeris variable:  ");
printf("\n        ------------------------------- \n");
printf("\nSat ID number                              prn: %d",   eph->prn);
printf("\nGPS week number                          wntoe: %d",   eph->wntoe);
printf("\nTime of ephemeris                          toe: %ld",  eph->toe);
printf("\nSqrt semi-major axis a1/2                    a: %e",   eph->a);
printf("\nEccentricity                                 e: %e",   eph->e);
printf("\nMean anomaly                                m0: %e",   eph->m0);
printf("\nRate of right ascension                omegadt: %e",   eph->omegadt);
printf("\nAscending node of orbit plane           omega0: %e",   eph->omega0);
printf("\nSin arg of perigree                       sinw: %e",   eph->sinw);
printf("\nCos arg of perigree                       cosw: %e",   eph->cosw);
printf("\nMean motion difference (n)                  dn: %e",   eph->n);
printf("\nInclination angle                           i0: %e",   eph->i0);
printf("\nRate of inclination angle                 idot: %e",   eph->idot);
printf("\nAmpl of cos harmonic corr term orbit       crc: %e",   eph->crc);
printf("\nAmpl of sin harmonic corr term orbit       crs: %e",   eph->crs);
printf("\nAmpl of cos harmonic corr term arg lat     cuc: %e",   eph->cuc);
printf("\nAmpl of sin harmonic corr term arg lat     cus: %e",   eph->cus);
printf("\nAmpl of cos harmonic corr term angle incl  cic: %e",   eph->cic);
printf("\nAmpl of sin harmonic corr term angle incl  cis: %e",   eph->cis);
wait();
printf("\nClock correction term a0                    a0: %e",   eph->af0);
printf("\nClock correction term a1                    a1: %e",   eph->af1);
printf("\nClock correction term a2                    a2: %e",   eph->af2);
printf("\nTime of capture                            toc: %e",   eph->toc);
printf("\nura                                        ura: %e",   eph->ura);
printf("\nAge of ephemeris data                     aode: %d",   eph->aode);
printf("\n");
wait();
return(1);
}
/*-----------------------------------------------------------------------*/
int uvw3geo(double u, double v, double w, double *Po, double *Lo, double *ho)
/*  18/02/93.  My own version of uvw2geo, translated directly from
    my m-file which performs the same function. uvw3geo takes 3 earth-fixed
    rectangular co-ordinates and converts them to polar co-ordinates,
    latitude (P), longitude (L) and altitude (h), according to the
    WGS84 model. Convergence checking here is a bit more robust than
    in uvw2geo.

    No parameters are passed implicitly (except standard globals).

    The function returns 0 if the process converged and 1 if it didn't.

*/
{
double P, L, h;
double a, f, r1, e2, P1, N;
int lcount, noconv;  /*  Count no. of iterations; noconv=1 signifies that
			 the process didn't converge.        */

   a=6378137.0;             /*  Parameters for WGS84 ellipsoid   */
   f=1/298.257223563;

   e2 = 2*f - f*f;          /*  Setup `constants'.    */
   r1 = sqrt(u*u + v*v);

/*--------  Calculate  P, L, h:     */

   L = atan(v/u);
   P1 = atan( w/((1-e2)*r1));

   noconv=0;
   lcount=0;
   while (1) {
      N = a/sqrt(1-e2*sin(P1)*sin(P1));
      P = atan((w/r1)*(1+(e2*N*sin(P1))/w));

      if (abs1(1-P1/P)<1E-12)
	 break;
      if ((lcount>100)&&(abs(1-P1/P)>1E-10)) {  /*  What to do if it didn't */
	 noconv=1;				/*  converge.     */
	 break;
      }
      P1=P;
      lcount++;
   }

// printf("\n Convergence of uvw3geo in %d steps.",lcount);  /*  0 !! */

   h = r1/cos(P) - N;

   *Po = P * 180/pi;
   *Lo = L * 180/pi;
   *ho = h;

   return(noconv);
}
/*-----------------------------------------------------------------------*/
double gdop(matrix s, vector x0)
/*  24/2/93. See CLM's book, page 46.
    Given approximate satellite positions s and approximate receiver
    position x0, gdop finds the dilution of prescision for the given
    geometry (satellite and receiver positions). The receiver position
    is non-critical, and an approximate value may be used (Say, within
    kilometres). Similarly, sat positions may be out by kilometres, as
    with old nav or almanac data.

    This function generates the "A" matrix which appears in the equations
    for DOP calculation and receiver position calculation - so, some
    of the code from findk() is repeated here (go, now, and look at
    the findk() function). The format of the satellite positions in
    the matrix s is defined in findk()'s calling functions.

    Assuming equal weighting in least squares method,
	gdop = sqrt(trace(n^-1))  where  n=A'*A
    pdop is similarly defined, but includes only the first 3 terms
    of n^-1 (excluding the time offset term)

    (' denotes transposition and * matrix multiplication)

*/
{
double result;
int i,j;
vector r0, xa, xb;  /*  r0 are the ranges from receiver to sat; xa and
			xb are for storing sat and receiver positions,
			respectively.  */

matrix m1;  /*  m1 is the matrix for the linearised system.   */
matrix n, p, q;  /*  p and q are two general-purpose matrices, used
		     for storing intermediate results below:  */

   /*  The bounds of p are defined in the transpose function.  */
   /*  The bounds of q are defined by matmat.  */
   /*  The bounds of n are defined by matinv.  */

   xa.n=3;  /*  Each coordinate is a vector of length 3 only           */
   xb.n=3;  /*  This is to the approx receiver position                */
   r0.n=4;  /*  There are 4 approximate pseudoranges- receiver to sat. */

   for (j=0;j<3;j++)
      xb.a[j]=x0.a[j]; /*  Assign the first 3 coords of x0 (which  */
		       /*  is length 4), to xb, to get vector      */
		       /*  of length 3 - for receiver position.    */


   for (i=0;i<4;i++) {  /*  Find each of the approx. pseudoranges  */
      for (j=0;j<3;j++)
	 xa.a[j]=s.a[i][j];  /*  Assign i-th satellite position to xa  */
			     /*  -have to do this to pass a vector of  */
			     /*  length 3.                             */

	 r0.a[i]=frange(xa,xb);  /*  Find the approx. pseudo-r for sat. i */
   }

   x0.a[3]=0;  /*  Has to be zeroed because the receiver clock error  */
	       /*  is re-calculated each time.                       */

   m1.n=4;  /*  The matrix has dimensions 4X4 - this will change       */
   m1.m=4;  /*  if the least squares method is used (more than 4 sats) */
   for (i=0;i<4;i++) {     /*  Find the matrix for the linearised system */
      for (j=0;j<3;j++)
	 m1.a[i][j]=(x0.a[j]-s.a[i][j])/r0.a[i];
   }
   for (i=0;i<4;i++)     /*  4th column of the matrix is all -c  */
      m1.a[i][3] = -c;   /*  m1 is the "A" matrix.  */

   transpose(&p, &m1); /*  Form the transpose of *a in p   */
   q = matmat(&p, &m1); /*  Multiply the two matrices, ie. n = a'*a    */
   n = matinv(q); /*  Find n^-1    */
   result = sqrt(trace(&n));  /*  Find the gdop  */
   return(result);   /*  Thats all, folks. */
}
/*-----------------------------------------------------------------------*/
double elevation(vector *s, vector *p)
/*  24/02/93.  Given a single satellite's coordinates in vector *s
    and the receiver position in vector *p, elevation() finds the
    elevation of the satellite above the horizon. ie. the angle between
    the straight line joining sat & receiver, and the horizontal, at the
    receiver. The sat and receiver positions may be approximate, only
    a rough estimate of the angle is required (to within a degree, say).
    This function allows the "mask angle" criterion to be applied to
    satellites. All coordinates (in *s and *p) must be earth-fixed
    rectangular co-ordinates. The returned angle is in degrees.

    The vector equation which gives the angle of elevation (phi) is:

		    (S - P) o P
       Sin(phi) =   -----------
		    |S - P|.|P|

   Where S and P are the vectors giving the satellite and receiver
   positions, respectively,  o  denotes the inner product (dot
   product) operation,  and |P| denotes the magnitude of vector P.

   Danger: the definition of *s and *p should set *s.n=3 and *p.n=3 (both
   the same), otherwise the vector subtract and dot product functions
   won't work.
*/
{
double result;
double x, y; /*  Doubles for storing intermediate results. */
vector a;  /*  Vector for storing temporary result.  */

   p->n = 3;  /*  Brute force. Because rl is length 4; only want 1st 3  */
   s->n = 3;
   /*  svecvec() sets the length of a to 3.     */

   a = svecvec(*s, *p);
   x = vecvec(a, *p);
   y = mag(a)*mag(*p);
   result = asin(x/y)*180/pi;
   return(result);

}
/*-----------------------------------------------------------------------*/
int onesat(vector *s, ephemeris oneph, double t)
/*  1/3/93. This function takes one eph record and one time (t is time
    since beginning of GPS week, as usual), and calculates that
    satellite's position for that time. The position is returned
    in *s, a vector of length 3. BEWARE of replacing fsats with
    this (go now, and have a look at fsats() and the comments contained
    therein). To use this function for accurate sat position calcs,
    the time has to be exact time of transmission, which needs to
    involve the pseudoranges measured at the receiver - Read the
    explanantion in fsats(). If you're confident that the time you
    pass to onesat() is really the time you want the sat pos for
    (think carefully about this) - the go ahead, use it. Remember,
    caveat emptor!
       The intended use of this function is in calculating dops and
    elevations (by  maskselect() and dopselect()), which don't require
    much time accuracy (being out by the order of seconds is ok). Just
    in case you do want a lot of accuracy, however, the satellite
    clock correction calculations have been retained here.
	The function eph2uvw() could just as well have been used.
    The only reason for having a separate function onesat() here, is to
    do the sat clock correction, which MAY be useful at some time
    in the future.

*/

{
vector spos;    /*  A vector, holding the position (u,v,w)
		    of the satellite.	     */
double tk;      /*  The tk parameter for the satellite, which is (t-toe),
		    corrected for satellite clock error. */
double t0;      /*  The corrected time which will be used in the ephemeris
		    algorithm, to calculate satellite position.  */


spos.n=3;  /* Dimension the vector which will return the satellite position */

/*  The correction of tk for end-of-week crossovers here needs to be
    repeated again in eph2uvw - because it is done here only for
    calculating sat clock corrections;  tk is not passed on
    to eph2uvw. */

    tk = t-(double)oneph.toe;   /*  Find tk for the satellite  */
    if (tk>302400.0) tk-=604800.0;  /*  Correct for possible end-of */
    if (tk<-302400.0) tk=604800.0;  /*  GPS-week crossovers         */


/*  Conceptual:  tx = t - pr/c + af0 + (t - toe)*af1 + (t - toe)^2*af2  */
t0 = t - (oneph.af0+tk*oneph.af1+tk*tk*oneph.af2);

/*  No need to correct for sat clock errors in eph2uvw - the above
    step does it. */

spos = eph2uvw(oneph,t0);  /*  Earth-ref'd position uvw of the satellite  */
*s = spos;

//printf("\nt0 is %f ",t0);
//ephdisp(&oneph);
//printf("\nposition: %f %f %f", spos.a[0], spos.a[1], spos.a[2]);
//wait();


return(1);
}
/*------------------------------------------------------------------------*/
int maskselect()
/*  1/3/93.  This function takes a set of satellites and their eph data
    and calculates the elevation of each satellite with respect to the
    receiver. It calculates a new list of satellites from the old one -
    the new list is identical to the old one, except that it does not
    contain any satellites which have an elevation of less than
    MASKMIN degrees.
       The receiver position comes from `rl', a global vector containing
    the latest receiver position calculated by findrec(). `rl' doesn't
    have to be very accurate because elevation and dop don't need to
    be very accurate.
       The strategy of overwriting the exsiting data structure with
    itself (ie. not introducing a new or temp structure) works because
    we know that the revised structure will always be smaller than the
    orignal one. Hence, the following:

    Implicit inputs:
       rl     (the latest receiver position)
       t_now  (the present time)

       zthesats[0][0]  (the number of sats available)
       zthesats[][]
       *zaeph[]
       zanobs[]

    Implicit outputs:

       All the data in the input data list above, but modified.

    The number (stored in zthesats[0][0]) of available satellites is
    reduced by the number of satellites which do not pass the elevation
    angle requirement. All the other output data is reduced accordingly,
    but the correspondence between elements in zthesats, *zaeph[]
    and anobs is maintained, so that all the data can be treated
    exactly as before (after this function has been called). This
    is what makes it possible to omit/include this function in
    checkselect, willy nilly.

*/
{
vector phis;    /*  Contains the set of elevation angles of the sats.  */
int i,j,N,passcount;
vector s;       /*  Calculated satellite position.          */

   N=zthesats[0][0];
   phis.n=N;

//   printf("\nSatellites:");  /*  Keep these displays here !  */
//   for (i=0;i<N;i++)
//      printf(" %d",zthesats[0][i+1]);
//   printf("\nSatellite elevations:");
   passcount=0;
   for (i=0;i<N;i++) {
	/*  onesat() sets the length of s to 3   */
	/*  See the comment in this position, in maskselect12().  */
	/*  Find this sat's position - could use t_now instead of anobs.t.*/
	onesat(&s, *zaeph[i], t_now);
	/*  We don't really need to create the following array, but just
	    do it here, it may be useful later:  */
	phis.a[i] = elevation(&s, &rl);  /*  Find sats's elevation angle. */

	/*  This is the meat:  */
	if (phis.a[i]>MASKMIN) {
	   /*  Fill up the existing arrays with data, from the bottom,
	       element-by-element, only with elements which pass the
	       elevation angle requirement:   */
	   zthesats[0][passcount+1]=zthesats[0][i+1];
	   zthesats[1][passcount+1]=zthesats[1][i+1];
	   zaeph[passcount]=zaeph[i];
	   zanobs[passcount]=zanobs[i];
	   passcount++;
	}
	else {
	   printf("\nSatellite %d was rejected on elevation.", zthesats[0][i+1]);
	}
	/*  At this point, passcount holds the total number of satellites
	    which pass the elevation angle criterion. Copy it into
	    zthesats[0][0]:   */
	zthesats[0][0]=passcount;
//	printf(" %6.2f", phis.a[i]);
   }
   /*  That concludes the proceedings.  (what a stupid comment).  */
   return(1);
}
/*------------------------------------------------------------------------*/
int maskselect12()
/*  2/3/93.  All the comments of maskselect() apply here. Only the
    input and output data structures are different, and these are
    listed here:

    Implicit inputs:
       rl     (the latest receiver position)
       t_now  (the present time)

       comcount (the number of satellites available)
       thesats1[][]
       thesats2[][]
       *aephx[]
       anobsx1
       anobsx2

    Implicit outputs:

       All the data in the input list above, but modified.

    The number (stored in comcount) of available satellites is
    reduced by the number of satellites which do not pass the elevation
    angle requirement. All the other output data is reduced accordingly.
    See comments in maskselect() above.

*/
{
vector phis;    /*  Contains the set of elevation angles of the sats.  */
int i,j,N,passcount;
vector s;       /*  Calculated satellite position.          */

   N=comcount;
   phis.n=N;

//   printf("\nSatellites:");  /*  Keep these displays here !  */
//   for (i=0;i<N;i++)
//      printf(" %d",thesats1[0][i]);
//   printf("\nSatellite elevations:");
   passcount=0;
   for (i=0;i<N;i++) {
	/*  onesat() sets the length of s to 3   */
	/*  Find this sat's position - could use anobs.t instead of t_now.
	    But its `safer' to use t_now - although part of anobs1 is updated
	    before this function is called (the part we want here, anobs1.t),
	    the rest of anobs1.t is only filled in below, in dopselect12() */
	onesat(&s, *aephx[i], t_now);
	/*  We don't really need to create the following array, but just
	    do it here, it may be useful later:  */
	phis.a[i] = elevation(&s, &rl);  /*  Find sats's elevation angle. */
	/*  Remember that the stat and roving units are close together,
	    so the elevation angles of the satellites with respect
	    them are very nearly equal - hence, it is immaterial whether
	    the stat or rov position is used. `rl', as used above
	    is constantly updated with the stat unit's position (could
	    just as well have used rov -- see main(), and rl=result1;  */

	/*  This is the meat:  */
	if (phis.a[i]>MASKMIN) {
	   /*  Fill up the existing arrays with data, from the bottom,
	       element-by-element, only with elements which pass the
	       elevation angle requirement:   */
	   thesats1[0][passcount]=thesats1[0][i];
	   thesats1[1][passcount]=thesats1[1][i];
	   /*  The following two lines are here really only in the
	       interests of preserving the signal strength information,
	       should it be wanted for some reason in the future:  */
	   thesats2[0][passcount]=thesats2[0][i];
	   thesats2[1][passcount]=thesats2[1][i];
	   aephx[passcount]=aephx[i];
	   anobsx1[passcount]=anobsx1[i];
	   anobsx2[passcount]=anobsx2[i];
	   passcount++;
	}
	else {
	   printf("\nSatellite %d was rejected on elevation.", thesats1[0][i]);
	}

	/*  At this point, passcount holds the total number of satellites
	    which pass the elevation angle criterion. Copy it into
	    comcount:   */
	comcount=passcount;
//	printf(" %6.2f", phis.a[i]);
   }
   /*   Done.    */
   return(1);
}
/*------------------------------------------------------------------------*/
int dopselect12()
/*  2/3/93.  All the comments in dopselect() apply here (see below). The only
    difference between the two pieces of code are the data structures
    used. See dopselect() below, for the comments.

  Implicit inputs: (for the purpose of the dop search)
	comcount - the number of satellites found in commons12(). This is
		   the number of valid sats in thesats1/2, anobsx1/2 and
		   *aephx[].
	*aephx[] - Sat ephs for calculating positions.
	rl       - The latest receiver position.
	t_now    - The present time.
  Implicit inputs: (for the purpose of transferring data for the best
  dop, to the output data structures):
	anobsx1[]
	anobsx2[]
	thesats1[][]
	thesats2[][]
	*aephx[]
  Implicit outputs: (data is transferred to these, from the structures
  listed above.)
	anobs1
	anobs2
	thesats[][4]
	aeph[4]

*/
{
double satpos[TRACKMAX][3];  /*  The receiver can't possibly be tracking more
			   than 10 satellites at a time. This matrix
			   stores the positions of all those satellites.
			   Replace 10 with TRACKMAX ? */
matrix foursat;  /*  Matrix of just 4 satellite positions, for
			   passing to gdop.      */
vector s;              /*  To store a satellite position. */

int L[MAXCOMBOS][4];  /*  Satellite combos matrix, to be filled with
		   L1, L2, L3, L4 or L5. */
int i,j,k,N;
int index;
int combos;  /*  The number of possible combinations of satellites. */
double dopset[MAXCOMBOS];  /*  An array of doubles, big enough to hold the
			dops for the worst-case number of satellites
			(8 at the moment)  */
double bestdop;    /*  The value of the best dop, corresponding to bestsats */
int bestsats[4];   /*  Store one row of L - the best possible comination
		       of satellites, by the gdop calculation. */

foursat.n=4;  /*  Number of rows  */
foursat.m=3;  /*  Number of columns  */


N=comcount;   /*  For m==1  (The source of N if in differential mode)  */

/*  The following switch selects appropriate values for L (the matrix
    of satellite combinations) for the number of satellites being
    tracked (ie. no. of pseudoranges we have). combos is the number
    of rows in L (ie. the number of sat combinations, taken 4 at a time).
    L and combos are synthesized manually here, although (in principle)
    it should be possible to generate them with formulae, so that any
    N can be catered for. (N greater than 7 is very unlikely, though).
    The formula for combos, for example, would be:

		     N!
       combos =   --------
		  (N-p)!p!

    Where p is the number of sats taken at a time (almost always 4), N is
    the total number of satellites, and ! denotes the factorial operator.

    SEE MORE COMMENTS IN dopselect() BELOW.
*/

   switch (N) {
      case 4:
	 combos=1;
	 for (i=0;i<combos;i++) {
	    for (k=0;k<4;k++) L[i][k]=L1[i][k];
	 }
	 break;
      case 5:
	 combos=5;
	 for (i=0;i<combos;i++) {
	    for (k=0;k<4;k++) L[i][k]=L2[i][k];
	 }
	 break;
      case 6:
	 combos=15;
	 for (i=0;i<combos;i++) {
	    for (k=0;k<4;k++) L[i][k]=L3[i][k];
	 }
	 break;
      case 7:
	 combos=35;
	 for (i=0;i<combos;i++) {
	    for (k=0;k<4;k++) L[i][k]=L4[i][k];
	 }
	 break;
      case 8:
	 combos=70;
	 for (i=0;i<combos;i++) {
	    for (k=0;k<4;k++) L[i][k]=L5[i][k];
	 }
	 break;
      default:
	 printf("\nToo many sats in dopselect12() !!");
   }


   /*  The following for loop works through all the `eligible'
       satellites, finding their positions and storing them in the satpos
       matrix. The positions will then be taken 4-at-a-time, to find
       dops for all possible combinations of satellites.  */
   for (i=0;i<N;i++) {
      /*  Find the satellite's position:  */
      /*   (below: could us t_now, or anobs1.t ?!)    */
      onesat(&s, *aephx[i], t_now);
      for (j=0;j<3;j++)
	 satpos[i][j] = s.a[j];  /*  Copy the sat's pos into satpos matrix */
   }

   for (i=0;i<combos;i++) {  /*  Work through all possible sat combinations */
			     /*  listed in L.     */
      for (j=0;j<4;j++) { /*  Deal with this particular combination of sats */
	 index=L[i][j];
	 /*  Count out elements with k here:   */
	 for (k=0;k<3;k++) foursat.a[j][k]=satpos[index][k];
      }
      /*  Get the dop for this combo of sats.  Store it in dopset[].
	  dopset[] will be `combos' elements big.   */
      dopset[i]=gdop(foursat, rl);
   }

   bestdop=1E+5;  /*  Start with any ridculous (large, useless) dop here */
   for (i=0;i<combos;i++) {
      if (dopset[i]<bestdop) {  /*  If we find a better dop,  */
	 bestdop = dopset[i];  /*  This one becomes the new bestdop  */
	 for (k=0;k<4;k++) bestsats[k] = L[i][k]; /*  Copy that row of L  */
      }
   }
   /*  bestsats[4] now contains the indexes for the sats which give the
       best dop. The elements of bestsats[] can be used as indexes in
       thesats1/2, anobs1/2 and aephx, to get all the data for the best
       sats into the output data structures (of this funtion). This
       is done below:

       The output data structures for this function are: (see above,
       at start of function).
	   anobs1
	   anobs2
	   thesats[][4]
	   aeph[4]
   */


   /*   Refer to firstselect12() here - the dimensions should be compatible,
	if you know what I mean.   */
   for (i=0;i<4; i++) {
      index=bestsats[i];
      aeph[i]=*aephx[index];
      /*  Copy the prns:  */
      thesats[0][i]=thesats1[0][index];
      /*  Copy the signal strengths: (but I don't think they'll
	  ever be used. Besides, these are the ss for the stationary
	  unit only).   */
      thesats[1][i]=thesats1[1][index];
      /*  Copy the pseudoranges for the stationary and roving units:  */
      anobs1.prs[i]=anobsx1[index];   /*  Make sure that anobs1.t is def'd! */
      anobs2.prs[i]=anobsx2[index];   /* ### - completes anobs1/2 defn.  */
   }
   /*  Thats the lot. Phew.  */
   return(1);
}
/*------------------------------------------------------------------------*/
int dopselect()
/*  2/3/93.  The function of dopselect() is to select the best combination
    of satellites to use in calculating a fix for a given observation.
    This function supersedes the previous function (firstselect()), which
    simply selected the first 4 satellites available. (Beware that this
    can lead to very bad dops, and hence almost unusable results,
    especially where differencing is being relied on.)
    If, for the particular observation, 4 satellites were being tracked,
    there is no choice - those 4 sats have to be used. For more
    satellites, the number of combinations which can be used rises quickly.
    Although there must be a more intelligent way of selecting the best
    sats to use, this function adopts the brute-force method of calculating
    the dop for every possible satellite combination. This can lead to
    quite a high computational overhead, especially where a large number
    of satellites are being tracked. The correspondence between the number
    of satellites and the number of combinations of statellites (taken 4
    at a time) is:

	    Satellites   Combinations
	    ----------   ------------
		4             1
		5             5
		6            15
		7            35
		8            70

    The computational overhead would presumably be of little consequence
    if post-processing is being done on logged data. The payoff associated
    with the overhead is that one is assured of the best possible results
    for the satellites in the record.

    A future alternative to this method (which is quite different), is
    instead of using only 4 satellites at a time, to use more satellites
    in the least-squares method of solution, as described in CLM's book.
    However, the advantages of doing this are not entirely clear, the method
    of assigning weights as yet undecided and the method of implementation
    and interpretation of results in the differencing methods completely
    obscure. Furthermore, it would require considerable changes to the
    software, notably the findrec() function and its associated sub-functions.

  The variables which are implicitly passed to and from the function are:
  Implicit inputs: (for the purpose of the dop search)
	zthesats[0][0] - the number of satellites found in checkselect().
			 This is the number of valid sats in zthesats,
			 zanobs[] and *aephx[].
	*zaeph[] - Sat ephs for calculating positions.
	rl       - The latest receiver position.
	t_now    - The present time.
  Implicit inputs: (for the purpose of transferring data for the best
  dop, to the output data structures):
	zanobs[]
	zthesats[][]
	*zaeph[]
  Implicit outputs: (data is transferred to these, from the structures
  listed above.)
	anobs
	thesats[][4]
	aeph[4]


NB:  Search for TRACKMAX and MAXCOMBOS in this code. These two important
     parameters are defined as follows:

     TRACKMAX - the maximum number of pseudoranges which can be
		recorded for eny given observation. It is presently
		set to 8.
    MAXCOMBOS - The maximum number of combinations of satellites taken
		four at a time, given that the total number of satellites
		is TRACKMAX. MAXCOMBOS is presently set to 70. This figure was
		derived from the following formula, which should be strongly
		adhered to:

				       N!
		  MAXCOMBOS     =   --------
				    (N-p)!p!

		Where N=TRACKMAX.
		NOTE that this hash-defines for MAXCOMBOS and TRACKMAX
		have to be changed MANUALLY, according to this formula,
		in the main file. Also, there is a switch statement (and
		possibly other code!) which has to be changed manually
		as well - look for it.

*/
{
double satpos[TRACKMAX][3];  /*  The satellite positions for all the satellites
			   being tracked, are stored in this matrix. They
			   are then used (in conjunction with the receiver
			   position) to calculated dops for satellite
			   combinations.          */
matrix foursat; /*  Foursat is used to store the positions of four
		    satellites, for the benefit of the gdop function.  */

vector s;  /*  One satellite position comes back in this vector.  */

int L[MAXCOMBOS][4];  /*  Satellite combos matrix, to be filled with
		   L1, L2, L3, L4 or L5. */
int i,j,k,N;
int index;  /*  The meaning of this variable becomes obvious on closer
		examination of the code.  */
int combos;  /*  The number of combinations which has to be worked through
		 (number of dops which has to be found.)  */
double dopset[MAXCOMBOS];  /*  70 is the max possible, given - assumes MAXSATS=8 satellites  */
double bestdop;  /*  For storing the best dop which was found.  */
int bestsats[4]; /*  Remember the combination of satellites which gave
		     the best dop.  */

foursat.n=4;  /*  Number of rows  */
foursat.m=3;  /*  Number of columns  */


N=zthesats[0][0];  /* For m==0  (The source of N if in non-diff mode)  */

   switch (N) {
      case 4:
	 combos=1;
	 for (i=0;i<combos;i++) {
	    for (k=0;k<4;k++) L[i][k]=L1[i][k];
	 }
	 break;
      case 5:
	 combos=5;
	 for (i=0;i<combos;i++) {
	    for (k=0;k<4;k++) L[i][k]=L2[i][k];
	 }
	 break;
      case 6:
	 combos=15;
	 for (i=0;i<combos;i++) {
	    for (k=0;k<4;k++) L[i][k]=L3[i][k];
	 }
	 break;
      case 7:
	 combos=35;
	 for (i=0;i<combos;i++) {
	    for (k=0;k<4;k++) L[i][k]=L4[i][k];
	 }
	 break;
      case 8:
	 combos=70;
	 for (i=0;i<combos;i++) {
	    for (k=0;k<4;k++) L[i][k]=L5[i][k];
	 }
	 break;
      default:
	 printf("\nToo many sats in dopselect() !!");
   }

   /*  See comments in dopselect12().  */
   for (i=0;i<N;i++) {
      onesat(&s, *zaeph[i], t_now); /*  Note: use *zaeph instead of *aephx, here  */
      for (j=0;j<3;j++)
	 satpos[i][j] = s.a[j];
   }

   for (i=0;i<combos;i++) {
      for (j=0;j<4;j++) {
	 index=L[i][j];
	 for (k=0;k<3;k++) foursat.a[j][k]=satpos[index][k];
      }
      dopset[i]=gdop(foursat, rl);
   }

   bestdop=1E+5;
   for (i=0;i<combos;i++) {
      if (dopset[i]<bestdop) {
	 bestdop = dopset[i];
	 for (k=0;k<4;k++) bestsats[k] = L[i][k];
      }
   }

   /*  bestsats[4] now contains the indexes for the sats which give the
       best dop. The elements of bestsats[] can be used as indexes in
       zthesats[][], anobs and *zaeph[], to get all the data for the best
       sats into the output data structures (of this funtion). This
       is done below:

       The output data structures for this function are: (see above,
       at start of function).
	     anobs
	     thesats[][4]
	     aeph[4]
   */

   /*   Refer to firstselect() here .  */
   for (i=0;i<4; i++) {
      index=bestsats[i];
      /*  Copy the eph data:   */
      aeph[i]=*zaeph[index];
      /* Copy the prns:  */
      thesats[0][i]=zthesats[0][index+1];
      /*  Copy the signal strengths (ss)  */
      thesats[1][i]=zthesats[1][index+1]; /* (Don't think ss will be used) */
      /*  Copy the pseudoranges:
	  ### Make sure that anobs1.t is def'd! */
      anobs.prs[i]=zanobs[index];
   }
   return(1);
}
/*------------------------------------------------------------------------*/
int firstselect12()
/*  2/3/93. The original selection algorithm, which simply selects
    the first four satellites. Keep this here so that it can easily
    be substituted for dopselect12() in commons12() and checkselect(),
    when so desired.

    This function MUST be placed as indicated in commons12().
*/
{
int i;

   /*  comrec is global, and has a maximum value of 4  */
   for (i=0;i<comrec;i++) {      /* ### completes anobs defs. (see above)  */
      anobs1.prs[i]=anobsx1[i];  /*  Copy pseudoranges for the stat unit  */
      anobs2.prs[i]=anobsx2[i];  /*       "       the roving unit  */
      thesats[0][i]=thesats1[0][i];  /*  Don't know what ss to put into  */
				     /*  thesats[1][] !?!                */
      aeph[i]=*aephx[i];         /*  Copy eph data for the common sats  */
   }
   return(1);
}
/*------------------------------------------------------------------------*/
int firstselect()
/*  3/3/93.  See the comment for firstselect12(). This function MUST be
    placed as indicated in checkselect().

*/
{
int i,m;

   m=4;
   if (zthesats[0][0]<4) {
      m=zthesats[0][0];
   }
   /*  m has a maximum value of 4   */
   for (i=0;i<m;i++) {  /*  Simply copy data for the first 4 satellites */
      thesats[0][i]=zthesats[0][i+1];  /*  Copy the prn  */
      thesats[1][i]=zthesats[1][i+1];  /*  Copy the signal strength */
      anobs.prs[i]=zanobs[i];  /* Copy the pseudoranges - complete anobs  */
      aeph[i]=*zaeph[i];  /* Copy the ephemeris records  */
   }
   return(1);
}
/*------------------------------------------------------------------------*/
int header()
/*  3/3/93.  Displays output format for data. */
{
   printf("\nu1         u2        u3          t            h m s  gdop     s1 s2 s3 s4");
   printf("\n------     ------    ------      ------       - - -  -------  -- -- -- -- ");

   return(1);
}
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/*======================================================================*/

