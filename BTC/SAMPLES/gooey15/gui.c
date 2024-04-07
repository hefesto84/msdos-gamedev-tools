//  DOS GUI V 1.52 (c) Copyright 1996-1997
//  by Glen E. Gardner, Jr.
//  Aegis@www.Night-Flyer.com


// The user may distribute any version of this software provided the  
// original unaltered program and all of it's files are included 
// in the distribution. 

//  This program is freeware, use it and enjoy.  


//  Written for Borland C 4.52  with Borland Extensions and Borland
// .BGI interface.
// 6-26-97  Fix added to properly set mouse graphics mode to 640X480 16 colors VGA mode.
// 12-9-98 Added check for retrace to reduce screen flicker when a button is clicked.

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <graphics.h>
#include <process.h>
#include <string.h>
#include <dir.h>
#include <time.h>


int GetX(void);
int GetY(void);
int GetKey(void);
void ShowCursor(void);
void HideCursor(void);
int ButtonStatus(void);
void SetMouseMode(int);
void PrintMousePosition(void);
void UpdateMousePosition(void);
void PrintMouseButtonStatus(void);
void UpdateMouseButtonStatus(void);
void CheckMouseButtonStatus(void);
void DrawOneButton(void);
void OnButtonClick(void);
void LabelButtons(void);
void RelabelButtons(int);
void About(void);
void InitializeMouse(void);
void Startup(void);
void LoadOne(void);
void PrintTime(void);
void LoadDesktopSettings(void);
void LoadAll(void);
void ExitProgram(void);
void LaunchIt(void);
void CheckRetrace(void);

// cpu register structure for dos function calls.
struct REGPACK reg;
union REGS regs;



// Button function map variables.
//int LaunchProgram;
int ToggleAboutMouse;
int DisplayAbout;
int DisplayTime;
int QuitProgram;
int EnableTimeDisplay;
int EnableMouse;
int EnableAbout;

// sets up button map structure.
char label[64]={"NONE"};

int x;
int y;
int height;
int width;
int ButtonColor;
int BorderColor;
int TextColor;
char Launch[64];
char CommandLine[128];
int TimeColor;
int AboutColor;
int BackgroundColor;

int i,X,Y,Xold,Yold,status,LeftButton,RightButton,MiddleButton;
int OldStatus,NumberOfButtons,MouseStats,KEY,num,ButtonNumber,flag;
int gdriver,gmode,Mode,toggle,Ttog,Ttog2,Ttog3;

char DriverPath[64];
char TempString[64];

time_t Tnew, Told;
char TimeString[28],key;


 FILE *filein;


// functions begin


void ExitProgram(void)
{
// quits the program.
closegraph();
exit(0);
}

int GetX(void)
{
// get mouse X position
	reg.r_ax=3;
	intr(51,&reg);
	return(reg.r_cx);
}

int GetY(void)
{
// get mouse Y position
	reg.r_ax=3;
	intr(51,&reg);
	return(reg.r_dx);
}


void ShowCursor(void)
{
// Turn on mouse cursor
	reg.r_ax=1;
	intr(51,&reg);
}

void HideCursor(void)
{
// Turn off mouse cursor.
	reg.r_ax=2;
	intr(51,&reg);
}

int ButtonStatus(void)
{
// get mouse button status
	reg.r_ax=3;
	intr(51,&reg);
	return(reg.r_bx);
}

void CheckMouseButtonStatus(void)
{
// gets mouse buttons pressed.

	status=ButtonStatus();

	LeftButton=0;
	RightButton=0;
	MiddleButton=0;

	if(status==1){LeftButton=1;}
	if(status==2){RightButton=1;}
	if(status==3){LeftButton=1;RightButton=1;}
	if(status==4){MiddleButton=1;}
	if(status==5){LeftButton=1;MiddleButton=1;}
	if(status==6){LeftButton=1;MiddleButton=1;}
	if(status==7){LeftButton=1;RightButton=1;MiddleButton=1;}
}


int GetKey(void)
{
// returns the first key pressed
	regs.h.ah=6;
	regs.h.dl=255;
	intdos(&regs,&regs);
	return(regs.h.al);
}

void SetMouseMode(int mode)
{
// Set mouse graphics mode
	int FontSize=12;
	reg.r_ax=40;
	reg.r_cx=mode;
	reg.r_dx=FontSize;
	intr(51,&reg);
}

void InitializeMouse(void)
{
//resets mouse and driver.
	reg.r_ax=0;
	intr(51,&reg);
}

