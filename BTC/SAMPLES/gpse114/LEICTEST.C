/*  3/1/93.

    This is a bit of a junkbox. It contains code which was originally
    used to test some of the functions in other leic files, plus odds
    and ends which may be used in future. It includes old versions of
    functions, or discarded functions, which may at some time or another
    be useful. Pick out the sections needed and patch them into main.

*/

vector *raw,*vals;
char *outstr,str1[100],str2[100],str3[100],str0[100];


	raw=(vector *)malloc(sizeof(vector));
	vals=(vector *)malloc(sizeof(vector));
	outstr = (char *)malloc(100);

#if 0
	/*  Original test for getnums  */
	getnums(vals,str1);
	vecdisp(vals);

	/*  Eventually - relegate all these tests to a file called
	       leictest.c and include here if necessary. Make var
	       declarations part of the leictest file.  */

	/*   Test for reph   */
	ofile("jnuk2.eff");  /*  jnuk2 Contains hand-crafted corruptions  */
	while (reph(raw_file, &aeblock)) {
	   edisp(&aeblock);
	   printf("\n\n");
	}
	cfile(raw_file);

/* Ultimately these file name inputs should be changed to reads of
   the entire current directory, display and choice of file by highlighting
   a file choice (etc, etc, etc)  */

	/*   Test for robs()   */
	if (!ofile("jnuk2.obs"))  /* jnuk2 Contains hand-crafted obs corruptions */
	    printf("\nFile opening was NOT successful. ");
	while (robs(raw_file, &anobs)) {
	   odisp(&anobs);
	   printf("\n\n");
	}
	cfile(raw_file);

	/* Test for feph  */
	ephrec[0]=4;
	ephrec[1]=11;
	ephrec[2]=12;
	ephrec[3]=13;
	ephrec[4]=14;
	ephrec[5]=25;
	ephrec[6]=26;
	ephrec[7]=27;
	k=feph(13);
	printf("\nShould be 2: %d",k);
	k=feph(14);
	printf("\nShould be 3: %d",k);
	k=feph(25);
	printf("\nShould be -1: %d",k);
	k=feph(4);
	printf("\nShould be -1: %d",k);
	k=feph(11);
	printf("\nShould be 0: %d",k);


	/*  Test ueph:  */
   for (j=0;j<3;j++) {
	m=ueph();  /*  Give ephfile a crack on the back of the head  */
	printf("\nNumber of records read is %d ",m);
	printf("\nEphrec[0] is %d",ephrec[0]);
	printf("\nEphrec[1] is %d",ephrec[1]);
	printf("\nEphrec[2] is %d",ephrec[2]);
	if (ephrec[0]>0) { /*  Check if any ephs exist */
	   for (i=1;i<(1+ephrec[0]);i++) {
	      printf("\n Sat prn read in: %d ", ethelot[i-1]->prn);
	   }
	}
	wait();
   }



      /*  Test for checkselect.  This test has to be done in close
	  conjunction with the contents of e.92 and o.92.  - ie. make
	  sure that the program comes up with values which are in
	  agreement with the contents of the files.   */
      robs(obsfile,&xobs);  /*  Try processing 3 observations.  */

      checkselect();  /*  Do time checks and satellite selection for this
			  particular observation.  */
      printf("\n Four satellites to be used are:");
      for (j=0;j<4;j++) {
	 printf(" %d", aeph[j].prn);
	 ephdisp(&aeph[j]);
      }
      for (j=0;j<4;j++) {
	 printf("\n prn from the aeph array: %d", aeph[j].prn);
	 printf("\n prn from thesats array:  %d", thesats[0][j]);
	 printf("\n and their sig strths , %d\n", thesats[1][j]);
      }
      printf("\n The pseudoranges for these satellites are:");
      for (j=0;j<4;j++) {
	 printf("\n     %f",anobs.prs[j]);
      }
      printf("\n The time associated with this observation is %f",anobs.t);
/*  Calculate a few positions in rectangular co-ords here. Compare them
    with the approx position given at top of obs file.   */
      result=findrec(aeph,anobs);
      printf("\nRect co-ords: %f %f %f", result.a[0], result.a[1], result.a[2]);
      printf("\nSat clk offset: %f\n", result.a[3]);
