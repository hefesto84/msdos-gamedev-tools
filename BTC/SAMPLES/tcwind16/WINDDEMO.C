#define MAIN 1
#include <stdarg.h>
#include <window.h>
#include <stdio.h>
#include <dos.h>
#include <bios.h>
#include <string.h>
#include <alloc.h>
#include <mem.h>




char *opnmsg;
char *endmsg;
char *compmsg;
char *pwndmsg;
char *prtmsg;
char *menu1;

main()
{
 int x;

pwndmsg=
 "    CALL: pwindf(col,row,color,""format"",arguments);\n"
 "\n   Prints a formated string in the current window, with col and row\n"
 "offset with respect to the start of the window. This function will not\n"
 "print outside of the window. If col is less than the left side then col\n"
 "will=left side.  If col is greater then the right side col will=left side.\n"
 "If the string is to long to fit within the window it will be truncated.\n"
 "If the string has a backward slash <n> inbedded in it, the rest of the\n"
 "string will be printed on the next line.\n"
 "\n   If row is less then the top row then row=top row. If row is greater\n"
 "then the bottom row then the window will scroll up to accomodate the new\n"
 "string.\n"
 "\n   Format can be any of the printf format qualifiers as listed in your\n"
 "manual.\n";

compmsg=
 "  These routines are supplied in LIBRARY format. There are five\n"
 "different LIBRARY modules as listed below:\n"
 "\nWINDS.LIB            Small   library module"
 "\nWINDM.LIB            Medium  library module"
 "\nWINDC.LIB            Compact library module"
 "\nWINDL.LIB            Large   libary module"
 "\nWINDH.LIB            Huge    library module\n"
 "\n  To use these routines link the module you wish to use with your\n"
 "program. The easiest way is to use the PROJECT capabilities of Turbo C.\n"
 "Be sure to use the correct LIBRARY MODULE for your application.\n"
 "\nOne of the sample programs supplied with Turbo C is an OBJECT MODULE\n"
 "called MCMVSMEM.OBJ this is a fast screen write utilitiy. This module\n"
 "must also be linked with your program as these window routines make\n"
 "extensive use of this function.\n"
 "\n  You must also include header file WINDOW.H in your program. This file\n"
 "has the needed defines and function prototypes in it.";

endmsg=
 "  These routines have been tested on a TANDY 1000A, PC LIMITED 286\n"
 "EGA unit and an IBM XT. They should work with any CGA,EGA  PC\n"
 "compatible computer. These have not been tested on a MONO system.\n"
 "\n  If you are having any trouble getting these routines to work on\n"
 "other machines, Please let me know and I will do what I can to help you\n"
 "solve the problem.\n"
 "\n  While these function have some error checking built in. It\n"
 "is the programers responsibility to ensure correct usage.\n"
 "\n If you have questions or comments you can reach me via mail on\n"
 "                  GENIE:  D. R. Evans\n"
 "\n While it is not necessary to have the source code to use the routines\n"
 "in this package, you may purchase this code by sending $20.00  and your\n"
 "return address to:\n"
 "                  Daniel R. Evans\n"
 "                  1902 Broughton Dr.\n"
 "                  Beverly, MA 01915";


 opnmsg=
  "  Included in this package are all the routines for memory models,\n"
  "SMALL-MEDIUM-COMPACT-LARGE-HUGE. To use these routines you will need\n"
  "the fast screen write utility supplied with Turbo C called MCMVSMEM.OBJ.\n"
  "\n  You are free to use these routines in any software you write, either\n"
  "for your own use or to sell without a fee of any kind. You may distribute\n"
  "these routines as long as the package remains intact. You may not charge\n"
  "more then a reasonable duplication fee. You may not sell this package as\n"
  "part of or as any kind of windowing software."
  "\n             There are 19 functions with this package.\n"
  "initwindow          clrscr          setcursor            screenmode\n"
  "clrwind             mbox            mline                attr\n"
  "mwind               rwind           wtitle               movwind\n"
  "pwindf              pwindfc         gowindcr             waitkey\n"
  "pullmenu            popupmenu       keyin\n"
  "\n  When you have finished with the pull menu, use ESC to exit";


 initwindow();
 screenmode(color80);
 clrscr(attr(white,blue),red);
 setcursor(off);
 shadow=FALSE;
 bordercolor=attr(yellow,red);
 windowcolor=attr(white,blue);
 for(x=0;x<79;x+=10)
   mline(x,0,24,windowcolor,vert,sglline);
 for(x=0;x<24;x+=3)
   mline(0,x,79,windowcolor,hornz,sldline);
 mwind(10,9,70,13,attr(yellow,blue),attr(yellow,red),sldbrdr);
 pwindfc(1,attr(white,blue),"Window routines 1.6 for BORLAND'S Turbo C");
 pwindfc(2,attr(white,blue),"By D.R. EVANS Copywrite (C) 1987");
 sleep(2);
 rwind();
 clrscr(attr(white,black),black);
 godomenu();
 shprint();
 expcomp();
 enddemo();
 for(x=currentwind;x>0;x--)
   rwind();
 setcursor(on);
 screenmode(color80);
 clrscr(attr(white,black),black);
}


