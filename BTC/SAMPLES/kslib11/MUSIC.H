#define	SPEAKER_PORT	0x61
#define	SAY				0x48
#define	SHUTUP			(0x48|0x2)
#define	SPEAK(c)		outp(SPEAKER_PORT, ( 0x8000 & c ) ? SAY : SHUTUP)

void	play(char *);
char	*mplay(char *);
void	tri(short *);
