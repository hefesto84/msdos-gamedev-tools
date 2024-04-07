#include <stdio.h>
#include <dos.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include "ansi.h"
#include "com.h"


void term_window(void);
void check_status(int);
void set_capture(void);
void restore_screen(void);
void change_line_settings(int);
void dial(int);
void printxy(int,int,char *);
void show_keys(void);

#define inkey() (kbhit()?getch():-1)

#define COM1            1
#define BAUD_RATE       9600
#define COM_SETTINGS    (BITS_7 | STOP_1 | EVEN_PARITY)

int capture = 0,
    g_echo  = 0;
FILE *capture_file;

int main(void)
{
   int     c, stop = 0;


   if(!ComStart(COM1))
   {
     printf("Error initializing com port");
     return(0);
   }
   ComSetParms(COM1, BAUD_RATE, COM_SETTINGS);

   term_window();

   while(!stop)
   {
       /*      If a character was recieved, print it to the console  */
      
       check_status(COM1);
       
       if((c=ComGetChar(COM1))!=S_EOF)
       {
	   putch(c);
	   if(capture)
	     putc(c,capture_file);
       }
       
       /* if the buffer is fulling fast!, stop and empty it */
       if(ComChkCnt(COM1) > 500)
       {
	  ComHandShake(COM1, DTR);
	  while(ComChkCnt(COM1) > 0)
	  {
	       putch(c);
	       if(capture)
		 putc(c,capture_file);
	  }
	  
	  ComHandShake(COM1, DTR | RTS);
       }


       /*      If a key has been pressed       */
       if((c=inkey()) != -1)
       {
	  if(c == 0)
	  {
	    switch(inkey()) 
	    {
	      case 0x3B:        /*  F1 - Help                  */
		show_keys();
		break;
	      case 0x3C:        /*  F2 - Change line settings  */
		change_line_settings(COM1);
		break;
	      case 0x3D:        /*  F3 - Dial a number         */
		dial(COM1);
		break;
	      case 0x3E:       
		set_capture();  /*  F4 - Capture toggle        */
		break;
	      case 0x3F:        /*  F5 - Toggle echo           */
		if(g_echo)
		  g_echo = 0;
		else
		  g_echo = 1;
		break;
	      case 0x40:         /*  F6 - Quit                  */
		stop = 1;
		break;
	    }
	  }
	  else 
	  {
	    ComPutChar(COM1,c);
	    if(g_echo)
	      putch(c);
	  }
       }      
   }    

   ComStop(COM1);
   if(capture)
     fclose(capture_file);

   restore_screen();
   clrscr();

   printf("Thanx for using The Simple Terminal.\n");
   
   return(0);
}

void term_window(void)
{
   window(1,25,80,25);
   textcolor(YELLOW);
   textbackground(RED);
   clrscr();
   printxy(1,1,"         บ             บ   THE SIMPLE TERMINAL   บ  F1 for help  บ             ");
   
   window(1,1,80,24);
   textcolor(YELLOW);
   textbackground(BLUE);
   clrscr();
}

void show_keys(void)    
{
    char buffer[4097]="";
    int  x,y;

    ComHandShake(COM1, DTR);

    x = wherex();
    y = wherey();
    gettext(1,1,80,25,buffer);

    clrscr();

    printxy(28,4, "ษออออออออน Valid Keys ฬอออออออป");         
    printxy(28,5, "บ                             บ");
    printxy(28,6, "บ F1  - Help                  บ");
    printxy(28,7, "บ                             บ");
    printxy(28,8, "บ F2  - Change line settings  บ");
    printxy(28,9, "บ                             บ");
    printxy(28,10,"บ F3  - Dial a number         บ");
    printxy(28,11,"บ                             บ"); 
    printxy(28,12,"บ F4 -  Capture Toggle        บ");
    printxy(28,13,"บ                             บ");
    printxy(28,14,"บ F5 -  Echo Toggle           บ");
    printxy(28,15,"บ                             บ");
    printxy(28,16,"บ F6 -  Quit                  บ");
    printxy(28,17,"บ                             บ");
    printxy(28,18,"บ Press ESC to exit           บ");
    printxy(28,19,"ศอออออออออออออออออออออออออออออผ");
    
    while(getch() != 27);

    clrscr();
    puttext(1,1,80,25,buffer);

    gotoxy(x,y);

    ComHandShake(COM1, DTR | RTS);
}

