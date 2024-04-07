/*  Leica_r.c
    Robert's functions and typedefs link into the program as a whole, here

*/

typedef struct raw_rinex
   {
      int      year, month, day;
      int      hour, min;
      double   sec;
      int      num_sats;
      int      sat_id[5];
      double   prs[5];
      int      snr[5];
   } raw_rinex;



typedef struct dms
   {
   int     deg;
   int     min;
   double  sec;
   } dms;

/*-------------------------------------------------------------------*/

extern  int  dayofweek(int  year, int month, int day);

extern  void get_names(char eph_name[80], char rin_move_name[80]);

extern  int open_gps (char      eph_name[80],
		      char      rin_name[80],
		      ephemeris eph[MAX_SATS],
		      int       *num_ephs);

extern  void read_gps(raw_rinex *rin);

extern  void close_gps(void);

extern  void choose_sats(raw_rinex raw_rin_mov, rinex *rin_mov ,
			  ephemeris eph[MAX_SATS] , ephemeris four_eph[4] );

extern void d2dms(double val_deg, double *val_dms);

extern void dms2d(double val_dms, double *val_deg);

extern void uvw2geo( double u,
		     double v,
		     double w,
		     double *phi,
		     double *lam,
		     double *h);

extern void geo2uvw( double phi,
		     double lam,
		     double h,
		     double *u,
		     double *v,
		     double *w );





void get_names(char eph_name[80], char rin_move_name[80])
   {
   clrscr();
   printf("\n");
   printf("          NAV - GPS Demonstration program - TESLA Consulting \n");
   printf("          컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴 \n");
   printf("\n               Bob Cole       ..oOo..      Gary Agnew");
   printf("\n\n\n\n");
   printf("\n  Input Ephemeris File name : ");
   scanf("%s", eph_name);
   printf("\n  Input Rinex File name     : ");
   scanf("%s", rin_move_name);

   }

/* ----------------------------------------------------------------- */


void d2dms(double val_deg, double *val_dms)
   {
   double  d_part, m_part, s_part;

   d_part = (double)((int) val_deg);
   m_part = (val_deg - d_part) * 0.6;
   s_part = ((m_part * 100) - (float)((int) (m_part * 100)))*0.006;
   m_part = (double)((int)(m_part * 100))/100;

   *val_dms = d_part + m_part + s_part;
   }


/* ----------------------------------------------------------------- */

void dms2d(double val_dms, double *val_deg)
   {
   double  d_part, m_part, s_part;

   d_part = (double)((int) val_dms);
   m_part = (val_dms - d_part);
   s_part = ((m_part * 100) - (float)((int) (m_part * 100)))/36;
   m_part = (double)((int)(m_part * 100))/60;

   *val_deg = d_part + m_part + s_part;
   }


/* ----------------------------------------------------------------- */

void uvw2geo( double u,
	      double v,
	      double w,
	      double *phi,
	      double *lam,
	      double *h)

   {
   double   n, e, f, temp_phi, err = 1.0;

   f = 1 / ONE_F;
   *lam = atan(v/u);
   e = sqrt(2*f - f*f);

   temp_phi = atan( (1 / (1- e*e)) * ( w / sqrt(u*u + v*v) ) );

   while (err > 0.00001)
      {
      n = A / sqrt( 1 - e * e * sin(temp_phi) * sin(temp_phi));
      *phi = atan( ( w / sqrt(u*u + v*v) ) *
	    ( 1 + (e*e * n * sin(temp_phi)  / w)) );
      err = *phi - temp_phi;
      temp_phi = *phi;
      }

      *h = ( sqrt(u*u + v*v) / cos(*phi)) - n;
      *phi *= 180/PI;
      *lam *= 180/PI;
   }

/* ----------------------------------------------------------------- */

void geo2uvw( double phi,
	      double lam,
	      double h,
	      double *u,
	      double *v,
	      double *w )
   {
   double n, e, f;

   phi *= PI/180;
   lam *= PI/180;

   f = 1 / ONE_F;
   e = sqrt(2*f - f*f);
   n = A / sqrt( 1 - e * e * sin(phi) * sin(phi));

   *u = (n + h) * cos(phi) * cos(lam);
   *v = (n + h) * cos(phi) * sin(lam);
   *w = ( n * (1 - e*e) + h) * sin(phi);
   }


/* ----------------------------------------------------------------- */


int dayofweek( int year, int month , int day )
   {
   static int days[] = {0, 1, 2, 3, 4, 5, 6, 0};
   int   index;

   if (month > 2)
      month = month - 2;
   else
      {
      month += 10;
      year--;
      }
   index = ((13*month-1)/5)+day+(year%100)+((year%100)/4)
	   +((year/100)/4)-2*(year/100)+77;
   index = index - 7 * (index / 7);

   return days[index];
   }

/*  GDA - 0 is sunday, 1=monday, etc. (?) - see t calc: index*24hrs, etc. */

