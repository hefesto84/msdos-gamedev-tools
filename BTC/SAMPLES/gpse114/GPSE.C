/*

GPSE version 1.14 Copyright(C) 1994  Gary David Agnew
GPSE comes with ABSOLUTELY NO WARRANTY;
This is free software, and you are welcome to redistribute it
under certain conditions: for details please see the accompanying
file, "GNU.DOC".




	  GPSE - GPS Engine, a program for GPS data post-processing.
	  Copyright (C)  1994 by Gary David Agnew

	  written by:   Gary Agnew   and   Bob Cole


This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
any later version.

This program is distributed in the the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

The full text of the GNU General Public License is contained in
the file "GNU.DOC", which is part of this package.


The Author(s) and associates can be contacted at the following
addresses:


                      
			      Gary D Agnew:
			      -------------

	 Postal Address:      P O Box 118
			      Wits, 2050
			      Johannesburg
			      South Africa

	Home Telephone:       + 27 11 782-5831

	Email:                agnew@odie.ee.wits.ac.za




			      Bob Cole:
			      --------

	 Postal Address:      6 Scully Str
			      Roosevelt Park
			      Johannesburg 2195
			      South Africa



TESLA Consulting:             Gary Hodkinson:
			      ---------------

	 Postal Address:      P O Box 783195
			      Sandton 2146
			      South Africa

	 Telephone:           + 27 11 442-8112
	 Fax:                 + 27 11 442-8130




The author(s) accept no responsibility whatsoever for loss of property
or health caused either directly or indirectly by the use of this software.
The user uses this software entirely at his/her own risk.

*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*               GPSE -- GPS DATA PROCESSING UTILITY                    */
/*               -----------------------------------                    */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                                                      */
/*    Lovingly hand-crafted in the spring of 1993 by:                   */
/*                                                                      */
/*                  Gary Agnew      Bob Cole                            */
/*                                                                      */
/*               Copyright (c) 1994, by Gary David Agnew                */
/*                                      Tesla Consulting                */
/*                                                                      */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/



/*----------------------------------------------------------------------*/

/*

This program was written for Borland's C++ compiler. For more information
on the program demo and environment, see the accompanying README file.



		 Notes on program structure:
            	 ---------------------------
*/
		  
/*  The program structure for the portion of (mathematical) code which
    calculates receiver position, is as follows:

			 Program structure
			 ~~~~~~~~~~~~~~~~~
	     (Top to bottom is pecking order, left to right is
	      more-or-less the sequence. )



			     main
			      |
	  ----------------------------------------
	 |					  |
    (io routines)                              findrec
						  |
		  -------------------------------------------------
		 |	                    |			   |
	       fsats                      findk                uvw2geo
		 |       		    |
		 |      	    -------------------------------------
	      eph2uvw   	   |        |         |        |         |
		 |       	 frange   svecvec   matinv   matvec   avecvec
	      --------		   |	              |
	     |        |	        --------            -------
	   nsolve   angle      |        |  	   |	   |
	     |		     svecvec   mag    	 factor   subst
	   nfunc				   |
						--------
					       |        |
					     amax      abs1


The program structure for the sections which parse files, do
satellite selection, etcetera, are a little less definite, and
so will be omitted. (Their order and precedence does not
follow the neat model of the receiver fix code). A reasonably good
idea of what is going on can be had by looking at main, however.

*/

