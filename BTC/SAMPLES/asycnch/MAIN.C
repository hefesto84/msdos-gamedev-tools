#include <stdio.h>
#include <conio.h>
#include "asynch.h"
void Beep(void);
int ctrlc(void)
{
	cputc(COM2,3);
	return 1;
}
main()
{
	int RetCode, c;
	if((RetCode = copen(COM2,2048,2400,NONE,DATA8,STOP1,0,0,0)) != 0)
	{
		printf("copen failed: Code: %d\n",RetCode);
		return -1;
	}
	clrscr();
	ctrlbrk(ctrlc);
	fclose(stdout);
	while(1)
	{
		if (kbhit())
		{
			c = getch();
			if (c == 0)
			{
				switch(getch()&0xFF)
				{
					case 68 :
						cclose(COM2);
						return 0;
					case 35 :
						ControlDTR(COM2,OFF);
						delay(500);
						ControlDTR(COM2,ON);
						break ;
					case 48 :
						SendBreak(COM2);
						break ;
					default :
						Beep();
						break;
				}
				
			}
			else
				cputc(COM2,c);
		}
		if ((c = cgetc(COM2)) >= 0)
			fprintf(stderr,"%c",c);
	}
}
void Beep()
{
	sound(1000);
	delay(250);
	nosound();
}

	
