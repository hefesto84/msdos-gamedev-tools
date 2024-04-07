                  /* Header File For Window Routines*/
                  /* copywrite (C)1987 by D.R.EVANS */


                         /* color defines */
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHTGRAY 7
#define DARKGRAY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define YELLOW 14
#define WHITE 15
#define BLINK 128
#define black BLACK
#define blue BLUE
#define green GREEN
#define cyan CYAN
#define red RED
#define magenta MAGENTA
#define brown BROWN
#define lightgray LIGHTGRAY
#define dark gray DARKGRAY
#define lightblue LIGHTBLUE
#define lightgreen LIGHTGREEN
#define lightcyan LIGHTCYAN
#define lightred LIGHTRED
#define lightmagenta LIGHTMAGENTA
#define yellow YELLOW
#define white WHITE
#define blink BLINK

                         /* var. defines */
#define TRUE 1
#define FALSE 0
#define ON 1
#define OFF 0
#define TOP 0
#define BOTTOM 1
#define LEFT 0
#define RIGHT 1
#define CENTER 2
#define UP 6
#define DOWN 7
#define HORNZ 0
#define VERT 1
#define MORE 0
#define PRET 1
#define MAXWIND 30 /* maximum number of windows allowed (depends on memory)*/
#define top TOP
#define bottom BOTTOM
#define left LEFT
#define right RIGHT
#define center CENTER
#define up UP
#define down DOWN
#define hornz HORNZ
#define vert VERT
#define more MORE
#define pret PRET
#define off OFF
#define on ON
#define nobrdr  0  /* no border */
#define dblbrdr 1  /* double border */
#define sglbrdr 2  /* single border */
#define dtpbrdr 3  /* double top & bottom, single sides */
#define dsdbrdr 4  /* doble sides, single top and bottom */
#define sldbrdr 5  /* solid border in color specified */
#define sglline 0  /* single line define for mline routine */
#define dblline 1  /* double line define for mline routine */
#define sldline 2  /* solid line define for mline routine */
#define NOBRDR nobrdr
#define DBLBRDR dblbrdr
#define SGLBRDR sglbrdr
#define DTPBRDR dtpbrdr
#define DSDBRDR dsdbrdr
#define SLDBRDR sldbrdr
#define SGLLINE sglline
#define DBLLINE dblline
#define SLDLINE sldline
#define color40 1 /* 40 col. color mode */
#define color80 3 /* 80 col. color mode */
#define COLOR40 color40
#define COLOR80 color80
#define BS 8
#define FORMFEED 12
#define CR 13
#define ESC 27
#define HOMEKEY 327
#define ENDKEY 335
#define UPKEY 328
#define DOWNKEY 336
#define PGUPKEY 329
#define PGDNKEY 337
#define LEFTKEY 331
#define INSKEY 338
#define RIGHTKEY 333
#define DELKEY 339
#define CTRLLEFTKEY 371
#define CTRLRIGHTKEY 372
#define F1 315
#define F2 316
#define F3 317
#define F4 318
#define F5 319
#define F6 320
#define F7 321
#define F8 322
#define F9 323
#define F10 324

                    /* function prototypes */

  /* setup window params. must be called before any of the functions below */
void initwindow(void);

void clrwind(unsigned char backgrd);
void clrscr(unsigned char backgrd, unsigned char border);
void screenmode(int mode);
void setcursor(char turnon);
void wtitle(unsigned char justify,unsigned char loc,
                 unsigned char color,char *header);
void mwind(int x1, int y1, int x2, int y2,
              int windcolor,int brdrcolor,int brdrsel);
void rwind(void);
void mbox(int x1, int y1, int x2, int y2, int brdrcolor,int brdrsel);
void mline(int col, int row, int length, int color, int linedir, int linetype);
void pwindf(int col, int row,int color, va_list arg_list, ...);
void pwindfc( int row, int color, va_list arg_list, ...);
void movwind(int newcol, int newrow);
void gowindcr( int col, int row);
void waitkey(int typeprint);
int keyin(void);
char attr(unsigned char foregrd,unsigned char backgrd);
char pullmenu(int loc, int menunum);
char popupmenu(int col, int row, char *windowstrptr, int btype);

/* borlands direct video write routine supplied with turbo C requires link to
   mcmvsmem.obj */
void far movescreenmem(char far *source, char far *dest, unsigned len,
 int snowcheck);


#if defined(MAIN)

char shadow;   /* TRUE= shadow to right of window */
char snow;     /* TRUE= slow screen writes eliminate snow */
int scrmode;   /* 40 or 80 cols. */
int rowend;    /* convieient variable to print at last row, causes scroll */
int bordercolor; /* current window border color */
int windowcolor; /* current window inside color */
int pullmenuloc; /* return loc in pull menu upon return */
unsigned char *pullmainmenu[7]; /* main menu pointers  */
unsigned char *pullsubmenu[7][7]; /* sub menu pointers */

#endif


/* These external variables are declared in windX.obj
   Default is :
               currentwindow = 0;
               plcolor       = attr(yellow,blue);
               plhicolor     = attr(white,black);
               pletcolor     = attr(cyan,blue);
               pbrdcolor     = attr(white,black);
*/
extern int currentwind; /* current active window */
extern int plcolor;     /* pull down menu main color */
extern int plhicolor;   /* pull down menu hi lited color */
extern int pletcolor;   /* pull down & popup menu letter selection color */
extern int pbrdcolor;   /* pull down menu boarder colors */
extern int ispullmenu;
extern int ispulled;


/* following variables are defined in the window object files. These Do not */
/* have to be defined outside of that file. Listed here for your information */

extern char border[6][7];
extern char colortable[128];
extern char colorcard;
extern char far *videoptr;
extern unsigned char far* scrsave[MAXWIND];
extern int windcolstart[MAXWIND], windcolend[MAXWIND];
extern int windrowstart[MAXWIND], windrowend[MAXWIND];
extern int windattr[MAXWIND];
extern int bordattr[MAXWIND];
extern int bordsel[MAXWIND];