void PrintMousePosition(void)
{
int temp;

// prints mouse position to screen.
char Xstring[8], Ystring[8];
temp=getcolor();
setcolor(AboutColor);
setfillstyle(0,BackgroundColor);
CheckRetrace();
bar(2,2,312,10);
moveto(2,2);
itoa(X,Xstring,10);
itoa(Y,Ystring,10);
outtext("Mouse Position:");
outtext(" X=");
outtext(Xstring);
outtext(" Y=");
outtext(Ystring);
setcolor(temp);
}

void UpdateMousePosition(void)
{
if((X!=Xold) || (Y!=Yold)){PrintMousePosition();}
}

void PrintMouseButtonStatus(void)
{
int temp;
// Prints status of mouse buttons to screen.
CheckMouseButtonStatus();
temp=getcolor();
setcolor(AboutColor);
setfillstyle(0,BackgroundColor);
CheckRetrace();
bar(2,13,384,21);
moveto(2,13);
outtext("Button Status:");
if(LeftButton==1){outtext(" Left=DOWN  ");}else{outtext(" Left=UP    ");}
if(MiddleButton==1){outtext("Middle=DOWN ");}else{outtext("Middle=UP   ");}
if(RightButton==1){outtext("Right=DOWN");}else{outtext("Right=UP");}
setcolor(temp);
}

void UpdateMouseButtonStatus(void)
{
if(status != OldStatus)
	{
	PrintMouseButtonStatus();
	PrintMousePosition();
	}
}


void DrawOneButton()
{
int temp;
temp=getcolor();
setcolor(BorderColor);
setfillstyle(1,BorderColor);
bar(x,y,x+width-1,y+height-1);
setfillstyle(1,ButtonColor);
bar(x+1,y+1,x+width-2,y+height-2);
setcolor(TextColor);
LabelButtons();
setcolor(temp);
}


void OnButtonClick(void)
{
 int i;

filein=fopen("GUI.INI","r");

	i=0;
	while((i<NumberOfButtons)&&(status!=OldStatus))
  {
	LoadOne();
	//Is the mouse clicked inside a button?
	if((X>x) && (X<x+width) && (Y>y) && (Y<y+height))
	{
			if(LeftButton==1)
			{
			RelabelButtons(BorderColor);
			delay(300);
			if(EnableAbout!=0){Ttog3++;if(Ttog3>1){Ttog3=0;}};
			if(EnableMouse!=0){Ttog2++;if(Ttog2>1){Ttog2=0;}};
			if(EnableTimeDisplay!=0){Ttog++;if(Ttog>1){Ttog=0;}};
			if(ToggleAboutMouse==1){toggle++;if(toggle>1){toggle=0;}};
			if(strncmpi(Launch,"NONE",4)!=0){LaunchIt();}
			if(QuitProgram==1){ExitProgram();}
			}

			if(LeftButton==0)
			{
			RelabelButtons(TextColor);
			}

			if(RightButton==1)
			{
			RelabelButtons(BorderColor);
			delay(300);
			}
			if(RightButton==0)
			{
			RelabelButtons(TextColor);
			}
	}
	 i++;
	}
	fclose(filein);

}


void RelabelButtons(int TColor)
{
int temp;
temp=getcolor();
setcolor(TColor);
CheckRetrace();
HideCursor();
DrawOneButton();
LabelButtons();
ShowCursor();
setcolor(temp);
}



void PrintTime(void)
{
int temp;
char *string,String[28];
temp=getcolor();
string=ctime(&Tnew);
String[0]=*string;
string[24]=0;
setcolor(TimeColor);
outtextxy(420,15,string);
setcolor(temp);
}


void LabelButtons(void)
{
int nums;
nums=strlen(label);
if(strncmpi(label,"NONE",4)!= 0)
	{
	outtextxy(x+((width/2)-((nums*8)/2)),y+(height/2)-4,label);
	}
}


void LaunchIt(void)
{
char path[64];

	// Launch an application.
if(strncmpi(Launch,"NONE",4)!=0)
			{
			strcpy(path,"X:\\");
			path[0]='A'+getdisk();
			getcurdir(0,path+3);
			closegraph();
			if(strncmpi(CommandLine," NONE",5)!=0)
			{
			spawnlp(P_WAIT,Launch,Launch,CommandLine,NULL);
			}
			if(strncmpi(CommandLine," NONE",5)==0)
			{
			spawnlp(P_WAIT,Launch,Launch,NULL,NULL);
			}
			chdir(path);
			Startup();
			}
}