//    displayresult(dms,result);
      wait();



/*  Code following was for testing ueph(). (Which has been superseded).  */
//   udates=0;    /*  No eph records read yet */ /*  dtc - with test for ueph */
//   etime=-1;    /*  Impossible - a sign that etime hasn't yet been defined  */

//   m=ueph();    /*  Read in a complete epoch of ephemeris records.  */
//   printf("\nNumber of eph records read = %d", m);
//   m=ueph();    /*  Read in a complete epoch of ephemeris records.  */
//   printf("\nNumber of eph records read = %d\n\n", m);



/*  Test that ethelot is getting the right data:  */
//   for (i=0;i<ephrec[0];i++)
//      ephdisp(ethelot[i]);
//   wait();


//  All of this has been relegated to killeph():
//   if (ephrec[0]>0) {  /*  Check if any space has been malloc'd for ephs */
//      for (i=1;i<(1+ephrec[0]);i++) {
//	 free(ethelot[i-1]);    /*  Free all the space allocated to  */
//      }                         /*  storing eph records.             */
//   }




//    /*  Test for checkselect:  This test has to be done in close conjunction
//	  with the contents of xx.92n/e.92 and xx.92o/o.92.  - ie. make
//	  sure that the program comes up with values which are in
//	  agreement with the contents of the files.   */
//      printf("\nFour satellites to be used are:");
//      for (j=0;j<4;j++) {
//	   printf(" %d", aeph[j].prn);
// //	 ephdisp(&aeph[j]);
//      }
//
//      printf("\nSatellite pseudos are:");
//      for (j=0;j<4;j++) {
//	   printf("   %f",anobs.prs[j]);
//      }
// //      printf("\nThe time associated with this observation is %f",anobs.t);


// // Test for debugging prs contents of stat unit before findrec:
//printf("\n\nFor result1, the pseudoranges are: \n ");
//for (i=0;i<4;i++) {
//   printf("   %f",anobs1.prs[i]);
//}
//printf("\nTime is: %f ", anobs1.t);
//printf("\nThe satellites being used are: %d %d %d %d", thesats[0][0], thesats[0][1], thesats[0][2], thesats[0][3]);
//printf("\nThe time of obs is: %d %d %d", (int)xobs1.a[0][3], (int)xobs1.a[0][4], (int)xobs1.a[0][5]);
//printf("\n");
//wait();


// //  Test for debugging contents of prs for rov unit before findrec:
//printf("\n\nFor result2, the pseudoranges are: \n ");
//for (i=0;i<4;i++) {
//   printf("   %f",anobs2.prs[i]);
//}
//printf("\nTime is: %f ", anobs2.t);
//printf("\nThe satellites being used are: %d %d %d %d", thesats[0][0], thesats[0][1], thesats[0][2], thesats[0][3]);
//printf("\nThe time of obs is: %d %d %d", (int)xobs2.a[0][3], (int)xobs2.a[0][4], (int)xobs2.a[0][5]);
//printf("\n");
//wait();


// // For debugging convergence/ pc hanging problem in return of findrec
// // This section of code goes just before findrec in the non-diff section
//printf("\n\nFor result, the pseudoranges are: \n ");
//for (i=0;i<4;i++) {
//   printf("   %f",anobs.prs[i]);
//}
//printf("\nTime is: %f ", anobs.t);
//printf("\nThe satellites being used are: %d %d %d %d", thesats[0][0], thesats[0][1], thesats[0][2], thesats[0][3]);
//printf("\n");
//wait();


//  /*  Code for testing differential mode, in main: display results before
//      subtraction:  */
//	 /*  Display the results1 and 2 on the screen: (debugging only) */
//	 printf("\nTwo individual results are: (stat, then roving)");
//	 printf("\n%f %f %f %e", result1.a[0], result1.a[1], result1.a[2], anobs1.t);
//	 printf("\n%f %f %f %e", result2.a[0], result2.a[1], result2.a[2], anobs1.t);
//	 printf("\nDifference result is:");


