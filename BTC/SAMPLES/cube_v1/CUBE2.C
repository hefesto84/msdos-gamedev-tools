/*******************************************************************
 *	CUBE2.C by  Hubert Lin & Andrew Lee			   *
 *              Mechanical Eng. of National Central University     *
 *					Started  4-27-1993	   *
 *					Finished 6-15-1993	   *
 *	Copyright(C) 1993		All Rights Reserved	   *
 *******************************************************************/

#define COS_SIN_45      0.5*1.414       /* cos45ø and sin45ø */
#define A90             3.1415926/2
#define XX              1
#define Yy              2
#define ZZ              3
#define xx              -1
#define yy              -2
#define zz              -3
#define UP              4
#define DN              -4
#define MID             0

#include <dos.h>
#include <math.h>
#include <alloc.h>
#include <ctype.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include "mouse.h"

struct TRANS_MATRIX          /* TransforCOS_SIN_45ation Matrix */
{
   double item[4][4];
};

struct XYZ                   /* Coordinate of Cube */
{
   double point[8][4];
};

double X[8] = { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 };
double Y[8] = { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 };
double pt[8] = { 0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 };
struct TRANS_MATRIX Trans_Rotate;
struct XYZ ABS;                 /* absolute coordinate 		*/
double MX[24] , MY[24];		/* midpoint of each line 	*/
int unhided_face[3];

struct TRANS_MATRIX local_to_abs =

	{  COS_SIN_45 ,        -0.5 ,         0.5 ,  0 ,
		    0 ,  COS_SIN_45 ,  COS_SIN_45 ,  0 ,
	  -COS_SIN_45 ,        -0.5 ,         0.5 ,  0 ,
		    0 ,           0 ,           0 ,  1 };

struct XYZ local = {  50 ,  50 ,  50 ,  1 ,
		     -50 ,  50 ,  50 ,  1 ,
		     -50 , -50 ,  50 ,  1 ,
		      50 , -50 ,  50 ,  1 ,
		      50 ,  50 , -50 ,  1 ,
		     -50 ,  50 , -50 ,  1 ,
		     -50 , -50 , -50 ,  1 ,
		      50 , -50 , -50 ,  1 };

double *face_point[6][4][10] = {
{{ &pt[0],&pt[1],&X[0],&Y[0],&X[1],&Y[1], &MX[0], &MY[0], &MX[1], &MY[1]} ,
 { &pt[1],&pt[2],&X[1],&Y[1],&X[2],&Y[2], &MX[2], &MY[2], &MX[3], &MY[3]} ,
 { &pt[2],&pt[3],&X[2],&Y[2],&X[3],&Y[3], &MX[4], &MY[4], &MX[5], &MY[5]} ,
 { &pt[3],&pt[0],&X[3],&Y[3],&X[0],&Y[0], &MX[6], &MY[6], &MX[7], &MY[7]} } ,

{{ &pt[4],&pt[5],&X[4],&Y[4],&X[5],&Y[5], &MX[8], &MY[8], &MX[9], &MY[9]} ,
 { &pt[5],&pt[6],&X[5],&Y[5],&X[6],&Y[6],&MX[10],&MY[10],&MX[11],&MY[11]} ,
 { &pt[6],&pt[7],&X[6],&Y[6],&X[7],&Y[7],&MX[12],&MY[12],&MX[13],&MY[13]} ,
 { &pt[7],&pt[4],&X[7],&Y[7],&X[4],&Y[4],&MX[14],&MY[14],&MX[15],&MY[15]} } ,

{{ &pt[0],&pt[1],&X[0],&Y[0],&X[1],&Y[1], &MX[0], &MY[0], &MX[1], &MY[1]} ,
 { &pt[1],&pt[5],&X[1],&Y[1],&X[5],&Y[5],&MX[16],&MY[16],&MX[17],&MY[17]} ,
 { &pt[5],&pt[4],&X[5],&Y[5],&X[4],&Y[4], &MX[9], &MY[9], &MX[8], &MY[8]} ,
 { &pt[4],&pt[0],&X[4],&Y[4],&X[0],&Y[0],&MX[21],&MY[21],&MX[20],&MY[20]} } ,

{{ &pt[2],&pt[3],&X[2],&Y[2],&X[3],&Y[3], &MX[4], &MY[4], &MX[5], &MY[5]} ,
 { &pt[3],&pt[7],&X[3],&Y[3],&X[7],&Y[7],&MX[22],&MY[22],&MX[23],&MY[23]} ,
 { &pt[7],&pt[6],&X[7],&Y[7],&X[6],&Y[6],&MX[13],&MY[13],&MX[12],&MY[12]} ,
 { &pt[6],&pt[2],&X[6],&Y[6],&X[2],&Y[2],&MX[19],&MY[19],&MX[18],&MY[18]} } ,

{{ &pt[1],&pt[2],&X[1],&Y[1],&X[2],&Y[2], &MX[2], &MY[2], &MX[3], &MY[3]} ,
 { &pt[2],&pt[6],&X[2],&Y[2],&X[6],&Y[6],&MX[18],&MY[18],&MX[19],&MY[19]} ,
 { &pt[6],&pt[5],&X[6],&Y[6],&X[5],&Y[5],&MX[11],&MY[11],&MX[10],&MY[10]} ,
 { &pt[5],&pt[1],&X[5],&Y[5],&X[1],&Y[1],&MX[17],&MY[17],&MX[16],&MY[16]} } ,

{{ &pt[0],&pt[3],&X[0],&Y[0],&X[3],&Y[3], &MX[7], &MY[7], &MX[6], &MY[6]} ,
 { &pt[3],&pt[7],&X[3],&Y[3],&X[7],&Y[7],&MX[22],&MY[22],&MX[23],&MY[23]} ,
 { &pt[7],&pt[4],&X[7],&Y[7],&X[4],&Y[4],&MX[14],&MY[14],&MX[15],&MY[15]} ,
 { &pt[4],&pt[0],&X[4],&Y[4],&X[0],&Y[0],&MX[21],&MY[21],&MX[20],&MY[20]} } };

