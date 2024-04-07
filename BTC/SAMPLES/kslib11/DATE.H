
struct datest {
	int		year;
	int		month;
	int		day;
};

typedef struct datest datest;

long	datelong(datest *);
datest	*longdate(datest *,long);
int		longdow(long);
int		monsum(int,int);
int		monlen(int,int);
char	*monthnam(int);
char	*monthname(int);
char	*daynam(int);
char	*dayname(int);
char	*daysuff(int);
int		leapyear(int);