//  /*  Code for testing in dopselect():   */
//   /*  This goes aftre the switch statement:  */
//printf("\nCombos is %d",combos);
//printf("\nt_now is %f",t_now);
//wait();
//
// ephdisp(zaeph[0]);
//
//printf("\nGot past onesat.");
//printf("\nGot past gdop.");
//
//  /*  This goes after the dopset[i] generation loop :  */
//printf("\nDopset is: ");
//for (i=0;i<combos;i++) printf(" %f",dopset[i]);
// /* Some rudimentary tests for dopselect();  */
// /*  These go just before the return(1) in dopselect():  */
//printf("\nIndexes for best sats are:");
//for (i=0;i<4;i++) printf(" %d", bestsats[i]);
//printf("\nBestsats are:");
//for (i=0;i<4;i++) printf(" %d", zthesats[0][1+bestsats[i]]);
//wait();


//  /*  Test for datypes() function:       */
//  infotype *info;
//  int i,j,k;
//  //char *dumstr="Gadly";
//  //char *tstr1="    2    C1      D1   T1  L1      # / TYPES OF OBSERV     ";
//  char *ofname="ZZ.92O";
//  char *str2="  ";
//
//     info=(infotype *)malloc(sizeof(infotype));
//     if (datypes(ofname, info)!=0) {
//
//        printf("\nThe receiver which generated %s appears to be a %s",ofname,info->rtype);
//        printf("\nThe number of finds = %d", info->colums);
//        printf("\nThe finds were:  ");
//        for (i=0;i<info->colums;i++) {
//  	 str2[0]=refstr[2*info->gindex[i]];
//  	 str2[1]=refstr[2*info->gindex[i]+1];
//  	 printf("\n%s, %s", str2, description[info->gindex[i]]);
//        }
//     }
//  //   printf("\nIndexes are: ");
//  //   for (i=0;i<info->colums;i++) {
//  //       printf("\n%d", info->gindex[i]);
//  //    }
//
//
//


//  /*  Tests for the substr function follow:   */
//
//  char *str1="GARYDAVIDAGNEW";
//  char *str2="                     ";
//
//     clrscr();
//     printf("\nlength of str1 is %d", strlen(str1));
//     printf("\nlength of str2 is %d", strlen(str2));
//     substr(str1,str2,4,5);
//     printf("\nThe substring is %s", str2);
//     wait();


/*  The `original' version of striso(): This is a `strict' function,
    producing a name with no spaces in it, only.       */
int striso2(char *dest, char *src)
/*  3/6/93.  Named "striso" for "string isolate". This function was
    created specifically to extract the marker name from a string
    (single line from obs file) passed to it. This function returns
    the first word it finds in the string, delimited by spaces. For
    example, if you pass it "   THEQUICK72  ALoAdOf RuBbIsH  ", it
    will return "THEQUICK72".   */
/*  This function has been superseded by the one above.  */
{
int i,n,N;
   N=strlen(src);
   for (i=0;i<N;i++) {if (src[i]!=' ') {n=i; break;} }
   for (i=0;i<(N-n);i++) { if ((dest[i]=src[i+n])==' ') {n=i; break;}}
   dest[n]='\0';
   return(0);
}
/*----------------------------------------------------------------------*/


/*  Check: for pseudorange differencing section */
//   uvw3geo(u, v, w, *Po, *Lo, *ho);
//     uvw3geo(rcr.a[0],rcr.a[1],rcr.a[2],&(rc.a[0]),&(rc.a[1]),&(rc.a[2]));
//printf("\nConverted co-ords:  ");
//for (i=0;i<3;i++) {
//   printf("\n plh = %f",rc.a[i]);
//}
//wait();





#endif

	free(raw);
	free(vals);
	free(outstr);


/*======================================================================*/
/*  This is NAV's original main, which has to be integrated here -
    it shows how to use the functions in leic*

*/

#if 0