godomenu()
{
char index;
 pullmainmenu[0]=
  "inTro\n"
  "Initilize\n"
  "Screen\n"
  "Windows\n"
  "Printing\n"
  "pOp Wind\n"
  "Next Menu\n"
  "TISWPON\n";

 pullsubmenu[0][2]=
  "Clearing Screen      \n"
  "Clearing Window      \n"
  "Setting Screen Mode  \n"
  "Turning Cursor ON/OFF\n"
  "Assigning Colors     \n"
  "SWMTA\n";

 pullsubmenu[0][3]=
 "Making A Window  \n"
 "Removing A Window\n"
 "Moving Windows   \n"
 "Adding A Title   \n"
 "Border Selection \n"
 "MRWAB\n";

 pullsubmenu[0][4]=
  "Formatted Window Printing         \n"
  "Centered Formatted Window Printing\n"
  "Going to a COL/ROW                \n"
  "FCG\n";

 pullsubmenu[0][5]=
 " Menu Operation     \n"
 " Popupmenu Syntax   \n"
 " Defining a Menu    \n"
 " Menu Return Code   \n"
 " Use of Return Code \n"
 " How to EXIT Menu   \n"
 "MPDRUE\n";



 pullmainmenu[1]=
 "Pull Menu\n"
 "Key input\n"
 "Sp Effect\n"
 "Line Draw\n"
 "Box Draw\n"
 "Global Var\n"
 "Prev Menu\n"
 "PKSLBGM\n";

 pullsubmenu[1][0]=
 "Menu Operation      \n"
 "Pullmenu Syntax     \n"
 "Defining a Menu     \n"
 "Menu Return Code    \n"
 "Use of Return Code  \n"
 "How to EXIT Menu    \n"
 "MPDRUE\n";

   while((index = pullmenu(TOP,0)) !=NULL) {
      switch(pullmenuloc)
      {
         case 0:
            mwind(2,0,77,23,windowcolor,bordercolor,dblbrdr);
            pwindfc(0,windowcolor,"Window Routines 1.6 for BORLAND'S TURBO C");
            pwindfc(1,windowcolor,"By D.R. Evans Copywrite (C) 1987");
            pwindf(0,3,windowcolor,"%s",opnmsg);
            waitkey(PRET);
            rwind();
            break;
         case 1:
            shinitwindow();
            rwind();
            shscroll();
            rwind();
            break;

         case 2:
            switch(index)
            {
               case 'S':
                  shclrscr();
                  rwind();
                  break;
               case 'W':
                  shclrwind();
                  rwind();
                  break;
               case 'M':
                  shscreenmode();
                  rwind();
                  break;
               case 'T':
                  shsetcursor();
                  rwind();
                  break;
               case 'A':
                  shattr();
                  rwind();
                  break;
            }
            break;
         case 3:
            switch(index)
            {
               case 'M':
                  shmwind();
                  rwind();
                  break;
               case 'R':
                  shrwind();
                  rwind();
                  break;
               case 'W':
                  shmove();
                  rwind();
                  break;
               case 'A':
                  shtitle();
                  rwind();
                  break;
               case 'B':
                  shborders();
                  break;
            }
            break;
         case 4:
            switch(index)
            {
               case 'F':
                  shpwindf();
                  rwind();
                  break;
               case 'C':
                  shpwindfc();
                  rwind();
                  break;
               case 'G':
                  shgowind();
                  rwind();
                  break;
            }
            break;
         case 5:
            shpopup(index);
            break;
         case 6:
            while((index = pullmenu(TOP,1)) != NULL) {
               switch(pullmenuloc)
               {
                  case 0:
                     shpullmenu(index);
                     break;
                  case 1:
                     shwait();
                     rwind();
                     break;
                  case 2:
                     shshadow();
                     rwind();
                     break;
                  case 3:
                     shmline();
                     rwind();
                     break;
                  case 4:
                     shmbox();
                     rwind();
                     break;
                  case 5:
                     shglobal();
                     rwind();
                     break;
                  case 6:
                     index = NULL;
                  break;
               }
               if(index == NULL)
                  break;
            }
      }
   }
}


