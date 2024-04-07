/*
	ProWindows *LITE* - Version 2.00TC
	Copyright 1988  Daniel P. Stasinski

	THIS IS NOT PUBLIC DOMAIN.  This is ShareWare.  You are allowed
	30 days to examine this program, after which time you are required
	to pay a one time licensing fee of $7.50 to:

		Daniel P. Stasinski
		30 North Street
		Willits, CA 95490

	Read the accompanying documentation for full information.
*/

#define		ON		1
#define		OFF		0
#define		CLICK		1
#define		BEEP		2
#define		GLISANDO	3
#define		V_BW80		2
#define		V_CO80		3
#define		V_MONO		7
#define		V_INVALID	0xff

typedef struct {
	int		row;
	int		col;
	int		rows;
	int		cols;
	int		attribute;
	int		border;
	int		shadow;
	int		zoom;
	char	far	*data;
	int		abytes;
	int		cbytes;
} wcb;	/* Window Control Block */


/* ***** GLOBAL VARIABLES ***** */

extern	wcb		window[100];
extern	int		WI;
extern	int		CREATEMODE;
extern	int		SNOW;
extern	int		SCREEN_MODE;
extern	int		SOUND_MODE;

/* ***** FUNCTION PROTOTYPES ***** */

extern int  pascal attr (int, int);
extern void pascal initpro (int, int, void (*)(), int);
extern void pascal popwindow (int, int, int, int, int, int, int, int);
extern void pascal removewindow (void);
extern void pascal clearwindow (void);
extern void pascal titlewindow (int, char *);
extern void pascal wprint (int, int, char *);		/* default color		*/
extern void pascal cwprint (int, int, int, char *);	/* custom color			*/
extern void pascal wcprint (int, char *);		/* default color - center	*/
extern void pascal cwcprint (int, int, char *);		/* custom color - center	*/
extern void pascal copyright (int, int);		/* MANDITORY			*/
