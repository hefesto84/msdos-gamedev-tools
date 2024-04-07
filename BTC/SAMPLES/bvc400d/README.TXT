      ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
      ³                                                               ³
      ³                      OptiVec for C / C++                      ³
      ³                                                               ³
      ³                      Shareware Version 4                      ³
      ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

Contents:
     1. Introduction
     2. System requirements
     3. Installation
     4. Running the examples
     5. Documentation
     6. What's New?
     7. Copyright
     8. Registered Version

1. Introduction
---------------
Use vector functions instead of loops - if it matters to you how fast and
how accurate your data are processed!
The largest portion of almost all programs processing numerical data
consists of loops. Replace these loops by the specialized vector
and matrix functions of OptiVec, and exploit all the advantages of
vectorized programming:

- OptiVec was almost entirely written in Assembly language. The result is a
  highly optimized and numerically accurate code, running 2-3 times faster
  than compiled loops.

- fully vectorized forms of all operators and functions of the language
  C/C++ for all integer and floating-point (including complex!) data types

- Additionally many other real and complex math functions, statistics,
  analysis, FFT techniques, graphics - in total more than 3000!

- Simple and logical syntax, for example
  "VF_exp( Y, X, size );"      for Y[i] = exp( X[i] ) with "float" arrays,
  "VD_FFT( Y, X, size, 1 );"   for double-precision forward FFT
  "ME_mulM( MZ, MX, MY, htX, lenX, lenY );"
                               for extended-precision matrix multiplication

2. System requirements
----------------------
This version of OptiVec is designed for IBM AT, PS/2 or compatibles,
equipped at least with a 80386 CPU and a 80387 coprocessor
(Visual C++ version: at least 486DX).

Depending on your choice when ordering or downloading,
- either for Borland C++ (Turbo C++), version 3.0 or higher, memory
  models Win32, Windows-LARGE, or  DOS-LARGE
- or for Microsoft Visual C++, version 5.0 or higher, "single-thread debug"
  or "multi-thread debug"

Registered version for Borland C/C++:
all memory models; separate libraries optimized for four degrees of
back-compatibility from Pentium III down to 286 with or without coprocessor.

Registered version for Microsoft Visual C++:
all PC models using the "single-thread", "multi-thread", or
"multi-thread DLL" run-time libraries, both for debug and release, in
versions for Pentium 4+, Pentium III+, and for 486DX/Pentium+.


3. Installation
---------------
Please run the installation program, INSTALL.EXE.
INSTALL.EXE moves all OptiVec files into their correct subdirectory
and starts the clock for the trial period.
You may change the default directory structure by hand when the
installation is complete.
To install OptiVec on Windows NT, 2000, XP etc., you need to log in
with administrator privileges.

After you completed the installation, you must set the library search path
and the include-file search path according to your OptiVec directory choice:
Say, your OptiVec directory is C:\OPTIVEC. Then, your search paths are:
C:\OPTIVEC\LIB          for the libraries,
C:\OPTIVEC\INCLUDE      for the include-files.

Add these paths to the standard settings in the menu
"Extras / Options / Directories" in MS Visual C++ or 
"Options / Project / Directories" of Borland C++ and BC++ Builder
(remember: in Borland C++, a semicolon is used to separate entries in
these fields.)

For MS Visual C++, you also have to include the Windows API import library:
In the menu   Project / Settings / Linker / Object and Library modules,
you have to add    user32.lib   (if it is not yet there).
Otherwise you would get the linker error
LNK2001: Unresolved external symbol __imp__MessageBoxA@16??


4. Running the examples
-----------------------

Check your installation by compiling and running the appropriate demo file.
Follow the instructions in the header of that file.
If you get compiler errors like "Cannot open include file ...", or a
linker error "Unresolved external", you probably did not set the library
and include paths correctly.

