/*******************************************************************
 *  CUBE1.C by  Hubert Lin & Andrew Lee                            *
 *              Mechanical Eng. of National Central University     *
 *					Started  4-27-1993	   *
 *					Finished 6-15-1993	   *
 *	Copyright(C) 1993		All Rights Reserved	   *
 *******************************************************************/

#define VERSION     "Version 1.0"

#define F            0   /* Front  face */
#define R            1   /* Right  face */
#define T            2   /* Top    face */
#define H            3   /* Bottom face */
#define L            4   /* Left   face */
#define B            5   /* beHind face */

#define RR           0   /* Red      */
#define OO           1   /* Orange   */
#define GG           2   /* Green    */
#define BB           3   /* Blue     */
#define YY           4   /* Yellow   */
#define WW           5   /* White    */

#define SRIMN        8   /* number of small_rim */
#define BRIMN       12   /* number of big_rim   */

#define CW          -1   /* clockwise */
#define CCW         -2   /* counterclockwise */

#define ESC         27   /* key code of Esc */
#define F1          59
#define F2          60
#define F3          61
#define F4          62
#define F5          63
#define F6          64

#define ON           1
#define OFF          0

#include <dos.h>
#include <alloc.h>
#include <ctype.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include "mouse.h"

void body( char );
char getmousech( MOUSE * );
void to_graphic_mode( void );
void initialize( void );
void handle( char );
void faceCW( char );
void faceCCW( char );
int  cond( char );
void reassignfacestruct( char );
void showcube( void );
void cube_rotate( char , int );
void savegame( void );
void loadgame( void );
void soundswitch( void );
void speedswitch( void );
void word( void );
void title( void );
void draw_win( void );
void sink( int , int , int , int , int );
void mem_error( void );
void file_error( char * );
void show_version( int , int , int );
void palette( int );
void shell( void );
void clr_kb_buff( void );
void sinkkey( char );
void help( void );
void message( int , char * );
void cubesound( void );

int first=0;		/* check if it is first time to draw the cube 	*/
int speed=30;		/* the rotation speed of the cube  		*/

int speeddata[] = {10, 30, 50};
			/* they are "Fast", "Normal", and "Slow".	*/

int speedcontrol = 1;	/* the controler of speeddata[]			*/
MOUSE mouse;		/* the structure of the cursor of the mouse	*/
int soundflag = ON;	/* the flag of sound ON or OFF			*/
int mu_install =OFF;	/* the flag of use_mouse or not_use_mouse	*/
int sq[49];		/* the color of the 48 squares, begins at sq[1]	*/

int condition[] =  {RR, BB, YY, OO, GG, WW};
	/* they are  F,  R,  T,  H,  L,  B ,respectively*/

int now=0, next=0;
int color[] ={ GREEN , LIGHTRED ,  RED  , DARKGRAY , LIGHTGRAY , WHITE };
             /* RED  ,    BROWN , GREEN ,     BLUE ,    YELLOW , WHITE */

/* the structure of the 6 faces */
struct one_face
{
    int *srim[SRIMN+1];      /* begins at srim[1]    */
    int *brim[BRIMN+1];      /* begins at brim[1]    */
} face[] =
{
    {    NULL ,  &sq[1] ,  &sq[2] ,  &sq[3] ,  &sq[4] ,  &sq[5] ,  &sq[6] ,
       &sq[7] ,  &sq[8] ,    NULL , &sq[39] , &sq[38] , &sq[37] , &sq[25] ,
      &sq[32] , &sq[31] , &sq[47] , &sq[46] , &sq[45] , &sq[21] , &sq[20] ,
      &sq[19] } ,

    {    NULL ,  &sq[9] , &sq[10] , &sq[11] , &sq[12] , &sq[13] , &sq[14] ,
      &sq[15] , &sq[16] ,    NULL , &sq[35] , &sq[34] , &sq[33] , &sq[17] ,
      &sq[24] , &sq[23] , &sq[43] , &sq[42] , &sq[41] , &sq[29] , &sq[28] ,
      &sq[27] } ,

    {    NULL , &sq[17] , &sq[18] , &sq[19] , &sq[20] , &sq[21] , &sq[22] ,
      &sq[23] , &sq[24] ,    NULL , &sq[33] , &sq[40] , &sq[39] ,  &sq[1] ,
       &sq[8] ,  &sq[7] , &sq[45] , &sq[44] , &sq[43] , &sq[13] , &sq[12] ,
      &sq[11] } ,

    {    NULL , &sq[25] , &sq[26] , &sq[27] , &sq[28] , &sq[29] , &sq[30] ,
      &sq[31] , &sq[32] ,    NULL , &sq[37] , &sq[36] , &sq[35] ,  &sq[9] ,
      &sq[16] , &sq[15] , &sq[41] , &sq[48] , &sq[47] ,  &sq[5] ,  &sq[4] ,
       &sq[3] } ,

    {    NULL , &sq[33] , &sq[34] , &sq[35] , &sq[36] , &sq[37] , &sq[38] ,
      &sq[39] , &sq[40] ,    NULL , &sq[11] , &sq[10] ,  &sq[9] , &sq[27] ,
      &sq[26] , &sq[25] ,  &sq[3] ,  &sq[2] ,  &sq[1] , &sq[19] , &sq[18] ,
      &sq[17] } ,

    {    NULL , &sq[41] , &sq[42] , &sq[43] , &sq[44] , &sq[45] , &sq[46] ,
      &sq[47] , &sq[48] ,    NULL , &sq[15] , &sq[14] , &sq[13] , &sq[23] ,
      &sq[22] , &sq[21] ,  &sq[7] ,  &sq[6] ,  &sq[5] , &sq[31] , &sq[30] ,
      &sq[29] }
};

