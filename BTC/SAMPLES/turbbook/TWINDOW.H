/* ------------------- twindow.h ----------------------- */

/*      Uncomment this for stacked windows 
 *      rather than layered windows.
 *
 * #define FASTWINDOWS
 *
 */

/* ------ window colors ---------- */
#define RED   4
#define GREEN 2
#define BLUE  1
#define WHITE (RED+GREEN+BLUE)
#define YELLOW (RED+GREEN)
#define AQUA (GREEN+BLUE)
#define MAGENTA (RED+BLUE)
#define BLACK 0
#define BRIGHT 8
#define DIM 0

#define BORDER 0
#define TITLE 1
#define ACCENT 2
#define NORMAL 3
#define ALL 4

#define TRUE 1
#define FALSE 0
#define ERROR -1
#define OK 0
/*page*/
/* ------------- window controller structures ----------- */
typedef struct field {	/* data entry field description	*/
	char *fmask;		/* field data entry mask		*/
	char *fprot;		/* field protection				*/
	char *fbuff;		/* field buffer					*/
	int ftype;			/* field type					*/
	int frow;			/* field row					*/
	int fcol;			/* field column					*/
	void (*fhelp)();	/* field help function			*/
	char *fhwin;		/* field help window			*/
	int flx, fly;		/* help window location			*/
	int (*fvalid)();	/* field validation function	*/
	struct field *fnxt;	/* next field on template 		*/
	struct field *fprv;	/* previous field on template 	*/
} FIELD;
typedef struct _wnd {
	int _wv;		  /* true if window is visible     */
	int _hd;		  /* true if window was hidden     */
	char *_ws;		  /* points to window save block   */
	char *_tl;		  /* points to window title		   */
	int _wx;		  /* nw x coordinate			   */
	int _wy;		  /* nw y coordinate			   */
	int _ww;		  /* window width				   */
	int _wh;		  /* window height			       */
	int _wsp;		  /* scroll pointer			       */
	int _sp;		  /* selection pointer		       */
	int _cr;		  /* cursor x location		       */
	int btype;		  /* border type				   */
	int wcolor[4];	  /* colors for window 			   */
	int _pn;		  /* previous normal color		   */
	struct _wnd *_nx; /* points to next window         */
	struct _wnd *_pv; /* points to previous window     */
	FIELD *_fh;		  /* points to 1st data entry fld  */	
	FIELD *_ft;		  /* points to last data entry fld */	
} WINDOW;
typedef struct w_menu {
	char *mname;
	char **mselcs;
	void (**func)();
} MENU;

#define SAV 	(wnd->_ws)
#define WTITLE  (wnd->_tl)
#define COL 	(wnd->_wx)
#define ROW 	(wnd->_wy)
#define WIDTH 	(wnd->_ww)
#define HEIGHT 	(wnd->_wh)
#define SCROLL  (wnd->_wsp)
#define SELECT  (wnd->_sp)
#define WCURS   (wnd->_cr)
#define WBORDER	(wnd->wcolor[BORDER])
#define WTITLEC	(wnd->wcolor[TITLE])
#define WACCENT	(wnd->wcolor[ACCENT])
#define WNORMAL	(wnd->wcolor[NORMAL])
#define PNORMAL (wnd->_pn)
#define BTYPE	(wnd->btype)
#define NEXT    (wnd->_nx)
#define PREV    (wnd->_pv)
#define WCOLOR  (wnd->wcolor)
#define VISIBLE (wnd->_wv)
#define HIDDEN  (wnd->_hd)
#define FHEAD	(wnd->_fh)
#define FTAIL   (wnd->_ft)

#define NW   	(wcs[wnd->btype].nw)
#define NE   	(wcs[wnd->btype].ne)
#define SE   	(wcs[wnd->btype].se)
#define SW   	(wcs[wnd->btype].sw)
#define SIDE 	(wcs[wnd->btype].side)
#define LINE 	(wcs[wnd->btype].line)
/*page*/
/* -------- function prototypes and macros -------- */

/* ------ general-purpose functions and macros ----- */
void clear_screen(void);
int vmode(void);
void cursor(int, int);
void curr_cursor(int *, int *);
int cursor_type(void);
void set_cursor_type(int);
int get_char(void);
int scroll_lock(void);
void vpoke(unsigned, unsigned, unsigned);
int vpeek(unsigned, unsigned);

/* ----- window functions and macros ------- */
WINDOW *establish_window(int, int, int, int);
void set_border(WINDOW *, int);
void set_colors(WINDOW *, int, int, int, int);
void set_intensity(WINDOW *, int);
void set_title(WINDOW *, char *);
void display_window(WINDOW *);
void delete_window(WINDOW *);
void clear_window(WINDOW *);
void hide_window(WINDOW *);
void wprintf(WINDOW *, char *, ...);
void wputchar(WINDOW *, int);
void close_all(void);
void wcursor(WINDOW *, int x, int y);
void error_message(char *);
void clear_message(void);
int get_selection(WINDOW *, int, char *);

#define reverse_video(wnd) wnd->wcolor[3]=wnd->wcolor[2]
#define normal_video(wnd) wnd->wcolor[3]=wnd->_pn
#define rmove_window(wnd,x,y) repos_wnd(wnd, x, y, 0)
#define move_window(wnd,x,y) repos_wnd(wnd, COL-x, ROW-y, 0)
#define forefront(wnd) repos_wnd(wnd, 0, 0, 1)
#define rear_window(wnd) repos_wnd(wnd, 0, 0, -1)
/*page*/
/* ----- internal to window processes ----- */
void accent(WINDOW *);
void deaccent(WINDOW *);
void scroll(WINDOW *, int);
void repos_wnd(WINDOW *, int, int, int);
void acline(WINDOW *, int);
#define accent(wnd)	acline(wnd, WACCENT)
#define deaccent(wnd) acline(wnd, WNORMAL)
#define clr(bg,fg,in) ((fg)|(bg<<4)|(in))
#define vad(x,y) ((y)*160+(x)*2)
#ifdef FASTWINDOWS
#define cht(ch,at) ((ch&255)|(at<<8))
#define displ(w,x,y,c,a) vpoke(VSG,vad(x+COL,y+ROW),cht((c),(a)))
#define dget(w,x,y) vpeek(VSG,vad(x+COL,y+ROW))
#define verify_wnd(w) (*(w)=listtail)!=0
#else
void displ(WINDOW *wnd, int x, int y, int ch, int at);
#endif
/* ------ editor function ------- */
void text_editor(WINDOW *, char *, int);
/* -------- menu function ------- */
void menu_select(char *name, MENU *mn);
/* ----- help functions ------- */
void load_help(char *);
void set_help(char *, int, int);
/* ----- data entry functions ---- */
void init_template(WINDOW *);
FIELD *establish_field(WINDOW *,int,int,char *,char *,int);
void clear_template(WINDOW *);
void field_tally(WINDOW *);
int data_entry(WINDOW *);
void wprompt(WINDOW *, int, int, char *);
void error_message(char *);
void clear_notice(void);
void field_window(FIELD *, char *, int, int);
#define field_protect(f,s)  f->fprot=s
#define field_help(f,h)     f->fhelp=h
#define field_validate(f,v) f->fvalid=v


