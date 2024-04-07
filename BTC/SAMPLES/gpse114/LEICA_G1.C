/*  30/11/92.  Leica_g1.c.  This module contains functions which are
	       (probably) subject to frequent changes.      */

/*===================================================================*/
int ofile(char fname[80])
{

	 if ((raw_file = fopen(fname,"rt"))==NULL)
		 return(0);
	 else
		 return(1);

}
/*-------------------------------------------------------------------*/
int cfile(FILE *anyfile)
{
	 fclose(anyfile);      /*  raw_file    */
	 return(1);
}
/*-------------------------------------------------------------------*/
int reph(FILE *anyfile, matrix *eblock)

/*  Read a block of ephemeris data from a rinex eph file.
    This rather robust little algorithm reads a single block of ephemeris
    data from a rinex-format eph. file, as in Leica files. Next time
    the function gets called, it will return the next block of data. As
    soon as the eof is reached, the function will return zero.
    The robustness comes from:
      - checking for eof at every line read.
      - rejecting damaged blocks and seeking out all non-damaged blocks of
    data.

    In this particular version, data is expected to appear in ascii blocks of
    the following form: (by `number', we mean any integer or floating-point
    number conforming to ieee format)
       10 numbers
	   4    "
	   4    "     (lines separated by crlf)
	   4    "
	   4    "
	   4    "
	   4    "
	 1   number
    Any amount of junk can appear between each of these blocks although
    (of course) it shouldn't, really.
*/

{

vector  *anyv, rsize;
int	fend, lookrec, i, j, k;
char    anystr[100];


  rsize.n=8;
  /*  Set up the block format for the data: each element here contains
      the correct number of numbers for each row in the eph block   */
  rsize.a[0]=10; rsize.a[1]=4; rsize.a[2]=4; rsize.a[3]=4;
  rsize.a[4]=4; rsize.a[5]=4; rsize.a[6]=4; rsize.a[7]=1;

   strcpy(eblock->message," ");
   eblock->err=0;

   /*  Set up anyv so that `getnums' has somewhere to return its result */
   anyv=(vector *)malloc(sizeof(vector));

   fend=0;
   lookrec=1;
   while (lookrec)  {
      k=0; /* begin again from top of structure spec if quit last time  */
      do {/* next 8 lines red all have lngth as specfied in rsize, else quit */
	 fgets(anystr, 200, anyfile);  /*  read a line from input file  */
	 /*  Careful: if line is 80 chars long and you read fgets(..,70,..)
	     then fgets doesn't move byte pointer to end of line, and next
	     time you start reading on same line again! So you make it 200
	     (too many) to force fgets to read to end of line, so that it
	     starts on next line the following time. */
	 if (feof(anyfile)) {fend=1; break;}  /* quit if end-of-file reached */
	 if (getnums(anyv,anystr)!=rsize.a[k]) break;/* any line not correct*/
	 for (i=0;i<rsize.a[k];i++) eblock->a[k][i]=anyv->a[i];  /* then quit */
	 k++;  /*  record the valid row, carry on till all rows done  */
      } while (k<8);
      if (k==8) break;  /*  Total lines in a block is 8  */
      if (fend) {
	 strcpy(eblock->message,"End of file reached. Trash this data.");
	 eblock->err=1;
	 break;
      }
   }
   free(anyv);
   return(!fend);   /*  return zero value if eof reached !  */
}                 /*  ie: don't try to read me again.         */
/*----------------------------------------------------------------------*/
int striso(char *dest, char *src)
/*  4/6/93.  Named "striso" for "string isolate". This function was
    created specifically to extract the marker name from a string
    (single line from obs file) passed to it. The marker name if found
    by working backwards from "MARKER", stripping all spaces trailing
    the name, until a non-blank char is found. This point defines the
    end of the name. All leading blanks are then stripped - leaving
    a pointer to the beginning of the name. The name is returned in *dest.
    For example, if you pass it "   THE QUICK 72         MARKER", it
    will return "THE QUICK 72".  See leictest.c file for a version of
    this which defines the name of the marker up to where the first space
    in the name occurs, then stops. ie. the name can't contain spaces.
*/
{
char *str1="MARKER";
int i,j,k;
int n,N,M;
char *cptr;

   cptr=strstr(src,str1);
   n=(cptr-src-1);
   /*  Strip the trailing blanks from the string:  */
   for (i=0;i<n;i++) { N=(n-i); if (src[n-i]!=' ') break; }
   /*  Strip the leading blanks: */
   for (i=0;i<n;i++) { M=i; if (src[i]!=' ') break; }
   /*  Copy the bit thats left (ie. the name):  */
   for (i=M;i<(N+1);i++) dest[i-M]=src[i]; 
   dest[N-M+1]='\0';  /*  Put in the string terminator  */
   return(0);

}
/*----------------------------------------------------------------------*/
int str2asc(char *dest, char *src)
/*  3/6/93.  This function has the purpose of converting a char string
    to another char string, where the latter is the decimal ascii
    representation of the former. For example, given input "ABC" will
    return the output string "65 66 67". This is all for the benefit
    of matlab, so that it can read the string in as ascii values in a
    matrix and then convert it to a character string. There is no other
    way to get matlab to accept a file containing mixed data and
    characters.      */
{
int i,n;
char anystr[80];

   n=strlen(src);
   strcpy(dest,"\0");
   for (i=0;i<n;i++) {
      sprintf(anystr," %d",(unsigned int)src[i]);
      strcat(dest,anystr);
   }
   strcat(dest," \0");

   return(0);
}
/*--------------------------------------------------------------------*/
int domarkers(char *src)
/*  3/6/93.  Note: At present, this function does not distinguish
    between the stationary and roving units files -- only one of them
    should contain marker names in their rinex obs files. If this is
    not so, then what will happen is that things (stat and rov markers)
    will get very mixed up inside the one file. The marker file inherits
    its name from the stat unit, but has the extension .93m (for example;
    where 'm' stands for marker.)     */
{

char anystr[80]; /*  General purpose wench string     */
char mstr[400];  /*  String for saving the site in matlab format  */
char astr[80];   /*  String for saving the thing in ordinary (non-matlab) format  */

   /*  If the string currently under test does not contain the "MARKER NAME"
       string, then exit the function forthwith. Otherwise, extract the marker
       name and write a [presfixes prevmarker] set to the marker file.  */
   if (strstr(src,"MARKER NAME")!=NULL) {
      striso(anystr,src);

      /*  Pad out to 20 chars - this is necessary, because matlab has to
	  have a rectangular matrix to read in. Padding out to 20 chars
	  implies that the maximum length of marker name we expect is 20
	  characters.    */
      if (strlen(anystr)<20) {
	 while (strlen(anystr)<20) strcat(anystr," ");
      }
      else { anystr[20]='\0';}  /*  Limit the length to 20 chars   */

//      printf("\nWhat you want is %s",anystr);
      str2asc(presstr, anystr); /*  Create it now, for writing next time  */
      sprintf(mstr,"%d ",presfixes); /* String with present no. fixes &
					previous marker name: */
      sprintf(astr,"%d ",presfixes); /*  Do the same thing for orinary format */
      strcat(mstr, prevstr);  /*  mstr - lumped presfixes and prev marker
				  name together for file writing.  */
      strcat(astr, aprevstr); /* astr - the same as mstr except that its in
				 ordinary (non-matlab) format.  */
      strcat(mstr, "\n");  /*  Put in a carriage return!   */
      strcat(astr, "\n");  /*  Do the same thing for astr.  */
      strcpy(prevstr,presstr); /*  prevstr has been used - shift the string reg. */
      strcpy(aprevstr,anystr); /*  Save the same thing in ordinary char format. */
      presfixes=0;  /*  Start counting observations for the next site.  */

      /*  reason for the following `if': to avoid writing the first
	  null and useless string to the marker file at the first site
	  encounter.  */
      if (!((totalfixes==0)&&(presfixes==0))) {
//         printf("\nThe other thing you wanted is %s",mstr);
	 mfile=fopen(mkfilenm,"at");  /*  Append to the marker file  */
	 /*  Only one of the fputs below should be in the code (not commented
	     out), so the choice of format (matlab or non) is made by
	     which string below (mstr or astr) is written to the marker file.  */
	 fputs(mstr,mfile);  /*  This creates the matlab-format file.  */
//         fputs(astr,mfile);  /*  This creates the ordinary (non-matlab) file. */
	 fclose(mfile);
      }
   }
   return(0);
}
/*----------------------------------------------------------------------*/
int rleica(FILE *anyfile, infotype *info, matrix *oblock)
/*
    (This function doesn't appear to pass any data implicitly)
    It passes m (the diff/non-diff mode) and obsfile1 (etc) data
    implicitly

*/

{

vector  *anyv, *satv, *clv;
int	fend, lookrec, i, j, k, nsats;
char    anystr[100], str2[100], satstr[40], clstr[15];

/*   satstr is a string of satellite PRNs from the first line of an
     observation record: eg. " 5G26G 3G17G12G16  " It gets used by
     function  satstr2v which returns the information contained in
     satstr, in the vector satv (see below). The exact max length of
     the string is 38 chars, which seems to indicate (@ 3 chars per
     PRN) that the max expected number of satellites is 12).

     satv contains information about the satellites in a particular
     observation record: the first element contains the number of
     satellites in the block, as does satv.n.  Subsequent elements (up
     to 9 of) contain the satellite PRNs, and the order of the PRNs in
     the same as the order of their data appearing in oblock.

     clstr is a string from the first line of an obs record, containing
     the receiver clock error. It gets used by getnum, which converts it
     to a number, in clv, so that it can be stored in oblock.
*/

   strcpy(oblock->message," ");
   oblock->err=0;
   oblock->m=10;

   /*  Set up anyv and clv so that `getnums' has somewhere to return its
       result. Same for satv and satstr2v. */
   anyv=(vector *)malloc(sizeof(vector));
   satv=(vector *)malloc(sizeof(vector));
   clv=(vector *)malloc(sizeof(vector));

   fend=0;
   lookrec=1;

   while (lookrec)  {
      fgets(anystr, 200, anyfile);  /*  read a line from input file  */
      if (feof(anyfile)) {fend=1; break;}  /* quit if end-of-file reached */

      /*  Note on the if and elseif below: if in non-diff mode, its safe to
	  always domarkers. If in diff mode, its safe to domarkers only when
	  we're reading roving file (obsfile2) data. Otherwise markers
	  (normally only one of them) from the stat file will creep in
	  and corrupt BOTH the markers file and the arithmetic (presfixes)
	  associated with marker file generation. (Take a minute to think
	  carefully about this). For the time being, the user can manually
	  examine the stationary obs file for its marker. Is it worth
	  creating a whole new marker file which contains only one line,
	  just for the stationary unit? This problem is one for future
	  consideration.  */
      if (mm==0) domarkers(anystr);  /*  Check if it contains marker & take appropriate action.  */
      else if (anyfile==obsfile2) domarkers(anystr);

//      printf("\nanystr is %s",anystr);
		       /* ##78 below is specific to the Leica - can be
			  made shorter, like 32, if the clock str missing. */
      if (strlen(anystr)>78) {  /* The following string operations are
      very dodgey if anystr is a short string - reject it immediately if
      its too short */

      /* dissect this proposed first line of the record, to see if it meets
	 all structural requirements: (test is in the next if statement) */
	for (i=0;i<38;i++) satstr[i]=anystr[i+30]; /* Copy relevant substring */
	satstr[38]='\0';  /*  for sat PRNs. Put in the string terminator  */

	   for (i=0;i<13;i++) clstr[i]=anystr[i+67]; /* Copy relevant substring */
	   clstr[13]='\0';  /* for receiver clock error. Put in string terminator */
      }
      else {
	strcpy(satstr," ");
	strcpy(clstr," ");
      }

//      printf("\nanystr is %s",anystr);
//      printf("\nclstr is %s",clstr);
//      printf("\nsatstr is %s",satstr);

      if ((getnums(anyv,anystr)==8)&&(getnums(clv,clstr)==1)&&(satstr2v(satv,satstr))) {
	/*   ==8 .. because sscanf reads the first digit of satstr    */
	/*  if true, then we've got the first line of an obs record.  */
	/* (getnums returns the number of numbers found in the string anystr.
	   what gets stored in anyv is the date and time.)  */
	nsats=(satv->n)-1;  /*  This is the number of satellite PRNs obtained */
	oblock->a[0][7]=clv->a[0]; /*  Record receiver clock error in oblock */
	for (i=0;i<7;i++) oblock->a[0][i]=anyv->a[i]; /* Record date, time, health */
	for (i=0;i<(nsats+1);i++) oblock->a[1][i]=satv->a[i];/* " #sats and PRNs */
	k=0; /* begin again from top of obs structure if quit last time  */
	do {/* next (nsats) lines red all have length 6, else quit */
	   fgets(anystr, 200, anyfile);  /*  read a line from input file  */
	   if (feof(anyfile)) {fend=1; break;}  /* quit if end-of-file reached */
	   if (getnums(anyv,anystr)!=info->colums) break;/* any line not correct*/
	   /* info->colums is 6, for Leica.  */
	   for (i=0;i<info->colums;i++) oblock->a[k+2][i]=anyv->a[i];  /* then quit */
	   k++;  /*  record the valid row, carry on till all rows done  */
	} while (k<nsats);
	if (k==nsats) {
		 oblock->n=nsats+2;  /* Size of obs matrix: 10 X (nsats+2)  */
		 break; /* Successful: quit the loop with no errs  */
	}
	if (fend) {
	   strcpy(oblock->message,"End of file reached. Trash this data.");
	   oblock->err=1;
	   break;
	}
      }
   }
   free(anyv);
   free(satv);
   free(clv);
   return(!fend);   /*  return zero value if eof reached !  */
}                 /*  ie: don't try to read me again.         */