struct condition_data
{
    int face[6];             /* face F , R , T , H , L , B */
    int lefttop[3];          /* face F , R , T */
    int cw[3][2];            /* rotate about x , y , z */
    int ccw[3][2];           /* rotate about x , y , z */
} conddata[] =
{
     {RR,BB,YY,OO,GG,WW, 1,1,1,  WW,BB, RR,YY, BB,OO,  YY,BB, RR,WW, GG,RR},
     {RR,WW,BB,OO,YY,GG, 3,7,3,  GG,WW, RR,BB, WW,OO,  BB,WW, RR,GG, YY,RR},
     {RR,GG,WW,OO,BB,YY, 5,5,1,  YY,GG, RR,WW, GG,OO,  WW,GG, RR,YY, BB,RR},
     {RR,YY,GG,OO,WW,BB, 7,7,7,  BB,YY, RR,GG, YY,OO,  GG,YY, RR,BB, WW,RR},

     {OO,GG,YY,RR,BB,WW, 1,1,5,  WW,GG, OO,YY, GG,RR,  YY,GG, OO,WW, BB,OO},
     {OO,WW,GG,RR,YY,BB, 3,3,3,  BB,WW, OO,GG, WW,RR,  GG,WW, OO,BB, YY,OO},
     {OO,BB,WW,RR,GG,YY, 5,5,5,  YY,BB, OO,WW, BB,RR,  WW,BB, OO,YY, GG,OO},
     {OO,YY,BB,RR,WW,GG, 7,3,7,  GG,YY, OO,BB, YY,RR,  BB,YY, OO,GG, WW,OO},

     {GG,YY,OO,BB,WW,RR, 7,1,7,  RR,YY, GG,OO, YY,BB,  OO,YY, GG,RR, WW,GG},
     {GG,RR,YY,BB,OO,WW, 1,1,3,  WW,RR, GG,YY, RR,BB,  YY,RR, GG,WW, OO,GG},
     {GG,WW,RR,BB,YY,OO, 3,5,3,  OO,WW, GG,RR, WW,BB,  RR,WW, GG,OO, YY,GG},
     {GG,OO,WW,BB,RR,YY, 5,5,7,  YY,OO, GG,WW, OO,BB,  WW,OO, GG,YY, RR,GG},

     {BB,OO,YY,GG,RR,WW, 1,1,7,  WW,OO, BB,YY, OO,GG,  YY,OO, BB,WW, RR,BB},
     {BB,WW,OO,GG,YY,RR, 3,1,3,  RR,WW, BB,OO, WW,GG,  OO,WW, BB,RR, YY,BB},
     {BB,RR,WW,GG,OO,YY, 5,5,3,  YY,RR, BB,WW, RR,GG,  WW,RR, BB,YY, OO,BB},
     {BB,YY,RR,GG,WW,OO, 7,5,7,  OO,YY, BB,RR, YY,GG,  OO,YY, BB,OO, WW,BB},

     {YY,BB,OO,WW,GG,RR, 1,3,5,  RR,BB, YY,OO, BB,WW,  OO,BB, YY,RR, GG,YY},
     {YY,RR,BB,WW,OO,GG, 3,3,5,  GG,RR, YY,BB, RR,WW,  BB,RR, YY,GG, OO,YY},
     {YY,GG,RR,WW,BB,OO, 5,3,5,  OO,GG, YY,RR, GG,WW,  RR,GG, YY,OO, BB,YY},
     {YY,OO,GG,WW,RR,BB, 7,3,5,  BB,OO, YY,GG, OO,WW,  GG,OO, YY,BB, RR,YY},

     {WW,GG,OO,YY,BB,RR, 1,7,1,  RR,GG, WW,OO, GG,YY,  OO,GG, WW,RR, BB,WW},
     {WW,RR,GG,YY,OO,BB, 3,7,1,  BB,RR, WW,GG, RR,YY,  GG,RR, WW,BB, OO,WW},
     {WW,BB,RR,YY,GG,OO, 5,7,1,  OO,BB, WW,RR, BB,YY,  RR,BB, WW,OO, GG,WW},
     {WW,OO,BB,YY,RR,GG, 7,7,1,  GG,OO, WW,BB, OO,YY,  BB,OO, WW,GG, RR,WW}
};

