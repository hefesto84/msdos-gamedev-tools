  extern void wsleep(unsigned);
  extern void wsound(unsigned,unsigned);

  extern void qfill(int, int, int, int, int, unsigned char);
  extern void qattr(int, int, int, int, int);
  extern void qfillc(int, int, int,int, int, int, unsigned char);
  extern void qattrc(int, int, int, int, int, int);

  extern void qstore(int, int, int, int, char *);
  extern void qrestore(int, int, int, int, char *);

  extern void qwritelv(int,int,int,int, char *);
  extern void qwrite(int,int,int, ...);
  extern void qwritec(int,int,int,int, char *);

  extern void gotorc(int,int);
  extern int cursorchange(int);
  extern char where_col(void);
  extern char where_row(void);
  extern void window_bios_scroll(int,int,int,int,int);
  extern int egacheck(int);
  extern int get_video_mode(void);
  extern void qpage(int);
  extern void qwritepage(int);

  extern void makewindow(int,int,int,int,int,int,enum BORDERS);
  extern void removewindow(void);
  extern void remove_windows(int);
  extern void remove_all_windows(void);
  extern void titlewindow(enum DIRTYPE, char *, int);
  extern void scrollwindow(int,int,enum DIRTYPE);
  extern void initwindow(int,int);
  extern int wiattr(int,int);
  extern void qbox(int,int,int,int,int,int,enum BORDERS);
  extern void qinit(void);

  extern int qwritef(int,int,int,char *,...);
  extern void vwrite(int,int,int,char * );
  extern int window_printf(int,char *,...);
  extern void window_write(int,char *);
  extern int getkey(int*);

  extern char *get_string(char *,int,int,int,int,int,int,int,char *,char,int *);
  extern int get_integer(int, int,int, int, int, int, char * , char * , int *);
  extern int get_menu(int, int, int, int, int, int, int, char *[], char *, int, int, enum BORDERS, enum DIRTYPE, int *, int, int (*)(int,int*));

#ifdef USE_LOCAL
  unsigned page0seg = 0xB800;
  unsigned qseg = 0xB800;
  int maxpage = 0;
  int cardwait = 1;
  int qwait = 1;
  int zoomeffect = 0;
  int zoomdelay = 11;
  int oldcursor = 8192;
  int active_window_row = 0;
  int active_window_col = 0;
  enum DIRTYPE shadoweffect = nodir;
#else
  extern unsigned page0seg,qseg;
  extern int maxpage,cardwait,qwait;
  extern int zoomeffect;
  extern int zoomdelay;
  extern int oldcursor;
  extern int active_window_col;
  extern int active_window_row;
  extern enum DIRTYPE shadoweffect;
#endif

