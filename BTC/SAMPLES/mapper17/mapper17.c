/*
 *	mapper.c
 *
 *	Version 1.7 by Steven R. Sampson, November 1988
 *
 *	Based on a program and article by William D. Johnston
 *	Copyright (c) May-June 1979 BYTE, All Rights Reserved
 *
 *	This program draws three types of map projections:
 *	Perspective, Modified Perspective, and Azimuthal Equidistant.
 *
 *	Compiled with Turbo-C V1.5
 */

#include <dos.h>
#include <math.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <graphics.h>

typedef int	bool;

/* Program Constants */

#define	FALSE	(bool) 0
#define	TRUE	(bool) ~FALSE

#define	PI	(3.141593F)
#define	HALFPI	(1.570796F)
#define	TWOPI	(2.0F * PI)		/* Two Pi alias 360 Degrees	     */

#define	RADIAN	(180.0F / PI )		/* One radian			     */
#define	TWO	(2.0F / RADIAN)		/* 2 degrees in radians		     */
#define	TEN	(10.0F / RADIAN)	/* 10 degrees in radians	     */
#define	EIGHTY	(80.0F / RADIAN)	/* 80 degrees in radians	     */
#define	EARTH	(6378.0F)		/* Mean radius of earth (Kilometers) */

/* Program Globals */

FILE	*fp;

float	angle, maxplot, center_lat, center_lon, lat, lon, distance,
	sin_of_distance, cos_of_distance, sin_of_center_lat, cos_of_center_lat,
	scale, g, h2, facing_azimuth, aspect;

int	option, center_x, center_y, grid_color, level = 5;
int	GraphDriver = DETECT, GraphMode;

char	optstring[] = "bcd:gilm:rsx?";
char	database[128] = "mwdbii";	/* default name 'MWDBII'	     */
					/* leave room for pathname!	     */
bool	boundaries = TRUE,		/* defaults to Boundaries, Islands   */
	countries  = FALSE,
	grids      = FALSE,
	islands    = TRUE,
	lakes      = FALSE,
	rivers     = FALSE,
	states     = FALSE,
	colors     = FALSE;

/* Forward Declarations, Prototypes */

extern	int	getopt(int, char **, char *);
extern	int	optind, opterr;
extern	char	*optarg;

float	parse(char *);
void	grid(void), plotmap(void), prompts(void), quit(void);
bool	compute(float *, float *, int *, int *);


main(argc, argv)
int	argc;
char	*argv[];
{
	register int	i;
	int		err, xasp, yasp;

	registerbgidriver(EGAVGA_driver);
	registerbgidriver(CGA_driver);

	setcbrk(TRUE);		/* Allow Control-C checking		     */
	ctrlbrk(quit);		/* Execute quit() if Control-C detected	     */

	while ((i = getopt(argc, argv, optstring)) != -1)  {
		switch (i)  {
		case 'b':
			boundaries = FALSE;
			break;
		case 'c':
			countries = TRUE;
			break;
		case 'd':
			strcpy(database, optarg);
			break;
		case 'g':
			grids = TRUE;
			break;
		case 'i':
			islands = FALSE;
			break;
		case 'l':
			lakes = TRUE;
			break;
		case 'm':
			level = atoi(optarg);
			break;
		case 'r':
			rivers = TRUE;
			break;
		case 's':
			states = TRUE;
			break;
		case 'x':
			colors = FALSE;
			break;
		case '?':
		default:
		      printf("Usage: mapper [/bcdgilmrsx]\n\n");
		      printf("  /b   Boundaries Off\n");
		      printf("  /c   Countries On\n");
		      printf("  /dn  Database ('MWDBII' Default)\n");
		      printf("  /g   Grid lines On\n");
		      printf("  /i   Islands Off\n");
		      printf("  /l   Lakes On\n");
		      printf("  /mn  Map Resolution (5 Default)\n");
		      printf("  /r   Rivers On\n");
		      printf("  /s   States On\n");
		      printf("  /x   Colors On\n\n");
		      printf("Defaults to Boundaries and Islands On\n");
		      exit(0);
		}
	}

	if ((fp = fopen(database, "rb")) == (FILE *)NULL)  {
		printf("\007Error: Can't locate Database '%s'\n", database);
		exit(1);
	}

	initgraph(&GraphDriver, &GraphMode, "");/* initialize graphics	     */
	err = graphresult();

	restorecrtmode();			/* get back to text mode     */

	if (err < 0)  {
		printf("Graphics Error - %s\n", grapherrormsg(err));
		exit(-1);
	}

	center_x = getmaxx() / 2;		/* get screen size for x, y  */
	center_y = getmaxy() / 2;
	getaspectratio(&xasp, &yasp);		/* squish factor for y axis  */
	aspect = (float)xasp / (float)yasp;

	prompts();				/* get the basic map info    */
	setgraphmode(GraphMode);		/*  and go to graphics mode  */

	if (GraphMode != CGAHI)  {
		setbkcolor(BLACK);		/* must be EGA or VGA then   */
		if (colors)
			grid_color = EGA_LIGHTRED;
		else
			grid_color = EGA_LIGHTGRAY;
	} else
		grid_color = LIGHTGRAY;		/* CGA only has two colors   */

	setcolor(LIGHTGRAY);

	/*
	 *	See if data plotting is even needed
	 */

	if (boundaries || countries || islands || lakes || rivers || states)
		plotmap();			/* display map on screen     */

	if (grids)
		grid();				/* draw lat & long ref lines */

	if (print)
		printscreen();			/* relay screen to printer   */

	sound(800);				/* 800 Hz for 1/4 a second   */
	delay(125);
	nosound();

	getch();				/* pause until key pressed   */
	closegraph();				/* graphics off		     */
	fclose(fp);				/* close database file	     */

	exit(0);
}

