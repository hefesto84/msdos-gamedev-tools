#define CFCHECKINIT if (!CFinited) CFinit();

#define MAXCOLOR	15

struct	CFdata	{
	char	typ;
	char	*cmd;
	void	*arg;
};

typedef struct CFdata CFdata;

int		CFcolcvt(char *);
int		CFcolval(char *);
char	*CFcolor(int);
void	CFinit(void);
int		CFopen(char *);
int		CFread(char *,char *);
int		CFconf(char *,CFdata *);
int		CFwrite(char *,CFdata *);