/*----------------------------------------------------------------------*/
int rgarmin(FILE *anyfile, infotype *info, matrix *oblock)
/*
     (See comments in rleica() )
*/

{

vector  *anyv, *satv, *clv;
int	fend, lookrec, i, j, k, nsats;
char    anystr[100], str2[100], satstr[40], clstr[15];

   strcpy(oblock->message," ");
   oblock->err=0;
   oblock->m=10;

   /*  Set up anyv and clv so that `getnums' has somewhere to return its
       result. Same for satv and satstr2v. */
   anyv=(vector *)malloc(sizeof(vector));
   satv=(vector *)malloc(sizeof(vector));
   clv=(vector *)malloc(sizeof(vector));

   fend=0;
   lookrec=1;

   while (lookrec)  {
      fgets(anystr, 200, anyfile);  /*  read a line from input file  */
      if (feof(anyfile)) {fend=1; break;}  /* quit if end-of-file reached */

      /*  See the comments in rleica() at this position.  */
      if (mm==0) domarkers(anystr);
      else if (anyfile==obsfile2) domarkers(anystr);

//      printf("\nanystr is %s",anystr);
      if (strlen(anystr)>32) {  /* The following string operations are
      very dodgey if anystr is a short string - reject it immediately if
      its too short */

      /* dissect this proposed first line of the record, to see if it meets
	 all structural requirements: (test is in the next if statement) */
	for (i=0;i<38;i++) satstr[i]=anystr[i+30]; /* Copy relevant substring */
	satstr[38]='\0';  /*  for sat PRNs. Put in the string terminator  */
	strcpy(clstr,"0");  /*  There is no clock error string in garmin.  */
      }
      else {
	strcpy(satstr," ");
	strcpy(clstr," ");
      }

//      printf("\nanystr is %s",anystr);
//      printf("\nclstr is %s",clstr);
//      printf("\nsatstr is %s",satstr);

      if ((getnums(anyv,anystr)==8)&&(getnums(clv,clstr)==1)&&(satstr2v(satv,satstr))) {
	/*   ==8 .. because sscanf reads the first digit of satstr    */
	/*  if true, then we've got the first line of an obs record.  */
	/* (getnums returns the number of numbers found in the string anystr.
	   what gets stored in anyv is the date and time.)  */
	nsats=(satv->n)-1;  /*  This is the number of satellite PRNs obtained */
	oblock->a[0][7]=clv->a[0]; /*  Record receiver clock error in oblock */
	for (i=0;i<7;i++) oblock->a[0][i]=anyv->a[i]; /* Record date, time, health */
	for (i=0;i<(nsats+1);i++) oblock->a[1][i]=satv->a[i];/* " #sats and PRNs */
	k=0; /* begin again from top of obs structure if quit last time  */
	do {/* next (nsats) lines red all have length 6, else quit */
	   fgets(anystr, 200, anyfile);  /*  read a line from input file  */
	   if (feof(anyfile)) {fend=1; break;}  /* quit if end-of-file reached */
	   if (getnums(anyv,anystr)!=info->colums) break;/* any line not correct*/
	   /* info->colums is 2, for Garmin.  */
	   for (i=0;i<info->colums;i++) oblock->a[k+2][i]=anyv->a[i];  /* then quit */
	   k++;  /*  record the valid row, carry on till all rows done  */
	} while (k<nsats);
	if (k==nsats) {
		 oblock->n=nsats+2;  /* Size of obs matrix: 10 X (nsats+2)  */
		 break; /* Successful: quit the loop with no errs  */
	}
	if (fend) {
	   strcpy(oblock->message,"End of file reached. Trash this data.");
	   oblock->err=1;
	   break;
	}
      }
   }
   free(anyv);
   free(satv);
   free(clv);
   return(!fend);   /*  return zero value if eof reached !  */
}                 /*  ie: don't try to read me again.         */

/*----------------------------------------------------------------------*/
int robs(FILE *anyfile, infotype *info, matrix *oblock)
/*
    (This function doesn't appear to pass any data implicitly
    Note that the comments here apply mainly to the substance of
    this function, which lies in its subfunctions - see rleica,
    rgarmin, etc.  robs is just a shell from which functions
    tailored for reading different receiver's data files, are called.)

    Read a block of observation data from a rinex obs file.
    This (also) rather robust little algorithm reads a single block of
    obs from a rinex-format observation file, as in Leica files. Next time
    the function gets called, it will return the next block of data. As
    soon as the eof is reached, the function will return zero.
    The robustness comes from:
      - checking for eof at every line read.
      - rejecting damaged blocks and seeking out all non-damaged blocks of
    data.

    The info is an input which gives details of the type of receiver
    used in the data capture. This is necessary, to define the structure
    of the data file, which differs among GPS units. When info is known,
    robs reads in only data blocks with the correct structure - all
    others are rejected.

    In this particular version, data is expected to appear in ascii blocks of
    the following form: (by `number', we mean any integer or floating-point
    number conforming to ieee format)
	 7 numbers, 17 chars giving sats & order, 1 number (receiver clock err)
	   6    "
	   6    "     (lines separated by crlf)
	   .    "
	   6    "   (total rows containing 6 cols changes, & equals noof sats)
    Any amount of junk can appear between each of these blocks although
    (of course) it shouldn't, really.

     The data output by this function (to the matrix oblock) has the following
     structure:
       -
      |	YY MM DD   HH MM SS.SSSSSSS   EventFlag ClkErr
      |	nsats               PRN   PRN   ...   PRN     (no of prns = nsats)
      |	CODE PHASE Strength CODE PHASE Strength       -
      |			   .                           |_   nsats of these
      |			   .                           |
      |	CODE PHASE Strength CODE PHASE Strength       -
       -
       LLI (loss of lock indicator) gets jammed between PHASE and Strength,
       giving less than 6 numbers per line when spaces are used to delimit
       numbers - this is used to reject entire records where the digit
       appears. (although it needn't (?) - for possible later improvement).

    NB:  This function assumes the sat clock error in the obs data
    is at a fixed position  (67 - search for 67 below) - if this position
    is changed (either manually, or by using a different GPS unit), blocks
    of data will be rejected.

    Now, although one might at first be tempted to reject obs records
    where the pseudoranges are negative (as typically occurs for
    a new site, when the unit has just been turned on), one finds
    that these negative data often give perfectly sensible results:
    they are negative simply because the receiver clock offset is so
    bad. After the first result, however, this is corrected, and so
    the pseudoranges look a bit nicer.

*/

{

int  fend;

   switch (info->rtypen)  {
      case LEICA:     fend=rleica(anyfile, info, oblock); break;
      case GARMIN:    fend=rgarmin(anyfile, info, oblock); break;
   }

   return(fend);   /*  return zero value if eof reached !  */
}                 /*  ie: don't try to read me again.         */
/*----------------------------------------------------------------------*/
int getnums(vector *anyv, char *anyst)
/*  Takes an everyday string called anyst and does its best to extract
    an arbitrary number of numbers. The numbers should be in ascii
    double-precision 'D' format, and should be separated by one or
    more spaces, or a '-' (not preceded by a 'D') and possibly some
    spaces. The output is the vector *anyv, the number of elements is
    returned in (*anyv).n and the elements in (*anyv).a[]. Error condition
    is returned in (*anyv).err, if this is non-zero then the results should
    be discarded.
*/
{

int i,j,k;
double x[]={0,0,0,0,0,0,0,0,0,0,0,0};
char *cptr,str1[100];

    anyv->err=1;
    anyv->n=0;

/*   Replace occurrences of 'D' with 'E' - sscanf doesn't recognise the
     'D' for double-precision exponentiation:                            */

    while ((cptr=strchr(anyst,'D'))!=NULL) *cptr='E';

    if ((i=sscanf(anyst,"%le %le %le %le %le %le %le %le %le %le",&x[0],&x[1],&x[2],&x[3],&x[4],&x[5],&x[6],&x[7],&x[8],&x[9]))!=EOF) {
       anyv->err=0;
       anyv->n=i;
       for (j=0;j<i;j++) anyv->a[j]=x[j];
    }

    return(anyv->n);

}

