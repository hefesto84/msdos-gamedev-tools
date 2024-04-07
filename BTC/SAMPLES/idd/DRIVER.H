/*
 *	driver.h ->	Device Driver Header File
 */


/*
 *	Status Word Bits
 */
#define	Error	0x8000
#define	Busy	0x0100
#define	Done	0x0080


/*
 *	Media Descriptor Byte Bits
 */
#define	TwoSided	1
#define	EightSector	2
#define	Removable	4


/*
 *	Error Return Codes
 */
#define	WriteProtect	0
#define	UnknownUnit	1
#define	DeviceNotReady	2
#define	UnknownCommand	3
#define	crcError	4
#define	BadLength	5
#define	SeekError	6
#define	UnknownMedia	7
#define	SectorNotFound	8
#define	NoPaper		9
#define	WriteFault	10
#define	ReadFault	11
#define	GeneralFailure	12


/*
 *	pseudo types
 */
typedef unsigned char	BYTE;
typedef unsigned int	WORD;
typedef void far	*DWORD;


/*
 *	parameter structures
 */
typedef struct
	{
	BYTE	Length;		/*  Request Header length	*/
	BYTE	Unit;		/*  Unit Code			*/
	BYTE	Command;	/*  Command Code		*/
	WORD	Status;		/*  Status			*/
	BYTE	reserved[8];	/*  DOS Reserved Area		*/
	}
	Request;

typedef struct
	{
	Request	InitReq;	/*  Request Header		*/
	BYTE	nUnits;		/*  number of units		*/
	DWORD	EndAddr;	/*  Ending Address		*/
	DWORD	BPBarray;	/*  ptr to BPB array		*/
	}
	InitParms;

typedef struct
	{
	Request	MediaReq;	/*  Request Header		*/
	BYTE	MediaDesc;	/*  Media Descriptor		*/
	BYTE	ReturnCode;	/*  Return Code			*/
	}
	MediaParms;

typedef struct
	{
	Request	BPBReq;		/*  Request Header		*/
	BYTE	MediaDesc;	/*  Media Descriptor		*/
	DWORD	Transfer;	/*  Transfer Address		*/
	DWORD	BPBTable;	/*  ptr to BPB table		*/
	}
	BPBParms;

typedef struct
	{
	Request	InOutReq;	/*  Request Header		*/
	BYTE	MediaDesc;	/*  Media Descriptor		*/
	DWORD	Transfer;	/*  Transfer Address		*/
	WORD	Count;		/*  Byte/Sector Count		*/
	WORD	Start;		/*  Starting Sector Number	*/
	}
	InOutParms;

typedef struct
	{
	Request	ndInputReq;	/*  Request Header		*/
	BYTE	theByte;	/*  Byte Read From Device	*/
	}
	ndInputParms;

typedef struct
	{
	Request	StatusReq;	/*  Request Header		*/
	}
	StatusParms;

typedef struct
	{
	Request	FlushReq;	/*  Request Header		*/
	}
	FlushParms;

typedef struct
	{
	WORD	BytesPerSector;
	BYTE	SecsPerAllocUnit;
	WORD	ReservedSectors;
	BYTE	FATCount;
	WORD	RootDirEntries;
	WORD	SectorsPerLogical;
	BYTE	MediaDesc;
	WORD	SecsPerFAT;
	}
	BPB;

typedef struct
	{
	BYTE	BootJump[3];
	BYTE	Name[8];
	BPB	BootBPB;
	WORD	SecsPerTrack;
	WORD	HeadCount;
	WORD	HiddenCount;
	}
	BootSector;


/*
 *	externals
 */
extern Request	ReqHdr;		/*  current Request Header	*/

extern DWORD EndAddr(void);		/*  returns Ending Address	*/


/*
 *	END of driver.h
 */
