#define MI	5

struct	DBfile {
	int		rs;							/* Record size (bytes)				*/
	int		rn;							/* Current record number			*/
	int		nr;							/* Number of records				*/
	void	*rec;						/* Pointer to temporary record		*/
	FILE	*fp;						/* Pointer to database file handle	*/
	FILE	*ifp;						/* Pointer to index file handle		*/
	int		(*ifunc)(void *,void *);	/* Pointer to index function		*/
	int		inr;						/* Number of records in index file	*/
	struct	DBfile *last;				/* Pointer to last structure		*/
	struct	DBfile *next;				/* Pointer to next structure		*/
};

typedef struct DBfile DBfile;

DBfile	*DBopen(char *,int,char *,int (*)());
void	DBend(void);
void	DBselect(DBfile *);
int		DBindex(char *,int (*)(void *,void *));
int		DBsetindex(int);
DBfile	*DBclose(void);
int		DBcmp(void *,void *);
int		DBcopy(void *,void *);
int		DBread(void *);
int		DBgo(int);
int		DBadd(void *);
int		DBwrite(void *);
int		DBreadn(int,void *);
int		DBiread(int);
int		DBiwrite(int,int);
int		DBrecno(void);
int		DBskip(int);
int		DBdelete(void);
int		DBfindfirst(void *);
int		DBfindnext(void *);
int		DBseekfirst(void *,int (*)());
int		DBseeknext(void *,int (*)());
int		DBtop(void);
int		DBbot(void);
int		DBvalid(void);