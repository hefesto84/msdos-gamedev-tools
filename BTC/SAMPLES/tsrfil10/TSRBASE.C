// Written by Scott Hall of the U. of Missouri - Columbia, USA

// if file operations are used in tsr, you MUST be in compact, large, or
// huge memory model for the getdta() and setdta() functions!!!

#pragma -mc                             // force compact memory model

#include <dos.h>
#include <conio.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>

#define VIDEOMEMORY_COLOR 0xB8000000L   // needed for direct screen writes
#define VIDEOBASE_COLOR 0xB800          // needed for movedata (screensaves)
#define VIDEOMEMORY_MONO 0xB0000000L
#define VIDEOBASE_MONO 0xB000

// prototypes of functions

// interrupt vectors
void    interrupt int_5(void);          // front door to tsr (print screen)
void    interrupt int_9(void);          // front door to tsr (key hit)
void    interrupt (*old_int9)(void);    // original int9
void    interrupt int_8(void);          // timer tick interrupt
void    interrupt (*old_int8)(void);    // original int8
void    interrupt int_28(void);         // DOS IDLE interrupt
void    interrupt (*old_int28)(void);   // original int28

// non resident
void    main(int, char **);
void    tsr(unsigned size);             // dos call to tsr func.
void    find_dos_active_flag(void);     // find flag location in memory

// resident
void    start(void);                    // actual start of tsr

// Screen handling functions
void    write_char(int x, int y, char ch, int attrib);
void    write_string(int x, int y, char *str, int attrib);
void    save_screen(char near *buffer);
void    restore_screen(char near *buffer);
void    wait_for_key(void);

// global variables

char    busy;                   // keep the tsr active only once at a time
char    far *vid_mem;           // video memory pointer location
unsigned vidbase;
char    far *old_dta;           // keep the file data area for running program
char    far *dos_active;        // make sure we don't interrupt dos
char    screenbuffer[80*25*2];  // place where screen is saved
int     interrupt_is_waiting;   // Tell int_8 to go in back door - 1=go

/**********************************************************************
*                           MAIN                                      *
**********************************************************************/

void main(int argc,char *argv[])
{
if(*argv[0]);                                   //trash a warning

if(argc==1)                                     // no arguments?
	{                                       // assume color
	vid_mem=(char far *)VIDEOMEMORY_COLOR;
	vidbase=VIDEOBASE_COLOR;
	}
else                                            // any arguments at all?
	{                                       // assume mono
	vid_mem=(char far *)VIDEOMEMORY_MONO;
	vidbase=VIDEOBASE_MONO;
	}

interrupt_is_waiting=0;                 // not waiting on dos to run

find_dos_active_flag();

disable();                              // precautionary measure

					// set up new interrupt vectors
//setvect(0x05,int_5);                  // for print screen button
old_int9 = getvect(0x09);
setvect(0x09,int_9);                    // for key hit method

old_int8 = getvect(0x08);
setvect(0x08,int_8);
old_int28 = getvect(0x28);
setvect(0x28,int_28);
enable();

tsr(1000);                              // leave 16k resident (adjust as needed)
}

/**********************************************************************
*                             TSR                                     *
**********************************************************************/

void tsr(unsigned size)                 // standard tsr bios call
{
union REGS r;

r.h.ah = 49;
r.h.al = 0;
r.x.dx = size;
int86(0x21,&r, &r);                     // last line executed
}                                       // never get to this line

/**********************************************************************
*                            INT 5                                    *
**********************************************************************/

void interrupt int_5(void)
{
if(!busy&&!*dos_active)                 // MUTEX around int5
	{
	busy = !busy;
	start();
	busy = !busy;
	}

else if (*dos_active)                   // if dos is busy, let int 8
	interrupt_is_waiting = 1;       // or int 28 run the program.
}

/**********************************************************************
*                            INT 9                                    *
**********************************************************************/

void interrupt int_9(void)
{
char far *keybuf = (char far *) 1050;   // head pointer of keyboard
int far *keybuffer = (int far *) 1050;

(*old_int9)();                          // use old keyboard routines first

if(*keybuf != *(keybuf+2))              // if buffer not empty
	{
	keybuf += *keybuf-30+5;         // go to character position
	if (*keybuf==59)
	// 59 = F1, 60 = F2, .... 68 = F10, 133= F11, 134=F12
	// 84 = <Shift> F1, ... 93 = <shift> F10, 135=S-F11, 136=S-F12
	// 94 = <ctrl F1> .... 103=<ctrl> F10, 137 = C-F11, 138=C-F12
	// 104 = <alt F1> .... 113= <ctrl> F10, 139 = A-F11, 140=A-F12

		{
		*(keybuffer+1)=*keybuffer;      // eat the key

		if(!busy&&!*dos_active)         // MUTEX around int9
			{
			busy = !busy;
			start();
			busy = !busy;
			}

		else if (*dos_active)           // if dos is busy, let int 8
			interrupt_is_waiting = 1;// or int 28 run the program.
		}
	}
}

/**********************************************************************
*                            INT 8                                    *
**********************************************************************/

void    interrupt int_8(void)
{
(*old_int8)();
if(!*dos_active&&!busy&&interrupt_is_waiting) // let it go in the back
	{                               // door if we want it too
	busy = !busy;
	start();
	busy = !busy;
	}
}

