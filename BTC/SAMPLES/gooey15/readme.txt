   
                                          DOS GUI  V 1.5
                                             
                        Copyright (c) 1996, 1997, 2003 by Glen E. Gardner, Jr.
                                   
                                     This program is freeware. 
                 The user may distribute any version of this software provided the  
                 original unaltered source, program and all of the files in this 
                 zipped archive are included in the distribution. 

                                     Glen.Gardner@verizon.net


                                      WARRANTY AND DISCLAIMER

                  This software is provided on an "as is" basis without warranty of any 
                  kind, expressed or implied, including but not limited to the implied 
                  warranties of merchantability and fitness for a particular purpose.  
                  The person using the software bears all risk as to the quality and 
                  performance of the software.  The author will not be liable for any 
                  special, incidental, consequential, indirect or similar damages due 
                  to loss of data or any other reason, even if the author or an agent 
                  of the author has been advised of the possibility of such damages.  


archive contents.

the archive "gooey.zip" contains the following:

GUI.EXE 	program executable.

GUI.INI 	button initialization file.

DESKTOP.INI  	Desktop initialization file

EGAVGA.BGI      graphics driver.

README.TXT  	This file



What is it?

DOS GUI (pronounced "gooey") is a graphical user interface for MS-DOS.  It supports 
640*480 resolution in 16 colors, and offers an easy to use "point-and-click" type of interface with 
user designable buttons, user programmable button functions, and a useful interface for the DOS
command line as well as the ability to launch DOS programs.  This program is FREEWARE.  Use it freely and enjoy.


What is it used for?

DOS GUI is intended to be used as a launchpad for MS-DOS utilities, applications, games, and operating systems (such as Windows 3.1X or OS/2).  DOS GUI is useful for those times when you need the speed of a graphical interface and the utility of the DOS command line.


What do I need to use it?

A fast PC/AT compatible with a 386 or higher processor, 640k ram, 1 mb free hard drive space, a VGA card capable of supporting 640*480 resolution in 16 colors, a two or three-button mouse, and MS-DOS 6.0 or higher (6.22 recommended).  A good mouse driver is recommended.  There are a number of cheezy mouse drivers out there that won't work well with DOS GUI. MICROSOFT has a good one, and so does LOGITECH. 

NOTE:   Version 1.5 will run on Windows 95' and Windows NT 4.0, but launching some applications with GUI.EXE may result in unpredictable behavior.  Most MS-DOS applications will work fine, but Win32 console programs may not exit nicely and often leave the GUI in an unpredictable state.



Installation

Unzip the archive "gooey.zip" and place all of the files at root on the drive containing the primary dos
partition.  For most people this will be "C:\".  DO NOT put the files in any directory, as GUI.EXE is not very "smart" and will sometimes have problems finding the path "home" after launching a program if the files are placed in a directory.

Add the path to start GUI.EXE to the DOS AUTOEXEC.BAT file: For most people this will be "C:\GUI.EXE".
Make sure this is the very last line in the autoexec.bat, and that the mouse driver is started
BEFORE GUI.EXE is executed.  When done, restart the computer and DOS GUI should be up and running!



Use

To activate a buttion, simply point and click with the left mouse button.  The button's programmed 
functions will then be executed. To exit DOS GUI, use a button programmed for the exit function, 
or press the escape key. To start DOS GUI from the command promt, type: "GUI" and press ENTER.



How it Works... READ THIS!!

The file DESKTOP.INI contains the information needed to configure the desktop;  Background Color, 
Text color, and function availability.  When GUI.EXE starts (or refreshes) it reads this file and sets up 
the desktop according to the values in this file.  You can use DOS EDIT to change the values to suit your preferences.

The form for an entry in a button list is: DESCRIPTION:<space>ENTRY
Note that EXACTLY ONE, and ONLY ONE space MUST be between the description string and the entry value.

DESKTOP.INI

begin

BackgroundColor: 0
TimeColor: 4
AboutColor: 3
DisplayTime: 1
DisplayAbout: 1
DriverPath: C:\

end


BackgroundColor:  Sets the color of the screen background (0-15)
TimeColor:        Sets the color of the time/date display (0-15)
AboutColor:       Controls the color of the About and Mouse displays (0-15)
DisplayTime:      Enables time/date display 1=ON  0=OFF
DisplayAbout:     Enables Mouse/About dialog display  1=ON 0=OFF
DriverPath:       The path to "EGAVGA.BGI", the graphics driver.  For most users:  "C:\".


READ THIS! (continued)

After the desktop settings are loaded, GUI.EXE starts the graphics mode and reads the file "GUI.INI".
This file contains an entry for each button appearing on the screen.  The size, color, position,
labelling, and function for each button is determined by the contents of it's entry.  Any number
of buttons is permitted, and the execution order of overlapping buttons is determined by list order.
DOS EDIT , or any text editor can be used to edit the button entries in GUI.INI.  New buttons can be added by simply copying and pasting a previous button entry list to the end of the entry list, then editing it as needed.


The form for an entry in a button list is: DESCRIPTION:<space>ENTRY
Note that EXACTLY ONE, and ONLY ONE space MUST be between the description string and the entry value.

A typical entry for a button is listed below;

begin..
 