/*
 *	Return to operator following Control-C
 */

void quit()
{
	closegraph();
	fclose(fp);

	exit(0);
}

/*
 *	Operator prompts and input.
 */

void prompts()
{
	char	temp[16];
	float	ret, altitude;

	printf("West Longitudes and South Lattitudes are negative\n");

	/* input the world Lat & Long that is to be centered on */
	/*   then convert the human notation to radians         */

	do  {
		printf("\nLatitude of the map center [+-]dd.mm : ");
		scanf("%s", temp);
		ret = parse(temp);
	} while (ret > 90.0F || ret < -90.0F);

	/* the arcosine function has trouble at 90 degrees */

	if (ret == 90.0F)
		ret = 89.9F;

	if (ret == -90.0F)
		ret = -89.9F;

	center_lat = ret / RADIAN;
	sin_of_center_lat = sin(center_lat);
	cos_of_center_lat = cos(center_lat);

	do  {
		printf("Longitude of the map center [+-]ddd.mm : ");
		scanf("%s", temp);
		ret = parse(temp);
	} while (ret > 180.0F || ret < -180.0F);

	center_lon = ret / RADIAN;

	do  {
		printf("\nSelect from the following options:\n\n");
		printf("  1 - Perspective Projection\n");
		printf("  2 - Modified Perspective Projection\n");
		printf("  3 - Azimuthal Equidistant Projection\n\n");
		printf("Choice : ");
		scanf("%d", &option);
	} while (option < 1 || option > 3);

	if (option == 3)  {
		maxplot = PI;		/* use HALFPI for less area	    */
		scale = (float)center_y / maxplot;
		return;
	}

	/* input the height above the terrain */

	printf("\nObserver altitude (km) : ");
	scanf("%f", &altitude);

	h2 = EARTH + altitude;
	maxplot = acos(EARTH / h2);

	/* the operator can orient the world upside down if they want */

	do  {
		printf("Observer facing azimuth (0 - 359 degrees) : ");
		scanf("%f", &facing_azimuth);
	} while (facing_azimuth < 0.0F || facing_azimuth > 360.0F);

	facing_azimuth = -facing_azimuth / RADIAN;

	/* calculate the scale for the polar coordinates */

	scale = (float)center_y / (EARTH * sin(maxplot));

	/* for the perspective projection */

	g = EARTH * (h2 - EARTH * cos(maxplot));
}


/*
 *	Convert the database to the desired projection by computation.
 *
 *	This code is a hand translation from BASIC to C based on Mr. Johnstons
 *	code.  It is a non-mathematicians idea of what he meant.
 *
 *	Return TRUE if offscale else FALSE.
 */

