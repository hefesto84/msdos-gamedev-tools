/*  The variables defined here are all global. They have been made so
    for the sake of the loosely coupled functions comprising main, in
    order to avoid laborious and unnecessary parameter passing. (ie.
    some of the functions in main really should be part of main,
    but have been separated from it, to improve readability. Other
    variables here are truly global in the sense that they `could be
    required in any function, at any time'.
*/

double c = 2.99792458E+08;   /*  Speed of light.    */

int ephrec[MAXEPHS];
/*  The structures ethelot and ephrec are tightly linked. The first element
    of ephrec says how many sets of ephemeris data have been found. The
    remaining elements (ephrec[0] of them) are the satellite prns. The eph
    data for each of the prns occurs in the same order in *ethelot[]. To find
    the eph data for a particular prn, the function feph should be used - it
    returns the index in ethelot, of that prn. ethelot[index] is then the
    desired ephemeris data for that satellite.
       See the global declaration of ethelot, below.

    ethelot is a dynamic data structure, and a new record is malloc'ed
    whenever a new satellite's eph data is found in the eph file being read.
    At the end of main program execution, the entire structure is freed.
*/


/*  Rudimentary error trapping, which requires future improvements:    */
char errmess[80]; /*  The text of the error message, which can be printf'ed */
int errcon; /* Error condition - the severity of the error.     */
/* Error severity as in the following table:
	 0 - no error condition exists
	 1 - reserved for future use
	 2 - a non-fatal error; use errmess as a warning
	 3 - reserved for future use
	 4 - the error is fatal, program execution must halt
This error checking scheme has not yet been put into effect in the program.
*/

char qq;  /*  General character for input from keyboard.     */
int  mm;  /*  mm=0 means run in non-differential mode.  mm=1 means
	      run in positional differential mode.  mm=2 means run in
	      pseudorange differential mode.              */

int colums;  /*  The number of columns in the observational data, for
		 example 6 in the leica and 2 in the garmin. Of course,
		 the garmin could probably be programmed to give 3 types
		 of data columns, etc.    */
long int etime; /*  Current eph info epoch time - see comments in ueph(). */
int udates=0;   /*  The number of times the eph records, ethelot, have
		    been updated.  */

time_t tstart;  /*  To measure the time taken in a session.  */
time_t tfin;

double t_now;   /*  The time of the current observation - used in conjunction
		    with t_last and dt_up, to decide whether to update the
		    eph information. This variable is modified in
		    checkselect(). */
double t_last;  /*  The time of the last eph information update. If t_now
		    is more than some nominal amount (say 3600s, for one
		    hour), then the eph information has to be adjusted so
		    that the eph records with the most accurate information
		    (ie. as close as possible to current time) are installed
		    in ephrec. The update is done by updeph(), and this
		    variable is modified in updeph().              */
double dt_up;   /*  Wench variable used in update eph calculations.  */



FILE *raw_file;
FILE *rinex_file;      /*  For Robert's function   */
FILE *efile;  /*  For use in one of Roberts/my_more_obscure funcs ? */

FILE *mfile;    /*  Marker file     */
 
FILE *ephfile;  /* They're global, ok. Just leave them alone.   */
FILE *obsfile;
FILE *outfile;

FILE *ephfile1;  /*  For the stationary unit  */
FILE *obsfile1;

FILE *ephfile2;  /*  For the roving unit  */
FILE *obsfile2;



char mkfilenm[80];  /* Name of the marker file - used in both m modes */

char ename[80];  /*  Name of the eph file opened */
char oname[80];  /*  Name of obs file  */
char opfile[80]; /*  Name of the output (results) file  */

char ename1[80]; /*  File names for the stationary unit  */
char oname1[80];

char ename2[80]; /*  File names for the roving unit */
char oname2[80];

// #include "leica_u.c"    was here


matrix xobs;  /*  A home for a piece of `raw' obs data, straight from robs().*/
matrix xobs1;
matrix xobs2;

double tobs1;  /*  Global tobs1/2 for xobs1/2 so that equaltimes() can  */
double tobs2;  /*  get at them.            */

rinex anobs; /*  For `an observation'. After checkselect(), part of the
		 contents of xobs end up here. anobs then gets passed to
		 findrec, for the calculations.  */
rinex anobs1;
rinex anobs2;

