#define TICKSPERSEC	18

#define FALSE		0
#define TRUE		1

#define NO			0
#define YES			1

#define OFF			0
#define ON			1

#define BELL		7
#define BACKSP		8
#define TAB			9
#define LINEFEED	10
#define FORMFEED	12
#define RETURN		13
#define XON			17
#define XOFF		19
#define ESCAPE		27
#define SPACE		32
#define CBACKSP		127

#if !defined(__COLORS)
	#define __COLORS

	enum COLORS {
		BLACK,		/* dark colors */
		BLUE,
		GREEN,
		CYAN,
		RED,
		MAGENTA,
		BROWN,
		LIGHTGRAY,
		DARKGRAY,	/* light colors (Blinking if specified as background)	*/
		LIGHTBLUE,
		LIGHTGREEN,
		LIGHTCYAN,
		LIGHTRED,
		LIGHTMAGENTA,
		YELLOW,
		WHITE
	};

#endif

#define LIGHTGREY	LIGHTGRAY
#define DARKGREY	DARKGRAY

#define	makenul(var) (var)[0] = '\0'

typedef unsigned int	uint;
typedef unsigned char	uchar;
typedef unsigned char	byte;
typedef unsigned long	ulong;

/*
*	Define names for specific length integers.
*/

typedef char				int8;
typedef int					int16;
typedef long int			int32;
typedef unsigned char		uint8;
typedef unsigned int		uint16;
typedef unsigned long int	uint32;