/*-----------------------------------------------------------------------*/
int satstr2v(vector *satv, char *satstr)
/*   Takes satellite PRN string as it comes from rinex obs file and converts
     it to a vector. The first element of the vector (as well as satv->n) is
     the number of satellites, and the subsequent elements contain the
     satellite PRNs, in the correct order.
	   satstr2v returns  a non-zero value if no error occurred. Zero
     is returned if something went wrong.   */
{
int i, j, s[13];    /*  satvals[13] would be nice     */
char *cptr;

//       " 5G26G 3G17G12G16                       "
// try a different number of spaces here!

/*   Replace occurrences of 'G' in satstr with spaces, so that all the
     information in satstr can be scanned in with sscanf:   */

    satv->err=1;  /*  Guilty until proven innocent.   */
    while ((cptr=strchr(satstr,'G'))!=NULL) *cptr=' ';
    if ((i=sscanf(satstr,"%d %d %d %d %d %d %d %d %d %d %d %d %d",\
      &s[0],&s[1],&s[2],&s[3],&s[4],&s[5],&s[6],&s[7],&s[8],&s[9],\
      &s[10],&s[11],&s[12]))!=EOF) {
	 for (j=0;j<i;j++) satv->a[j]=(double)s[j]; /* Copy values to return var */
	 satv->n=i;  /*  Vector size equals no. of elements scanned in.  */
	 if (i==(s[0]+1))
		 satv->err=0; /*  No of PRNs scanned equals number claimed   */
			      /*  in the first element of satstr - otherwise */
			      /*  data inconsistent or corrupted.            */
    }
    else
	 satv->n=0;  /*  We'd have quite a serious problem with data, here. */

    return(!satv->err);

}
/*-----------------------------------------------------------------------*/
int puteph(ephemeris *eph, matrix *z)
{
/*   The object of this little swapping routine is to take Leica's rinex nav
     data and put it into the format which was used in the original eph alg.
     Originally, the eph algorithm was written for Magellan data, which
     does NOT conform to the Rinex standard fully. See, for example, all
     the NB's below. Further, the Magellan eph algorithm is slightly
     different to the accepted one - compare the Magellan code with the
     Leica code for eph algorithms - to avoid re-writing software, the
     Leica code is a hybrid of the RINEX and Magellan algorithms, passing
     sinw and cosw,  as per the Magellan data, but passing  sqrt(a) and dn,
     as per the rinex convention. The equation for Ok in the Leica eph
     algorithm has been implemented as per convention, and the 'true' (rinex)
     m0 is passed to the algorithm (as opposed to Magellan's fake m0 and Ok).

     Future code for different GPS units should have their own file reading
     and data extraction modules, and also their own conversion modules, such
     as this one. From this point on, though, (assuming they all use the same
     eph, etc, algorithms) the same code is used for all.

*/

 eph->prn=(int)z->a[0][0]; /*  Satellite ID number  */
 eph->wntoe=(int)z->a[5][2];     /*  Week number from original GPS launch date */
 eph->toe=(long int)z->a[3][0];  /* Time of ephemeris, eph data taken & ref. to  */
 eph->a=z->a[2][3];        /* (NB: SQUARE ROOT OF Semi-major axis, for Leica)  */
 eph->e=z->a[2][1];        /* Eccentricity  */
 eph->m0=z->a[1][3];       /* Mean anomaly at reference time  */
 eph->omegadt=z->a[4][3];  /* Rate of right ascension  */
 eph->omega0=z->a[3][2];   /* Longitude or asc node of orbit plane (wkly epoch)*/
 eph->sinw=sin(z->a[4][2]);/* (NB): Sine of argument of perigree  */
 eph->cosw=cos(z->a[4][2]);/* (NB): Cosine of argument of perigree  */
 eph->n=z->a[1][2];        /* Mean motion (NB: actually DIFFERENCE, dn for Leica) */
 eph->i0=z->a[4][0];       /* Inclination angle at reference time  */
 eph->idot=z->a[5][0];     /* Rate of inclination angle  */
 eph->crc=z->a[4][1];      /* Ampl of cos harmonic corrn term to orbit radius  */
 eph->crs=z->a[1][1];      /* Ampl of sin harmonic corrn term to orbit radius  */
 eph->cuc=z->a[2][0];      /* Ampl of cos harmonic corrn term to arg of lat  */
 eph->cus=z->a[2][2];      /* Ampl of sin harmonic corrn term to arg of lat  */
 eph->cic=z->a[3][1];      /* Ampl of cos harmonic corrn term to angle incl  */
 eph->cis=z->a[3][3];      /* Ampl of sin harmonic corrn term to angle incl  */
 eph->af0=z->a[0][7];      /* Poly correction term a0 for satellite clock.  */
 eph->af1=z->a[0][8];      /* Poly correction term a1 for satellite clock.  */
 eph->af2=z->a[0][9];      /* Poly correction term a2 for satellite clock.  */
 eph->toc=z->a[3][0];      /* Time of capture (equals TOE)..(or AODC in Leica?) */
 eph->ura=z->a[6][0];      /* User Range Accuracy (=SV Accuracy, see ICD pg. 67 */
 eph->aode=(int)z->a[1][0];/* Age of ephemeris data  */

 /*  Inputs unused:  a[5][1]  Codes on L2 channel (Seems to always be 0)
		     a[5][3]  L2 P data flag (Seems to always be 0)
		     a[6][0]  SV Accuracy ( 1 .. 9 )
		     a[6][1]  SV health, MSB (Seems to always be 0)
		     a[6][2]  TGD(s) (Could this be group delay, for phase?)
		     a[6][3]  AODC(s) (Looks just like AODE)
		     a[7][0]  Transmission time of message - (Looks just
			      like TOE)

 Outputs unassigned: None (!)

 */

 return(1);
}
/*-----------------------------------------------------------------------*/
int checkselect()
/*  13/1/93  Checkselect performs the following functions:

    (i)  this has changed ..> Finds the first four satellites in the current obs record
    ('xobs') for which there is data in the ephrec. Later, this can be
    improved to selecting the satellites on the basis of signal strength as
    well, and for diff methods, on the basis of agreement with the other
    obs record. The selected satellites for the current obs record are
    returned in the global variable thesats.
    (ii) Copies the eph data for the 4 selected satellites from ethelot
    into aeph[].
    (iii)  Copies the time and 4 pseudoranges  for the 4 selected
    satellites from xobs into anobs.
    (iv)   Checks that times agree for the eph and obs files. Later, other
    types of consistency check may be put in here.

    aeph[] and anobs (of type rinex) above, are the correct data
    and type for findrec(), which should then be called to produce
    a positional fix for the receiver.

    Ultimately, checkselect() should return zero when some sort of
    error occurs. At the moment it returns the number of sats for which eph
    records were found. (4 is the max. number). - if less than 4, there's
    a problem and findrec() shouldn't be attempted.

*/
{

double t;
long int tks[4];

int i,j,k,m,n;

/*  See main() for the definitions of zthesats, zanobs and *zaeph */
/*  Structure of these variables, in array format:


(index)         0        1        2        3        4       ...     N (6 or 7, normally)
		-----    -----    -----    -----    -----           -----

zthesats        no.      prn      prn      prn      prn     ...      prn
		 -        ss      ss        ss       ss     ...       ss

thesats         prn      prn      prn      prn
		 ss       ss       ss       ss

zanobs          pr       pr       pr       pr       pr      ...      pr
anobs.prs       pr       pr       pr       pr


*zaeph          ephrec   ephrec   ephrec   ephrec   ephrec  ...      ephrec
aeph            ephrec   ephrec   ephrec   ephrec


NB: The `ephrec's above are in no way related to the ephrec data structure -
    they indicate only the each element is a record of type ephemeris.
*/

/*    See which satellites in the observation record we have eph information
      for:      */
   j=0;
   zthesats[0][0]=0;
   /*  Trust the obs data (esp. xobs.a[1][0], the no. of sats in an obs) */
   m=(int)xobs.a[1][0];
   for (i=1;i<=m;i++)  { /* Work through the list of satellites in obs */
      k=feph((int)xobs.a[1][i]);  /* See if this satellite is in the eph record */
      if (k>=0) {  /*  If so, then do the following:  */
	 j++; /*  Increment the number of matches found  */
	 zthesats[0][j]=xobs.a[1][i]; /* Store the sat prn in`zthesats'array */
	 zthesats[0][0]=j; /*  Remember the number of matches found   */
	 zthesats[1][j]=xobs.a[1+i][2]; /* Copy sig strength from 3rd col. of xobs */
	 zanobs[j-1]=xobs.a[1+i][0]; /* Copy pseudorange for sat to anobs */
/* NB: A complete nother column (col. 4) of prs is being ignored here!!  */
/*  See the rinex definition - the stuff being ignored is P2, pseudorange
    using p-code on L2.                               */
	 zaeph[j-1]=ethelot[k]; /* Copy the eph data for that sat to aeph  */
      }  
   }

// printf("\nAfter see eph, zthesats[0][0] = %d",zthesats[0][0]);
// printf("\nafter see eph, m (xobs) = %d", m);


/*     At this point, we have a complete set of pseudoranges (in zanobs)
       and pointers to eph data (in zaeph), and the corresponding
       number and list of satellites for which both observational data and
       ephemeris data exist (in zthesats). There should be a minimum of 4 sats
       but typically there would be more. The task now is to select only
       4 of these, for use in `findrec()', the function which calculates
       the receiver position.  Also, in selecting these 4 satellites, their
       data must be put into data structures which match the types in the
       parameter list of findrec(). This is done below, and in subsequent
       functions such as maskselect and dopselect. For clarity,
       the correspondences between data we have at this point, and
       the structures the data will be copied to, are:

       int        zthesats[2][MAXEPHS]  -----    int       thesats[2][4]
       double     zanobs[MAXEPHS]       -----    rinex     anobs
       ephemeris  *zaeph[MAXEPHS]       -----    ephemeris aeph[4]

       Any 4 satellites in the `eligible' list will do, for the purposes
       of the findrec function. The question of which satellites
       to select is  a completely open-ended problem, and fertile
       ground for software development and experimentation. Satellite
       selection depends on the following factors:

	   - The signal strength for each satellite
	   - The dop for different combinations of satellites
	   - The ura (user range accuracy) for each satellite
	   - SV health
	   - The mask angle (elevation above the horizon) of the sat
	   - The aode (age of eph data)
	   - Possibly other factors, such as atmospheric conditions

       Before beginning to decide how to make the selection, based on
       these factors, the exact meaning of each factor and how it affects
       the results, needs to be discovered. Even then, how does one decide
       on the order in which the above should be used to screen the sats,
       and the algorithm?

       At the moment, this software does the selection like this:
	   - Make sure there is up-to-date eph data for all sats
	   - Toss out all sats which have an elevation below the 5
	     degree threshold.
	   - Calculate dops for all combinations of satellites, and
	     select the set which gives the best dop.
       Which could be considered a somewhat simplistic algorithm. To
       illustrate why, consider the following hypothetical
       situation:
	   There are 6 sats in an obs record. One is removed because
       its elevation is 4.9 degrees. The remaining 5 combinations
       give bad dops, the best of which is 12.99, so the resulting
       fix is not very good. As it turns out, if the sat with 4.9 deg
       elevation had been used, the dop would have been 3.05, and a
       much better fix would have been had, because the 4.9 deg mask
       angle has been conservatively selected. One can't improve the
       algorithm by making the critical mask angle smaller (say, 2
       degrees), because then, for a particular combination of
       satellites including the one with the small elevation, one may have
       a very good dop, and end up with a poor fix - a much better fix
       might have been had by taking the combination with a slightly
       larger dop. But how do you build this kind of intelligence
       into the software? And imagine how complex these algorithms must
       become if all the factors in the list above, are taken into
       account.

       Before one can start to make further improvements in the selection
       algorithm, however, the meaning and significance of all the
       factors affecting fix accuracy have to be better understood. For
       example, what does signal strength mean? - does the accuracy of
       the pseudorange deteriorate with signal strength? How? (linearly,
       or exponentially near 4? 5?, not at all?) What DOES deteriorate with
       signal strength, if not pseudorange accuracy. If nothing deteriorates,
       then why have it at all? etc. etc.

       But I'm only the programmer here. Who am I to question these things?

*/

   getime(&t, &xobs);  /*  Get the time of current observation, xobs.  */
   anobs.t=t;  /*  Part of the definition of anobs - see below. */
   t_now=t;  /*  Set the time of the current obs, for checking in main(),
		    as to whether or not an update of the ephemeris records
		    is needed.      */

/*    m and zthesats[0][0] are the equivalent of comrec and comcount in
      commons12(). The operations immediately below are required in
      commons12() but not in checkselect(). However, they are inserted
      here for consistency. - maskselect() acquires the number of satellites
      through zthesats, but the only way maskselect12 can get the number
      of satellites is from comcount (equivalent of zthesats[0][0]) */
   m=4;
   if (zthesats[0][0]<4) {
      m=zthesats[0][0];
   }

/*  Put maskselect() here:  */
/*  Note! maskselect will modify zthesats[0][0] AND m, so a repeat of the
    above operations is necessary here:  */

   /*  This totalfixes condition is required here because we need to do at
       least one receiver fix (even if approximate) before we can calc
       elevation angles of satellites!    */

   if (totalfixes>1) {
     maskselect();
   }

   /* This function returns m, the no of sats with ephs.   m has a max
      value of 4. Only the satellites which have been selected on the dop
      basis will be checked for eph data age. A future improvement here
      may be to do a dop-age tradeoff: even if dop is good a sat may be
      rejected if its data is very old (see icd/gps notes for curve of
      accuracy vs. sat age.). The algorithm for doing this could be
      quite hairy!                    */
   m=4;
   if (zthesats[0][0]<4) {
      m=zthesats[0][0];
   }

/*  Put firstselect()  OR  dopselect() here:
    Try to do dopselect() only if there are enough satellites:
*/
   if (zthesats[0][0]>3) {
      dopselect();
//      firstselect();  /*  Simply select the first 4 satellites  */
   }

/*  Calculate t: this calculation serves two purposes, to provide data
    for the date check, and the t input for findrec.

    A simple method of checking dates in the eph and obs files is used
    here: toe (from eph file) and t (from obs file) are compared.
    (only date and time are given in obs file - t isn't given
     explicitly, it has to be calculated from the date and time.)
    TOE should not be more than a few hours behind, or in front of t,
    otherwise the satellite ephemeris is inaccurate. Since the values of t and
    toe are modulo 1 week, there is a small but definite chance that
    an eph file from many weeks ago (or ahead), in integral multiples of
    weeks, could look like it belongs with the obs file. In this case
    the satellite prns shouldn't match. If they happen to, the results
    will simply be trash. A better way of doing the date comparison (for
    future improvement) would be to convert the dates in the eph and obs
    files to seconds since the beginning of the century, long int t1 and
    t2. This is an unambiguous check - if they differ by more than 1 hour,
    generate a warning, if they differ by more than a day, generate an error.

    The date comparison for the obs file is done with each of the four
    selected eph records. If any one of them is bad, a message is generated.
*/

/*  The time is found in getime (see above)  */
      for (i=0;i<m;i++) { /* Use m (not 4)- don't try to go beyond what exists */
	 tks[i]=aeph[i].toe-(long int)t;
	 if (tks[i]>302400)  tks[i]-=604800;
	 if (tks[i]<-302400) tks[i]+=604800;
	 /*  This correction is necessary here, so that the date
	     comparison makes sense. Were this method of date comparison
	     not being used, this correction could be left to eph2uvw() -
	     go now, and have a look at the code for eph2uvw(). */
      }

   /*  NB: have to have the if below, because we DON'T want to check
       sat eph data age if m<=3. If there are <=3 satellites, dopselect
       doesn't get called (the calcs won't be done), hence aeph is not
       created/updated, hence aeph data is nonexistent or meaningless.  */
   if (m>3) {
      for (i=0;i<m;i++) {  /*  See comment in for loop above  */
	 if (abs1((double)tks[i]) > abs1((double)VERYOLD)) {
	    /*  Generate a more serious message  */
	    printf("\nWarning: satellite %d data is very old.",aeph[i].prn);
	    /* Later, possibly put these into error reporting strings.   */
//	    printf("\nt is %d",(long int)t);
	 }
	 else if (abs1((double)tks[i]) > abs1((double)OLD)) {
	    /*  Generate a message  */
	    printf("\nWarning: satellite %d data is oldish.",aeph[i].prn);
//	    printf("\nt is %d",(long int)t);
	 }
      }
      /*  See comment about the age checking above, in commons12().    */
   }
   return(m);
}
/*-----------------------------------------------------------------------*/
int checkselect1()
/*  16/2/93  checkselect1() performs the following function: (the
    same comment apply to checkselect2().) It finds the first four
    satellites in the current obs1 record ('xobs1') for which there
    is data in the ephrec, and copies the data for the satellites to
    the structures zthesats1, zanobs1 and zaeph1.

    The following are passed implicitly, as global variables:

       matrix    xobs1

       int       zthesats1[2][MAXEPHS];
       double    zanobs1[MAXEPHS];
       ephemeris *zaeph1[MAXEPHS];

    The structure of these variables, in array format, is: (take a look
    at this table when struggling to figure out what the **** is going
    on with the data in this function)

(index)         0        1        2        3        4       ...     N (6 or 7, normally)
		-----    -----    -----    -----    -----           -----

zthesats        no.      prn      prn      prn      prn     ...      prn
		 -        ss      ss        ss       ss     ...       ss

thesats         prn      prn      prn      prn
		 ss       ss       ss       ss

zanobs,         pr       pr       pr       pr       pr      ...      pr
anobsx1,
anobsx2

anobs1.prs,     pr       pr       pr       pr
anobs2.prs

*zaeph,         ephrec   ephrec   ephrec   ephrec   ephrec  ...      ephrec
*aephx

aeph            ephrec   ephrec   ephrec   ephrec


*/
{

int i,j,k,m,n;

/*  Find sats for which eph information exsists:  */
   j=0;
   zthesats1[0][0]=0;
   /*  Trust the obs data (esp. xobs1.a[1][0], the no. of sats in an obs) */
   m=(int)xobs1.a[1][0];
   for (i=1;i<=m;i++)  { /* Work through the list of satellites in obs1 */
      k=feph((int)xobs1.a[1][i]);  /* See if this satellite is in the eph record */
      if (k>=0) {  /*  If so, then do the following:  */
	 j++; /*  Increment the number of matches found  */
	 zthesats1[0][j]=xobs1.a[1][i]; /* Store the sat prn in`zthesats1'array */
	 zthesats1[0][0]=j; /*  Remember the number of matches found   */
	 zthesats1[1][j]=xobs1.a[1+i][2]; /* Copy sig strength from 3rd col. of xobs1 */
	 zanobs1[j-1]=xobs1.a[1+i][0]; /* Copy pseudorange for sat to anobs1 */
/* NB: A complete nother column (col. 4) of prs is being ignored here!!  */
	 zaeph1[j-1]=ethelot[k]; /* Copy the eph data for that sat to aeph  */
	 /*  Note in the above line: the differences between zaeph1 and
	     zaeph2 are `dummy' because all of the eph data in ethelot comes
	     from ename1, anyway !                    */
      }
   }

/*     At this point, we have a complete set of pseudoranges (in zanobs)
       and pointers to eph data (in zaeph), and the corresponding
       number and list of satellites for which both observational data and
       ephemeris data exist (in zthesats). There should be a minimum of 4 sats
       but typically there would be more. The task now is to select
       4 of these which are common to both the obs1 and obs2 records. This
       is done by the common12() function.
*/

   if (zthesats1[0][0]<4) {
/* Generate an error message - not enough eph and/or obs data for this
   observation. The value of reporting information explicitly (by
   returning it) or by generating an error condition, is dubious -
   commons12() checks the no. of matches and therefore (indirectly)
   confirms that there are enough satellites at this point  */
      printf("\nStat record has only %d sats with eph records.", zthesats1[0][0]);
      if (errcon<3) {
	 errcon=2;
	 strcpy(errmess,"Stat obs record doesn't have enough sats with eph records.");
      }
   }

   return(1);
}
/*-----------------------------------------------------------------------*/
int checkselect2()
/*  16/2/93  checkselect2(): see the comments for checkselect1; operations
    are performed on the following data: in: xobs2, out: zthesats2, zanobs2,
    zaeph2.

    The following are passed implicitly, as global variables:

       matrix    xobs2

       int       zthesats2[2][MAXEPHS];
       double    zanobs2[MAXEPHS];
       ephemeris *zaeph2[MAXEPHS];

    See comments in checkselect1(), on the structure of these variables

*/