/* ----------------------------------------------------------------- */

   int open_gps(char      eph_name[80],
	     char      rin_name[80],
	     ephemeris eph[MAX_SATS],
	     int       *num_ephs)
   {
   FILE    *eph_file;
   char    str[100];
   int     count;
   double  patch_var_to_fix_a_bug_in_tc;
	   /* remember the electric monk */


   *num_ephs = 0;
   if (!((eph_file = fopen(eph_name,"rt")) == NULL))
      {
      while ( !feof(eph_file) )
	 {
	 fscanf(eph_file, "%d %d %ld", (&eph[*num_ephs].prn),
		&eph[*num_ephs].wntoe, &eph[*num_ephs].toe);
	 fgets(str, 80, eph_file);

	 /* Madness takes it's toll !!!!!!!!!!!!!!! */
	 /* A patch to get fscanf to link the floating point formats.
	    Why it doesn't when referencing an array of struct.double
	    is a mystery to me ?? - Watch out Dennis Ritchie, we
	    know where you live !! */

	 fscanf(eph_file, "%le", &patch_var_to_fix_a_bug_in_tc);
	 eph[*num_ephs].a = patch_var_to_fix_a_bug_in_tc;
	 fscanf(eph_file, "%le %le", &eph[*num_ephs].e,
			  &eph[*num_ephs].m0);
	 fgets(str, 80, eph_file);
	 fscanf(eph_file,"%le %le",&eph[*num_ephs].omegadt,
			 &eph[*num_ephs].omega0 );
	 fgets(str, 80, eph_file);
	 fscanf(eph_file,"%le %le",&eph[*num_ephs].sinw,
			 &eph[*num_ephs].cosw );
	 fgets(str, 80, eph_file);
	 fscanf(eph_file,"%le %le %le",&eph[*num_ephs].n,
			 &eph[*num_ephs].i0, &eph[*num_ephs].idot );
	 fgets(str, 80, eph_file);
	 fscanf(eph_file,"%le %le",&eph[*num_ephs].crc,
			 &eph[*num_ephs].crs );
	 fgets(str, 80, eph_file);
	 fscanf(eph_file,"%le %le",&eph[*num_ephs].cuc,
			 &eph[*num_ephs].cus );
	 fgets(str, 80, eph_file);
	 fscanf(eph_file,"%le %le",&eph[*num_ephs].cic,
			&eph[*num_ephs].cis );
	 fgets(str, 80, eph_file);
	 fscanf(eph_file,"%le %le %le",&eph[*num_ephs].af0,
			&eph[*num_ephs].af1, &eph[*num_ephs].af2);
	 fgets(str, 80, eph_file);
	 fscanf(eph_file,"%le",&eph[*num_ephs].toc);
	 fgets(str, 80, eph_file);
	 fscanf(eph_file,"%le %d",&eph[*num_ephs].ura, &eph[*num_ephs].aode);
	 fgets(str, 80, eph_file);
	 *num_ephs += 1;
	 }
      fclose(eph_file);

      if ((rinex_file = fopen(rin_name,"rt")) == NULL)
	 return 0;
      else
	 {
	 /* get rid of 16 lines garbage in RINEX file */
      	 for (count = 1; count <= 16; count++)
	 fgets(str, 100, rinex_file);
	 return 1;
	 }
      }
   else
      return 0;
   }


/* ------------------------------------------------------------------- */

   void read_gps(raw_rinex *raw_rin_mov)

   /* Reads next RINEX entry from RINEX file previously opened with
      Open_GPS */


   {

      int              count;
      char             str[100];

      fscanf(rinex_file,"%d %d %d %d %d %le %*d %d",
			&raw_rin_mov->year, &raw_rin_mov->month, &raw_rin_mov->day, &raw_rin_mov->hour,
			&raw_rin_mov->min, &raw_rin_mov->sec, &raw_rin_mov->num_sats);
      for (count = 0; count < raw_rin_mov->num_sats; count++)
	 fscanf(rinex_file,"%d", &raw_rin_mov->sat_id[count] );
      fgets(str, 100, rinex_file);
      for (count = 0; count < raw_rin_mov->num_sats; count++)
      {
	 fscanf(rinex_file,"%le %d",&raw_rin_mov->prs[count],
			    &raw_rin_mov -> snr[count] );
          fgets(str, 100, rinex_file);
      }
   }


/* ------------------------------------------------------------------- */

   void close_gps(void)
   {
      fclose(rinex_file);
   }

/* ------------------------------------------------------------------- */

void choose_sats(raw_rinex raw_rin_mov, rinex *rin_mov ,
		  ephemeris eph[MAX_SATS] , ephemeris four_eph[4] )
   {
      int year;
      int dow;
      int  id_sat[4];
      int i, j;

      year = 1900 + raw_rin_mov.year;
      /* must allow for 2000 as well - later */
      dow = dayofweek(year, raw_rin_mov.month, raw_rin_mov.day);

      rin_mov -> t    = (double)dow * 24 *3600 +
			(double)raw_rin_mov.hour * 3600 +
			(double)raw_rin_mov.min * 60 + raw_rin_mov.sec;

      for (i = 0; i < 4; i++)
	 {
	 rin_mov -> prs[i] = raw_rin_mov.prs[i];
	 id_sat[i] = raw_rin_mov.sat_id[i];
	 }
      for (i = 0 ; i < 4 ; i++)
	 {
	 for (j=0 ; j < 5 ; j++)
	    {
	    if (eph[j].prn == id_sat[i])
	       {
	       four_eph[i] = eph[j];
	       }
	    }
	 }
   }
/* ------------------------------------------------------------------- */