OptiVec for Borland C/C++:
  VDEMOB.BPR  is a BC++ Builder 4+ project,
  VDEMOW.CPP  is for Windows with Borland C++ 5.0 or higher.
  VDEMO.CPP   is a DOS program,
  Separate versions of the Windows demo program are supplied for
  older BC versions:
  VDEMOW1.CPP is for Borland C++ 3.x with OWL 1.0
  VDEMOW2.CPP is for Borland C++ 4.0 with OWL 2.0

  For BC++ Builder 4+, open the project VDEMOB.BPR, compile and run.

  For Windows95/98/2000/XP, or NT with Borland C++ 4.x, 5.x or higher
      (without OWL),
      either do a command-line compile:
          a) 32-bit: type
               BCC32 -W -Iinclude vdemow.cpp lib\vcf3w.lib
          b) 16-bit: type
               BCC -ml -W -Iinclude vdemow.cpp lib\vcl3w.lib
      and run the program by typing:  vdemow

      or create a project containing
      VDEMOW.CPP and VCF3W.LIB.
      In the TargetExpert, choose as target
        Application [.EXE]  for Win32, GUI model.
        Tick the boxes for Standard Libraries ClassLibrary, and Runtime.
        Use Static linking and Single-Thread

  For Windows95/98/2000/XP or NT with Borland C++ 4.x and OWL 2.0
      create a project containing
      VDEMOW2.CPP and VCF3W.LIB.
      In the TargetExpert, choose as target
        Application [.EXE]  for Win32, GUI model.
        Tick the boxes for Standard Libraries OWL, ClassLibrary, and Runtime.
        Use Static linking and Single-Thread (which means: don't check the
        Multithread box).

  For 16-bit Windows with Borland C++ 4.x, 5.x or higher (without OWL)
      create a project containing
      VDEMOW.CPP and VCL3W.LIB.
      In the TargetExpert, choose as target
        Application [.EXE]  for Windows 3.x, model LARGE.
        Tick the boxes for Standard Libraries ClassLibrary, and Runtime.
        Use, as you prefer, either Dynamic or Static linking.

  For DOS, create a project containing
     VDEMO.CPP,  VCL3.LIB, and  GRAPHICS.LIB.
     Choose the application either as DOS Standard or as DOS Overlay.
     VCL3.LIB may or may not be used as an overlay (option to be set:
     "Project/Local Options/Overlay this module").
     Choose the memory  model LARGE.
     Check the floating-point options. Neither "No floating-point" nor
     "Floating-point emulation" should be selected.

  For 16-bit Windows with Borland C++ 4.x and OWL 2.0
      create a project containing
      VDEMOW2.CPP and VCL3W.LIB.
      In the TargetExpert, choose as target
        Application [.EXE]  for Windows 3.x, model LARGE.
        Tick the boxes for Standard Libraries OWL, ClassLibrary, and Runtime.
        Use, as you prefer, either Dynamic or Static linking.


OptiVec for Microsoft Visual C++:
    create a project as a "Win32 application", containing
    VDEMOW.CPP  and  OVVCSD.LIB.
    Choose single-thread debug.

Data-fitting functions demo:
    There is a demo program available for the data-fitting functions,
    FITDEMO.CPP (for Borland C++, DOS), or FITDEMOW.CPP for Windows.
    The latter file can be used with Borland C++ (4.x or higher) or Visual C++,
    under Windows 3.1 or Windows 95/98/2000/NT/XP.
    With BC++ Builder, you can alternatively open the project FITDEMOB.BPR.

    Follow the instructions given above for VDEMOW.CPP and VDEMOB.BPR.
    With 16-bit BC++, you must additionally include the library MCL3.LIB
    (for DOS) or MCL3W.LIB (for Windows).


5. Documentation
----------------
The full OptiVec documentation is contained in the files
HANDBOOK.HTM, FUNCREF.HTM, MATRIX.HTM, and CMATH.HTM to be read with
an HTML browser like Netscape or IE.


6. What's New?
--------------
Version 4.0:
a) Pentium 4 libraries
b) Mixed floating-point/integer Arithmetic operations like VF_addVI, VD_mulVUS
c) The rounding-to-integer functions now employ "silent" saturation instead of
   generating an error message in the case of overflow.