/* the initial color of the 3 visible faces ( Front, Right, Top )	*/
struct vfacedata
{
    int color[8];
} vface[] =
{
    { GREEN , GREEN , GREEN , GREEN , GREEN , GREEN , GREEN , GREEN } ,
    {  DARKGRAY ,  DARKGRAY ,  DARKGRAY ,  DARKGRAY ,  DARKGRAY ,  DARKGRAY ,
       DARKGRAY ,  DARKGRAY } ,
    { LIGHTGRAY , LIGHTGRAY , LIGHTGRAY , LIGHTGRAY , LIGHTGRAY , LIGHTGRAY ,
      LIGHTGRAY , LIGHTGRAY }
};

/* defines the mouse clickable areas and the corresponding keys */
struct mu_click
{
        int cl, ct, cr, cb;     /* left, top, right, bottom points of the area */
	char key;		/* the corresponding keys of the area */
} def_click[]=
{
    {  18+(65+1)*0 , 253 ,  18+(65+1)*0+64 , 253+44 , 'f' },
    {  18+(65+1)*1 , 253 ,  18+(65+1)*1+64 , 253+44 , 'r' },
    {  18+(65+1)*2 , 253 ,  18+(65+1)*2+64 , 253+44 , 't' },
    {  18+(65+1)*3 , 253 ,  18+(65+1)*3+64 , 253+44 , 'l' },
    {  18+(65+1)*4 , 253 ,  18+(65+1)*4+64 , 253+44 , 'b' },
    {  18+(65+1)*5 , 253 ,  18+(65+1)*5+64 , 253+44 , 'h' },
    { 426+(65+1)*0 , 253 , 426+(65+1)*0+64 , 253+44 , 'x' },
    { 426+(65+1)*1 , 253 , 426+(65+1)*1+64 , 253+44 , 'z' },
    { 426+(65+1)*2 , 253 , 426+(65+1)*2+64 , 253+44 , 'y' },

    {  18+(65+1)*0 , 253+46 ,  18+(65+1)*0+64 , 253+44+46 , 'F' },
    {  18+(65+1)*1 , 253+46 ,  18+(65+1)*1+64 , 253+44+46 , 'R' },
    {  18+(65+1)*2 , 253+46 ,  18+(65+1)*2+64 , 253+44+46 , 'T' },
    {  18+(65+1)*3 , 253+46 ,  18+(65+1)*3+64 , 253+44+46 , 'L' },
    {  18+(65+1)*4 , 253+46 ,  18+(65+1)*4+64 , 253+44+46 , 'B' },
    {  18+(65+1)*5 , 253+46 ,  18+(65+1)*5+64 , 253+44+46 , 'H' },
    { 426+(65+1)*0 , 253+46 , 426+(65+1)*0+64 , 253+44+46 , 'X' },
    { 426+(65+1)*1 , 253+46 , 426+(65+1)*1+64 , 253+44+46 , 'Z' },
    { 426+(65+1)*2 , 253+46 , 426+(65+1)*2+64 , 253+44+46 , 'Y' },

    { 526 , 62+(21+7)*0 , 526+80 , 62+(21+7)*0+20 , F2 },
    { 526 , 62+(21+7)*1 , 526+80 , 62+(21+7)*1+20 , F3 },
    { 526 , 62+(21+7)*2 , 526+80 , 62+(21+7)*2+20 , F4 },
    { 526 , 62+(21+7)*3 , 526+80 , 62+(21+7)*3+20 , F5 },
    { 526 , 62+(21+7)*4 , 526+80 , 62+(21+7)*4+20 , F6 }
};

/************************* begins the main() *************************/

void main( void )
{
	int i;
	char ch = 0;

	#ifndef	__LARGE__
		clrscr();
		printf("\n\nNeeds to compile in Large Model.\n");
		exit(1);
	#endif

        if( strcmp( getenv( "HAVE_CUBE " ) , " YES" ) == 0 )
        {
                printf( "\nMagic Cube has already executed !\n" );
                printf( "Type EXIT to return to Magic Cube . . .\n" );
                exit(1);
        }
	to_graphic_mode();
	initialize();
	word();
	setactivepage(0);
	setvisualpage(0);
	title();
	draw_win();
	setactivepage(1);
	draw_win();

	/* assign the colors to 48 &squares */
        for( i=1 ; i<=48 ; i++ )
        sq[i] = color[ (int) (i-1)/8 ];

        body( ch );
	showcube();
	clr_kb_buff();
        while( ch != ESC )
        {
                ch = -1;
                if( !kbhit() )
                {
                   if( click_button(&mouse, LEFT_B) )
                       ch = getmousech(&mouse);
                }
                else
                   ch = getch();
                handle( ch );
                if( ch != -1 )
                    body( ch );
        }
        closegraph();
}