shpopup(char index)
{
char *popsyn,*menudef,*exitdef,*opdef,*retcdef,*usedef;

 usedef=
  "  This is one way to use the character returned from the popupmenu\n"
  "function. This is part of the routine used in this program\n"
  "\nchar index;\n"
  "\nwhile((index = popupmenu(10,5,menu1,dblbrdr) != NULL)\n"
  " rwind();      \\* Remove the popupwindow !!! popup does not remove !!!*\\\n"
  " switch(index)\n"
  " {\n"
  "   case 'P':\n"
  "     mwind(0,11,79,23,windowcolor,bordercolor,sldbrdr);\n"
  "     wtitle(CENTER,TOP,bordercolor,\" POPUPMENU SYNTAX \");\n"
  "     pwindfc(0,windowcolor,\"CALLING CONVENTION\");\n"
  "     pwindf(0,1,windowcolor,\"%s\",popsyn);\n"
  "     waitkey(PRET);\n"
  "     rwind();\n"
  "     break;\n"
  " }\n"
  "}\n";

  retcdef=
   "   The popup menu routine returns to the calling function with\n"
   "the character of the menu item you selected.\n"
   "   For example if you select Defining a Menu the \"D\" is returned\n"
   "to the calling function";

  opdef=
   "  To operate the menu, you use the up or down arrow keys\n"
   "to move the hilited bar and the enter key to make the hilited\n"
   "selection. You may also enter the hilited character to make\n"
   "your selection.";

  exitdef=
   "  If you hit \" ESC \" you will return to the calling function\n"
   "with the return value of NULL.";

  popsyn =
        "popupmenu(col, row, menu, bordtype);\n"
        "\n WHERE:\n"
        "         col and row are the upper left hand corner of the window\n"
        "         Menu name is a pointer to a string (Explained in Menu Definition)\n"
        "         Windowcolor is defined by the global variable plcolor\n"
        "         Bordercolor is defined by the global variable pbrdcolor\n"
        "         Selectable Character is defined by the global variable pletcolor\n"
        "         Bordertype is the type of border around the window,same as mwind()\n";
  menudef=
        "Declare a pointer variable to a string.\n"
        "char *menu1;   \\* set up pointer *\\\n"
        "menu1 =                     Initilize pointer.\n"
        " \" MAIN MENU \\n\"             In the first line define the title,displayed\n"
        " \"Menu Operation      \\n\"  on the top border. On the next line, start\n"
        " \"Defining a Menu     \\n\"  defining the menu selections. You can have up\n"
        " \"PopUpMenu Syntax    \\n\"  to 20 menu selections in one window. All of the\n"
        " \"Menu Return Code    \\n\"  menu selections must be the same length.\n"
        " \"Use of Return Code  \\n\"    The popup menu routine checks the first menu\n"
        " \"How to EXIT Menu    \\n\"  selection after the title, and uses this\n"
        " \"MDPRUE\\n\";               length to decide the width of the menu.\n"
        "                              if the length = 1 i.e.\" \\n\" no title is used"
        "\n  The last line of menu definition defines the characters that\n"
        "can be directly entered to select an item from the menu, these must\n"
        "be all CAPS. All the characters in this last line must also be\n"
        "distinct. The coresponding letter in each menu selection must also\n"
        "be in CAPS.\n"
        "\n  For example the last line in this menu is \"MDPRUE\" this\n"
        "coresponds to the P in \"PopUpMenu Syntax\", the D in \"Defining Menu\",\n"
        "the M in \"Menu Operation\", the R in \"Menu Return Code\" and ....\n";

    switch(index)
    {
      case 'P':
        mwind(0,11,79,23,windowcolor,bordercolor,sldbrdr);
        wtitle(CENTER,TOP,bordercolor," POPUPMENU SYNTAX ");
        pwindfc(0,windowcolor,"CALLING CONVENTION");
        pwindf(0,1,windowcolor,"%s",popsyn);
        waitkey(PRET);
        rwind();
        break;
      case 'D':
        mwind(0,0,79,24,windowcolor,bordercolor,sldbrdr);
        wtitle(CENTER,TOP,bordercolor," MENU DEFINITION ");
        pwindfc(0,windowcolor,"MENU SETUP");
        pwindf(0,1,windowcolor,"%s",menudef);
        waitkey(PRET);
        rwind();
        break;
      case 'M':
        mwind(0,9,79,16,windowcolor,bordercolor,sldbrdr);
        wtitle(CENTER,TOP,bordercolor," OPERATING MENU ");
        pwindf(0,1,windowcolor,"%s",opdef);
        waitkey(PRET);
        rwind();
        break;
      case 'R':
        mwind(0,9,79,16,windowcolor,bordercolor,sldbrdr);
        wtitle(CENTER,TOP,bordercolor," RETURN CODE ");
        pwindf(0,1,windowcolor,"%s",retcdef);
        waitkey(PRET);
        rwind();
        break;
      case 'U':
        mwind(0,0,79,24,windowcolor,bordercolor,sldbrdr);
        wtitle(CENTER,TOP,bordercolor," PROGRAM SETUP ");
        pwindf(0,1,windowcolor,"%s",usedef);
        waitkey(PRET);
        rwind();
        break;
      case 'E':
        mwind(0,10,79,14,windowcolor,bordercolor,sldbrdr);
        wtitle(CENTER,TOP,bordercolor," EXITING MENU ");
        pwindfc(0,windowcolor,"EXITING MENU WITHOUT A SELECTION");
        pwindf(0,1,windowcolor,"%s",exitdef);
        waitkey(PRET);
        rwind();
        break;
    }
}