{
int i,j,k,m,n;

   j=0;
   zthesats2[0][0]=0;
   m=(int)xobs2.a[1][0];
   for (i=1;i<=m;i++)  { /* Work through the list of satellites in obs2 */
      k=feph((int)xobs2.a[1][i]);  /* See if this satellite is in the eph records */
      if (k>=0) {  /*  If so, then do the following:  */
	 j++; /*  Increment the number of matches found  */
	 zthesats2[0][j]=xobs2.a[1][i]; /* Store the sat prn in`zthesats2'array */
	 zthesats2[0][0]=j; /*  Remember the number of matches found   */
	 zthesats2[1][j]=xobs2.a[1+i][2]; /* Copy sig strength from 3rd col. of xobs2 */
	 zanobs2[j-1]=xobs2.a[1+i][0]; /* Copy pseudorange for sat to anobs2 */
/* NB: A complete nother column (col. 4) of prs is being ignored here!!  */
	 zaeph2[j-1]=ethelot[k]; /* Copy the eph data for that sat to aeph  */
	 /*  Note in the above line: the differences between zaeph1 and
	     zaeph2 are `dummy' because all of the eph data in ethelot comes
	     from ename1, anyway !                    */
      }
   }


/*     See comment in checkselect1()         */
   if (zthesats2[0][0]<4) {
/*  See comment in checkselect1()       */
      printf("\nRov record has only %d sats with eph records.", zthesats2[0][0]);
      if (errcon<3) {
	 errcon=2;
	 strcpy(errmess,"Roving obs record doesn't have enough sats with eph records.");
      }
   }

   return(1);
}
/*-----------------------------------------------------------------------*/
int updeph(matrix *aobs)

/*  Remember: in updeph(matrix *aobs) above, aobs is being passed only so
    that updeph can get a hold of the time of obs  */

/*  Passed implicitly (at the moment) are the name of the eph file
    and udates. */
/*  14/1/93   This function supersedes ueph(). It employs a different
    algorithm for updating the ephemeris records held in memory. It
    may be a little less efficient, but should be much more robust than
    ueph. The cause of our problem in finding an algorithm for updating
    the eph record is that we have no idea why the ephemeris file is
    structured the way it is: (i)  When and why does the GPS unit decide to
    download more eph data?  (ii) How can you tell where these boundaries
    are - (between downloads). toe in the record certainly offers no clues,
    it can be whatever the satellite decides it should be. (iii) The Leica
    eph file's satellite prns and toes seem to be pretty scrambled, and
    the only reliable way of getting the best ephs for a particular
    observations (or set of), seems to be: (ie. the algorithm is:)

    Using the date and time for the current observation calculate tobs,
    the time of current observation, as toe would be calculated for eph
    data - ie. seconds since the beginning of the GPS week. (Later, to
    reduce the ambiguity factor, make this seconds since the beginning
    of the century.
    Open the eph file and make a complete pass through it, then close it.
    During this pass, add every new satellite found to the eph record.
    If a satellite is already in the eph record, check if toe for the record
    just read in is closer to tobs than the one in the eph record. If it
    is closer, overwrite the eph data in the eph record with the new one. If
    not, ignore the record just read in.

    When software is running in differential mode, it will be important
    to select 4 common satellites to both sets of data, as well as use
    the same eph data for all 4 satellites. This amounts to stipulating
    that only ONE of the eph files be used to get eph data for BOTH sets
    of observations. This will be done.

    For more comments on eph file usage, see the function opnfiles12(),
    below.

    Improvements:
	-  Make ename an input arg (pass it explicitly).
	-  ...
*/