struct XYZ multiple( struct XYZ , struct TRANS_MATRIX );
void draw_cube( void );
void screen_point( float );
void showcube( void );
void view_face( void );
int  hit( char );
void mid_point( void );
void trans_xyz( int );
void trash( void );

/* the main program of the rotation */
void body( char ch )
{
   extern first , speed;
   int code , per_motion , i , error = 1 , page = 0;
   float small_big_ratio = 1;
   int dd = 10;

   if( first == 0 )
   {
      hide_mouse();
      ABS = multiple( local , local_to_abs );
      view_face();
      screen_point( small_big_ratio );
      mid_point();
      setactivepage( page );
      draw_cube();
      delay( dd );
      setvisualpage( page );
      show_mouse();
   }
   first = 1;
   page = 1;
   code = hit(ch);
   error = 1;
   switch( code )
   {
      case UP :
         speed = 10;
         dd = 10;
         break;
      case DN :
         speed = 30;
         dd = 10;
         break;
      case MID :
         speed = 20;
         dd = 10;
         break;
      case XX :
      case Yy :
      case ZZ :
      case xx :
      case yy :
      case zz :
         trans_xyz( code );
         error = 0;
         break;
      default :
         break;
   }
   if( error == 0 )
      for( per_motion=0 ; per_motion<speed ; ++per_motion )
      {
         switch( code )
         {
            case XX :
            case Yy :
            case ZZ :
            case xx :
            case yy :
            case zz :
               local = multiple( local , Trans_Rotate );
               break;
            default :
               break;
         }
         ABS = multiple( local , local_to_abs );
         view_face();
         screen_point( small_big_ratio );
         mid_point();
         setactivepage( page );
         draw_cube();
         delay( dd );
         setvisualpage( page );
         page = ( page == 1 )? 0 : 1;
     }
     setactivepage( !page );
     showcube();
     setvisualpage(!page );
}

/* calculate the midpoints of each line */
void mid_point( void )
{
   int i , j , k;

   for( i=0 ; i<6 ; ++i )
	  for( j=0 ; j<3 ; ++j )
		 if( unhided_face[j] == i )
		 {
			for( k=0 ; k<4 ; ++k )
			{
			   *face_point[i][k][6] = *face_point[i][k][2] +
				 ( *face_point[i][k][4] - *face_point[i][k][2] )/3;
			   *face_point[i][k][7] = *face_point[i][k][3] +
				 ( *face_point[i][k][5] - *face_point[i][k][3] )/3;
			   *face_point[i][k][8] = *face_point[i][k][2] +
				 ( *face_point[i][k][4] - *face_point[i][k][2] )*2/3;
			   *face_point[i][k][9] = *face_point[i][k][3] +
				 ( *face_point[i][k][5] - *face_point[i][k][3] )*2/3;
			}
			break;
		 }
}