shwait()
{
char *shwaitmsg,*shwaitmsg1,*shwaitmsg2,*shwaitmsg3;
shwaitmsg=
 " CALL: waitkey(PRET or MORE);\n"
 "\n  If waitkey is called with MORE,\n"
 "than a blinking - MORE - is printed\n"
 "on the bottom border. The program\n"
 "halts and waits for a key to be pressed\n";
shwaitmsg1=
 "\n  If waitkey is called with\n"
 "PRET, than PRESS ENTER is printed\n"
 "on the bottom border. The program\n"
 "halts and waits for a key to be\n"
 "pressed.\n";
shwaitmsg2=
 "\n  You can use the MORE qualifier\n"
 "when you are going to scroll additional\n"
 "information on the screen\n";
shwaitmsg3=
 "\n  You can use the PRET qualifier\n"
 "when you are finished\n";
 mwind(20,7,61,16,attr(yellow,blue),attr(yellow,red),sldbrdr);
 wtitle(center,top,bordercolor," WAITKEY ");
 pwindf(0,1,attr(yellow,blue),"%s",shwaitmsg);
 waitkey(MORE);
 pwindf(0,rowend,attr(yellow,blue),"%s",shwaitmsg1);
 waitkey(PRET);
 pwindf(0,rowend,attr(yellow,blue),"%s",shwaitmsg2);
 waitkey(MORE);
 pwindf(0,rowend,attr(yellow,blue),"%s",shwaitmsg3);
 waitkey(PRET);
}

shglobal()
{
char *shgbmsg,*shgbmsg1,*shgbmsg2;
 shgbmsg=
  "  There are eight global variables and one\n"
  "Definition that you need to be aware of.\n"
  "\nsnow:      If snow = TRUE then snow suppresion used for all routines.\n"
  "           WSNOW is assigned as an enviromental variable,\n"
  "           initwindow() checks the enviroment for WSNOW=remove.\n"
  "           if this exists it uses snow suppresion else snow defaults\n"
  "           to FALSE. This can be overridden by assigning snow=TRUE after\n"
  "           the call to initwindow();\n"
  "\nshadow:    If shadow = TRUE all windows displayed after this assignment\n"
  "           will have shadows. initwindow() sets this to FALSE.\n"
  "\nwindowcolor:\n"
  "bordercolor:\n"
  "             These two variables are convienent when you want to display\n"
  "           a number of windows with the same default colors. You assign\n"
  "           them to a color with the attr function\n"
  "           i.e. windowcolor = attr(yellow,blue);";
 shgbmsg1 =
  "\nplcolor:\n"
  "             This variable is used to define the Pullwindow & Popwindow\n"
  "           inside colors.\n"
  "\nphicolor:\n"
  "             This variable is used to define the color of the hilighted\n"
  "           bar used in Pullwindow & Popwindow\n"
  "\npletcolor:\n"
  "             This variable is used to define the color of the hilighted\n"
  "           character used in the Pullwindow & Popwindow routines\n"
  "\npullmenuloc:\n"
  "             This variable is set by the pull menu routine, to the \n"
  "           location of the hilited bar (0-7). This can then be used \n"
  "           in a switch statement.\n"
  "\npullmainmenu[]:\n"
  "             This is the Main Pull down menu pointer. You can have up to\n"
  "           seven Main pull down menus. This pointer is set to NULL in the\n"
  "           initwindow() routine.\n";
  shgbmsg2=
  "\npullsubmenu[][]:\n"
  "             This pointer defines each sub menu. the first array is the\n"
  "           same as the pullmainmenu[] pointer.  i.e. if pullmainmenu[0] \n"
  "           then pullsubmenu[0][0] would define the first main menu and \n"
  "           first sub menu.  You can have 1 sub menu for each item in a \n"
  "           Main menu for a total of 49 sub menus\n"
  "\nrowend:\n"
  "             This variable is set to the bottom border by the make window\n"
  "           routine. You then use this to print on the bottom line of the\n"
  "           window, which causes the window to scroll. If you print on the\n"
  "           last line the window will not scroll. You must use rowend as \n"
  "           the row to print on for scrolling.\n"
  "\nMAXWIND:   This define is in window.h, currently set for 30 windows.\n"
  "           You should set this value to the number of windows your\n"
  "           application expects to use. The initwindow(); routine uses\n"
  "           this define to set up the arrays for the window routines\n"
  "\n \n";

 mwind(0,0,79,24,attr(yellow,blue),attr(yellow,red),sldbrdr);
 wtitle(center,top,bordercolor," GLOBAL VARIABLES");
 pwindf(0,1,attr(yellow,blue),"%s",shgbmsg);
 waitkey(MORE);
 pwindf(0,rowend,attr(yellow,blue),"%s",shgbmsg1);
 waitkey(MORE);
 pwindf(0,rowend,attr(yellow,blue),"%s",shgbmsg2);
 waitkey(PRET);
}