void check_status(int port)
{
   int x,y;
   static int cap       = -1;
   static int status    = -1;
   static int l_echo    = -1;
   char *status_strs[] = {"Offline", "Online "};
   char *cap_str[]     = {"Capture Off", "Capture On "};
   char *echo_str[]    = {"Echo Off", "Echo On "};
				   

   if(status != ComChkCd(port))
   {
      status =  ComChkCd(port);
      
      x = wherex();
      y = wherey();
      window(1,1,80,25);
      textcolor(YELLOW);
      textbackground(RED);
      printxy(2,25,status_strs[status]);
      window(1,1,80,24);
      textcolor(YELLOW);
      textbackground(BLUE);
      gotoxy(x,y);
   }  

   if(cap != capture)
   {
      cap = capture;
      
      x = wherex();
      y = wherey();
      window(1,1,80,25);
      textcolor(YELLOW);
      textbackground(RED);
      printxy(12,25,cap_str[cap]);
      window(1,1,80,24);
      textcolor(YELLOW);
      textbackground(BLUE);
      gotoxy(x,y);
   }
   
   if(l_echo != g_echo)
   {
      l_echo = g_echo;
      
      x = wherex();
      y = wherey();
      window(1,1,80,25);
      textcolor(YELLOW);
      textbackground(RED);
      printxy(69,25,echo_str[l_echo]);
      window(1,1,80,24);
      textcolor(YELLOW);
      textbackground(BLUE);
      gotoxy(x,y);
   }
}

void change_line_settings(int port)
{
    int settings[]={(EVEN_PARITY | BITS_7 | STOP_1),    
		    (NO_PARITY   | BITS_8 | STOP_1),
		    (EVEN_PARITY | BITS_7 | STOP_1),    
		    (NO_PARITY   | BITS_8 | STOP_1),
		    (EVEN_PARITY | BITS_7 | STOP_1),    
		    (NO_PARITY   | BITS_8 | STOP_1)};
    int baud_rates[]={9600,9600,2400,2400,1200,1200};
    char buffer[4097]="";
    int  x,y,choice;

    ComHandShake(COM1, DTR);

    x = wherex();
    y = wherey();
    gettext(1,1,80,25,buffer);

    clrscr();

    printxy(31,4, "ษออออน Settings ฬอออป");         
    printxy(31,5, "บ                   บ");
    printxy(31,6, "บ 1 - 9600  E,7,1   บ");
    printxy(31,7, "บ                   บ");
    printxy(31,8, "บ 2 - 9600  N,8,1   บ");
    printxy(31,9, "บ                   บ");
    printxy(31,10,"บ 3 - 2400  E,7,1   บ");
    printxy(31,11,"บ                   บ");
    printxy(31,12,"บ 4 - 2400  N,8,1   บ");
    printxy(31,13,"บ                   บ");
    printxy(31,14,"บ 5 - 1200  N,7,1   บ");
    printxy(31,15,"บ                   บ");
    printxy(31,16,"บ 6 - 1200  N,8,1   บ");
    printxy(31,17,"บ                   บ");
    printxy(31,18,"บ Press ESC to exit บ");
    printxy(31,19,"ศอออออออออออออออออออผ");
    
    do
    {
      choice = getch();
    }while( ((choice < '1') || (choice > '6')) && (choice != 27));

    if(choice != 27)
      ComSetParms(port, baud_rates[choice-'1'], settings[choice-'1']);

    clrscr();
    puttext(1,1,80,25,buffer);

    gotoxy(x,y);

    ComHandShake(COM1, DTR | RTS);
}

void dial(int port)
{
   
   char number[32],
	buffer[4097]="";
   int  x,y;

    number[0] = 30;
    x = wherex();
    y = wherey();
    gettext(1,1,80,25,buffer);

    clrscr();
    gotoxy(25,15); 
    cprintf("Number: ");
    cgets(number);

   clrscr();

   if(strcmp(number+2,"") != 0)
   {  
     ComPutStr(port,"ATDT ");
     ComPutStr(port,number+2);
     ComPutCrLf(port);
   }

    puttext(1,1,80,25,buffer);

    gotoxy(x,y);
}

void set_capture(void)
{
   char file_name[13]="",
	buffer[4097]="";
   int  x,y;
   
   if(capture)
   {  
      fclose(capture_file);
      capture = 0;
   }
   else
   {
      x = wherex();
      y = wherey();
      gettext(1,1,80,25,buffer);

      file_name[0] = 12;
    
      clrscr();
      gotoxy(25,15); 
      cprintf("Capture file name: ");
      cgets(file_name);

      clrscr();

      if(strcmp(file_name+2,"") != 0)
      {  
	 if((capture_file = fopen(file_name+2,"a")) == NULL)
	 {  
	   printxy(25,17,"Error: can not open capture file");
	   sleep(2);
	 }
	 else
	   capture = 1;
      }
      puttext(1,1,80,25,buffer);
      gotoxy(x,y);
   }    
}

void restore_screen(void)
{
   window(1,1,80,25);
   clrscr();
}

void printxy(int x, int y, char *string)
{
   int l,r;

   l = wherex();
   r = wherey();
   
   gotoxy(x,y);
   cprintf(string);

   gotoxy(l,r);
}