/**********************************************************************
*                            INT 28                                   *
**********************************************************************/

void    interrupt int_28(void)
{
(*old_int28)();
if(!busy&&interrupt_is_waiting)         // The dos idle interrupt
	{                               // method of trying to start
	busy = !busy;                   // the tsr
	start();
	busy = !busy;
	}
}

/**********************************************************************
*            FIND DOS ACTIVE FLAG                                     *
**********************************************************************/

void find_dos_active_flag(void)         // undocumented dos call :-)
{                                       // *dos_active = 1 when dos is
union REGS r;                           // busy and 0 when dos is not busy
struct SREGS s;

r.h.ah=0x34;
int86x(0x21,&r, &r, &s);
dos_active=(char far *)MK_FP(s.es,r.x.bx);
}

/**********************************************************************
*                         START                                       *
**********************************************************************/

void start(void)
{
interrupt_is_waiting = 0;               // interrupt is now running

				// needed if file operations in tsr
				// if you use these 2 lines you MUST
				// use compact, large, or huge model
				// <alt> options, compiler, code generation
old_dta=getdta();               // save file data area of interrupted program
setdta((char far *)MK_FP(_psp,0x80));   // set file area to the correct place

save_screen(screenbuffer);

//
//
//
// put your program here

write_string(10,10,"TSR ACTIVATED",0x0F);

//
//
//

wait_for_key();

restore_screen(screenbuffer);
				// needed if file operations in trs
setdta(old_dta);                // set file data area back to what it was
}


/**********************************************************************
*                   WAIT FOR KEY                                      *
**********************************************************************/

// This function waits for a key press to occur.  Kind of like DOS pause
// note that this doesn't work right is dos EDIT, QBASIC, or DOSSHELL
// the problem is that it takes about 8 key hits or so for those programs
// to notice the key hit, and then the keys pressed aren't eaten.

void wait_for_key(void)
{
int key_hit;
int far *t2 = (int far *) 1050;         // keyboard buffer area
char far *t = (char far *) 1050;        // keyboard buffer area

key_hit = 0;                            // no key hit yet

enable();
while(!key_hit)
	{
	key_hit = *t - *(t+2);          // is keyboard buffer empty?
	}
disable();

*(t2+1) = *t2;                  // eat the key press
}

/**********************************************************************
*                        WRITE CHAR                                   *
**********************************************************************/

// note that x and y are 0 based.  attrib 0x07 is dos gray or use colors
// in conio.h

void write_char(int x, int y, char ch, int attrib)
{
char far *v;

v = vid_mem;
v += y*160 + x*2;
*v++ = ch;
*v = attrib;
}

/**********************************************************************
*                         WRITE STRING                                *
**********************************************************************/

// note that x and y are 0 based.  attrib 0x07 is dos gray or use colors
// in conio.h

void write_string(int x, int y, char *str, int attrib)
{
for( ; *str; str++,x++)
	write_char(x, y, *str, attrib);
}

/**********************************************************************
*                   SAVE SCREEN                                       *
**********************************************************************/

/* saves the contents of the color screen in buffer */
void save_screen(char near *buffer)
{
movedata(vidbase, 0, _DS, (unsigned)buffer, 80*25*2);
}

/**********************************************************************
*                   RESTORE SCREEN                                    *
**********************************************************************/

/* restores the contents of the mono screen in buffer */
void restore_screen(char near *buffer)
{
movedata(_DS, (unsigned)buffer, vidbase, 0, 80*25*2);
}

/**********************************************************************
*                   READ FILE                                         *
**********************************************************************/

// to use this function, you must use the setdta() and getdta() functions
// in start().

int read_file()
{
int     fd;
char    input[50];

if((fd =_open("c:\\temp\\temp.in",O_RDONLY))<0) // open the file
	{
	write_string(3, 3, "OOPS--can't open the file", 0x8F);
	return(-1);                             // return failure
	}

lseek (fd,0,SEEK_SET);                          // rewind to start of file
						// (optional)
if((_read(fd,input,49))<0)                      // handle, string, #bytes
	write_string(3,10,"OOPS--read error1",0x8F);

input[49]=0;                                    // null terminate

_close(fd);                                     // close the file

return (0);                                     // return success
}

/**********************************************************************
*                WRITE FILE                                           *
**********************************************************************/

// to use this function, you must use the setdta() and getdta() functions
// in start().

int write_file(void)
{
int fd;
char cr=0x0D, lf=0x0A;

if((fd =_open("c:\\temp\\temp.out",O_WRONLY))<0)
	if((fd =_creat("c:\\temp\\temp.out",_A_NORMAL))<0)
		{
		write_string(3,10,"OOPS--write error1",0x8F);
		return (-1);
		}

lseek (fd,0,SEEK_END);                          // jump to end of file

if(_write (fd,"HELLO THERE",strlen("HELLO THERE"))==-1)
	{
	write_string(3,1,"OOPS--write error1",0x8F);
	return (-1);
	}
if(_write (fd,&cr,1)==-1)
	write_string(3,2,"OOPS--write error2",0x8F);

if(_write (fd,&lf,1)==-1)
	write_string(3,3,"OOPS--write error3",0x8F);

_close(fd);                                     // close the file

return (0);                                     // return success
}