ButtonX: 300                   
ButtonY: 30
Height: 30
Width: 80
ButtonColor: 7
BorderColor: 15
TextColor: 1
Label: PLAY
Launch: C:\GAMES\MYGAME.EXE
CommandLine: NONE
Exit: 0
ToggleDisplay: 0
EnableMouse: 0
EnableAbout: 0
TimeDisplay: 0

end..

All about entries...


ButtonX:      Sets the X coordinate (up/down) of the upper-left-hand corner of the button (0-640).

ButtonY:      Sets the Y coordinate (left/right) of the upper-left-hand corner of the button (0-480).

Height:       The height of the button (in pixels), 480, max.

Width:        The width of the button (in pixels), 640 max.

ButtonColor:  The color of the button (0-15).

BorderColor:  The color of the border around the edge of the button (0-15).

TextColor:    The color of the text label for the button (0-15).

Label:        The text label for the button.  May not contain any white space charachters.  
	      Using the label "NONE" (all upper case) results in a blank button with no label.

Launch:       The full path and name of the program to be launched when the button is clicked.  
	      The word "NONE" (all uppercase) disables the launch option.

CommandLine:  The MS-DOS command line is fully supported here. An entry of "NONE" (all uppercase) 
              disables the command line function.

Exit:         1=Exit DOS GUI when clicked.  0=Don't exit.

ToggleDisplay: 1=Toggle the display dialog between mouse status display and "about" when clicked.  
	       0=Toggle Disabled.

EnableMouse:   1=Toggle mouse status display on/off when clicked. 0=Toggle Disabled.

EnableAbout:   1=Toggle About dialog on/off when clicked. 0=Toggle Disabled.

TimeDisplay:   1=Toggle time/date display on/off when clicked. 0=Toggle Disabled.


READ THIS!  (continued).

While GUI.EXE is reading GUI.INI, it renders each button to the screen as it finds it in the file.
The order in whick the buttons are drawn and executed is the same as the order of the list , top-down.
When GUI.EXE is finished reading the list, the program will idle, scanning for the escape key (exit) for a change in mouse status while servicing the desktop clock.  
DOS GUI is event driven.  When a mouse button is clicked, GUI.EXE will read GUI.INI one button at a time, and determine if the mouse was clicked inside that button.  If the left mouse button was clicked inside one of the GUI buttons, it's enabled functions are carried out, and GUI.EXE moves on to check the next button, and so on , until the list is finished.  At the same time, the program will refresh the screen.  If the LAUNCH function is enabled for a particular button, the graphics mode and mouse driver will be reset, and the desktop and screen refreshed upon returning from the child process.  

Note that each button allows multiple functions to be enabled simultaneously.  The order in which these functions will be carried out are listed below.

Button function Priority list

1) Enable/disable "About" dialog display
2) Enable/disable mouse status display
3) Enable/disable time/date display
4) Toggle between About/Mouse display
5) Launch a program
6) Exit DOS GUI


READ THIS! (continued).

Any combination of these button functions may be enabled.  However, the priority ranking always
remains the same.  Multiple buttons may have the same functions enabled, or any combination therof.
Buttons my be overlayed.  That is, several buttons may share the same X-Y coordinates on the 
screen, with only the topmost button actually visible.  When such an overlay is clicked on, ALL of 
the buttons in the overlay are executed, in the order they appear in GUI.INI. In turn, the button 
functions for each button will be executed according to the priority list.


Errata:

This program was compiled using Borland C++ 4.52 as a DOS .EXE file.  GUI.EXE was 
written in ANSI C, with Borland extensions for dos function calls, and the Borland Graphics Interface for the GUI.

This version replaces the v1.0 beta (DOS only) version, and is not compatible with it's .ini files.

Known Bugs & Recent Fixes:  

GUI.EXE has been recompiled to run on computers with an intel 386 
processor and 640k memory.  Slow 386 machines may experience poor
performance with GUI.EXE.

Some users experienced problems with the mouse pointer disappearing
when gui.exe was started.  This was caused by the program not setting
the mouse graphics mode.  The program was changed to make sure the mouse
mode was in agreement with the graphics mode.  The source code reflects 
this change.

Users launching programs on a second hard drive may experience problems
with the path not being found.  In this situation, use gui.exe to launch 
a .bat file that calls the desired program and then reset the path back
to that needed to find GUI and it's files.  This also allows the gui files
to be placed in a directory , instead of at root.

An example .bat file to launch a program on D drive in the "stuff" 
directory, with gui.exe on drive C:


begin example

d:
cd stuff
myprog.exe
cd ..
c:

end example

This example was chosen for clarity..  Smart dos users will find 
more efficient ways of implimenting a .bat file..


Running out of memory after starting one or more dos sessions from 
DOS GUI...

Dos sessions can be started from GUI.EXE by launching COMMAND.COM.
Additionally, GUI.EXE can be called and run from that DOS session..
Eventually, the computer runs out of memory and will refuse to launch
any further sessions (usually after 4 or 5 gui sessions are running).

This is not really a bug, but a feature of DOS.

When leaving a DOS session that was launched by GUI.EXE, type "EXIT"
to end the dos session and return to the GUI, thus freeing up the 
memory used up by the dos session.

If you run short on memory after only one gui session is launched, 
it may be that much of your conventional memory is being used by tsr's
or drivers. Try removing unneeded tsr's and device drivers to 
make more memory available. (remember: 640k ought to be enough 
for anybody)


End of bug report



