#define bcd2bin(x) (((x) >> 4) & 0x0f)*10+((x) & 0x0f)

int		readcmos(int port);
void	updateclock(void);