/************************* End of main() *****************************/

/* turn to the graphic mode, this game needs a EGA/VGA monitor */
void to_graphic_mode( void )
{
	int gdriver = DETECT , gmode , gcode;

        registerbgidriver( EGAVGA_driver );
        initgraph( &gdriver , &gmode , "" );
	closegraph();
        if( !((gdriver==EGA) || (gdriver==VGA)) )
        {
                printf("\nThis game needs a EGA/VGA monitor!\n");
                exit(1);
        }
        gdriver = EGA;
        gmode = EGAHI;
        initgraph( &gdriver , &gmode , "" );
	gcode = graphresult();
        if( gcode != grOk )
	{
		printf("\nGraphics System Error: %s\n", grapherrormsg(gcode) );
		exit(1);
	}
}

/* detect if there is a mouse */
void initialize( void )
{
	extern unsigned Cursor[][32];

	delay(1);
	init_mouse();
        set_graphic_cursor( 5 , 1 , Cursor[0] );
	show_mouse();
}

void handle( char ch )
{
	extern speed;

	sinkkey( ch );
	switch( ch )
	{
		case 't' :
		case 'T' :
		case 'f' :
		case 'F' :
		case 'r' :
		case 'R' :
		case 'l' :
		case 'L' :
		case 'h' :
		case 'H' :
		case 'b' :
		case 'B' :
			   if( islower( ch ) )
				   faceCW( ch );
			   else
			   {
				   ch = tolower( ch );
				   faceCCW( ch );
			   }
			   break;
		case 'x' :
		case 'X' :
		case 'y' :
		case 'Y' :
		case 'z' :
		case 'Z' :
			   if( islower( ch ) )
				   cube_rotate( ch , CW );
			   else
			   {
				   ch = tolower( ch );
				   cube_rotate( ch, CCW );
			   }
			   break;
		case F1 :
			   help();
			   break;
		case F2 :
			   savegame();
			   break;
		case F3 :
			   loadgame();
			   break;
		case F4 :
			   speedswitch();
			   break;
		case F5 :
			   soundswitch();
			   break;
		case F6 :
			   shell();
			   break;
		default : ;
	}
}

/* change the structure of a specified face by ClockWise */
void faceCW( char ch )
{
	int *temp[3] , i;

	for( i=0 ; i<2 ; i++ )
		temp[i] = face[ cond(ch) ].srim[ i+SRIMN-1 ];
	for( i=SRIMN ; i>=3 ; i-- )
		face[ cond(ch) ].srim[i] = face[ cond(ch) ].srim[i-2];
	for( i=1 ; i<=2 ; i++ )
		face[ cond(ch) ].srim[i] = temp[i-1];

	for( i=0 ; i<3 ; i++ )
		temp[i] = face[ cond(ch) ].brim[ i+BRIMN-2 ];
	for( i=BRIMN ; i>=4 ; i-- )
		face[ cond(ch) ].brim[i] = face[ cond(ch) ].brim[i-3];
	for( i=1 ; i<=3 ; i++ )
		face[ cond(ch) ].brim[i] = temp[i-1];

	reassignfacestruct( ch );
}

/* change the structure of a specified face by CounterClockWise */
void faceCCW( char ch )
{
    int *temp[3] , i;

    for( i=0 ; i<2 ; i++ )
	   temp[i] = face[ cond(ch) ].srim[i+1];
    for( i=1 ; i<=SRIMN-2 ; i++ )
	   face[ cond(ch) ].srim[i] = face[ cond(ch) ].srim[i+2];
    for( i=7 ; i<=8 ; i++ )
	   face[ cond(ch) ].srim[i] = temp[i-7];

    for( i=0 ; i<3 ; i++ )
	   temp[i] = face[ cond(ch) ].brim[i+1];
    for( i=1 ; i<=BRIMN-3 ; i++ )
	   face[ cond(ch) ].brim[i] = face[ cond(ch) ].brim[i+3];
    for( i=10 ; i<=BRIMN ; i++ )
	   face[ cond(ch) ].brim[i] = temp[i-10];

    reassignfacestruct( ch );
}

