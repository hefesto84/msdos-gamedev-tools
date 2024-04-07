/*****************************************
                MGABRA.H
       Include in your source!
*****************************************/

int far cdecl swping(void);
int far cdecl clear(void);
int far cdecl swpscr(int mode);
int far cdecl hotkey(int keyval);
int far cdecl timer(int dosticks, int tsrticks);
int far cdecl swap(void);
int far cdecl tsrset(int key, int dosticks, int tsrticks, int mem, int scswap, int int28);
int far cdecl swapno(void);
int far cdecl swapyes(void);
int far cdecl status(int id);
int far cdecl id(int id);
int far * cdecl far firstsr();
int far * cdecl far nextsr();
int far cdecl procst(int far *f);
int far cdecl procid(int far *f);
int far cdecl tsrs(void);
int far cdecl un(int far *f);
int far * cdecl far check(int id);
int far cdecl putchr(int row, int col, int page, int att, char c);
int far cdecl conout(char far *s);
int far cdecl inkey(void);
int far cdecl bykey(void);

/*****************************************
                END OF MGABRA.H
*****************************************/