int main(void)
   {
   char  eph_name[80], rin_move_name[80];
   ephemeris eph[MAX_SATS];
   ephemeris four_eph[4];
   int       num_ephs;
   int  count;
   raw_rinex  raw_rin_mov;
   rinex rin_mov;
   int i,j,k;
   vector posn;
   double lat,lon,alt;

   ginit();

   get_names(eph_name, rin_move_name);

   if ( open_gps(eph_name, rin_move_name, eph, &num_ephs) )
      {
      for (count = 0 ; count <= 400; count++)
	 {
	 read_gps(&raw_rin_mov);
	 choose_sats(raw_rin_mov, &rin_mov , eph, four_eph );
	 posn=findrec(four_eph, rin_mov);
//	 printf("\nPosition: %f %f %f %e",posn.a[0],posn.a[1],posn.a[2],posn.a[3]);
	 uvw2geo(posn.a[0],posn.a[1],posn.a[2],&lat,&lon,&alt);
	 printf("\n %lf %lf %lf ",lat,lon,alt);
	 }
      close_gps();
      }
   else
      {
      printf("file does not exist");
      }



   wait();
   return 0;
   }

#endif

/*-------------------------------------------------------------------*/
/*    Retrenched functions:
/*-------------------------------------------------------------------*/
int ueph() /*  ?? Don't bother with FILE *anyfile i/p ?  */
/*  NOT presently being used - old algorithm. Assumes ephfile is
    opened outside of this function. Compare with updeph(), the function
    which supersedes ueph().

    udates, etime are passed to this function implicitly, as a global vars.

*/


/*  13/1/93

    For "update ephemeris data structure". This function reads in all the
    epemeris data it can find for the current 1-hour epoch. (and epochs
    preceding the current one.) It returns the number of eph records read,
    regardless of whether they are new or updates. If the number returned
    is zero, the end of the eph file has been reached, or possibly some
    other error has occurred.

    The algorithm is to keep reading in satellite eph records until a time
    in an eph record is one hour ahead of the current epoch (called etime).
    The variable etime is global, so that a record of it is kept for next time
    an update is required, and also so that the obs read algorithm can decide
    when eph updates are required (for later improvement).

    To understand why this algorithm is the way it is, you have to look
    at the eph data files produced by leica. (Please do. Sets of data
    come in approx hourly batches, and updates for particular satellites
    are a bit sporadic, with eph data for the previous hour sometimes
    appearing suddenly.) The format of the eph file data, and the
    algorithm here are therefore quite leica-specific, so don't assume
    they'll work on other types of data (Magellan, for example, is quite
    different, their eph data isn't even in standard Rinex format).

*/
{
static matrix aeblock; /* `aeblock' - for `an ephemeris block' .     */
static int eoeph=0; /*  For `end of eph (file)'  */
int i,j,k,N;
int aprn;  /* `aprn' - for `a prn'.   */
long int ptime;  /* `ptime' - for `present time'  */
int recsread;  /*  for `records read' - counts the number of eph records
		   read in from ephfile, for each call of this function.  */
recsread=0;  /*  Reset the record count to zero.   */

/*  Only set up aeblock the first time this function is called  */

if (udates==0) {

	aeblock.n=8;
	aeblock.m=10;
	strcpy(aeblock.message," ");
	aeblock.err=0;
	for (i=0;i<MAXN;i++){
		for (j=0;j<MAXN;j++) {
			aeblock.a[i][j] = 0.0;
		}
	}
	eoeph=!(reph(ephfile,&aeblock)); /* Read a block of eph into aeblock from efile */
	/*  Set etime to the toe contained in aeblock, the first
	    budding eph record of the new etime epoch           */
	if (!eoeph) {
//	   recsread+=1;
	   etime=(long int)aeblock.a[3][0]; /*  toe from aeblock  */
	}
}
//??
if (eoeph) return(recsread);

do {
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
   */
      puteph(ethelot[k],&aeblock); /* Convert to eph, store in ethelot */
   }
   eoeph=!(reph(ephfile,&aeblock));  /* Read in another block of eph data */
   if (!eoeph) {
      recsread+=1;
      ptime=(long int)aeblock.a[3][0];    /*  toe from aeblock  */
      /*  ptime in this `if' is here only to make sure that ptime
	  is defined if not eof, otherwise the assigned value is
	  meaningless. This may be nitpicking.  */
   }
} while ((!eoeph)&&(ptime<=etime));
/*   Check the time (toe) for eph data just read in, with the current epoch
     time. If its suddenly ahead (normally 1hr), stop reading, because the
     next epoch has been reached. Save this first data of the next epoch, by
     leaving it in aeblock, for the next update.
*/

etime=ptime;
udates+=1;

return(recsread);
}
/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/