void About(void)
{
setfillstyle(0,0);

bar(2,2,390,20);
if(Ttog3==0)
{
outtextxy(2,2,"DOS Graphical User Interface  V1.5");
outtextxy(2,13,"Copyright 1996, by Glen E. Gardner, Jr.");
}
}

void Startup(void)
{

Mode=2;

LoadDesktopSettings();
InitializeMouse();
// Selects vga driver.
gdriver=9;
// Sets graphics mode to VGAHI (640X480, 16 color).
gmode=18;

initgraph(&gdriver,&Mode,"DriverPath");

SetMouseMode(gmode);
setcolor(AboutColor);
setbkcolor(BackgroundColor);
LoadAll();
ShowCursor();
setwritemode(1);
status=1;
}

void LoadDesktopSettings(void)
{// get desktop settings
char TempString[64];
filein=fopen("DESKTOP.INI","r");
fscanf(filein,"%s%d",&TempString,&BackgroundColor);
fscanf(filein,"%s%d",&TempString,&TimeColor);
fscanf(filein,"%s%d",&TempString,&AboutColor);
fscanf(filein,"%s%d",&TempString,&DisplayTime);
fscanf(filein,"%s%d",&TempString,&DisplayAbout);
fscanf(filein,"%s%s",&TempString,&DriverPath);
fclose(filein);
}

void LoadAll(void)
{
LoadDesktopSettings();
filein=fopen("GUI.INI","r");
NumberOfButtons=1;
flag=1;
while(flag>0)
	{
LoadOne();
	DrawOneButton();
	NumberOfButtons++;
	}
fclose(filein);
NumberOfButtons=NumberOfButtons-2;
}

void LoadOne(void)
{
int z=0;
char TempString[64];
fscanf(filein,"%s%d",&TempString,&x);
fscanf(filein,"%s%d",&TempString,&y);
fscanf(filein,"%s%d",&TempString,&height);
fscanf(filein,"%s%d",&TempString,&width);
fscanf(filein,"%s%d",&TempString,&ButtonColor);
fscanf(filein,"%s%d",&TempString,&BorderColor);
fscanf(filein,"%s%d",&TempString,&TextColor);
fscanf(filein,"%s%s",&TempString,&label);
fscanf(filein,"%s%s",&TempString,&Launch);
fscanf(filein,"%s",&TempString);
while(z<128){CommandLine[z]='\32';z++;}
fgets(*&CommandLine,128,filein);
fscanf(filein,"%s%d",&TempString,&QuitProgram);
fscanf(filein,"%s%d",&TempString,&ToggleAboutMouse);
fscanf(filein,"%s%d",&TempString,&EnableMouse);
fscanf(filein,"%s%d",&TempString,&EnableAbout);
flag=fscanf(filein,"%s%d",&TempString,&EnableTimeDisplay);

}

void CheckRetrace(void)
{
/* wait till a retrace starts (blak screen) */
while(!(inp(0x3DA)&8)){}
}


 // program begins
void main(void)
{
//initialize mouse and graphics screen
Startup();
while(KEY!=27)
{//hang out here till escape is pressed.
	KEY=GetKey();

	// get the time
	Told=Tnew;
	time(&Tnew);

	// update the time display as needed

	if((Tnew!=Told)&&(DisplayTime==1))
		{
		setfillstyle(0,0);
		bar(418,13,620,27);
		if(Ttog==1){PrintTime();}
		}

	// keep track of last mouse position.
	Xold=X;
	Yold=Y;

	// Keep track of last button state.
	OldStatus=status;

	// get current mouse position.
	X=GetX();
	Y=GetY();

	// get current mouse button status.
	CheckMouseButtonStatus();

	// Generate display panel info as needed.
	if((DisplayAbout==1)&&(toggle==1)&&(Ttog2==0))
	{

	if((Xold!=X)||(Yold!=Y)|| (status!=OldStatus))
		{
		// print updated mouse position to screen.
		UpdateMousePosition();

		// print updated mouse button states to screen.

		UpdateMouseButtonStatus();
		}
	 }
	if((DisplayAbout==1)&&(toggle==0))
	{
	if(Told!=Tnew)
		{
		About();
		}
	}

	// Evaluate mouse button clicks.
	if(status != OldStatus)
	{
	OnButtonClick();
	}

}

// exit graphics mode
closegraph();
}
// program ends