d) Visual C++ version only: the data type quad is now equivalent to __int64.

Version 3.3.4 / 3.3.5:
e) Bug fix in V?_spectrum (only 32-bit versions affected)
f) Compatibility problem with Visual C++ .NET 2003 fixed

Version 3.3.2 / 3.3.3:
g) Bug fixes in MDp_FFT and MEp_FFT  (MFp_FFT was not affected),
   in the P6 version of VF_rotateCoordinates, and in V_free (there
   was a bug affecting thread safety in multithread applications)
h) New function VF_powexp for the calculation of x^r * exp(x)

Version 3.3:
i) Extension of VecObj for matrices
k) Parallel-enhanced series of Fourier-Transform methods, both one- and
   two-dimensional; 1.5 to 2.5 times as fast as previous FFT routines
l) Bug fixes in the complex accumulation functions and several of the
   mixed-type accumulation functions (VCF_accV, VF_accVLI etc.)
m) accumulation of two vectors at once: VF_acc2V
n) addition / subtraction of two vectors at once: VF_add2V, VF_sub2V
o) coordinate transformation: VF_rotateCoordinates, VCF_rotateCoordinates

Version 3.2.3:
p) Faster matrix inversion
q) MF_block_equM,  MF_block_equMT,  MF_equMblock,  MF_equMblockT etc.:
   functions to extract blocks from matrices or copy them back
   (you need no longer use MF_submatrix and MF_submatrix_equM for
   that purpose).
r) complex random numbers: VCF_random etc.;
   initialization of matrices with random numbers: MF_random etc.

Version 3.2.2:
s) Function MFsym_sqrt
t) Bug fixes in the P6 version of VFu_sqrt, VF_cmp_inclrange0C, ...CC,
   VF_cmp_exclrange0C, ...CC, VCDx_divV, and V?_derivC
u) All nonlinfit functions check if the "best guess" A values lie within
   the specified limits before chi-square is calculated for the first time.
   Thereby, possible failure due to input A values outside the limits is
   avoided.


7. Copyright
------------
The copyright owner of this product as a whole and of all its constituent
parts is
         OptiCode
         Dr. Martin Sander Software Development
         Steinachstr. 9A
         D-69198 Schriesheim
         Germany
         e-mail: MartinSander@optivec.com

This Shareware version of OptiVec is freely distributable in unchanged form.
For the distribution of applications created using OptiVec, you need the
registered version. The detailed licence terms are described in chapter 1.2
of the file HANDBOOK.HTM.


8. Registered Version
---------------------
If you like OptiVec and decide to use it, please be fair and register.
The registered version

-  supports all memory models of Windows95, NT, 3.x, and DOS of Borland C++,
   or all PC debug and release targets of Visual C++, respectively.

-  has individually optimized libraries for each degree of processor
   backward-compatibility:
      Pentium 4+
      Pentium III+
      486DX/Pentium+
      386+ (387 coprocessor required, Borland C++ only)
      286+ (no coprocessor required, Borland C++ only).

-  comes with printed documentation.

-  entitles you to two years of free updates
   (by downloading from our web site)

-  costs USD 249 or EUR 199 for the commercial edition,
         USD  99 or EUR  89 for the educational edition,
   and can be ordered by e-mail from the author or through the
   following secure-server Internet sites:

   SWREG:
   OptiVec for Borland C++ or MSVC:
       http://www.swreg.org/soft_shop/47/shopscr3.shtml

   ShareIt:
   OptiVec for Borland C++:
      http://www.shareit.com/programs/101557.htm         (English handbook)
      http://www.shareit.com/deutsch/programs/101556.htm (German handbook)
   OptiVec for MSVC:
      http://www.shareit.com/programs/103421.htm

See chapter 1.3 of the file HANDBOOK.HTM for further details about
ordering.


    * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

Copyright (C) OptiCode - Dr. Martin Sander Software Dev. 1996-2004