shprint()
{
char *prtmsg;
 prtmsg=
  "                    LIST OF ALL WINDOW ROUTINES\n"
  "\n  void initwindow(void);"
  "\n  void clrscr(attr(FG,BG),border color);"
  "\n  void clrwind(Background Color);"
  "\n  void setcursor(ON or OFF);"
  "\n  void screenmode(color80 or color40);"
  "\n  int  attr(Foreground color,Background color);"
  "\n  void mwind(Col, Row, Endcol, Endrow, WindowColor, BorderColor,Bordertype);"
  "\n  void mbox(Col, Row, Endcol, Endrow, Bordercolor, Bordertype);"
  "\n  void mline(Col, Row, windcolor, Length, Linedir, Linetype);"
  "\n  void wtitle(Justify, Loc, attr(FG,BG), TITLE);"
  "\n  void movwind(Newcol, Newrow);"
  "\n  void rwind(void);"
  "\n  void pwindfc(Row, attr(FG,BG), FORMAT, ARGUMENTS);"
  "\n  void pwindf(Col, Row, attr(FG,BG), FORMAT, ARGUMENTS);"
  "\n  void gowindcr(Col, Row);"
  "\n  void waitkey(MORE or PRET);"
  "\n  int keyin(void);"
  "\n  char pullmenu(Loc, MenuNumber);"
  "\n  char popupmenu(col, row, menu, bordertype);\n"
  "\n If you have a printer hit the PRINT SCREEN key for a hard copy listing";

 mwind(0,0,79,24,attr(white,blue),attr(yellow,red),sglbrdr);
 wtitle(center,top,attr(black,green)," PRINT ");
 pwindf(0,1,attr(white,blue),"%s",prtmsg);
 waitkey(PRET);
}

enddemo()
{
 mwind(0,0,79,24,attr(white,blue),attr(yellow,red),sldbrdr);
 pwindf(0,1,attr(white,blue),"%s",endmsg);
 waitkey(PRET);
}

expcomp()
{
 mwind(0,0,79,24,attr(white,blue),attr(yellow,red),sldbrdr);
 wtitle(center,top,attr(black,green)," COMPILING ");
 pwindf(0,1,attr(white,blue),"%s",compmsg);
 waitkey(PRET);
}


shpwindfc()
{
char *pcwndmsg;

pcwndmsg=
 "CALL: pwindfc(row,color,format,arguments);\n"
 "\n   Prints a formatted string centered\n"
 "in the current window at the row specified\n"
 "\n   Format can be any of the printf format\n"
 "qualifiers as listed in your manual.\n";

 mwind(15,5,65,19,attr(yellow,blue),attr(yellow,red),sldbrdr);
 wtitle(center,top,attr(black,green)," PWINDFC ");
 pwindfc(1,attr(yellow,blue),"%s",pcwndmsg);
 waitkey(PRET);
}

shpwindf()
{
 mwind(0,0,79,24,attr(cyan,black),attr(black,blue),dtpbrdr);
 wtitle(center,top,attr(black,green)," PWINDF ");
 pwindf(0,1,attr(cyan,black),"%s",pwndmsg);
 waitkey(PRET);
}

shmline()
{
char *mlinemsg;
mlinemsg=
 "       CALL: mline(col,row,length,color,linedir,linetype);\n"
 "\nDraws a line on the screen starting at the col and row specified\n"
 "and extending in linedir (hornz, vert) for length spaces. The line\n"
 "will be drawn in the line type selected.\n"
 "\nAvailable line types are double,single and solid.These are defined\n"
 "in window.h";

 mwind(2,0,77,12,attr(cyan,black),attr(black,green),dtpbrdr);
 wtitle(center,top,attr(black,green)," MLINE ");
 pwindf(0,1,attr(yellow,black),"%s",mlinemsg);
 waitkey(PRET);
}

shmbox()
{
char *mboxmsg;
mboxmsg=
 "    CALL: mbox(col,row,endcol,endrow,bordercolor,bordertype);\n"
 "\nDraws a box on the screen starting at the col and row specified\n"
 "and extending to the diagonal corners specified by endcol and endrow.\n"
 "\nThe box will be drawn in the bordercolor and bordertype selected.\n"
 "\nThis function is used by mwind to draw the border and can be called\n"
 "separately to draw a box on the screen without defining a window.";

 mwind(2,10,77,23,attr(cyan,black),attr(black,green),dtpbrdr);
 wtitle(center,top,attr(black,green)," MBOX ");
 pwindf(0,1,attr(yellow,black),"%s",mboxmsg);
 waitkey(PRET);
}

shtitle()
{
char *titlemsg;
 titlemsg=
  "CALL: wtitle(justify, loc, color, \"TITLE\");\n"
  "Where justify= center,  left,  right.\n"
  "      loc    = top,     bottom\n"
  "      color  = any legal FG/BG color combination\n"
  "      TITLE = any string enclosed in quotes\n"
  "\ncenter,left,right,top,bottom are defined in window.h\n"
  "in both upper and lower case.";

 mwind(10,7,70,20,attr(cyan,black),attr(yellow,blue),sglbrdr);
 wtitle(center,top,bordercolor," WTITLE ");
 pwindf(0,1,attr(yellow,black),"%s",titlemsg);
 waitkey(PRET);
}

