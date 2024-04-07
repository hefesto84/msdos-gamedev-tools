
#define RSHIFT	0x01
#define LSHIFT	0x02
#define CTRL	0x04
#define ALT		0x08
#define L_SCR	0x10
#define L_NUM	0x20
#define L_CAP	0x40
#define L_INS	0x80

#define EOI		0x20

void	trapbreak(void);
void	endtrap(void);
void	interrupt traphandler(void);
int		getshftstate(void);
void	kbreset(void);
void	settimer(ulong hz);
char	*binary(uint16 val,int digits);
uint8	locks(uint8 on,uint8 off);