bool compute(lat, lon, x, y)
register float	*lat, *lon;
register int	*x, *y;
{
	float	sin_of_lat,
		cos_of_lat,
		abs_delta_lon,			/* absolute value	     */
		delta_lon,			/* x distance from center    */
		delta_lat,			/* y distance from center    */
		temp;				/* temporary storage	     */

	/* normalize the longitude to +/- PI */

	delta_lon = *lon - center_lon;

	if (delta_lon < -PI)
		delta_lon = delta_lon + TWOPI;

	if (delta_lon > PI)
		delta_lon = delta_lon - TWOPI;

	abs_delta_lon = fabs(delta_lon);

	/*
	 *	If the delta_lon is within .00015 radians of 0 then
	 *	the difference is considered exactly zero.
	 *
	 *	This also simplifys the great circle bearing calculation.
	 */

	if (abs_delta_lon <= 0.00015F)  {
		delta_lat = fabs(center_lat - *lat);

		if (delta_lat > maxplot)
			return TRUE;		/* offscale		     */

		if (*lat < center_lat)
			angle = PI;
		else
			angle = 0.0F;

		sin_of_distance = sin(delta_lat);
		cos_of_distance = cos(delta_lat);
	}

	/*
	 *	Check if delta_lon is within .00015 radians of PI.
	 */

	else if (fabs(PI - abs_delta_lon) <= 0.00015F)  {
		delta_lat = PI - center_lat - *lat;

		if (delta_lat > PI)  {
			delta_lat = TWOPI - delta_lat;
			angle = PI;
		} else
			angle = 0.0F;

		if (delta_lat > maxplot)
			return TRUE;		/* offscale		     */

		sin_of_distance = sin(delta_lat);
		cos_of_distance = cos(delta_lat);
	}

	/*
	 *	Simple calculations are out, now get cosmic.
	 */

	else  {
		sin_of_lat = sin(*lat);
		cos_of_lat = cos(*lat);

		cos_of_distance = sin_of_center_lat * sin_of_lat +
				    cos_of_center_lat * cos_of_lat *
				      cos(delta_lon);

		distance = acos(cos_of_distance);

		if (distance > maxplot)
			return TRUE;		/* offscale		     */

		sin_of_distance = sin(distance);

		temp = (sin_of_lat - sin_of_center_lat * cos_of_distance) /
			(cos_of_center_lat * sin_of_distance);

		if (temp < -1.0F || temp > 1.0F)
			return TRUE;		/* offscale		     */

		angle = acos(temp);

		if (delta_lon < 0.0F)
			angle = TWOPI - angle;
	}

	if (facing_azimuth != 0.0F)  {
		angle = angle - facing_azimuth;
		if (angle < 0.0F)
			angle = TWOPI + angle;
	}

	angle = HALFPI - angle;

	if (angle < -PI)
		angle = angle + TWOPI;

	switch (option)  {
	case 1:
		temp  = (scale * (g * sin_of_distance)) /
				(h2 - EARTH * cos_of_distance);
		break;
	case 2:
		temp = scale * EARTH * sin_of_distance;
		break;
	case 3:
		temp = scale * distance;
	}

	/* convert polar to rectangular, correct for screen aspect */

	*x = center_x + (int)(temp * cos(angle));
	*y = center_y - (int)(temp * sin(angle) * aspect);

	return FALSE;
}

/*
 *	Read the database and plot points or lines.
 *
 *	The database is Micro World Data Bank II.  It's based on the
 *	CIA WDB-II tape available from NTIS.  Micro WDB-II was created
 *	by Micro Doc.  Placed in the public domain by Fred Pospeschil
 *	and Antonio Riveria.  Check on availability at:
 *	1-402-291-0795  (6-9 PM Central)
 *
 *	Austin Code Works has something called: The World Digitized
 *	that sounds like the same thing ($30.00), 1-512-258-0785
 *
 *	Lone Star Software has something called: The World Digitized
 *	that sounds like the same thing ($6.00), 1-800-445-6172.
 *
 *	Database is in Intel word order:
 *	code_lsb, code_msb, lat_lsb, lat_msb, lon_lsb, lon_msb
 *
 *	Code:	Integer, two meanings:
 *		1.  Detail Level (1 Highest - 5 Lowest)
 *
 *		2.  Header (1xxx - 7xxx)	Command Line Options
 *
 *			1xxx	Boundaries		/b
 *			2xxx	Countries		/c
 *	(decimal)	4xxx	States			/s
 *			5xxx	Islands			/i
 *			6xxx	Lakes			/l
 *			7xxx	Rivers			/r
 *
 *	Lat & Long:  Integer
 *		Representing Minutes of degree
 */

