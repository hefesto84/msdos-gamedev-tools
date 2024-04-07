/*
*  KB.H
*/

#define	K_UPPER		0x0001	/* Accept upper case letters only				*/
#define K_BELL		0x0002	/* Sound bell if invalid key pressed			*/
#define K_RETURN	0x0004	/* Return <RETURN> if valid key pressed			*/

#define	ZFLAG	64			/* Weighted Bit Value for 'Z' in flags register	*/

/*
*	Values returned from keyboard functions for extended keys.
*	Many kept compatible with Greenleaf functions, whatever they are.
*/

#define F1			0x80
#define F2			0x81
#define F3			0x82
#define F4			0x83
#define F5			0x84
#define F6			0x85
#define F7			0x86
#define F8			0x87
#define F9			0x88
#define F0			0x89

#define SF1			0x90
#define SF2			0x91
#define SF3			0x92
#define SF4			0x93
#define SF5			0x94
#define SF6			0x95
#define SF7			0x96
#define SF8			0x97
#define SF9			0x98
#define SF0			0x99

#define CF1			0xA0
#define CF2			0xA1
#define CF3			0xA2
#define CF4			0xA3
#define CF5			0xA4
#define CF6			0xA5
#define CF7			0xA6
#define CF8			0xA7
#define CF9			0xA8
#define CF0			0xA9

#define AF1			0xE0
#define AF2			0xE1
#define AF3			0xE2
#define AF4			0xE3
#define AF5			0xE4
#define AF6			0xE5
#define AF7			0xE6
#define AF8			0xE7
#define AF9			0xE8
#define AF0			0xE9

#define ALT1		0xB0
#define ALT2		0xB1
#define ALT3		0xB2
#define ALT4		0xB3
#define ALT5		0xB4
#define ALT6		0xB5
#define ALT7		0xB6
#define ALT8		0xB7
#define ALT9		0xB8
#define ALT0		0xB9

#define HOME		0x8A	/* HOME key		*/
#define CURLF		0x8B	/* <-			*/
#define END			0x8C	/* END key		*/
#define CURUP		0x8D	/* up arrow		*/
#define CURDN		0x8E	/* down arrow	*/
#define PGUP		0x9A	/* PgUp			*/
#define CURRT		0x9B	/* ->			*/
#define PGDN		0x9C	/* PgDn			*/
#define INSERT		0x9D	/* Ins			*/
#define DELETE		0x9E	/* Del			*/
#define CHOME		0xAA	/* Ctrl Home	*/
#define CCURLF		0xAB	/* Ctrl <-		*/
#define CEND		0xAC	/* Ctrl End		*/
#define CPRTSC		0xAE	/* Ctrl PrtSc	*/
#define CPGUP		0xBA	/* Ctrl PgUp	*/
#define CCURRT		0xBB	/* Ctrl ->		*/
#define CPGDN		0xBC	/* Ctrl PgDn	*/
#define AMINUS		0xBD	/* Alt -		*/
#define AEQUAL		0xBE	/* Alt =		*/
#define STAB		0x8F	/* Shift Tab	*/

#define ALTA		0xC1
#define ALTB		0xC2
#define ALTC		0xC3
#define ALTD		0xC4
#define ALTE		0xC5
#define ALTF		0xC6
#define ALTG		0xC7
#define ALTH		0xC8
#define ALTI		0xC9
#define ALTJ		0xCA
#define ALTK		0xCB
#define ALTL		0xCC
#define ALTM		0xCD
#define ALTN		0xCE
#define ALTO		0xCF
#define ALTP		0xD0
#define ALTQ		0xD1
#define ALTR		0xD2
#define ALTS		0xD3
#define ALTT		0xD4
#define ALTU		0xD5
#define ALTV		0xD6
#define ALTW		0xD7
#define ALTX		0xD8
#define ALTY		0xD9
#define ALTZ		0xDA
#define THENUL		0xDB

/*
*	Function Prototypes
*/

void	KBsetfunc(void (*func)(void));
void	KBsettrap(uchar key,void (*func)(void));
uchar	KBgetch(void);
uchar	KBnwgetch(void);
uchar	KBgetc(char *,char *,int);
uchar	KBgets(char *,uint,int);
uchar	KBgetn(double *,uint,uint);
int		KBcheck(void);
