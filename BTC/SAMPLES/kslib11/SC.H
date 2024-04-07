
struct SCdata {
	char	type;
	int		len;
	int		dec;
	int		x;
	int		y;
	int		tabdes;
	int		btabdes;
	void	*val;
	int		(*valid)(void);
};

typedef struct SCdata SCdata;

int		SCnf(SCdata *);
uchar	SCgetscreen(SCdata *,int *);
int		SCclear(SCdata *);
int		SCdisplay(SCdata *);
uchar	SCgetfield(SCdata *);
int		SCshow(SCdata *);