int thesats[2][4];  /*  Contains the 4 selected satellites (first row),
			and their signal strengths (second row) for the
			current obs record - see checkselect().    */


/*  `ethelot' - for ephemeris, the lot. Has to be global. See inside updeph();
    see the declaration of ephrec above (global)  */
ephemeris *ethelot[MAXEPHS];
ephemeris aeph[4]; /* To pass the 4 sats' ephs' to alleph[] in findrec */

/*  See comments in checkselect1(), on the structure of the following
    variables:               */
int zthesats1[2][MAXEPHS];     /*  ----cf.    int         thesats[2][4]  */
double zanobs1[MAXEPHS];       /*  ----cf.    rinex       anobs          */
ephemeris *zaeph1[MAXEPHS];    /*  ----cf.    ephemeris   aeph[4]        */

int zthesats2[2][MAXEPHS];
double zanobs2[MAXEPHS];
ephemeris *zaeph2[MAXEPHS];

int comcount,comrec;
ephemeris *aephx[MAXEPHS];
int thesats1[2][MAXEPHS], thesats2[2][MAXEPHS];
double anobsx1[MAXEPHS], anobsx2[MAXEPHS];

int zthesats[2][MAXEPHS];     /*  -----    int         thesats[2][4]  */
double zanobs[MAXEPHS];       /*  -----    rinex       anobs          */
ephemeris *zaeph[MAXEPHS];    /*  -----    ephemeris   aeph[4]        */

char *str2="   ";
infotype info, info1, info2;  /* Information about the obs files  */
char *refstr="L1L2C1C2P1P2D1D2T1T2";
char *description[]={"Phase measurement on L1 (full cycles)",
		     "Phase measurement on L2 (full cycles)",
		     "Pseudorange using C/A code on L1 (metres)",
		     "Pseudorange using C/A code on L1 and P2-P1 code (metres)",
		     "Pseudorange using P-code on L1 (metres)",
		     "Pseudorange using P-code on L2 (metres)",
		     "Doppler frequency on L1 (Hz)",
		     "Doppler frequency on L1 (Hz)",
		     "Transit Integrated Doppler on 150MHz (cycles)",
		     "Transit Integrated Doppler on 400MHz (cycles)"};

double gdop0, gdop1, gdop2;

rinex cobs;  /*  For storing corrected pseudorange data -- see inside
		 function findrec. These corrected prs get used in
		 findrec to do sats calculation, as well as to pass back
		 corrected prs for pseudorange differencing in main.  */
matrix csats;  /*  Corrected satellite positions - from corrected
		   pseudorange data, as in cobs above, and for the same
		   purpose.        */

rinex cobs1;
matrix csats1;  /*  These are to remember the values contained in cobs
		    and csats, before they are overwritten by the next
		    function call.  */

vector rl, xjohannesburg;


/* `L' matrices defining combinations of different satellites. For
   more information, see the `dopselect' functions.   */

int L1[][4]={{0, 1, 2, 3}}; /*  1 combination for 4 satellites   */

int L2[][4]={{0, 1, 2, 3},  /*  5 combinations for 5 satellites  */
	     {0, 1, 2, 4},
	     {0, 1, 3, 4},
	     {0, 2, 3, 4},
	     {1, 2, 3, 4}};

int L3[][4]={{0, 1, 2, 3},  /*  15 combinations for 6 satellites  */
	     {0, 1, 2, 5},
	     {0, 1, 2, 4},
	     {0, 1, 4, 5},
	     {0, 1, 3, 5},
	     {0, 1, 3, 4},
	     {0, 3, 4, 5},
	     {0, 2, 4, 5},
	     {0, 2, 3, 5},
	     {0, 2, 3, 4},
	     {2, 3, 4, 5},
	     {1, 3, 4, 5},
	     {1, 2, 4, 5},
	     {1, 2, 3, 5},
	     {1, 2, 3, 4}};