/*-----------------------------------------------------------------------*/
/*          Notes on functions and variables for differencing:
	    -------------------------------------------------

1.  Any function name that ends with a digit, eg. checkselect1() or
    checkselect2(), or opnfiles12(), is used only when the software
    is being run in differencing mode.
2.  Normally the function is related to, but not necessarily exactly
    the same as the function for non-differencing mode, for example
    checkselect1() and checkselect().
3.  Where the function has either 1 or 2 at the end, the digit pertains to
    data for the stationary or roving unit, respectively.
4.  Where the function has 12 at the end, eg. opnfiles12(), it pertains
    equally to the stationary and roving data.
5.  Where it has been necessary to create new global variables to
    implement differencing, the same convention has been adopted in
    naming these variables.
6.  Commenting similar sections of code in `sister' functions has been
    avoided - where commenting seems sparse in the differencing-mode
    code, see the `equivalent' section of code in the non-differencing
    function, where commenting should be adequate.

These philosophies have been adopted to keep differencing and
non-differencing code in the same program, but logically separable.
Also, it reduces risk in code development: the completed, working
non-differencing code is not corrupted at all, and can be relied on
during development.
*/
/*------------------------------------------------------------------------*/
/*            General comments on the software:
	      ---------------------------------


1.  Real programming techniques have been used throughout.

2.  The program is monolithic - it could all be put into one file. However,
it has been split up for the sake of readability, and the pieces are
related simply with hash-includes.

3.  Clever tricks with the dynamic allocation of memory (for space
efficiency) have not been used, to avoid trouble later.

4.  This code should be modifiable without too much difficulty, to
work on Magnavox and other units, as long as their data comes in
standard Rinex format. Beware of Magellan, who don't use standard
Rinex.

     The following set of files go together, to produce a complete
     executable file:


    -    leica_h.c      All the h files needed by the program
    -    leica_g1.c     Gary  - subject to change; higher level
    -    leica_g2.c     Gary  - lower level; less likely to be changed
    -	 leic_r.c       Bob
    -	 gpse.c         (main) (this file)
    -	 leica_u.c      Gary - Typedefs, defines and non-GPS specific (general) utils
    -    leica_v.c      Gary - All global variables.

    -	 leica__.h      (? - common all funcs):  Not in place yet.


5.  The file names above all start with "leic" for historical reasons. It
    should be possible to change this without too much difficulty, all
    it should involve is changing the actual file names and the
    corresponding names where they are referred to in <hash>includes.
    Future plan is to replace "leic" with the name "gpse", for gps
    engine. the correspondence between the old and new names would then be:

     leic_h.c         gpse_h.c
     leic_g1.c        gpse_g1.c
     leic_g2.c        gpse_g2.c
     leic_r.c         gpse_r.c
     lg.c             gpse_m.c
     leic_u.c         gpse_u.c
     leic_v.c         gpse_v.c

6.   The // comment style has been used extensively, often to remove
     redundant code. All this code can be deleted without affecting
     program operation. However, the reason they have been left in is
     that they were introduced and found to be useful during debugging.
     They are valuable indicator of what tests to do where. The author
     recommends retaining them, and if a different (Non-Borland C++)
     compiler is used, to replace the <slash><slash> style with the
     <slash><star> <star><slash> style.

*/
/*-------------------------------------------------------------------*/
#define MAXEPHS 20   /* Use MAX_SATS=20 in leica_u.c ?  */
/*  The maximum number of ephemeris records expected - this would normally
    bear some relation to the maximum number of satellites in various
    functions.  Perhaps MAXEPHS should be merged with MAX_SATS=20 (see
    leica_u.c).  */

#define OLD 9000  /*  2.5 hour - used to determine age of ephemeris data
		      (with respect  to a given obs file).   */

#define VERYOLD 86400  /*  24 hours - "  */

#define MASKMIN 5  /*  The minimum satellite elevation which is
		       acceptable. See maskselect(). See ICD pg. 42  */

#define TRACKMAX 8    /*  For later? - in satpos defn (see dopselect) */
#define MAXCOMBOS 70   /*  Also for possible use later, in dopselect(). */

#define LEICA 1
#define GARMIN 2
#define MAGNAVOX 3  /*  These haven't been implementd - for future work: */
#define MAGELLAN 4
#define SERCEL 5
#define BARCOM 6
#define TRIMBLE 7
#define SONY 8