shshadow()
{
int y,z;
char *shmsg;
shmsg=
 " There is one special effect\n"
 "available called shadowing. This\n"
 "is set by the global var. (shadow).\n"
 "if shadow=TRUE then shadowing is\n"
 "in effect.";
 shadow=TRUE;

 mwind(20,7,59,16,attr(yellow,blue),attr(yellow,red),sldbrdr);
 wtitle(center,top,bordercolor," SHADOW ");
 pwindf(0,1,attr(yellow,blue),"%s",shmsg);
 waitkey(PRET);
 mwind(0,0,79,24,attr(white,blue),attr(white,blue),dblbrdr);
 for(y=2,z=0;z<=15;z+=6) {
   mwind(2,y+z,20,y+3+z,attr(white,red),attr(yellow,red),dtpbrdr);
   pwindfc(1,attr(yellow,blue),"shadow");
   mwind(28,y+z,48,y+3+z,attr(white,red),attr(yellow,red),dtpbrdr);
   pwindfc(1,attr(yellow,blue),"shadow");
   mwind(58,y+z,76,y+3+z,attr(white,red),attr(yellow,red),dtpbrdr);
   pwindfc(1,attr(yellow,blue),"shadow");
 }
 waitkey(PRET);
 for(y=0;y<10;y++)
    rwind();
 shadow = FALSE;
}

shattr()
{
char *attrmsg;
attrmsg=
 "  CALL: attr(fg color, bg color);\n"
 "Sets the color attributes. Two global\n"
 "var have been defined in window.h,\n"
 "windowcolor and bordercolor. You can\n"
 "assign these to specific colors\n"
 "i.e.windowcolor=attr(blue,red) or use\n"
 "the attr call directly as in\n"
 "wtitle(left,top,attr(white,red),\"TEST\")";

 mwind(20,7,61,18,attr(yellow,blue),attr(yellow,red),sldbrdr);
 wtitle(center,top,bordercolor," ATTR ");
 pwindf(0,1,attr(yellow,blue),"%s",attrmsg);
 waitkey(PRET);
}

shgowind()
{
char *gowindmsg;
 gowindmsg=
  "  CALL: gowindcr(int col, int row);     \n"
  "Places the cursor at the col and row    \n"
  "specified. These cordinates are with    \n"
  "respect to the window. i.e.gowindcr(0,0)\n"
  "is the top left corner of the window.     ";

 mwind(18,7,65,18,attr(yellow,blue),attr(yellow,red),sldbrdr);
 wtitle(center,top,bordercolor," GOWINDCR ");
 pwindfc(1,attr(yellow,blue),"%s",gowindmsg);
 setcursor(ON);
 gowindcr(0,0);
 putch('L');
 gowindcr(70,0);
 putch('R');
 gowindcr(0,12);
 putch('L');
 gowindcr(70,12);
 putch('R');
 gowindcr(15,6);
 waitkey(PRET);
 setcursor(OFF);
}

shmove()
{
char *movmsg;
 movmsg=
  "   CALL: movwind(newcol,newrow);\n"
  "Erases the current window and moves\n"
  "it to the col and row specified.\n"
  "Effects the current window only.\n"
  "Press Return for a demo of the move\n"
  "function.";

 mwind(20,7,59,16,attr(yellow,blue),attr(yellow,red),sldbrdr);
 wtitle(center,top,bordercolor," MOVWIND ");
 pwindf(0,1,attr(yellow,blue),"%s",movmsg);
 waitkey(PRET);
 movwind(0,0);
 sleep(1);
 movwind(40,11);
 sleep(1);
}

shrwind()
{
char *rwndmsg;
 rwndmsg=
  "       CALL: rwind();\n"
  "Removes last window placed on screen.\n"
  "There must be as many windows removed\n"
  "as their are windows created.\n";

 mwind(20,7,59,16,attr(yellow,blue),attr(yellow,red),sldbrdr);
 wtitle(center,top,bordercolor," RWIND ");
 pwindf(0,1,attr(yellow,blue),"%s",rwndmsg);
 waitkey(PRET);
}

shsetcursor()
{
char *setcmsg;
setcmsg=
 "   CALL: setcursor(on/off);\n"
 "\nTurns the cursor on and off.\n"
 "ON and OFF are defined in window.h,\n"
 "in both upper and lower case";

 mwind(18,7,59,16,attr(white,blue),bordercolor,dsdbrdr);
 wtitle(center,top,bordercolor," SETCURSOR ");
 pwindf(0,1,attr(white,blue),"%s",setcmsg);
 waitkey(PRET);
}

shmwind()
{
char *mwndmsg;
mwndmsg=
 "  CALL: mwind(col,row,endcol,endrow,windowcolor,bordercolor,bordertype);\n"
 "\nDraws a window on the screen starting at the col and row specified\n"
 "and extending to the diagonal corners specified by endcol and endrow.\n"
 "\nThe window will be drawn in the windowcolor with a border of \n"
 "bordercolor.\n\nPress return for bordertypes";

 mwind(2,8,77,21,attr(yellow,black),bordercolor,dsdbrdr);
 wtitle(center,top,bordercolor," MWIND ");
 pwindf(0,1,attr(yellow,black),"%s",mwndmsg);
 waitkey(PRET);
}
shinitwindow()
{
 char *iwndmsg;

  iwndmsg=
   "         CALL: initwindow();\n"
   "\nThis function must be called before\n"
   "any other routines in this package are\n"
   "used.It sets up the window arrays, video\n"
   "pointers, and checks the enviroment for\n"
   "a define WSNOW=remove. If WSNOW is set\n"
   "in the enviroment then all routines write\n"
   "to the screen using snow supression.";

 mwind(18,5,61,18,attr(white,black),bordercolor,dblbrdr);
 wtitle(center,top,bordercolor," INITWINDOW ");
 pwindf(0,1,attr(white,black),"%s",iwndmsg);
 waitkey(PRET);
}