/* draw the cube motion */
void draw_cube( void )
{
   int i , j , k;

   hide_mouse();
   setfillstyle( SOLID_FILL , BLACK );
   bar( 160, 40 , 350 , 220);
   setcolor( BLUE );
   for( i=0 ; i<6 ; ++i )
	  for( j=0 ; j<3 ; ++j )
		 if( i == unhided_face[j] )
		 {
			for( k=0 ; k<4 ; ++k )
			line( *face_point[i][k][2] , *face_point[i][k][3] ,
				 *face_point[i][k][4] , *face_point[i][k][5] );
			line( *face_point[i][0][6] , *face_point[i][0][7] ,
				 *face_point[i][2][8] , *face_point[i][2][9] );
			line( *face_point[i][0][8] , *face_point[i][0][9] ,
				 *face_point[i][2][6] , *face_point[i][2][7] );
			line( *face_point[i][1][6] , *face_point[i][1][7] ,
				 *face_point[i][3][8] , *face_point[i][3][9] );
			line( *face_point[i][1][8] , *face_point[i][1][9] ,
				 *face_point[i][3][6] , *face_point[i][3][7] );
			break;
		 }
		 show_mouse();
}

/* find the visible face from six faces */
void view_face( void )
{
   int i , j , k=0;
   double distance=-1 , max;

   for( i=0 ; i<8 ; ++i )
	  if( ABS.point[i][2] > distance )
	  {
		 distance = ABS.point[i][2];
		 max = i;
	  }
   for( i=0 ; i<6 ; ++i )
	  for( j=0 ; j<4 ; ++j )
		 if( ( *face_point[i][j][0] == max ) ||
			 ( *face_point[i][j][0] == max ) )
		 {
			unhided_face[k] = i;
			++k;
			break;
		 }
}

/* transfer the 3-D points to 2-D screen */
void screen_point( float ratio )
{
   int i;

   for( i=0 ; i<8 ; ++i )
   {
      X[i] = ( 255 + ABS.point[i][0] )*ratio;
      Y[i] = 350-( 220  + ABS.point[i][1] )*ratio;
   }
}

/* manage the key hit */
int hit( char code )
{
   switch( code )
   {
	  case 'x' :
		 return( xx );
	  case 'y' :
		 return( yy );
	  case 'z' :
		 return( zz );
	  case 'X' :
		 return( XX );
	  case 'Y' :
		 return( Yy );
	  case 'Z' :
		 return( ZZ );
	  case '+' :
		 return( UP );
	  case '-' :
		 return( DN );
	  case '=' :
		 return( MID );
   }
   return( -11 );
}

/* maxtrix multiplication [8x4]*multiple=[8x4] */
struct XYZ multiple( struct XYZ source , struct TRANS_MATRIX transfer )
{
   int i , j , k;
   double temp = 0;
   struct XYZ result;

   for( i=0 ; i<8 ; ++i )
   {
	  for( j=0 ; j<4 ; ++j )
	  {
		for( k=0 ; k<4 ; ++k )
		   temp += source.point[i][k]*transfer.item[k][j];
		result.point[i][j] = temp;
		temp = 0;
	  }
   }
   return( result );
}

/* fill the Trans_Rotate with 0 */
void trash( void )
{
   int i , j;
   for( i=0 ; i<4 ; ++i )
	  for( j=0 ; j<4 ; ++j )
	  {
		 Trans_Rotate.item[i][j] = 0;
		 Trans_Rotate.item[i][j] = 0;
		 Trans_Rotate.item[i][j] = 0;
	  }
}

/* produce the transformation matrix */
void trans_xyz( int mode )
{
	extern speed;
   double angle = A90/speed;

   trash();
   if( mode < 0 )
	  angle *= -1;
   switch( abs(mode) )
   {
	  case 1 :
		 Trans_Rotate.item[0][0] = 1;
		 Trans_Rotate.item[1][1] = cos( angle );
		 Trans_Rotate.item[1][2] = sin( angle );
		 Trans_Rotate.item[2][1] = (-1)*sin( angle );
		 Trans_Rotate.item[2][2] = cos( angle );
		 Trans_Rotate.item[3][3] = 1;
		 break;
	  case 3 :
		 Trans_Rotate.item[0][0] = cos( angle );
		 Trans_Rotate.item[0][2] = (-1)*sin( angle );
		 Trans_Rotate.item[1][1] = 1;
		 Trans_Rotate.item[2][0] = sin( angle );
		 Trans_Rotate.item[2][2] = cos( angle );
		 Trans_Rotate.item[3][3] = 1;
		 break;
	  case 2 :
		 Trans_Rotate.item[0][0] = cos( angle );
		 Trans_Rotate.item[0][1] = sin( angle );
		 Trans_Rotate.item[1][0] = (-1)*sin( angle );
		 Trans_Rotate.item[1][1] = cos( angle );
		 Trans_Rotate.item[2][2] = 1;
		 Trans_Rotate.item[3][3] = 1;
		 break;
	  default :
		 break;
   }
}