/* convert a direction-face to a color-face */
int cond( char ch )
{
	/* do NOT change the order of condmark */
    char condmark[] = { 'f' , 'r' , 't' , 'h' , 'l' , 'b' };
    int i;

    for( i=0 ; i<sizeof(condmark)/sizeof(char) ; i++ )
	   if( ch == condmark[i] )
		  return( condition[i] );
    return 0;
}

/* rearrange the structure of the 4 faces perpendicular to a specified face */
void reassignfacestruct( char ch )
{
    int i , j , k , begin ;
    struct { int color[5]; } thisface[]=
    {
        12 , 1 , 2 , 3 ,  4 ,
         3 , 3 , 4 , 5 ,  7 ,
         6 , 5 , 6 , 7 , 10 ,
         9 , 7 , 8 , 1 ,  1
    };
    struct bst { int color[4] , num[4]; } bstruct[]=
    {
        { YY , BB , WW , GG ,
          10 ,  1 , 10 ,  7 },       /*RR*/
        { YY , GG , WW , BB ,
           4 ,  1 ,  4 ,  7 },       /*OO*/
        { YY , RR , WW , OO ,
           1 ,  1 ,  7 ,  7 },       /*GG*/
        { YY , OO , WW , RR ,
           7 ,  1 ,  1 ,  7 },       /*BB*/
        { OO , BB , RR , GG ,
           4 ,  4 ,  4 ,  4 },       /*YY*/
        { OO , GG , RR , BB ,
          10 , 10 , 10 , 10 }        /*WW*/
    };
    struct sst { int color[12] , num[12]; } sstruct[]=
    {
	{ YY , YY , YY , BB , BB , BB , WW , WW , WW , GG , GG , GG ,
	   7 ,	6 ,  5 ,  1 ,  8 ,  7 ,  7 ,  6 ,  5 ,	5 ,  4 ,  3 },	/*RR*/
	{ YY , YY , YY , GG , GG , GG , WW , WW , WW , BB , BB , BB ,
	   3 ,	2 ,  1 ,  1 ,  8 ,  7 ,  3 ,  2 ,  1 ,	5 ,  4 ,  3 },	/*OO*/
	{ YY , YY , YY , RR , RR , RR , WW , WW , WW , OO , OO , OO ,
	   1 ,	8 ,  7 ,  1 ,  8 ,  7 ,  5 ,  4 ,  3 ,	5 ,  4 ,  3 },	/*GG*/
	{ YY , YY , YY , OO , OO , OO , WW , WW , WW , RR , RR , RR ,
	   5 ,	4 ,  3 ,  1 ,  8 ,  7 ,  1 ,  8 ,  7 ,	5 ,  4 ,  3 },	/*BB*/
	{ OO , OO , OO , BB , BB , BB , RR , RR , RR , GG , GG , GG ,
	   3 ,	2 ,  1 ,  3 ,  2 ,  1 ,  3 ,  2 ,  1 ,	3 ,  2 ,  1 },	/*YY*/
	{ OO , OO , OO , GG , GG , GG , RR , RR , RR , BB , BB , BB ,
	   7 ,	6 ,  5 ,  7 ,  6 ,  5 ,  7 ,  6 ,  5 ,	7 ,  6 ,  5 }	/*WW*/
    };

	for( i=0 ; i<4 ; i++ )
	{
	   begin = bstruct[ cond(ch) ].num[i];
	   for( j=0 ; j<5 ; j++ )
	   {
		  if( (j==0) || (j==4) )
			face[ bstruct[cond(ch)].color[i] ].brim[begin] =
			face[cond(ch)].brim[ thisface[i].color[j] ];
		  else
			face[ bstruct[cond(ch) ].color[i]].brim[begin] =
			face[cond(ch)].srim[ thisface[i].color[j] ];
		  if( begin==1 )
			begin = 12;
		  else
			begin--;
	   }
	}
	for( i=0 ; i<12 ; i++ )
	   face[ sstruct[cond(ch)].color[i] ].srim[ sstruct[cond(ch)].num[i] ]
		  = face[cond(ch)].brim[i+1];

	for( i=0 ; i<3 ; i++ )
	{
	   begin = conddata[next].lefttop[i];
	   for( j=0 ; j<8 ; j++ )
	   {
		  vface[i].color[j] = *face[ condition[i]].srim[begin ];
		  if( begin==8 )
			 begin = 1;
		  else
			 begin++;
	   }
	}
}

