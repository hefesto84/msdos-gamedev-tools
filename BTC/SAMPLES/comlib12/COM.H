/* prototypes */
int  ComStart(int);                  /* installation/initialization             */
void ComSetParms(int,int,int);       /* Set the Paramters baud, stop bits, etc. */
void ComHandShake(int,int);          /* Raise or lower DTR and RTS              */
void ComClrBuff(int);                /* Clear the internal input buffer         */
int  ComGetChar(int);                /* receive a serial character              */
int  WComGetChar(int,int);           /* wait max secs for character in buffer   */
int  WComGetStr(int,char *,int,int); /* get string user input from port         */
int  ComWaitStr(int,char *,int);     /* wait max secs for string in buffer      */
void ComPutChar(int, char);          /* send a character                        */
void ComPutStr(int, char *);         /* send a string                           */
void ComPutCrLf(int);                /* send a carriage return & linefeed       */
int  ComPutFile(int port, char *);   /* send a file                             */
int  ComChkCnt(int);                 /* input buffer character count            */ 
int  ComChkDsr(int);                 /* check data set ready                    */
int  ComChkCd(int);                  /* check  carrier dectect                  */
int  ComChkRi(int);                  /* check ring indicator                    */
int  ComChkCts(int);                  /* check clear to send                    */
void ComStop(int);                   /* exit cleanup routine                    */


#define S_EOF    -1       /* if no characters are available in buffer   */
#define FAILURE   0
#define SUCCESS   1

/* Values returned from ComChk?? functions */
#define LOW       0       
#define HIGH      1


/* Defines for Com Port Paramaters */
#define BITS_8          0x03
#define BITS_7          0x02
#define STOP_1          0x00
#define STOP_2          0x04
#define EVEN_PARITY     0x18
#define ODD_PARITY      0x08
#define NO_PARITY       0x00

/* Defines used for ComHandShake function  */
#define DTR             0x01
#define RTS             0x02
#define USER            0x04
#define LOOPBACK        0x10


/* More defines */
#define CR              0x0D
#define LF              0x0A