{
static matrix aeblock; /* `aeblock' - for `an ephemeris block' .     */
static int eoeph=0; /*  For `end of eph (file)'  */
int i,j,k,N;
int aprn;  /* `aprn' - for `a prn'.   */
int recsread;  /*  for `records read' - counts the number of eph records
		   read in from ephfile, for each call of this function.  */

double tobs;   /* Time of obs data - we want to get as close as possible
		    to this, with the eph records.  */
long int toe, age; /*  toe for the record just read in, and age for its
		       `distance' from the time of observation.   */
long int ptoe, page; /*  Exactly the same as above (toe and age), except
			 that the data applies to the eph record which is
			 already in the ethelot database, and is (possibly)
			 about to be overwritten.      */

   recsread=0;  /*  Reset the record count to zero.   */

   getime(&tobs, aobs);  /*  Calculate tobs, the time for current observatn */
   t_last=tobs;          /*  Record this as the observational time at which
			     the last update of the eph database was made -
			     so that a decision (in main()) can be made, as
			     to when the dbase should be updated again.  */

   /*  Beware here: this (below) assumes that ename, the name of the
       ephemeris file has already been read in, in opnfiles() :      */

   ephfile=fopen(ename,"rt");
   if (ephfile==NULL) {  /*  This should never happen - an earlier check */
			 /*  in opnfiles should have caught this first.  */
      printf("\nCouldn't open ephfile successfully.  ");
   }
   /*  Define aeblock to read records into  */
   aeblock.n=8; /* Necessary here, because the aeblock defined in main()  */
   aeblock.m=10;  /* can't be reached here.   */
   strcpy(aeblock.message," ");
   aeblock.err=0;
   for (i=0;i<MAXN;i++){
      for (j=0;j<MAXN;j++) {
	 aeblock.a[i][j] = 0.0;
      }
   }


//   ephrec[0]=0;
   killeph();  /* Release all memory dynamically allocated to ethelot in
		  the last update of eph data. killeph() also resets
		  ephrec[0] to 0, indicating that the eph database is empty */

   eoeph=!(reph(ephfile,&aeblock));  /* Read in the first block of eph data */

   while (!eoeph) {
      aprn=(int)aeblock.a[0][0];    /*  satellite id (prn), from aeblock  */
      /*  Deposit the contents of aeblock into a record in ethelot:   */
      if ((k=feph(aprn))<0) {  /*  Check to see if the satellite is in ethelot */
	 /*  If the satellite in aeblock doesn't exist in ethelot, then create its
	     record and transfer the data from aeblock to ethelot, using puteph */
	 ephrec[0]=ephrec[0]+1; /*  Tell the dbase that it's one record bigger  */
	 N=ephrec[0];  /*  This is the new number of available prns.  */
	 ephrec[N]=aprn;  /*  Add the prn to the record of available prns */
	 ethelot[N-1]=(ephemeris *)malloc(sizeof(ephemeris)); /*  Create the space */
	 puteph(ethelot[N-1],&aeblock); /* Convert to eph, store in ethelot */
      }
      else {
      /* If the satellite in aeblock does exist in ethelot, then overwrite the old
	 data for the prn in ethelot, with the new data from aeblock, using puteph.
	 However, only overwrite if the the block just red in is more
	 up-to-date than the one already in ethelot.
      */
	 toe=(long int)aeblock.a[3][0];  /*  Get toe for this eph reading.  */
	 age=(long int)abs1((double)toe-tobs); /*  Calculate the
	 `distance' of tobs from toe. All the type conversions are necessary
	 because stupid abs doesn't work for long ints.    */

	 ptoe=ethelot[k]->toe;  /*  Get toe for same prn which is already in ethelot  */
	 page=(long int)abs1((double)ptoe-tobs); /*  Calculate
	 `distance' for eph which is already in ethelot  */

	 if (age<page)  {   /*  If the eph data just read in is closer, in
	    time, to the one in ethelot, then overwrite the record in
	    ethelot with the new one. Otherwise, do nothing (ignore rec in) */
	    puteph(ethelot[k],&aeblock); /* Convert to eph, store in ethelot */
	 }
      }
      recsread+=1;  /*  We just red in another eph record.  */
      /*  ephrec[0] keeps track of the total number of satellites we have
	  eph records for.  */
      eoeph=!(reph(ephfile,&aeblock));  /* Read in another block of eph data */
   }
   fclose(ephfile);     /*  Close the ephemeris file  */

if (mm>0)  {  /*  If we're in diff mode, repeat the whole damn reading
		  exercise for the mobile eph record.        */
   ephfile=fopen(ename2,"rt");
   if (ephfile==NULL) {  /*  This should never happen - an earlier check */
			 /*  in opnfiles should have caught this first.  */
      printf("\nCouldn't open ephfile %s", ename2);
   }
   eoeph=!(reph(ephfile,&aeblock));  /* Read in the first block of eph data */
   while (!eoeph) {
      aprn=(int)aeblock.a[0][0];    /*  satellite id (prn), from aeblock  */
      /*  Deposit the contents of aeblock into a record in ethelot:   */
      if ((k=feph(aprn))<0) {  /*  Check to see if the satellite is in ethelot */
	 /*  If the satellite in aeblock doesn't exist in ethelot, then create its
	     record and transfer the data from aeblock to ethelot, using puteph */
	 ephrec[0]=ephrec[0]+1; /*  Tell the dbase that it's one record bigger  */
	 N=ephrec[0];  /*  This is the new number of available prns.  */
	 ephrec[N]=aprn;  /*  Add the prn to the record of available prns */
	 ethelot[N-1]=(ephemeris *)malloc(sizeof(ephemeris)); /*  Create the space */
	 puteph(ethelot[N-1],&aeblock); /* Convert to eph, store in ethelot */
      }
      else {
      /* If the satellite in aeblock does exist in ethelot, then overwrite the old
	 data for the prn in ethelot, with the new data from aeblock, using puteph.
	 However, only overwrite if the the block just red in is more
	 up-to-date than the one already in ethelot.
      */
	 toe=(long int)aeblock.a[3][0];  /*  Get toe for this eph reading.  */
	 age=(long int)abs1((double)toe-tobs); /*  Calculate the
	 `distance' of tobs from toe. All the type conversions are necessary
	 because stupid abs doesn't work for long ints.    */

	 ptoe=ethelot[k]->toe;  /*  Get toe for same prn which is already in ethelot  */
	 page=(long int)abs1((double)ptoe-tobs); /*  Calculate
	 `distance' for eph which is already in ethelot  */

	 if (age<page)  {   /*  If the eph data just read in is closer, in
	    time, to the one in ethelot, then overwrite the record in
	    ethelot with the new one. Otherwise, do nothing (ignore rec in) */
	    puteph(ethelot[k],&aeblock); /* Convert to eph, store in ethelot */
	 }
      }
      recsread+=1;  /*  We just red in another eph record.  */
      /*  ephrec[0] keeps track of the total number of satellites we have
	  eph records for.  */
      eoeph=!(reph(ephfile,&aeblock));  /* Read in another block of eph data */
   }
   fclose(ephfile);     /*  Close the ephemeris file  */
}

   udates+=1;
   return(recsread);
}
/*-----------------------------------------------------------------------*/
getime(double *tim, matrix *aobs)
/*  14/1/93  This function is used by updeph(), main and checkselect. It
    accepts one observation record *aobs, in matrix format, and returns
    tobs, as *tim. tobs is the time of observation, the same as t in
    the ephemeris algorithm, and is calculated from the date and time
    appearing in the observation record. Later, a special function
    should be created, which finds the number of seconds since the
    beginning of the century, from a date and time. This would be
    used by both eph and obs data to reduce the chance of thinking that
    eph data is valid when its actually an integer number weeks older
    than it looks.
*/
{
int year, month, day;
double hour, minute, second, t;
int dow; /* `day of week', to save result from day_of_the_week()  */

      year=(int)aobs->a[0][0];   /* year of observation, from aobs matrix  */
      month=(int)aobs->a[0][1];  /* month  "  */
      day=(int)aobs->a[0][2];    /* day  "  */
      hour=aobs->a[0][3];   /* hour  "  */
      minute=aobs->a[0][4]; /* minute  "  */
      second=aobs->a[0][5]; /* second  "  */

      year = 1900 + year;  /*  Adapted from Robert's choose_sats():  */
      /* must allow for 2000 as well - later */
      dow = dayofweek(year, month, day);
      t   =   (double)dow * 24 *3600 +
	      hour * 3600 +
	      minute * 60 + second;

      *tim=t;
      return(1);
}
/*-----------------------------------------------------------------------*/
int killeph()
/*  19/1/93.  This function belongs to updeph(). Remember that each time
    updeph is called, the entire ethelot structure is re-created. To take
    account of the fact that different eph files may be scanned by this
    function, we need to free the memory allocated by updeph each time.
    Otherwise, we could end up with a lot of unneeded sat ephs in ethelot.
*/
{
int i,N;

   N=ephrec[0];
   if (N>0) {  /*  Only try to release the memory if there is an  */
      for (i=0;i<N;i++)  /*   ethelot structure   */
	 free(ethelot[i]);
   }
   ephrec[0]=0;  /*  Declare ethelot empty   */
   return(1);
}
/*-----------------------------------------------------------------------*/
int feph(int prn)    /* ephrec passed implicitly, sloppily */
/*  13/1/93   */
/* Given a satellite prn, this function returns the index of that prn in
   the ephrec array, minus one. This index can then be used to access the eph
   data for that satellite, in the `ethelot' data structure. If the satellite
   prn cannot be found, the value -1 is returned.

   This function makes use of the global structure ephrec.
*/
{
int N,i,j,found;

	N=ephrec[0];  /*  The number of eph records we have  */
	found=-1;     /*  (Default is not found)  */
	if (N>0) {    /*  Any prn will not be found if there are no records */
	   for (i=1;i<(N+1);i++) {
	      if (ephrec[i]==prn) found=(i-1);  /*  Index in ethelot is one */
	   }                                    /*  less than corresponding */
	}                                       /*  index in ephrec.        */
	return(found);

}
/*-----------------------------------------------------------------------*/
int datypes(char *ofname, infotype *info)
/*  15/5/93.  Takes a character string and finds out what rinex data
    is expected in the observational record. The string passed to this
    function should be the line from the obs file containing
    " # / TYPES OF OBSERV" which, according to the rinex definition,
    supplies a list of the types af data in the columns. eg. if that
    same line contains "C1 L1" it means that we have pseudorange using
    C/A code on L1, and phase on L1. Verbatim, from the rinex defn:

    L1, L2 :  Phase meas. on L1 and L2 (full cycles)
    C1     :  Pseudorange using C/A code on L1 (metres)
    C2     :  Pseudorange using C/A code on L1 and P2-P1 code (metres)
    P1, P2 :  Pseudorange using P-code on L1, L2 (metres)
    D1, D2 :  Doppler frequency on L1, L2 (Hz)
    T1, T2 :  Transit Integrated Doppler on 150 (T1) and 400MHz (T2) (cycles)

*/
{

FILE *anyfile;
char anystr[100];
int i,j;
char *str2="   ";
char *cptr;
int  findex[10]; /* There can be a maximum of 10 data types (& columns) only */
int  fpos[10];
int  nfound;
int  gindex[10], gpos[10], ntypes; /* These are complimentary to the above 3 variables */


   anyfile=fopen(ofname,"rt");   /*  Open the obs file.  */
   if (anyfile==NULL) {
/*      printf("\nCould not find the file %s", ofname);  */
/*  This error condition will be reported by opnfiles().  */
      return(0);
   }

   for (i=0; i<20; i++) {  /*  Look through the 1st 20 lines, max.  */
      fgets(anystr, 200, anyfile);  /*  read a line from obs file  */
      if (feof(anyfile)) {i=20; break;}  /* quit if end-of-file reached */
      if (strstr(anystr,"# / TYPES OF OBSERV")!=NULL) break;
   }
   fclose(anyfile);

   if (i>=20) {
      /*  Create an error message & abort - couldn't find the
	  right string in the file.  */
      printf("\nObservation file doesn't seem to have rinex data type definition.");
      return(0);
   }
 else {

   /* Pass 1 - The tactic here is to go through the list of elements in
      refstr one-by-one, and remember (in findex, fpos and nfound) which
      elements were found.  */
   nfound=0;
   str2[2]='\0';  /* Set the string terminator for str2 (otherwise trouble) */
   for (i=0;i<10;i++) {         /*  Pass 1 - makes a list of types & their positions in anystr.  */
      str2[0]=refstr[2*i];    /*  First and second elements of str2, for  */
      str2[1]=refstr[2*i+1]; /*  example C1                          */
      if ((cptr=strstr(anystr,str2))!=NULL) {  /*  See if str2 is in anystr */
	findex[nfound]=i;  /*  Record the index in refstr of this symbol */
	fpos[nfound]=cptr-anystr;  /*  Record position of the find in the input string.   */
	nfound++;  /*  Count the number of finds */
      }
   }

   /*  Pass 2 - The tactic here is to go through findex and fpos, sorting
       the elements into gindex and gpos so that the order in which they
       appear in gindex and gpos is the correct column-order in the obs
       file.       */
   ntypes=0;
   for (i=0;i<80;i++) {   /*  Search to the maximum of 80 chars in anystr  */
      for (j=0;j<nfound;j++) {  /*  Go through the list of symbols found earlier  */
	 if (fpos[j]==i) { /* ( Go through all possible symbol positions)  */	/*  This brute-force search method puts the numbers in fpos (the
	     positions of the symbols in anystr) into ascending order in
	     gpos - ie. the symbols are sorted into the correct order as
	     they appear from left to right in anystr.   */
	    gindex[ntypes]=findex[j]; /* remember its position in the refstr */
	    info->gindex[ntypes]=findex[j]; /* Put it into outgoing data as well */
	    gpos[ntypes]=fpos[j];  /*  remember its position in anystr  */
	    ntypes++;
	 }
      }
      if (ntypes==nfound) break;
   }

   /* It doesn't look like gpos[i] will be useful outside of this algorithm,
      so it hasn't been included in the structure. If it turns out to be
      useful, though, just include it in the structure, and include a
      write to info->gpos[i] below gpos.      */

   info->ntypes=ntypes;


   /*  At this point, all the analyses on the file will have been done and
       data has been written to the info structures. Some intelligent
       software can be placed below, to make guesses about the type
       of receiver, etc. for example:       */

   /*  The intelligence in these will ultimately have to reside in the
       if statements (something more sophisticated than if ntypes==4).  */

  if (info->ntypes==4) {  /*  Jump to the conclusion that its a Leica  */
     strcpy(info->rtype,"Leica");
     info->rtypen=LEICA;  /*  Signifies a Leica  */
     info->colums=6;
  }
  else if (info->ntypes==2) {  /*  Jump to the conclusion its a Garmin  */
     strcpy(info->rtype,"Garmin");
     info->rtypen=GARMIN;  /* Signifies a Garmin  */
     info->colums=2;
  }
  else {
     strcpy(info->rtype,"Unknown");  /*   Give up, don't know.   */
     info->rtypen=0;
  }
 }
   return(1);
}
/*-----------------------------------------------------------------------*/
void opnfiles()
/*  The only reason this has been made a function is to get it out of main,
    to make code in main more readable, and to add a small amount of
    flexibility. opnfiles() asks for the names of one leica eph file
    and one leica obs file, opens them, and assigns var names to them.
*/