/* repaint the color of the 3 visible faces */
void showcube( void )
{
	int i, j;

	/* pt[][9][2]  the coordinates of the points used to floodfill */
	int pt[][9][2] = {
	{ 192 , 113 , 216 , 129 , 240 , 146 , 241 , 170 , 241 , 196 , 216 ,
	  176 , 195 , 160 , 192 , 137 , 216 , 153 } ,
	{ 263 , 148 , 288 , 131 , 312 , 115 , 314 , 138 , 315 , 159 , 291 ,
	  176 , 260 , 194 , 265 , 171 , 285 , 156 } ,
	{ 253 ,  61 , 278 ,  79 , 300 ,  96 , 280 , 108 , 250 , 127 , 230 ,
	  110 , 204 ,  90 , 230 ,  78 , 250 ,  92 } };

	setcolor( DARKGRAY );
	hide_mouse();
	for( i=0 ; i<3 ; i++ )
	   for( j=1 ; j<=9 ; j++ )
	   {
		  if( j==9 )
			 setfillstyle( SOLID_FILL , color[condition[i]] );
		  else
			 setfillstyle( SOLID_FILL , vface[i].color[j-1] );
			 floodfill( pt[i][j-1][0]+i , pt[i][j-1][1] , BLUE );
	   }
	   show_mouse();
}

/* rearrange the data of the 3 visible faces when the cube rotates */
void cube_rotate( char ch , int status )
{
	int i , j , begin;

	for( now=0 ; now<24 ; now++ )
	   if( (condition[F]==conddata[now].face[F]) &&
		   (condition[R]==conddata[now].face[R]) )
		  break;

	for( next=0 ; next<24 ; next++ )
	{
	   if( (status==CCW) &&
		(conddata[next].face[F]==conddata[now].ccw[ch-'x'][F]) &&
		(conddata[next].face[R]==conddata[now].ccw[ch-'x'][R]) )
		  break;
	   if( (status==CW) &&
		(conddata[next].face[F]==conddata[now].cw[ch-'x'][F]) &&
		(conddata[next].face[R]==conddata[now].cw[ch-'x'][R]) )
		  break;
	}

	for( i=0 ; i<6 ; i++ )
	   condition[i] = conddata[next].face[i];

	for( i=0 ; i<3 ; i++ )
	{
		begin = conddata[next].lefttop[i];
		for( j=0 ; j<8 ; j++ )
		{
		   vface[i].color[j] = *face[ condition[i] ].srim[begin];
		   if( begin==8 )
			  begin = 1;
		   else
			  begin++;
		}
	}
}

/* returns a corresponding key if the mouse clicks on the clickable areas */
char getmousech( MOUSE *mu1 )
{
	int k;

	for( k=0 ; k<(sizeof(def_click)/sizeof(struct mu_click)) ; k++ )
	   if( (mu1->x>=def_click[k].cl) && (mu1->x<=def_click[k].cr) &&
		   (mu1->y>=def_click[k].ct)&&(mu1->y<=def_click[k].cb) )
		  return def_click[k].key;
}

/* show simple message, like "Saving", "Loading", etc. */
void message( int color , char *msg )
{
	hide_mouse();
	setcolor( color );
	settextjustify( CENTER_TEXT , TOP_TEXT );
	outtextxy( 565 , 40 , msg );
	show_mouse();
}

void savegame( void )
{
	int i , j;
	FILE *fp;
	char *filename = "CUBE.SAV";
	char *savemsg = "Saving...";

	if( ( fp=fopen( filename , "w" ) )==NULL )
	   file_error( filename );
	message( 3 , savemsg );
	for( i=0 ; i<6 ; i++ )
	{
	   fprintf( fp , "%d " , condition[i] );
	   for( j=1 ; j<=SRIMN ; j++ )
		  fprintf( fp , "%d " , *face[i].srim[j] );
	   for( j=1 ; j<=BRIMN ; j++ )
		  fprintf( fp , "%d " , *face[i].brim[j] );
	}
	fclose( fp );
	sleep( 1 );
	message( BLACK , savemsg );
}

void loadgame( void )
{
	int i , j;
	FILE *fp;
	char *filename = "CUBE.SAV";

	if( ( fp=fopen( filename, "r" ) )==NULL )
	{
		sound(1000);
		delay(50);
		nosound();
		message(3, "Fail !");
		sleep(1);
		message(BLACK, "Fail !");
	}
	else
	{
		message( 3 , "Loading.." );
		for( i=0 ; i<6 ; i++ )
		{
		   fscanf( fp , "%d" , &condition[i] );
		   for( j=1 ; j<=SRIMN ; j++ )
			  fscanf( fp , "%d" , face[i].srim[j] );
		   for( j=1 ; j<=BRIMN ; j++ )
			  fscanf( fp , "%d" , face[i].brim[j] );
		}
		fclose( fp );
		cube_rotate( 'x' , CCW );
		cube_rotate( 'x' , CW );
		showcube();
		sleep( 1 );
		message( BLACK , "Loading.." );
	}
}

/* toggles the sound ON/OFF */
void soundswitch( void )
{
	extern soundflag;
	char *soundmsg[] = { "Sound Off" , "Sound On" };

	soundflag = ( soundflag==ON )? OFF: ON;
	message( 3 , soundmsg[soundflag] );
	sleep( 1 );
	message( BLACK , soundmsg[soundflag] );
}