int L4[][4]={{3, 4, 5, 6},    /*  35 combinations for 7 satellites  */
	     {2, 4, 5, 6},
	     {2, 3, 5, 6},
	     {2, 3, 4, 6},
	     {2, 3, 4, 5},
	     {1, 4, 5, 6},
	     {1, 3, 5, 6},
	     {1, 3, 4, 6},
	     {1, 3, 4, 5},
	     {1, 2, 5, 6},
	     {1, 2, 4, 6},
	     {1, 2, 4, 5},
	     {1, 2, 3, 6},
	     {1, 2, 3, 5},
	     {1, 2, 3, 4},
	     {0, 4, 5, 6},
	     {0, 3, 5, 6},
	     {0, 3, 4, 6},
	     {0, 3, 4, 5},
	     {0, 2, 5, 6},
	     {0, 2, 4, 6},
	     {0, 2, 4, 5},
	     {0, 2, 3, 6},
	     {0, 2, 3, 5},
	     {0, 2, 3, 4},
	     {0, 1, 5, 6},
	     {0, 1, 4, 6},
	     {0, 1, 4, 5},
	     {0, 1, 3, 6},
	     {0, 1, 3, 5},
	     {0, 1, 3, 4},
	     {0, 1, 2, 6},
	     {0, 1, 2, 5},
	     {0, 1, 2, 4},
	     {0, 1, 2, 3}};

int L5[][4]={{0, 1, 2, 3},  /*  70 combinations for 8 satellites  */
	     {0, 1, 2, 4},
	     {0, 1, 2, 5},
	     {0, 1, 2, 6},
	     {0, 1, 2, 7},
	     {0, 1, 3, 4},
	     {0, 1, 3, 5},
	     {0, 1, 3, 6},
	     {0, 1, 3, 7},
	     {0, 1, 4, 5},
	     {0, 1, 4, 6},
	     {0, 1, 4, 7},
	     {0, 1, 5, 6},
	     {0, 1, 5, 7},
	     {0, 1, 6, 7},
	     {0, 2, 3, 4},
	     {0, 2, 3, 5},
	     {0, 2, 3, 6},
	     {0, 2, 3, 7},
	     {0, 2, 4, 5},
	     {0, 2, 4, 6},
	     {0, 2, 4, 7},
	     {0, 2, 5, 6},
	     {0, 2, 5, 7},
	     {0, 2, 6, 7},
	     {0, 3, 4, 5},
	     {0, 3, 4, 6},
	     {0, 3, 4, 7},
	     {0, 3, 5, 6},
	     {0, 3, 5, 7},
	     {0, 3, 6, 7},
	     {0, 4, 5, 6},
	     {0, 4, 5, 7},
	     {0, 4, 6, 7},
	     {0, 5, 6, 7},
	     {1, 2, 3, 4},
	     {1, 2, 3, 5},
	     {1, 2, 3, 6},
	     {1, 2, 3, 7},
	     {1, 2, 4, 5},
	     {1, 2, 4, 6},
	     {1, 2, 4, 7},
	     {1, 2, 5, 6},
	     {1, 2, 5, 7},
	     {1, 2, 6, 7},
	     {1, 3, 4, 5},
	     {1, 3, 4, 6},
	     {1, 3, 4, 7},
	     {1, 3, 5, 6},
	     {1, 3, 5, 7},
	     {1, 3, 6, 7},
	     {1, 4, 5, 6},
	     {1, 4, 5, 7},
	     {1, 4, 6, 7},
	     {1, 5, 6, 7},
	     {2, 3, 4, 5},
	     {2, 3, 4, 6},
	     {2, 3, 4, 7},
	     {2, 3, 5, 6},
	     {2, 3, 5, 7},
	     {2, 3, 6, 7},
	     {2, 4, 5, 6},
	     {2, 4, 5, 7},
	     {2, 4, 6, 7},
	     {2, 5, 6, 7},
	     {3, 4, 5, 6},
	     {3, 4, 5, 7},
	     {3, 4, 6, 7},
	     {3, 5, 6, 7},
	     {4, 5, 6, 7}};

int totalfixes=0;       /*  The total number of fixes calculated during the
			    session.   */
int presfixes=0;        /*  Fixes for the present site.  */
char aprevstr[80]=" ";
char prevstr[80]="32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32";
/*  Have to lag name writing by one fix  */
char presstr[80]="                    \0";
/*  Assist with name writing lag  */

/*---------  This boundary is here only for historical reasons  -------*/

matrix outmat;

vector outvec;             /*  Global return vector            */
vector nulvec;             /*  Null vector, for global use     */
double outdoub, nuldoub;   /*  Same, except for generic double */
matrix nulmat;             /*  Null matrix, for global use     */
matrix outmat;             /*  Global return matrix            */

/*-------------------------------------------------------------------*/