{
int i,j,k;


	printf("\n\nEnter ephemeris file name, including extension (xx.93n) : ");
	scanf("%s",ename);
	strlwr(ename);
	if (ename[strlen(ename)-1]!='n') {
	   printf("The file has to end in `n'! \n");
	   exit(0);
	}
	j=strlen(ename);
	strcpy(oname,ename);  /*  oname is the same as ename, except ... */
	oname[j-1]='o';  /*   ... that it has an 'o' at the end!   */
	strcpy(opfile,ename);
	opfile[j-1]='f';  /*  Create output (result) file name   */
	strcpy(mkfilenm,ename);
	mkfilenm[j-1]='m';  /*  Create marker output (result) file name   */

/* NO!:  ephfile=fopen(ename,"rt"); ... ephfile is opened in updeph() !  */
/*  ephfile is opened below, only to check that it exists.  */
	 ephfile=fopen(ename,"rt");
	 if (ephfile==NULL) {
	    printf("\nSorry old bean, can't find the eph file %s\n",ename);
	    exit(0);
	 }
	 fclose(ephfile); /*  Close it again. Will be re-opened by updeph() */

	 /*  Open mkfile in "wt" mode to delete any existing file. Later, in
	     robs, it will be opened in "at" (append) mode.      */
	 mfile=fopen(mkfilenm,"wt");
	 fclose(mfile);

	 j=datypes(oname, &info);  /*  Find out info on the obs file.  */
	 if (j!=0) {
	    printf("\nThe receiver which generated %s appears to be a %s",oname,info.rtype);
	    for (i=0;i<info.ntypes;i++) {
	       str2[0]=refstr[2*info.gindex[i]];
	       str2[1]=refstr[2*info.gindex[i]+1];
	       printf("\n          %s, %s", str2, description[info.gindex[i]]);
	    }
	    printf("\n");
	 }

	 obsfile=fopen(oname,"rt");   /*  Open this one for real.  */
	 if (obsfile==NULL) {
	    printf("\nUm, `fraid that the obs file %s is nowhere to be seen.\n",oname);
	    exit(0);
	 }

	 /*  Open the file to write the results to, in ascii:  */
	 outfile=fopen(opfile,"wt");  /*  Open for real, too  */
	 if (outfile==NULL) {
	    printf("\nQuitting, couldn't open result file %s\n",opfile);
	    exit(0);
	 }
}
/*-----------------------------------------------------------------------*/
void opnfiles12()
/*  opnfiles12() asks for the names of two leica eph files
    and two leica obs files (records for the stationary and the roving
    units), opens them, and assigns var names to them.
*/
{
int i,j,k;

	printf("\n\nEnter the ephemeris file name for the stationary GPS");
	printf("\nunit, including extension (eg. xx.93n) : ");
	scanf("%s",ename1);

	strlwr(ename1);
	if (ename1[strlen(ename1)-1]!='n') {
	   printf("The file has to end in `n'! \n");
	   exit(0);
	}

	j=strlen(ename1);
	strcpy(oname1,ename1);  /*  oname1 is the same as ename1, except ... */
	oname1[j-1]='o';  /*   ... that it has an 'o' at the end!   */

	strcpy(ename,ename1);
	/*  This is a semi-temporary-possibly-permanent fix to the question
	    of the best way of using the eph data from the 2 eph files.
	    What is being done here (for the time-being) is to use
	    only the stationary unit's eph data, and to borrow the variable
	    `ename' from the non-differencing section so that mods to
	    updeph aren't necessary. Although it may look like something
	    is being lost here (eph data from roving unit being completely
	    ignored), nothing is, really: think about it: (a) Exactly the
	    same eph data has to be used for obs from different units, when
	    doing differencing. (b) The sats and ephs to be used have to be
	    common to the two units, so sats selected from the roving unit
	    will always have eph data in the eph file of the stationary
	    unit -
	    This leads naturally to the conclusion that using eph data from
	    one of the units only, is acceptable. Make the chosen unit
	    (arbitrarily) the stationary one.

	    A possible improvement for the future here (to make the best
	    possible use of the eph data available) is to lump the two
	    eph files ename1 and ename2 together in one huge temporary
	    file, ename12. Then read in ename12, treating it just like
	    ename in the non-differencing section and making eph data
	    updates and selection from it. Actually, theres no need to
	    create another file - use the same algorithm as before,
	    but when you get to the end of the first eph file, close
	    it and open the second one -  then carry on, just as before!
	*/


	printf("\nEnter the ephemeris file name for the roving GPS");
	printf("\nunit, including extension (yy.93n) : ");
	scanf("%s",ename2);
	strlwr(ename2);
	if (ename2[strlen(ename2)-1]!='n') {
	   printf("The file has to end in `n'! \n ");
	   exit(0);
	}

	j=strlen(ename2);
	strcpy(oname2,ename2);  /*  oname2 is the same as ename2, except ... */
	oname2[j-1]='o';  /*   ... that it has an 'o' at the end!   */

	/*  The line immediately below: used to contain ename1 in place
	    of ename2 - this named the output file after the stat unit, but
	    now its after the roving unit.  */
	strcpy(opfile,ename2);
	opfile[j-1]='f';  /*  Create output (result) file name  */
	/* For the time-being, we'll stick with the convention that the
	   output file name (opfile) is the same as the file name of the
	   stationary GPS unit. This isn't really satisfactory, and a better
	   scheme should be though of in the not-too-distant-future.  */
	/*  See comment as for opfile above: marker filename comes from
	    roving unit:  */
	strcpy(mkfilenm,ename2);
	mkfilenm[j-1]='m';  /*  Create marker output (result) file name   */

/* NO!:  ephfile=fopen(ename,"rt"); ... ephfile is opened in updeph() !  */
/*  ephfiles are opened below only to check that they exist.  */


	 /*  Check for the existence of the roving eph file, even though
	     it isn't being used at the moment. It may be used at some time
	     in the future.        */
	 ephfile2=fopen(ename2,"rt");
	 if (ephfile2==NULL)
	    printf("\nBeware: can't find the eph file %s\n",ename2);
	 fclose(ephfile2);

/*  This is the place to read in ephfile1 (ename1) and ephfile2 (ename2)
    and to fuse them into one big temporary file, ename12. strcpy
    ename12 to ename and the borrow ename and ephfile, as before: read
    in ephfile with the same code as below, and in updeph. Remember
    to delete ename12 before quitting the program - in clsfiles12(). ---
    5/6/93  This has now been solved differently: each ephfile is opened
    scanned and closed, in succession. eph recs are updated as needed. This
    solution is satisfactory as it is - leave it alone.  */

	  /*  For the time being, borrow the name `ephfile' - it should
	      really be ephfile1, but this would involve
	      changing the updeph function.         */
	 ephfile=fopen(ename,"rt");
	 if (ephfile==NULL) {
	    printf("\nSorry old bean, can't find the eph file %s\n",ename);
	    exit(0);
	 }
	 fclose(ephfile); /*  Close it again. Will be re-opened by updeph() */

	 /*  Open mkfile in "wt" mode to delete any existing file. Later, in
	     robs, it will be opened in "at" (append) mode.      */
	 mfile=fopen(mkfilenm,"wt");
	 fclose(mfile);

/*  The next couple of paragraphs of code open the obs files and analyse
    them, for the sake of the robs function. A guess is made at the type
    and format of data in the obs files.          */

	 j=datypes(oname1, &info1);  /* Find out info on the two obs files. */
	 if (j!=0) {
	    printf("\nThe receiver which generated %s appears to be a %s",oname1,info1.rtype);
	    for (i=0;i<info1.ntypes;i++) {
	       str2[0]=refstr[2*info1.gindex[i]];
	       str2[1]=refstr[2*info1.gindex[i]+1];
	       printf("\n          %s, %s", str2, description[info1.gindex[i]]);
	    }
	    printf("\n");
	 }
	 j=datypes(oname2, &info2);
	 if (j!=0) {
	    printf("\nThe receiver which generated %s appears to be a %s",oname2,info2.rtype);
	    for (i=0;i<info2.ntypes;i++) {
	       str2[0]=refstr[2*info2.gindex[i]];
	       str2[1]=refstr[2*info2.gindex[i]+1];
	       printf("\n          %s, %s", str2, description[info2.gindex[i]]);
	    }
	    printf("\n");
	 }

	 obsfile1=fopen(oname1,"rt");   /*  Open it and keep it open  */
	 if (obsfile1==NULL) {
	    printf("\nUm, `fraid that the obs file %s is nowhere to be seen.\n",oname1);
	    exit(0);
	 }

	 obsfile2=fopen(oname2,"rt"); /*  Open it and keep it open  */
	 if (obsfile2==NULL) {
	    printf("\nThe obs file %s isn't here.\n",oname2);
	    exit(0);
	 }

	 /*  Open the file to write the results to, in ascii:  */
	 outfile=fopen(opfile,"wt");  /*  Open for real, too  */
	 if (outfile==NULL) {
	    printf("\nQuitting, couldn't open result file %s\n",opfile);
	    exit(0);
	 }
}
/*-----------------------------------------------------------------------*/
void clsfiles()
/*  To be used at the end of main(), only in conjunction with opnfiles()  */
{
// NO!:	fclose(ephfile);   ... done in updeph().  /*  ephemeris file  */
	domarkers(" XXXX  MARKER NAME");  /*  Flush marker information  */
	fclose(obsfile);     /*  obs file        */
	fclose(outfile);     /*  output file     */
}
/*-----------------------------------------------------------------------*/
void clsfiles12()
/*  To be used at the end of main(), only in conjunction with opnfiles12()  */
{
// NO!:	fclose(ephfile);   ... done in updeph().  /*  ephemeris file  */
	domarkers(" XXXX  MARKER NAME");  /*  Flush marker information  */
	fclose(obsfile1);     /*  stat obs file        */
	fclose(obsfile2);     /*  roving obs file        */
	fclose(outfile);     /*  output file     */
}
/*-----------------------------------------------------------------------*/
void gintro()
/* Intro screen and notices, etc.  Read comment for opnfiles.       */
{
char anyc;   /*  (Could just use qq here?)  */
char *anys="   ";

// clrscr();

printf("\nGPSE version 1.14 Copyright(C) 1994  Gary David Agnew");
printf("\nGPSE comes with ABSOLUTELY NO WARRANTY;");
printf("\nThis is free software, and you are welcome to redistribute it");
printf("\nunder certain conditions: for details please see the accompanying");
printf("\nfile, `GNU.DOC'.\n");

wait();
clrscr();
printf("\n                   G P S   E n g i n e");
printf("\n                   -------------------\n\n");
printf("\n             Copyright    (c) Gary Agnew, 1993");
printf("\n                          (c) Tesla Consulting");
// printf("\n\nAt the moment, this program is able to process data for the");
// printf("\nfollowing GPS units:");
// printf("\n                           Leica");
// printf("\n                           Garmin");
printf("\n\nNotes? (y/n) "); anyc=getche();
if ((anyc=='Y')||(anyc=='y')) {
   printf("\n\n                      Notes:\n");
   printf("\n 0.  The eph file should have an `n' at the end (eg. xx.93n).");
   printf("\n 1.  The obs file will be assumed to have the same name as the");
   printf("\n     eph file, except for the `o' at end (eg. xx.93o).");
   printf("\n 2.  The output file has the same name, except that it");
   printf("\n     has an `f' (for fix) at the end (eg. xx.93f).");
   printf("\n 3.  In the differencing-mode, the output file name follows");
   printf("\n     the name of the roving GPS unit (eg. yy.93f).");
   printf("\n 4.  In ordinary and differencing mode a marker file is created.");
   printf("\n     The marker file contains the name and number of points for");
   printf("\n     each site, as extracted from the observation file. The marker");
   printf("\n     file name is inherited from the roving GPS unit, and its");
   printf("\n     extension ends in `m'.");
   printf("\n 5.  Any key suspends calcs; `q' quits. ");
}
printf("\n\nFirst off, which would you like to run in ?");
printf("  \nnon-differential         (a) ");
printf("  \npositional differential  (b)");
printf("  \npseudorange differential (c)");
printf("\n\nAnswer the question. ");
anyc=getche();
anys[0]=anyc; anys[1]='\0';
strlwr(anys);
anyc=anys[0];
switch (anyc) {
   case 'a': mm=0; break;  /* mm=0 signifies non-differential mode        */
   case 'b': mm=1; break;  /* mm=1 signifies positional differential mode */
   case 'c': mm=2; break;  /* mm=2 signifies pseudorange dofferential mode */
   default : mm=0;
}
}
/*-----------------------------------------------------------------------*/
void displayresult(vector result, double tobs)
/*  Passed implicitly:
	-  the output file, outfile
	-  thesats[2][4] or aeph[]->prn, for the 4 satellite prns used.
	-  xobs (for the time), in the case of non-diff, and
	   xobs1, in the case of differencing.
*/
{
char outstring[80];  /*  String for outputting results to a file in ascii */
char outstring2[80]; /*  String for screen output.   */
char notherstr[80];  /*  Another string, for sat prns - can tack this
			 onto outstring if you want to output the sat prns
			 used in each calculation. */
char timstr[80]; /*  String for the time - to output to a file.  */
char dopstr[80]; /*  String for putting the dops into ascii form.   */

/*  The format of output to the file is:

      x    y    z   tobs   hr min sec   gdop   sat1    sat2   sat3   sat4

      phi  lam  h   tobs ..


Where differencing has been done, the format is similar, except for
some small differences:

      dx   dy   dz   tobs  hr min sec   gdop   sat1    sat2   sat3   sat4

      phi  lam  h    tobs ..


(result.a[3] contains the satellite clock error)

*/


/*  Use thesats[2][4] to output to the ascii file, the sats used for each
    calculation:  */

   strcpy(outstring,"\n");
   sprintf(outstring2,"%f %f %f %e", result.a[0], result.a[1], result.a[2], tobs);
   strcat(outstring, outstring2);

   if (mm==0) { /*  Get time string from xobs for non-differencing: */
//      sprintf(timstr," %d:%d:%d", (int)xobs.a[0][3], (int)xobs.a[0][4], (int)xobs.a[0][5]);
      sprintf(timstr," %d %d %d", (int)xobs.a[0][3], (int)xobs.a[0][4], (int)xobs.a[0][5]);
      /*  Comment out the above line when no longer needed - it is purely
	  for Matlab's sake. (Revert back to the line where hour min sec
	  are separated by colons).            */
      sprintf(dopstr, " %f", gdop0);
   }

   else  {      /*  Get time string from xobs1 for differencing:   */
//      sprintf(timstr," %d:%d:%d", (int)xobs1.a[0][3], (int)xobs1.a[0][4], (int)xobs1.a[0][5]);
      sprintf(timstr," %d %d %d", (int)xobs1.a[0][3], (int)xobs1.a[0][4], (int)xobs1.a[0][5]);
      /*  Comment out the above line when no longer needed - it is purely
	  for Matlab's sake. (Revert back to the line where hour min sec
	  are separated by colons).            */
      sprintf(dopstr, " %f", gdop1);
      /*  gdop1 and gdop2 are approximately the same, because the two
	  receivers are close together.  */

   }

   strcat(outstring, timstr);
   strcat(outstring, dopstr);

   sprintf(notherstr," %d %d %d %d", thesats[0][0], thesats[0][1], thesats[0][2], thesats[0][3]);
   strcat(outstring, notherstr);
   /* Comment out the above line if you don't like it. (ie. having the sats
      used in the calculation recorded in the output file. */

   fputs(outstring, outfile);
//   printf("\n%f %f %f %e", result.a[0], result.a[1], result.a[2], tobs);

/*  Print result on the screen. Use outstring2 here, if the list of
    satellites is unwanted. */
   printf("%s",outstring);

}
/*-----------------------------------------------------------------------*/
int ephinfo(int m)
/*  16/02/93.  Function to display possibly useful facts about the eph
    info, just after an update has been made.

    ephrec is passed implicitly.
*/
{
int i;

   printf("\nNumber of eph records read: %d",m);
   printf("\nNumber of sats in eph database: %d", ephrec[0]);
   printf("\nThe satellite prns are: ");
   for (i=1;i<(1+ephrec[0]);i++)
      printf("%d   ",ephrec[i]);

   return(1);
}
/*-----------------------------------------------------------------------*/
int commons12()
/*  16/02/93.  This function takes two obs records from different GPS
    units, for the same instant in time, and finds observations
    for four sats common to the two sets of data (if possible). The
    function returns the number of matches found. The maximum number
    will always be 4 (the algorithm halts after 4 common sats have been
    found). If the number returned is less than 4, then 4 common sats
    couldn't be found and the data should be ignored. This may happen
    because
	 (a)  The GPS units weren't set to track the same satellites.
	 (b)  The GPS units were unable to track the same satellites.
	 (c)  Data files have been mixed up.

  This function can be thought of as a modified version of the bottom
  of checkselect(), for the non-differential mode case. It has to be
  called straight after checkselect1() and checkselect2() have been
  called. Part of the code in the non-differential version of checkselect
  (the ...) has been moved here for the diff version.

  zthesats1/2, zanobs1/2, zaeph1/2 are all passed into this
  function implicitly, as global variables.

  thesats, anobs1, anobs2 and aeph are passed out implicitly (as global vars)

*/
{

double t;
long int tks[4];

int i,j,k,m1,m2;

/*  Provide the bridging step between all common sats and the selected 4 */

   /*  comcount is the counter for the number of common sats found
       in the data - set it to 0 at the beginning of this algorithm.  */
   comcount=0;

   m1=zthesats1[0][0];  /*  Get the number of sat prs in the stat obs */
   m2=zthesats2[0][0];  /*                   "        roving obs */
   /*  At the moment, a brute-force search method is being used here, to
       find satellites common in the set of observations. Since the number
       of loops here is limited to a few hundred (m1 * m2), the fact
       is of no consequence. */
   for (i=1;i<(m1+1);i++) {  /*  Go through all the sats in zthesats1  */
      for (j=1;j<(m2+1);j++) { /* For each zthesats1 look at all zthesats2  */
	 if (zthesats2[0][j]==zthesats1[0][i]) { /* If there's a match, ..*/
	    thesats1[0][comcount]=zthesats1[0][i]; /*  Record it in thesats */
	    thesats2[0][comcount]=zthesats2[0][j]; /* .. Redundant, really! */
	    thesats1[1][comcount]=zthesats1[1][i];
	    thesats2[1][comcount]=zthesats2[1][j];
	    anobsx1[comcount]=zanobs1[i-1];
	    anobsx2[comcount]=zanobs2[j-1];
	    aephx[comcount]=zaeph1[i-1];  /*  May have to make it zaeph !? */

	    comcount+=1;
	 }
      }
   }
   /*  At this point, we have a list of satellite prns common to both
       obs records, usually numbering more than 4 (`comcount' of them),
       saved in thesats1 and thesats2. The reason for having two variables
       (thesats1 and thesats2) instead of one (thesats12) is that the signal
       strengths for the same satellites recorded at different GPS units are
       different - thesats1[1][] has the signal strengths for the stationary
       unit and thesats2[1][] the ss for the roving unit.
       We could select the sats with the strongest signals, according to
       some algorithm (eg. select the 4 highest values of ss1 * ss2, where
       ss1 and ss2 are the signal strengths corresponding to thesats1 and
       thesats2, respectively.) however, until we have a better
       understanding of what these signal strengths mean, we will simply
       select the first 4 satellites, ignoring signal strength (as was
       done for the non-differential mode).
   */
   if (comcount>3)
      comrec=4; /*  comrec is the number of sats we record data for - 4 max */
   else
      comrec=comcount; /* If the no. is less than 4, then only do that no. */

   /*  The for loop below is where the first 4 sats are simply copied
       into the outgoing variables anobs1, anobs2, thesats and aeph.
       - The more sophisticated algorithm might use the signal strengths
       in thesats1 and thesats2 to make a better selection.

       When the selection of the 4 common satellites is done, their
       data must be put into data structures which match the types in the
       parameter list of findrec(). This is done below. For clarity,
       the correspondences between data we have at this point, and
       the structures the data will be copied to, are:

       int        thesats1[2][MAXEPHS]   -----    int       thesats1[2][4]
       double     anobsx1[MAXEPHS]       -----    rinex     anobs1
       ephemeris  *aephx[MAXEPHS]        -----    ephemeris aeph[4]

   */

    getime(&t, &xobs1);  /*  Get time of obs for current obs.  */

    anobs1.t=t;   /*  This starts the definition of anobs1 and  */
    anobs2.t=t;   /*  anobs2 (see inside firstselect12() and
		      dopselect12()) - ###  */

    t_now=t;  /*  Set the time of the current obs, for checking in main(),
		  as to whether or not an update of the ephemeris records
		  is needed.      */

/*  maskselect12() goes here:
    maskselect12() removes satellites from the `eligible' list, which
    have an elevation of less than some fixed amount, MASKMIN, which
    at the moment is nominally 5 degrees.
*/

   /*  This totalfixes check is absolutely necessary - see the reason given
       in the equivalent code in checkselect().   */
   if (totalfixes>1) {
      maskselect12();
   }

   if (comcount>3)  /*  comrec is the equivalent of m in checkselect()  */
      comrec=4; /*  comrec is the number of sats we record data for - 4 max */
   else
      comrec=comcount; /* If the no. is less than 4, then only do that no. */

/*  dopselect12()   OR    firstselect12() goes here:  */
/*  firstselect12() simply selects the 1st 4 available satellites.  */
/*  dopselect finds the set of 4 satellites which gives the lowest (best)
    dop figure.
       If the mode is non-differencing, anobs will be written with obs
    data. If it is differencing, anobs1 and anobs2 will be written with
    the obs data. Regardless of mode, thesats and aeph will contain the
    satellite prns and eph data, respectively.
*/

   if (comcount>3) { /* Only try to dopselect12() if there are enough sats */
      dopselect12();
//      firstselect12();
   }

/*  Calculate t: this calculation serves two purposes, to provide data
    for the date check, and the t input for anobs1.t and anobs2.t,
    for findrec(). See the comments in checkselect() - they apply here -
    do a text search on "Calculate t: this calc".

    The date comparison for the obs file is done with each of the four
    selected eph records. If any one of them is bad, a message is generated.

    Since the times for obs1 and obs2 are identical, it makes no difference
    which is used in the time calcs. xobs1 is used (arbitrarily) below:
*/

/*  The time is found in getime, above.  */
      for (i=0;i<comrec;i++) {  /*  comrec should always be 4    */
	 tks[i]=aeph[i].toe-(long int)t;
	 if (tks[i]>302400)  tks[i]-=604800;  /*  Correct for end-of week */
	 if (tks[i]<-302400) tks[i]+=604800;  /*  crossovers  */
	 /*  This correction is necessary here, so that the date
	     comparison makes sense. Were this method of date comparison
	     not being used, this correction could be left to eph2uvw() -
	     go now, and have a look at the code for eph2uvw(). */
      }

   /*  Only check eph data (aeph) age if there are more than 3 sats - see
       the reason in checkselect().        */
   if (comrec>3) {  /*  (Actually comrec HAS to be 4 for this to exec:) */
      for (i=0;i<comrec;i++) {
	 if (abs1((double)tks[i]) > abs1((double)VERYOLD)) {
	    /*  Generate a more serious message  */
	    printf("\nWarning: satellite %d data is very old.",aeph[i].prn);
	    /* Later, possibly put these into error reporting strings.   */
	 }
	 else if (abs1((double)tks[i]) > abs1((double)OLD)) {
	    /*  Generate a message  */
	    printf("\nWarning: satellite %d data is oldish.",aeph[i].prn);
	 }
      }
      /*  The error messages (2 warnings) above should never actually
	  occur - because t is periodically checked and updeph() is
	  called every hour or so. If the warnings do occur, its a sign
	  that inadequate eph data was collected during the original
	  datalogging session. Only the GPS unit can be blamed for this (?)

	  NOTE: that ephcheck() has to be correctly placed, within the main
	  program, after equaltimes()/robs() and before
	  commons12()/checkselect(), otherwise the error messages could
	  occur if there are large time gaps in the obs file.  */

   }
   return(comrec);  /*  This is how many common sats we're passing back
			data for - if its less than 4, there's a problem -
			check, and take appropriate action in main()  */
}
/*-----------------------------------------------------------------------*/
int equaltimes()
/*  17/02/93.  This function looks through the obs files, obsfile1 and
    obsfile2, to find the next occurrence of records in the files whose
    times match. If, in the process of this search, the end of either
    obs file is reached, the function returns zero.

    Passed implicitly: obsfile1, obsfile2, xobs1, xobs2, tobs1, tobs2
*/
{
int neoobs1, neoobs2, neoobs12;
long int itobs1, itobs2;

   neoobs1=robs(obsfile1, &info1, &xobs1); /*  Read an obs record from stat unit */
   neoobs2=robs(obsfile2, &info2, &xobs2); /*         "      roving unit file    */
   neoobs12=(neoobs1 && neoobs2);  /*  Both files must not be at an end  */
   getime(&tobs1, &xobs1);  /*  Get the time of observation for the record */
   getime(&tobs2, &xobs2);  /*  that has just been read in.                */
   itobs1 = (long int)tobs1;  /*  See below for explanation.   */
   itobs2 = (long int)tobs2;
   /*  Converting tobs1 and 2 from doubles to long ints is for safety:
       making equality comparisons with doubles (eg. (tobs1==tobs2)) is
       very dodgey, and could lead to trouble - who knows, due to a
       quirk in the machine's arithmetic, you could get slightly different
       tobs'es coming out, for exactly the same integer input data. Therefore,
       the numbers have been converted to integers. Now, this scheme will
       always work where the date and time given in the obs record is
       an exact number of seconds, like it is in the Leica data. However,
       big trouble could happen if we get data where the time come in
       fractions of seconds - BEWARE! The solution in this case would be
       to make itobs1 and itobs2 real numbers, but round them off to
       however many decimal places are needed - only a study of the data
       will reveal how many places is "safe" to use. It should then be
       safe to make equality comparisons with itobs1 and itobs2.
   */

   /*  Now, This while loop should execute rarely - its here to take care of
       cases where one of the obs files has an invalid or missing record.
       An exception to this rule is at the very beginning of file
       reading: the stat or roving unit may have been started quite some
       time before the other, and this while loop would have to execute
       to get the one file to `catch up' to the where the other one starts.
   */
   while ((itobs1 != itobs2) && (neoobs12)) {
      /* Times aren't the same - we have to equalise */
      if (itobs1 > itobs2) {  /*  obsfile2 has to `catch up' with obsfile1, */
	 neoobs2=robs(obsfile2, &info2, &xobs2); /* ...by reading obsfile2.  */
	 getime(&tobs2, &xobs2);  /*  Find robs for record just read in  */
	 itobs2 = (long int)tobs2;  /*  `Safety' conversion to integer  */
      }
      if (itobs2 > itobs1) {  /*  obsfile1 has to catch up with obsfile2  */
	 neoobs1=robs(obsfile1, &info1, &xobs1); /* ... by reading obsfile1... etc. */
	 getime(&tobs1, &xobs1);
	 itobs1 = (long int)tobs1;
      }
      neoobs12=(neoobs1 && neoobs2);  /*  Both files must not be at an end  */
   }

   /*  This could be useful, to see which file ended the process: make
       the obs file eof look like an error: (But don't use it for now.) */
   if (!neoobs1) {
      errcon=4;
      strcpy(errmess,"End of stationary unit's obs file has been reached.");
   }
   if (!neoobs2) {
      errcon=4;
      strcpy(errmess,"End of roving unit's obs file has been reached.");
   }

   return(neoobs12);
}
/*-----------------------------------------------------------------------*/
int ephcheck(matrix *aobs)
/*  17/02/93. Looks at the current time of obs, given in t_now, from
    *aobs, and compares it with the time of the last eph data update,
    given in t_last. t_last is set in updeph only. if the difference
    between the two time is larger than some set amount (one hour,
    or 3600 seconds, for the time being, then the eph update algorithm
    updeph(), is called.

    Passed implicitly are t_now (although it is overwritten in this
    function), t_last, and dt_up.
*/
{
int m;

   /*  Do the check for whether eph data needs to be updated, and
       update if necessary:
   */
   getime(&t_now, aobs);  /*  WARNING: t_now is also set in checkselect()
			      and commons12() - should be removed from
			      these functions.                */
   dt_up=t_now-t_last; /* Calc time diff between now and the last update */
   if (dt_up>302400.0)  dt_up-=604800.0; /* Sat/Sun midnight crossover  */
   if (dt_up<-302400.0) dt_up+=604800.0; /* correctn.(otherwise trouble) */
   dt_up=abs1(dt_up);  /*  abs so that its magnitude can be examined */
   /*  Update eph data if last update was more than an hour ago. This
       figure (3600 seconds) is subject to change, as necessary:  */
   if (dt_up>3600) {  /*  If last update more than an hour ago:  */
      printf("\nUpdating eph information ... ");
      m=updeph(aobs); /* Pass updeph() aobs, which gives current time */
      m=m;    /*  Shut the warning generator up.  */
//    ephinfo(m);  /*  Display eph info about the update.   */
   }
   return(1);
}
/*-----------------------------------------------------------------------*/

