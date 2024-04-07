#define MAXWNDW 30

enum BORDERS {  nobrdr,blankbrdr,singlebrdr,doublebrdr,mixedbrdr,solidbrdr,
                evensolidbrdr,thinsolidbrdr,lhatchbrdr,mhatchbrdr,
                hhatchbrdr,userbrdr
             };
enum DIRTYPE {  nodir,up,down,cleartop,clearbottom,clearleftside,clearrightside,
                topleft,topright,topcenter,bottomleft,bottomright,bottomcenter,
                leftside,rightside,left,right
             };

typedef struct {
                 unsigned char tl[2], th, tr[2], lv, rv, bl[2], bh, br[2];
               }  BRDRREC;
typedef struct {
                 int     wsrow, wscol, wsrows, wscols, wswattr, wsbattr;
                 enum BORDERS  wsbrdr;
                 enum DIRTYPE  wsshadow;
                 int     wslastx, wslasty;
               }  wndwstattype;

typedef char         *byteptr;
typedef char         str160[160];
