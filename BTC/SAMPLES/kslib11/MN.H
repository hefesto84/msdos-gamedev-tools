/*
*  MN.H
*/

#define CENTRE		0x0001
#define RIGHT		0x0002
#define	AUTOEXIT	0x0004
#define MAYBEXIT	0x0008

#define MAXOPT 20

typedef struct MNitem MNitem;
typedef struct MNdata MNdata;

struct MNitem {
	char	*desc;
	struct	MNdata	*menu;
	void	(*func)();
};

struct MNdata {
	uint	row,col;
	uint	wid,hei;
	uint	attr;
	char	*header;
	MNitem	option[MAXOPT];
};

uchar	MNmenu(MNdata *);
uchar	MNselect(uint,uint,int,int,int *,void (*func)(uint,uint));
void	MNcolor(uint,uint);
void	MNshowitem(uint,uint);