/* show the title of the game */
void title( void )
{
	unsigned titlesize;
	void *buff;
	char *name = "CUBE.TIT";
	FILE *fp;

	titlesize = 50408;
	if( (buff=(void *) malloc(titlesize))==NULL )
	   mem_error();
	if( ( fp=fopen( name ,"rb" ) )==NULL )
	   file_error( name );
	fread( buff , titlesize , 1 , fp );
	fclose( fp );
	palette( 1 );
	hide_mouse();
	cleardevice();
	clr_kb_buff();
	putimage( 55 , 90 , buff , COPY_PUT );
	show_version( 363 , 233 , LIGHTBLUE );
	free( buff );
	show_mouse();
	while( !kbhit() )
	   if( (mu_install) && (which_pressed(&mouse)>0) )
		   break;
	hide_mouse();
	cleardevice();
	show_mouse();
}

/* draw the main window of the game */
void draw_win(void)
{
	unsigned winsize;
	void *winbuff[4];
	char *winname = "CUBE.WIN";
	FILE *fp;
	int i , j;
	int width = 65 , hight = 45;
	char *facekey[][6] = { "f" , "r" , "t" , "l" , "b" , "h" ,
			       "F" , "R" , "T" , "L" , "B" , "H" };
	char *cubekey[][3] = { "x" , "z" , "y" , "X" , "Z" , "Y" };

	hide_mouse();
	palette( 0 );
	winsize = 28024;
	if( ( fp=fopen( winname, "rb" ) )==NULL )
	   file_error( winname );
	for( i=0 ; i<4 ; i++ )
	{
	   if( ( winbuff[i]=malloc(winsize) )==NULL )
		  mem_error();
	   fread( winbuff[i] , winsize , 1 , fp );
	   putimage( 160*i , 0 , winbuff[i] , COPY_PUT );
	   free( winbuff[i] );
	}
	fclose( fp );

	setcolor( BLUE );
	for( i=0 ; i<2 ; i++ )
	{
	   for( j=0 ; j<6 ; j++ )
		   outtextxy( 18+j*(width+2)+2 , 253+(hight+2)*i+2 , facekey[i][j] );
	   for( j=0 ; j<3 ; j++ )
		   outtextxy( 426+j*(width+2)+2 , 253+(hight+2)*i+2 , cubekey[i][j] );
	}
	show_version( 524 , 243 , BLACK );
	palette( 2 );
	show_mouse();
}

/* draw shaded rectangles */
void sink( int left , int top , int right , int bottom , int status )
{
	setcolor( ( status==ON )? WHITE : BLUE);
	line( left , top , right , top );
	line( left , top , left , bottom );

	setcolor( ( status==OFF )? WHITE : BLUE );
	line( right , bottom , right , top);
	line( right , bottom , left , bottom );
}

/* exit the game when go out of the memory */
void mem_error(void)
{
	closegraph();
	printf( "\nOut of memory!\n" );
	exit(1);
}

/* exit the game when file reading error */
void file_error( char *filename )
{
	closegraph();
	printf( "\nCan not open file '%s'!\n", filename );
	exit(1);
}

/* show the version of the game */
void show_version( int x , int y , int color )
{
	settextjustify( LEFT_TEXT , TOP_TEXT );
	setcolor( color );
	outtextxy( x , y , VERSION );
}

