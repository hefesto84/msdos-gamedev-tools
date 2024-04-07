// Written by Scott Hall of the U. of Missouri - Columbia, USA.  
// This program will redirect your print screen button so that 
// it goes to a file.  The file name is at the beginning of the 
// start() function.  It can easily be modified to print screens
// that are larger than 80x25.  

#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <dos.h>

#define VIDMEM 0xB8000000L                      // or 0xb0000000 for mono

int busy=0;                                     // mutual exclusion (MUTEX)

void interrupt int_5();
void tsr(unsigned size);                        // standard tsr bios call
void write_char(int x, int y, char ch, int attrib);
void write_string(int x, int y, char *str, int attrib);
void main(void);
void start(void);

void main(void)
{
struct address  {
	char far *p;
		};

struct address far *addr = (struct address far *) 20;

addr->p = (char far *) int_5;

tsr(500);                               // keeps 500 * 16 bytes resident (8k)
}

void tsr(unsigned size)                 // standard tsr bios call
{                                       // you can also use keep()
union REGS r;

r.h.ah = 49;                            // function 0x31
r.h.al = 0;
r.x.dx = size;
int86(0x21,&r, &r);                     // last line executed
}                                       // never get to this line

void interrupt int_5()                  // print screen button starts here
{
if(!busy)
	{
	busy = !busy;                   // mutex around tsr
	start();
	busy = !busy;
	}
}

void write_char(int x, int y, char ch, int attrib)
{                                       // displays 1 character at (x,y)
char far *v;

v = (char far *) VIDMEM;
v += y*160 + x*2;
*v++ = ch;
*v = attrib;
}

void write_string(int x, int y, char *str, int attrib)
{                                       // writes string str at (x,y)
for( ; *str; str++,x++)
	write_char(x, y, *str, attrib);
}

void start(void)
{
int fd,x,y;
char cr=0x0D, lf=0x0A;

if((fd =_open("c:\\temp\\log.txt",O_WRONLY))<0) // open the file
	if((fd =_creat("c:\\temp\\log.txt",_A_NORMAL))<0) // try to make new
		write_string(1,1,"OOPS--write error1",0x8F);

lseek (fd,0,SEEK_END);                          // jump to end of file

for(y=0;y<25;y++)                               // grab lines 0 to 24
	{
	for(x=0;x<80;x++)                       // grab rows 0 through 79
		if(_write(fd,(char far *)(VIDMEM+160*y+2*x),1)==-1)
			write_string(1,2,"OOPS--write error2",0x8F);

	if(_write (fd,&cr,1)==-1)               // put a cr and lf at end
		write_string(1,3,"OOPS--write error3",0x8F); // of line

	if(_write (fd,&lf,1)==-1)
		write_string(1,4,"OOPS--write error4",0x8F);
	}

if(_write (fd,"---<END OF SCREEN>---",21)==-1)          // show end of screen
	write_string(1,5,"OOPS--write error5",0x8F);

if(_write (fd,&cr,1)==-1)               // put a cr and lf at end
	write_string(1,6,"OOPS--write error6",0x8F); // of line

if(_write (fd,&lf,1)==-1)
	write_string(1,7,"OOPS--write error7",0x8F);

_close(fd);                                     // close the file
}