shscroll()
{
int x;
char *scrlmsg;

 scrlmsg=
 "  With the two print window functions\n"
 "( pwindf and pwindfc ) specifying a row\n"
 "of -1 or a row equal to (var:rowend) cause the\n"
 "window to scroll. In the first case down and\n"
 "the second case up.\n"
 "  A scrolling demo is next. Watch it, the\n"
 "scroll goes fast";
 mwind(18,5,70,18,attr(white,black),bordercolor,dblbrdr);
 wtitle(center,top,bordercolor," SCROllING ");
 pwindf(0,1,attr(white,black),"%s",scrlmsg);
 waitkey(MORE);
 for(x=0;x<=40;x++)
   pwindf(0,rowend,attr(white,black),"scroll line up #%d",x);
 waitkey(PRET);
 for(x=0;x<=40;x++)
   pwindf(0,-1,attr(white,black),"scroll down line #%d",x);
 waitkey(PRET);
}


shclrscr()
{
char *clrmsg;
clrmsg=
 "CALL:clrscr(attr(FG,BG), bord color);\n"
 "\nClears screen and border to specified\n"
 "color. Will not clear individual\n"
 "windows. The colors are defined in\n"
 "window.h, in both upper and lower\n"
 "case.";

 mwind(18,4,59,15,attr(black,green),bordercolor,sldbrdr);
 wtitle(center,top,bordercolor," CLRSCR ");
 pwindf(0,1,attr(black,green),"%s",clrmsg);
 waitkey(PRET);
}

shclrwind()
{
char *clrwndmsg;
 clrwndmsg=
 "   CALL: clrwind(color);\n"
 "\nClears current window to\n"
 "selected color.";

 mwind(18,5,59,14,attr(yellow,blue),bordercolor,sldbrdr);
 wtitle(center,top,bordercolor," CLRWIND ");
 clrwind(blue);
 pwindf(0,1,attr(yellow,blue),"%s",clrwndmsg);
 waitkey(PRET);
}

shscreenmode()
{
char *scrmsg;
scrmsg=
 "   CALL: screenmode(size)\n"
 "\nSets the display screen mode.\n"
 "color80 and color40 are defined\n"
 "in window.h. The window functions\n"
 "support both 80 and 40 col. modes.";

 mwind(18,6,59,16,attr(cyan,red),bordercolor,sglbrdr);
 wtitle(center,top,bordercolor," SCREENMODE ");
 pwindf(0,1,attr(cyan,red),"%s",scrmsg);
 waitkey(PRET);
}

shborders(void)
{
 int x;

 bordercolor=attr(yellow,red);
 windowcolor=attr(white,blue);
 mwind(10,0,72,8,windowcolor,bordercolor,dblbrdr);
 pwindfc(0,windowcolor,"Border selection");
 pwindf(1,1,windowcolor,"1) mwind(10,10,30,14,bordercolor,bordercolor,nobrdr);");
 pwindf(1,2,windowcolor,"2) mwind(32,10,52,14,windowcolor,bordercolor,dblbrdr);");
 pwindf(1,3,windowcolor,"3) mwind(55,10,75,14,windowcolor,bordercolor,sglbrdr);");
 pwindf(1,4,windowcolor,"4) mwind(10,16,30,20,windowcolor,bordercolor,sldbrdr);");
 pwindf(1,5,windowcolor,"5) mwind(32,16,52,20,windowcolor,bordercolor,dtpbrdr);");
 pwindf(1,6,windowcolor,"6) mwind(55,16,75,20,windowcolor,bordercolor,dsdbrdr);");
 mwind(7,10,27,14,bordercolor,bordercolor,nobrdr);
 wtitle(left,top,bordercolor," #1 ");
 pwindfc(1,windowcolor,"No Border");
 mwind(30,10,50,14,windowcolor,bordercolor,dblbrdr);
 wtitle(left,top,bordercolor," #2 ");
 pwindfc(1,windowcolor,"Double Border");
 mwind(53,10,73,14,windowcolor,bordercolor,sglbrdr);
 wtitle(left,top,bordercolor," #3 ");
 pwindfc(1,windowcolor,"Single Border");
 mwind(7,16,27,20,windowcolor,bordercolor,sldbrdr);
 wtitle(left,top,bordercolor," #4 ");
 pwindfc(1,windowcolor,"Solid Border");
 mwind(30,16,50,20,windowcolor,bordercolor,dtpbrdr);
 wtitle(left,top,bordercolor," #5 ");
 pwindfc(1,windowcolor,"Mixed Border");
 mwind(53,16,73,20,windowcolor,bordercolor,dsdbrdr);
 wtitle(left,top,bordercolor," #6 ");
 pwindfc(1,windowcolor,"Mixed Border");
 waitkey(PRET);
 for(x=1;x<=7;x++)
   rwind();
}