/* change the palettes */
void palette( int num )
{
	struct palettetype palette[] =
	{
		{16, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{16, 0,56,4,60,2,58,20,62,1,57,5,61,3,59,7,63},	/* for title */
		{16, 0,56,4,60,2,58,20,54,1,57,5,61,38,59,7,63}	/* for game */
	};

	setallpalette( &palette[num] );
}

/* DOS shell */
void shell( void )
{
	hide_mouse();
	closegraph();
	printf( "\nCUBE %s\nType \"EXIT\" to return to CUBE ...\n", VERSION );
        putenv( "HAVE_CUBE = YES" );
	system( "" );
	to_graphic_mode();
	initialize();
	draw_win();
	setactivepage(1);
	draw_win();
	first=0;
	body( 0 );
}

/* clear the keyboard buffer */
void clr_kb_buff( void )	  /*  clear the keyboard buffer   */
{
	union REGS reg;

	reg.h.ah = 0x0c;
	intdos( &reg , &reg );
}

void sinkkey( char ch )
{
	int i;

	for( i=0 ; i<(sizeof(def_click)/sizeof(struct mu_click)) ; i++ )
	   if( ch==def_click[i].key )
	   {
		   hide_mouse();
		   sink( def_click[i].cl, def_click[i].ct ,
			 def_click[i].cr, def_click[i].cb , OFF );
		   show_mouse();
		   while( (mu_install) && (which_pressed(&mouse)!=0) );
		   delay( 100 );
		   hide_mouse();
		   sink( def_click[i].cl , def_click[i].ct ,
			 def_click[i].cr , def_click[i].cb , ON );
		   show_mouse();
		   cubesound();
	   }
}

/* show the function keys in details */
void help( void )
{
	int i;
	void *helpbuff;
	unsigned helpsize;
	char *helpmsg[] =
	{
		"F1 : Help Menu" ,
		"F2 : Save Game" ,
		"F3 : Load Game" ,
		"F4 : Rotate Speed" ,
		"F5 : Sound Switch" ,
		"F6 : DOS Shell" ,
		"Esc: Quit Game"
	};

	hide_mouse();
	set_x_range( 20 , 500 );
	set_y_range( 10 , 240 );
	helpsize = imagesize( 160 , 40 , 350 , 220 );
	if( ( helpbuff=malloc(helpsize) )==NULL )
	   mem_error();
	getimage( 160 , 40 , 350 , 220 , helpbuff );
	setfillstyle( SOLID_FILL , BLACK );
	bar( 100 , 40 , 380 , 220 );
	show_version( 180 , 160 , 13 );
	setcolor( LIGHTGRAY );
	for( i=0 ; i<sizeof(helpmsg)/sizeof(helpmsg[0]) ; i++ )
	   outtextxy( 180 , 70+(2+textheight("H"))*i , helpmsg[i] );
	show_mouse();
	while( !kbhit() )
	   if( (mu_install) && (which_pressed(&mouse)>0) )
		  break;
	hide_mouse();
	bar( 100 , 40 , 380 , 220 );
	putimage( 160 , 40 , helpbuff , COPY_PUT );
	free( helpbuff );
	clr_kb_buff();
	set_x_range(0 , getmaxx() );
	set_y_range( 0 , getmaxy() );
	show_mouse();
}

/* change the rotation speed of the cube */
void speedswitch( void )
{
	char *speedmsg[] = { "Fast" , "Normal" , "Slow" };

	speedcontrol = ++speedcontrol%3;
	speed = speeddata[ speedcontrol ];
	message( 3 , speedmsg[ speedcontrol ] );
	sleep( 1 );
	message( BLACK , speedmsg[ speedcontrol ] );
}

/* show the programmers of the game in Chinese */
void word( void )
{

	int page = 1;
	unsigned name_size , wall_size , i , j , k;
	void *name_buff[4] , *wall_buff;
	char *name = "cube.wrd";
	FILE *fp_name;

	hide_mouse();
	palette( 1 );
	wall_size = 26408;
	name_size = 74272/4;
	for( i=0 ; i<4 ; ++i )
	   if( ( name_buff[i] = malloc( name_size ) )==NULL )
		mem_error();
	if( ( wall_buff = malloc( wall_size ) )==NULL )
		mem_error();

	if( (fp_name = fopen( name , "rb" ))==NULL )
		file_error( name );
	for( i=0 ; i<4 ; ++i )
		fread( name_buff[i] , name_size , 1 , fp_name );
	fread( wall_buff , wall_size , 1 , fp_name );
	fclose( fp_name );

	setvisualpage( 0 );
	for( j=0 ; j<4 ; ++j )
	{
		if( kbhit() )
			break;
		for( i=0 ; i<90 ; i+=2 )
		{
			if( kbhit() )
				break;
			setactivepage( page );
			cleardevice();
			putimage( 76 , 210-i , name_buff[j] , COPY_PUT );
			putimage( 48 , 210 , wall_buff , COPY_PUT );
			setvisualpage( page );
			page = ( page == 0 )? 1 : 0;
		}
		if( i==90 )
			for( i=0 ; i<8 ; ++i )
			{
				setactivepage( page );
				outtextxy( 400 , 120 , "  " );
				setvisualpage( page );
				page = ( page == 0 ) ? 1 : 0;
				delay( 80 );
			}
		if( j==3 )
		{
			setactivepage( page );
			cleardevice();
			putimage( 76 , 131 , name_buff[3] , COPY_PUT );
			putimage( 48 , 210 , wall_buff , COPY_PUT );
			setvisualpage( page );
			for( i=0 ; i<544 ; ++i )
			{
				setcolor( GREEN );
				line( 48 , 216 , 48+i , 216 );
				line( 48 , 217 , 48+i , 217 );
				delay( 1 );
			}
		}
	}
	getch();
	for( i=0 ; i<4 ; ++i )
		free( name_buff[i] );
	free( wall_buff );
	show_mouse();
}

void cubesound(void)
{
	extern soundflag;

	if(soundflag)
	{
	   sound(700);
	   delay(20);
	   nosound();
	}
}
