#if __STDC__
#define _Cdecl
#else
#define _Cdecl  cdecl
#endif

#define RND_LIN 0
#define RND_ADD 1

void    _Cdecl  setseed(long s);
long    _Cdecl  seed(void);
long    _Cdecl  ticks(void);
long    _Cdecl  rndmize(void);
void    _Cdecl  rndpick(int f);
long    _Cdecl  lrnd(void);
int     _Cdecl  rnd(int n);
double  _Cdecl  drnd(void);
void    _Cdecl  shuffle(int *a, int n);