shpullmenu(char index)
{
 char *usedef,*retcdef,*opdef,*exitdef,*pullsyn,*menudef;

 usedef=
  "  This is one way to use the character returned from the popupmenu\n"
  "function. This is part of the routine used in this program\n"
  "\nchar index;\n"
  "\nwhile((index = pullmenu(TOP,0) != NULL)\n"
  " switch(pullmenuloc)\n"
  " {\n"
  "   case 0:  /* The Global var pullmenuloc defines where the hilited\n"
  "               bar was when you hit return. Index is returned from\n"
  "               the function if you were in a sub menu at the time */\n"
  "     switch(index) {\n"
  "        case 'P':\n"
  "           mwind(0,11,79,23,windowcolor,bordercolor,sldbrdr);\n"
  "           wtitle(CENTER,TOP,bordercolor,\" POPUPMENU SYNTAX \");\n"
  "           pwindfc(0,windowcolor,\"CALLING CONVENTION\");\n"
  "           pwindf(0,1,windowcolor,\"%s\",popsyn);\n"
  "           waitkey(PRET);\n"
  "           rwind();\n"
  "        break;\n"
  "     }\n"
  " }\n";

  retcdef=
   "   The pull menu routine returns to the calling function with\n"
   "the character of the SUB menu item you selected.\n"
   "   For example if you select Defining a Menu the \"D\" is returned\n"
   "to the calling function and pullmenuloc is set to 0";

  opdef=
   "  To operate the menu, you use the left or right arrow keys\n"
   "to move the hilited bar and the enter key or down arrow\n"
   "to enter a SUB menu or make the selection. You may also\n"
   "enter the hilited character to make your selection\n";

  exitdef=
   "  If you are in a SUB menu and hit \" ESC \" you will return to the \n"
   "Main pull down menu or the calling function with a return value of\n"
   "NULL.";
  pullsyn =
        "pullmenu(location,menunum);\n"
        "\n WHERE:\n"
        "         location is TOP or BOTTOM\n"
        "         menunum is the number of the main menu (0 to 7)\n";

  menudef=
        "pullmainmenu =                     Initilize pointer.\n"
        "\"inTro\\n\"                       Define all menu selections\n"
        "\"Initilize\\n\"                   You can have up to seven (7)\n"
        "\"Screen\\n\"                      menu selections\n"
        "\"Windows\\n\" \n"
        "\"Printing\\n\" \n"
        "\"pOp Wind\\n\" \n"
        "\"Next Menu\\n\" \n"
        "\"TISWPON\\n\"  \n"
        "\n  The last line of menu definition defines the characters that\n"
        "can be directly entered to select an item from the menu, these must\n"
        "be all CAPS. All the characters in this last line must also be\n"
        "distinct. The coresponding letter in each menu selection must also\n"
        "be in CAPS.\n"
        "\n  For example the last line in this menu is \"TISWPON\" this\n"
        "coresponds to the T in \"inTro\", the I in \"Initilize\",\n"
        "the S in \"Screen\", the W in \"windows\" and ....\n";

    switch(index)
    {
     case 'P':
        mwind(0,11,79,23,windowcolor,bordercolor,sldbrdr);
        wtitle(CENTER,TOP,bordercolor," PULLMENU SYNTAX ");
        pwindfc(0,windowcolor,"CALLING CONVENTION");
        pwindf(0,1,windowcolor,"%s",pullsyn);
        waitkey(PRET);
        rwind();
        break;
      case 'D':
        mwind(0,0,79,24,windowcolor,bordercolor,sldbrdr);
        wtitle(CENTER,TOP,bordercolor," MENU DEFINITION ");
        pwindfc(0,windowcolor,"MENU SETUP");
        pwindf(0,1,windowcolor,"%s",menudef);
        waitkey(PRET);
        rwind();
        break;
      case 'M':
        mwind(0,9,79,16,windowcolor,bordercolor,sldbrdr);
        wtitle(CENTER,TOP,bordercolor," OPERATING MENU ");
        pwindf(0,1,windowcolor,"%s",opdef);
        waitkey(PRET);
        rwind();
        break;
      case 'R':
        mwind(0,9,79,16,windowcolor,bordercolor,sldbrdr);
        wtitle(CENTER,TOP,bordercolor," RETURN CODE ");
        pwindf(0,1,windowcolor,"%s",retcdef);
        waitkey(PRET);
        rwind();
        break;
      case 'U':
        mwind(0,0,79,24,windowcolor,bordercolor,sldbrdr);
        wtitle(CENTER,TOP,bordercolor," PROGRAM SETUP ");
        pwindf(0,1,windowcolor,"%s",usedef);
        waitkey(PRET);
        rwind();
        break;
      case 'E':
        mwind(0,10,79,14,windowcolor,bordercolor,sldbrdr);
        wtitle(CENTER,TOP,bordercolor," EXITING MENU ");
        pwindfc(0,windowcolor,"EXITING MENU WITHOUT A SELECTION");
        pwindf(0,1,windowcolor,"%s",exitdef);
        waitkey(PRET);
        rwind();
        break;
    }
}