void plotmap()
{
	struct	{ short code, lat, lon; } coord;
	float	lat, lon;
	int	x, y;
	bool	point;

	point = TRUE;
	while (fread(&coord, sizeof coord, 1, fp) > 0)  {

		if (kbhit())  {
			grids = print = FALSE;
			getch();
			return;
		}
			
		/*
		 *	Skip data that has been optioned out.
		 */

		if (coord.code < level)
			continue;

		if (coord.code > 5)  {		/* must be a header	     */

			point = TRUE;

			switch (coord.code / 1000)  {
			case 1:
				if (boundaries)  {
					if (colors)
						setcolor(EGA_LIGHTGRAY);
					break;
				}
				else
					continue;
			case 2:
				if (countries)  {
					if (colors)
						setcolor(EGA_BROWN);
					break;
				}
				else
					continue;
			case 4:
				if (states)  {
					if (colors)
						setcolor(EGA_BROWN);
					break;
				}
				else
					continue;
			case 5:
				if (islands)  {
					if (colors)
						setcolor(EGA_LIGHTGRAY);
					break;
				}
				else
					continue;
			case 6:
				if (lakes)  {
					if (colors)
						setcolor(EGA_BLUE);
					break;
				}
				else
					continue;
			case 7:
				if (rivers)  {
					if (colors)
						setcolor(EGA_GREEN);
					break;
				}
				else
					continue;
			}
		}

		/*  Convert database minutes of a degree to radians */

		lat =  (float) coord.lat / 60.0F / RADIAN;
		lon =  (float) coord.lon / 60.0F / RADIAN;

		if (compute(&lat, &lon, &x, &y))  {
			point = TRUE;		/* offscale		     */
			continue;
		}

		if (point)  {
			putpixel(x, y, getcolor());/* put down a dot	     */
			moveto(x, y);
			point = FALSE;
		}
		else
			lineto(x, y);		/* connect the dots	     */
	}
}

/*
 *	parse +-ddd.mm
 *
 *	Change human degrees, and minutes to computer decimal.
 *	Probably designed a monster for a simple solution here...
 */

float parse(string)
char	*string;
{
	char	*ptr, degrees[8], minutes[8];
	float	num;

	strcpy(degrees, "       ");		/* pre-load with blanks      */
	strcpy(minutes, "       ");

	/* if no decimal point we assume a whole number */

	if ( (ptr = strchr(string, '.')) == (char *)NULL )
		return atof(string);

	/* else use the decimal point to offset */

	*ptr++ = '\0';

	strcpy(degrees, string);
	num = atof(degrees);

	switch (strlen(ptr))  {
	case 0:
		return atof(string);
	case 1:
	case 2:
		strcpy(minutes, ptr);
		break;
	default:
		return 361.0F;	/* This will produce an error		     */
	}

	if (num >= 0.0F)
		num += atof(minutes) / 60.0F;
	else
		num -= atof(minutes) / 60.0F;

	return num;
}


/*
 *	Draw grid lines from -180 to +180 Degrees (Longitude Lines),
 *	as well as +80 to -80 Degrees (Lattitude Lines).
 */

void grid()
{
	float	lat, lon;
	int	x, y, pass1;

	setcolor(grid_color);

	for (lon = -PI; lon <= PI; lon += TEN)  {
		pass1 = TRUE;
		for (lat = EIGHTY; lat > -EIGHTY; lat -= TEN)  {
			if (!compute(&lat, &lon, &x, &y))  {
				if (pass1)  {
					putpixel(x, y, grid_color);
					moveto(x, y);
					pass1 = FALSE;
				} else
					lineto(x, y);
			} else
				pass1 = TRUE;
		}

		if (kbhit())  {
			print = FALSE;
			getch();
			return;
		}
	}

	for (lat = EIGHTY; lat > -EIGHTY; lat -= TEN)  {
		pass1 = TRUE;
		for (lon = -PI; lon <= PI; lon += TEN)  {
			if (!compute(&lat, &lon, &x, &y))  {
				if (pass1)  {
					putpixel(x, y, grid_color);
					moveto(x, y);
					pass1 = FALSE;
				} else
					lineto(x, y);
			} else
				pass1 = TRUE;

		}

		if (kbhit())  {
			print = FALSE;
			getch();
			return;
		}
	}
}

/* EOF */
