/*
*	WN.H - Header file for WN.C
*/

#define SINGLE	0x0001
#define DOUBLE	0x0002
#define CURSON	0x0004
#define POPUP	0x0008

#define WNCHECKINIT if (!WNinitialised) WNinit()

#define BKGR(x) (x<<4)
#define WNclear() clrscr()
#define WNgotoxy(x,y) gotoxy(x,y)
#define WNgetx wherex
#define WNgety wherey
#define WNputs cputs
#define WNprintf cprintf

struct WNwin {
	int		x;
	int		y;
	int		c;
	int		curs;
	int		oulx,ulx;
	int		ouly,uly;
	int		olrx,lrx;
	int		olry,lry;
	int		attr;
	char	*buf;
	struct	WNwin	*last;
	struct	WNwin	*next;
};

typedef struct WNwin WNwin;

void		WNsavecurs(void);
void		WNloadcurs(void);
void		WNputc(char);
void		WNsavedos(void);
void		WNloaddos(void);
WNwin *		WNopen(int,int,int,int,int,int,char *);
WNwin *		WNclose(void);
void		WNinit(void);
void		WNend(void);
WNwin *		WNselect(WNwin *);
void		WNsave(void);
void		WNload(void);
void		WNbox(int,int,int,int,int);
void		WNmessage(WNwin *,int,int,int,char *);
void		WNdisplay(int,int,char *);
int			WNcolor(int);
void		WNfore(int);
void		WNback(int);
void		WNcursor(int);
int			WNgcursor(void);
void		WNhline(int,int,int,int);
void		WNvline(int,int,int,int);