/*  It will please the reader to know that these are all the
    hash-includes. (ie. there aren't any, hidden in other files -
    apart from some standard includes, in one place in leica_h)
*/

#include "leica_h.c"  /*  Contains all the h-files and includes needed.  */
#include "leica_u.c"  /*  Typedefs, some defines and general(non-GPS) utils */
#include "leica_v.c"  /*  All global variables  */
#include "leica_r.c"  /*  Robert's sections of code (utilities)  */
#include "leica_g2.c" /*  General (non-GPS specific) utils.  */
#include "leica_g1.c" /*  Most of the main functions    */

/*===================================================================*/
void main()
{
float x;
int comrecn, comrec12;
int i,j,k,m,n;
matrix nulmat, aeblock;
char anystr[80];

vector v1;       /*  General-purpose vectors for pseudorange diff calcs:
		     stores satellite positions one-at-a-time, for range
		     calculations.          */
vector rc, rcr;  /*  "Correct" stat receiver position, in polar and
		      rectangular co-ordinates respectively  */
vector rs;       /*  Vector for keeping "correct" ranges for stat unit.  */
vector re;       /*  Vector for keeping pseudorange corrections, to be
		     applied to roving unit pseudoranges.     */
rinex anobsrr;   /*  For storing the final, corrected pseudoranges for the
		     roving unit; so that findrec() can be called, this
		     variable is of type rinex.        */
FILE *recfile;   /*  A file containing the stationary receiver's WGS84
		     position in ordinary ascii.   */

vector result, result1, result2, result12; /*  For storing rect results */
vector resultp, resultp1, resultp2, resultp12;  /* Store polar results  */

int neoobs=1;  /*  For signaling the end of the observation file  */
int carryon=1; /*  Derived from keyboard signal to quit.  */
int conv;      /*  Signifies whether or not a process converged  */
int conv1, conv2;  /*  Whether or not each pos converged, for diff mode. */


   tstart=time(NULL);
   str2[2]='\0';


/*  This sets the number of columns in the obs data file. (see leica_v.m)  */

/*  Initialisations here. Initialise variables for the sake of modules
    like svecvec in leic_u.
*/

      /*  Initialise pr-diff vectors:   */

   v1.n=3;   rc.n=3;   rcr.n=3;  /*  These contain co-ordinates  */

   rs.n=4;   re.n=4;             /*  These contain pseudoranges  */


/*     xjohannesburg must be defined globally (see above). For an
       explanation of why this was done, see findrec()     */
   xjohannesburg.n=4;
   xjohannesburg.a[0]=  5247000;
   xjohannesburg.a[1]=  2508000;
   xjohannesburg.a[2]= -2611000;
   xjohannesburg.a[3]=0;

   rl.n=4;
   rl = xjohannesburg;

   result.n=4;     
   result.err=0;

   resultp.n=4;
   resultp.err=0;

   result1.n=4;
   result1.err=0;
   result2.n=4;
   result2.err=0;
   result12.n=4;
   result12.err=0;

   resultp1.n=4;
   resultp1.err=0;
   resultp2.n=4;
   resultp2.err=0;
   resultp12.n=4;
   resultp12.err=0;

   csats.n=4; /*  4 rows for 4 satellites  */
   csats.m=3; /*  3 co-ordinates per satellite. These m and n values are
		  re-assigned when fsats is called, anyway */
   csats1.n=4;
   csats1.m=3;

   aeblock.n=8;
   aeblock.m=10;
   strcpy(aeblock.message," ");
   aeblock.err=0;
   for (i=0;i<MAXN;i++){
	for (j=0;j<MAXN;j++) {
		   aeblock.a[i][j] = 0.0;
	 }
   }
   nulmat=aeblock;
   nulmat=nulmat;  /*  Shut the warning generator up.  */
   /*  (This variable is not currently used. However, it does
	have important historical roots, and will be left here
	for possible future use).         */

   ephrec[0]=0; /*  There are no ephemeris records.   */
   gintro();    /*  Pop up the hello screen  */

switch (mm) {
case 0:  /*  Code for non-differencing mode follows:    */

   /* Ask for the filenames here (in opnfiles()): */
   opnfiles();  /*  Get file names and open them, for the big edp.  */

   /*  The first observations are sometimes bad - a possible improvement
       for later is to have some sort of algorithm which decides if data are
       ok. In the meantime use the bad data anyway, and generate rubbish
       results & error messages - leave it up to the user's discretion.

       Note that negative pseudoranges in the first obs record are
       not necessarily trash: Mostly, they actually make sense, and are
       negative only because of a gross offset due to the large initial
       receiver clock error.
   */

   /*  If first data record is bad, just include an extra
       neoobs=robs(obsfile, &info, &xobs); here, to toss it away.  */

   neoobs=robs(obsfile, &info, &xobs); /* Read in obs record for selecting eph data
				  and to calculate the first fix in the
				  while loop below     */
   m=updeph(&xobs);
   /*  Remember: in updeph(&xobs) above, xobs is being passed only so
       that updeph can get a hold of the time of obs  */
   ephinfo(m);  /*  Display information about the eph update.  */

   header();

   /* Keep doing calculations while: (i) Not at end of obs file
				    (ii) User has not interrupted   */
   while ((neoobs)&&(carryon)) {

      comrecn=checkselect(); /* Do time checks and satellite selection for this
			       particular observation.  */

      /*  Attempt to find a result and display it only if there are enough
	  satellites available - if checkselect() has reported at
	  least 4 satellites:    */
      if (comrecn>3) {
	  /*  THE BIG STEP - calcs actually done here:  */
	 conv=findrec(&gdop0, &result, aeph, anobs);
	 rl=result;
	 uvw3geo(result.a[0], result.a[1], result.a[2], &resultp.a[0], &resultp.a[1], &resultp.a[2]);

	 /*  Output result to file/screen in rectangular (result) or
	     polar (resultp) co-ordinates:
	     The outcome of the convergence test here can be used to
	     reject the data, if necessary - eg. don't write it to disk  */
	 displayresult(resultp,anobs.t);
	 totalfixes++;
	 presfixes++;
	 if (!conv) printf("\n We have a convergence problem in the above dat. ");

      }
      else {
	 /* Generate an error message - not enough eph and/or obs data for this
	    observation */
	 printf("\nOops, record has only %d sats with eph records, ",comrecn);
	 printf(" %d:%d:%d", (int)xobs.a[0][3], (int)xobs.a[0][4], (int)xobs.a[0][5]);
      }

      neoobs=robs(obsfile, &info, &xobs); /*  robs() returns 0 at eof  */

      /*  Beware of changing the position of ephcheck() here - see
	  comments in commons12().      */
      ephcheck(&xobs); /*  Check whether eph data needs to be updated and
			   update if necessary.  */

      if (kbhit()) {  /*  Check if the keyboard has been hit  */
	 qq=getch();  /*  If so, get the character  */
	 if (qq=='q')
	    carryon=0;  /*  If its a 'q', then quit.   */
	 else {
	    qq=getch();  /*  Otherwise halt and wait for another character. */
	    if (qq=='q') carryon=0; /*  If its a 'q' then quit, otherwise */
	 }                          /*  carry on.          */
      }
   }

   if (carryon==0) printf("\nYou quat.");
   if (neoobs==0) printf("\nEnd of obs file has been reached.");

   clsfiles();    /*  Close all the files    */

break;  /*  End of code for non-differencing mode.  */

case 1:   /*  (mm==1) Code for positional differencing mode follows:   */

   opnfiles12();  /*  Get file names for stat and roving units
		    and open them, for the big edp.  */
   /*  equaltimes() in the differencing section sort-of replaces
       the simpler neoobs=robs() in the non-differencing section -
       see above.               */
   /*  If first data record is bad, just include an extra
       neoobs=equaltimes(); here, which will toss the first set away.  */

   neoobs=equaltimes();  /*  Look through the two obsfiles, to find the
			     first set of records where the times match.
			     This set of obs records will be used to
			     update the eph data and is also the first
			     to be used to calculate a fix (see while
			     loop below)  */
   m=updeph(&xobs1);     /* Use time in xobs1 to select the eph data. Can't
			    use ephcheck here - have to use updeph directly
			    the first time, to set t_last.   */
   ephinfo(m);  /*  Display information about the eph update.  */

   header();

   while ((neoobs)&&(carryon)) {

      checkselect1();        /*  Find sats with eph data in xobs1   */
      checkselect2();        /*  Find sats with eph data in xobs2   */
      comrec12=commons12();  /*  Select common sats between xobs1 and xobs1,
				 all with valid eph data  */

      /*  Only do commons12() and findrecs, displayresult, etc, if
	  comrec12>3, ie. if there are enough sats reported by commons12(): */
      if (comrec12>3) {

	 /*  Using aeph, anobs1 and anobs2, this is where the special
	     differential method should be inserted as a function. At
	     the moment, conventional positional differencing is done.
	     A function which does pseudorange differencing should be
	     written and called here.  */

	 /*  Calculate rectangular co-ords of the stationary unit, result1: */
	 conv1=findrec(&gdop1, &result1, aeph, anobs1);
	 rl=result1;
	 /*  Get stationary result in polar co-ords,   resultp1   :  */
	 uvw3geo(result1.a[0], result1.a[1], result1.a[2], &resultp1.a[0], &resultp1.a[1], &resultp1.a[2]);

	 /*  Calculate rectangular co-ords of the roving unit, result2:   */
	 conv2=findrec(&gdop2, &result2, aeph, anobs2);
	 /*  Get rover result in polar co-ords,   resultp2   :  */
	 uvw3geo(result2.a[0], result2.a[1], result2.a[2], &resultp2.a[0], &resultp2.a[1], &resultp2.a[2]);

	 conv=((conv1)&&(conv2));  /*  Both results have to have converged */

	 /*  Subtract the two result vectors: (note: the fourth element
	     of result12 is the difference between the two clock
	     errors and is (for the time being) meaningless.)   */
	 /*  Find the difference of the rectangular results:  */
	 result12=svecvec(result2, result1);
	 result12=result12;  /*  Shut the warning generator up.  */

	 /*  Find the difference of the polar results:  */
	 resultp12=svecvec(resultp2, resultp1);

	 /*  We'd want to display   result12   or   resultp12   here.
	     The outcome of the convergence test here can be used to
	     reject the data, if necessary - eg. don't write it to disk  */
	 displayresult(resultp12,anobs1.t); /* o/p result to screen & file  */
	 totalfixes++;
	 presfixes++;
	 if (!conv) printf("\n We have a convergence problem in the above dat. ");
      }
      else {
	 printf("\nWhoops, records at %d:%d:%d have ", (int)xobs1.a[0][3], (int)xobs1.a[0][4], (int)xobs1.a[0][5]);
	 printf("only %d common sats.", comrec12);
	 /*  Display the hour, minute and second of the bad records  */
      }

      neoobs=equaltimes(); /*  equaltimes() returns 0 at the end of either
			       observation file.  */

      /*  Beware of changing the position of ephcheck() here - see the
	  comments in commons12().  */
      ephcheck(&xobs1);  /*  See that the eph dbase is up-to-date. Times
			     in xobs1 and xobs2 should be the same, at this
			     point.  */

      if (kbhit()) {  /*  Check if the keyboard has been hit  */
	 qq=getch();  /*  If so, get the character  */
	 if (qq=='q')
	    carryon=0;  /*  If its a 'q', then quit.   */
	 else {
	    qq=getch();  /*  Otherwise halt and wait for another character. */
	    if (qq=='q') carryon=0; /*  If its a 'q' then quit, otherwise */
	 }                          /*  carry on.          */
      }
   }

   if (carryon==0) printf("\nYou quat.");
   if (neoobs==0) printf("\nEnd of obs file has been reached.");
   clsfiles12();    /*  Close all the files    */

break; /*  End of code for positional differencing mode.   */

case 2:   /*  (mm==2) Code for pseudo-range differencing mode follows:   */

/*  Notes on the pseudorange differencing algorithm:
    -----------------------------------------------

    (See CM's book, pg.51)

    The stationary unit's
    position is calculated,as per usual, to give a result consisting of a
    position and a clock error (i). This calculation also has the side
    effect of producing (within the findrec function), the variables
    csats (ii) and cobs (iii), which are the satellite postions and
    pseudoranges for the stat unit, corrected for satellite clock errors.
    cobs (iii) is not corrected for stat receiver clock errors (although
    it could easily be, using (i)). The csats positions are then used in
    conjunction with the "correct" stat unit position (iv) to produce a
    set of "correct" ranges (v) to the stat unit. From these correct
    ranges are then subtracted the pseudoranges (iii), to produce a set
    of pseudorange errors (vi), which are assumed to be caused mainly by
    S A and atmospheric conditions, and that they are very much the same
    as the errors present in the roving unit's measured pseudoranges.
    These error terms can then be added to the pseudoranges for the roving
    unit (vii) to produce a set of correct ranges (viii) for the roving
    unit. These correct ranges are then used to calculate the roving
    unit's position, as per usual.

    Notes on satellite and receiver clock error in the above algorithm:

    (a)  Receiver clock error:  the stat receiver clock error (i) can be
    removed from the stat receiver's pseudoranges (iii), before the
    pseudorange error is determined and passed to the next part of the
    algorithm. However, this is not strictly necessary. Since the stat
    unit's clock error manifests itself as a range error of equal size
    in all the pseudoranges, it will ultimately be transferred to the
    roving unit, to appear as a roving unit clock error which will be
    removed in the final position calculation for the roving unit. It
    should be borne in mind that the clock error which comes out of
    the final calculation for the roving unit contains both the stat
    and roving unit's clock errors. If for some reason this is not
    desirable, the stat unit's clock error should be removed at the
    point indicated above.

    (b)  Satellite clock error correction: the satellite clock errors
    present in the measured pseudoranges for the stat unit have to be
    removed. This is necessary, to produce pseudorange errors which
    are free of satellite clock bias, and which are due only to
    stat receiver clock bias, S A and atmospheric conditions (etc.).
    This means that the only other source of sat clock bias that
    remains, is in the roving unit pseudoranges, and it is one which
    is removed in the final roving unit position calculation. --- If the
    sat clock bias is NOT removed from cobs (iii), it will be included
    in the pr error which is passed on to correct the roving unit prs,
    where it will cancel with the roving unit pr error due to sat clock
    bias. This will leave "correct" ranges for the roving unit which
    are FREE of sat clock bias, and the final calculation (findrec)
    will RE-INTRODUCE the sat clock error (albeit negative) by
    attempting to correct for it where it doesn't exist. For this
    reason it is important to remove the sat clock bias from the
    stat unit pseudoranges (iii).

*/

   opnfiles12();  /*  Get file names for stat and roving units
		    and open them, for the big edp.  */
/*  Ask for the correct stationary coordinates over here.  */
   printf("\nRead stationary unit's WGS84 co-ordinates from a file called alice? (y/n) ");
   qq=getche();
   if (qq=='n'||qq=='N') {
      printf("\n\nLatitude: "); scanf("%lf", &(rc.a[0]));
      printf("Longitude: "); scanf("%lf", &(rc.a[1]));
      printf("Height: "); scanf("%lf", &(rc.a[2]));
   }
   else {
      recfile=fopen("alice","rt");   /*  Open file with receiver co-ords.  */
      if (recfile==NULL) {
	 printf("\n\nCould not find file `alice' in the current directory.\n");
	 exit(0);
      }
      j=0;
      for (k=0;k<3;k++) {  /*  Read 3 numbers into the receiver position var:  */
	 fgets(anystr, 200, recfile);
	 i=sscanf(anystr,"%lf",&(rc.a[k]));   /*  Try %le ?!  */
	 if (i!=1) {j=1;}  /*  Flag an error if string doesn't return a value */
      }
      if (j==1) {
	 printf("\n\nThe receiver input file appears to be corrupted in some");
	 printf("\nway. Check that it consists of the three WGS84 co-ordinates in");
	 printf("\nordinary ascii, each one on its own line: Latitude (decimal degrees), ");
	 printf("\nLongitude (decimal degrees) and Height (metres). \n");
	 fclose(recfile);
	 exit(0);
      }
   }

   /*  phi, lam, h, *u, *v, *w   */
   geo2uvw(rc.a[0],rc.a[1],rc.a[2],&(rcr.a[0]),&(rcr.a[1]),&(rcr.a[2]));
   /*  Convert the receiver position in WGS84 to rectangular -
       this is necessary, so that the ranges to the satellites
       can be calculated.           */

   /*  equaltimes() in the differencing section sort-of replaces
       the simpler neoobs=robs() in the non-differencing section -
       see above.               */
   /*  If first data record is bad, just include an extra
       neoobs=equaltimes(); here, which will toss the first set away.  */

   neoobs=equaltimes();  /*  Look through the two obsfiles, to find the
			     first set of records where the times match.
			     This set of obs records will be used to
			     update the eph data and is also the first
			     to be used to calculate a fix (see while
			     loop below)  */
   m=updeph(&xobs1);     /* Use time in xobs1 to select the eph data. Can't
			    use ephcheck here - have to use updeph directly
			    the first time, to set t_last.   */
   ephinfo(m);  /*  Display information about the eph update.  */

   header();  /*  Put the column names at the top of the data display
		  screen - ie. what the numbers mean.  */

   while ((neoobs)&&(carryon)) {

      checkselect1();        /*  Find sats with eph data in xobs1   */
      checkselect2();        /*  Find sats with eph data in xobs2   */
      comrec12=commons12();  /*  Select common sats between xobs1 and xobs1,
				 all with valid eph data  */

      /*  Only do commons12() and findrecs, displayresult, etc, if
	  comrec12>3, ie. if there are enough sats reported by commons12(): */
      if (comrec12>3) {

	 /*  Using aeph, anobs1 and anobs2, this is where the special
	     differential method should be inserted as a function. At
	     the moment, conventional positional differencing is done.
	     A function which does pseudorange differencing should be
	     written and called here.  */

	 /*  Calculate rectangular co-ords of the stationary unit, result1: */
	 /*  Find the position of the stationary unit, as per usual:  */
	 conv1=findrec(&gdop1, &result1, aeph, anobs1);
	 cobs1=cobs; /*  (Warning: keep these 3 lines of code together!!) */
	 csats1=csats;  /*  Highjack the corrected stat receiver pseudoranges
			    and satellite positions before they get
			    overwritten:  cobs and csats are passed out of
			    findrec() implicitly!             */

	 rl=result1;  /*  rl is the "latest receiver position" (needs
			  only be approximate), which is used in the
			  dopselect algorithm to find the dop associated
			  with particular satellite combinations. */
	 /*  Get stationary result in polar co-ords,   resultp1   :  */
//	 uvw3geo(result1.a[0], result1.a[1], result1.a[2], &resultp1.a[0], &resultp1.a[1], &resultp1.a[2]);

/*----------- pr-diff titbits follow:  */
	 for (i=0;i<4;i++) {  /*  Calculate ranges for 4 satellites, rs.  */
	    for (j=0;j<3;j++)   /*  Copy 3 co-ords to a vector, for frange()  */
	       v1.a[j]=csats1.a[i][j];   /*  CSATS1 GETS USED HERE  */
	    rs.a[i]=frange(rcr, v1);
	 }

	 for (i=0;i<4;i++) {  /*  Calculate the pseudorange error vector re. */
	    re.a[i]=rs.a[i]-cobs1.prs[i];  /*  COBS1 GETS USED HERE:  */
//	    printf("\nPr error no. %d is %f",i,re.a[i]);
//	    re.a[i]=0.0;
	 }


	 for (i=0;i<4;i++)    /*  Calculate corrected rover pseudorange  */
	    anobsrr.prs[i]=re.a[i]+anobs2.prs[i];

	 anobsrr.t=anobs2.t;  /*  Have to copy the time, too - could equally
				  have used anobs1.  */

/*-------------- pr-diff titbits end.  */
	 /*  Calculate rectangular co-ords of the roving unit, result2, as
	     per usual, except that we use anobsrr here:   */
	 conv2=findrec(&gdop2, &result2, aeph, anobsrr);
	 /*  Remember that result2.a[3] is the COMPOSITE clock error !!  */

	 /*  Get rover result in polar co-ords,   resultp2   :  */
	 uvw3geo(result2.a[0], result2.a[1], result2.a[2], &resultp2.a[0], &resultp2.a[1], &resultp2.a[2]);

	 conv=((conv1)&&(conv2));  /*  Both results have to have converged */

	 /*   No subtraction of the results here: result2 is exactly
	      what we want!  */
	 result12=result12;  /*  Shut the warning generator up.  */
	 resultp12=resultp12;

	 /*  We'd want to display   result2   or   resultp2   here.
	     The outcome of the convergence test here can be used to
	     reject the data, if necessary - eg. don't write it to disk  */
	 displayresult(resultp2,anobsrr.t); /* o/p result to screen & file  */
	 totalfixes++;                      /*   (could use anobs1.t)   */
	 presfixes++;
	 if (!conv) printf("\n We have a convergence problem in the above dat. ");
      }
      else {
	 printf("\nWhoops, records at %d:%d:%d have ", (int)xobs1.a[0][3], (int)xobs1.a[0][4], (int)xobs1.a[0][5]);
	 printf("only %d common sats.", comrec12);
	 /*  Display the hour, minute and second of the bad records  */
      }

      neoobs=equaltimes(); /*  equaltimes() returns 0 at the end of either
			       observation file.  */

      /*  Beware of changing the position of ephcheck() here - see the
	  comments in commons12().  */
      ephcheck(&xobs1);  /*  See that the eph dbase is up-to-date. Times
			     in xobs1 and xobs2 should be the same, at this
			     point.  */

      if (kbhit()) {  /*  Check if the keyboard has been hit  */
	 qq=getch();  /*  If so, get the character  */
	 if (qq=='q')
	    carryon=0;  /*  If its a 'q', then quit.   */
	 else {
	    qq=getch();  /*  Otherwise halt and wait for another character. */
	    if (qq=='q') carryon=0; /*  If its a 'q' then quit, otherwise */
	 }                          /*  carry on.          */
      }
   }

   if (carryon==0) printf("\nYou quat.");
   if (neoobs==0) printf("\nEnd of obs file has been reached.");
   clsfiles12();    /*  Close all the files    */

break; /*  End of code for pseudorange differencing mode.   */

}  /*  End of code for all modes.       */

   killeph();     /*  Free the memory dynamically allocated for eph records */
   printf("\nTotal fixes calculated in this session: %d",totalfixes);
   tfin=time(NULL);
   x=((float)totalfixes)/((float)tfin-(float)tstart);
   printf("\nAverage rate, in fixes per second, was: %5.2f", x);

}
/*===================================================================*/


