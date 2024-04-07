            TEGL WINDOWS TOOLKIT II Release 2.0 Intro Pack

               Copyright 1991 TEGL SYSTEMS CORPORATION
                         All rights reserved.


This introductory package is for letting interested programmers try
out the TEGL WINDOWS TOOLKIT II before buying. You may use to create
programs on a trial basis. You may not sell or give away any programs
that use any portion of this toolkit.

This distribution contains virtually all the features of our commercial
version. It lacks: source code and documentation (we have provided
sample programs and a quick guide). Programs created from this intro
pack will display a copyright notice when they finish, a button with the
copyright is always installed in the application and if a program is run
for more than 20 minutes a copyright notice will appear.

This package may be distributed freely provided all files are included
and the distribution is not changed in any way.

Buy the TEGL WINDOWS TOOLKIT II and you will receive:

       * latest version, with COMPLETE SOURCE CODE
       * Reference manual
       * Technical support
       * Royalty free distribution rights on compiled applications

The TEGL WINDOWS TOOLKIT II Release 2.0 contains the following:

       * graphics interface (TGI), a replacement for the BGI
         thats 2-6x faster.
       * virtual memory manager (use EMS and hard disk)
       * window manager
       * event programming
       * icon editor
       * font editor
       * world coordinates
       * dialogue management
       * supports CGA, Hercules, EGA, VGA and SuperVGA
       * supports 256 colors (up to 1024x768)
       * 40+ fonts


This distribution contains no executable files. It is assumed that you
have the target compiler and can compile the example programs yourself.

Files in this distribution:
       readme.txt      This file
       intropak.com    Self extracting archive (contains example programs)
       tutor.com       Self extracting archive (contains example programs
                       and tutorial instuctions).
       header.com      Self extracting archive of the teglsys.h header file.

       ONE (or all of)
       tclib.exe       Self extracting archive for Turbo C 2.0
                       Turbo C++ 1.0 and Borland C++ 2.0
       mslib.exe       Self extracting archive for Quick C 2.0 (& later)
                       Microsoft C 5.1 & 6.0

INSTALLATION

       To install the intro pack just copy intropak.com, tutor.com,
       header.com and the appropriate library archive to a
       subdirectory on your harddrive then run each program.
       Running these programs extracts them to the disk. After it
       has run you will have over a 100 files taking up more than a
       megabyte of disk space. You don't have to extract them all
       into the same directory, its up to you.

       The tutorial files contain embedded example files that can
       be extracted using the extract.exe untiliy. There is also a
       batch file called 'exall.bat' that will extract all the
       example files from the tuturial.

       The files are:

       *.c             Example programs
       teglsys.h       The header file for the library
       teglsys.lib     The TEGL WINDOWS TOOLKIT II library.
       examples.txt    A list of the example programs with a note on
                       what aspect they illustrate.
       extract.exe     Program to extract example files
       order.txt       An order form.
       quick.txt       A quick overview of the system.
       menus.txt       The menuing system
       buttons.txt     How to use Square buttons
       picklist.txt    How to use picklists
       frames.txt      Introduction to frames
       twquick.txt     Quick introduction to the high level windows.

COMPILING THE PROGRAMS:

ONLY THE LARGE MEMORY MODEL IS SUPPORTED. Be sure to set this in the
integrated environment, the batch files use the command line toggle.
Additionally, byte alignment is used, word alignment with cause the
programs to hang.

We have provided a number of batch files for compiling the demo programs
using the command line compiler(s). They are:

tc2comp.bat    - Turbo C 2.0
tcccomp.bat    - Turbo C++ 1.0
bcccomp.bat    - Borland C++ 1.0
qc2comp.bat    - Quick C 2.0 (& later)
msccomp.bat    - Microsoft C 5.1 & 6.0

There are a NUMBER OF ASSUMPTIONS made in these batch files.

       1. The compiler is in the path.
       2. The teglsys.h and teglsys.lib are either in the current
          directory or in the compiler's directory.

The Turbo and Borland compilers search for a file named 'turboc.cfg' in
the directory they are started from. This file contains directives for
locating where header files and libary files are located. We assume that
you have set these.


Since the Microsoft compiler and linker use environment variables to
determine the where header files and library files are to be found you
must edit these to reflect how you system is set up.

TURBO AND BORLAND INTEGRATED ENVIRONMENTS:

To compile under the integrated environment you must create a project file
and name 'teglsys.lib' as part of it.

Depending on the amount of memory available in your system you may be able
to run some of the programs under the integrated environment. HINTS: be
sure debugging is off, and you may want to set the maxwindowsize variable
to a value of 16000. The protected mode compiler (bcx) usually has enough
memory to run these programs.

We have found the integrated environments tend to hang easily, don't be
surprised.

QUICK C INTEGRATED ENVIRONMENT:

The 'telgsys.h' header file is greater than the capacity of the Quick
C integrated environment. Complain to Microsoft. Only the command line
compiler can be used.



-----------------------------------------------------------------------------

                       TEGL SYSTEMS CORPORATION
                  Suite 780, 789 West Pender Street
                     Vancouver, British Columbia
                            Canada V6C 1H2
                         Phone (604) 669-2577
                       Facsimile (604) 688-9530
                   Technical Support (604) 681-0914



