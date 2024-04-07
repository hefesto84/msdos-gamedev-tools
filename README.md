# MSDOS Game Development Tools

This repository is a compilation of msdos game development tools ready to be used in DOSBox, DOSBOX-X, FreeDOS and MSDOS.

It includes:

* Div Games Studo 2.0 (Spanish Version, but the english translation is easy to find).
* Borland Turbo C compiler and editor.
* Assembly Tools.
* DJGPP compiler.
* CWSDPMI binary.
* Allegro Library (already prepared for DJGPP)
* RHIDE editor.
* A bat file to prepare the environment.
* Mesa Library (OpenGL support...without GPU!)

In order to use this repository you have to follow these steps:

# Instructions (DOSBox, DOSBox-X), MSDOS/FreeDOS below.

## Instructions for DOSBox, DOSBox-X

Mount the unit as floppy disk (it allows to enable the automatic rescan of the files if you want to use an external IDE)

### For Windows:

```mount H C:\msdos-gamedev-tools -t floppy```

### For Linux:

```mount H /home/your_username/msdos-gamedev-tools -t floppy```

### For OSX:

```mount H /Users/your_username/msdos-gamedev-tools -t floppy```

## Instructions for MSDOS / FreeDOS

Nothing special to do here. Just copy/paste the contents of the repository in your C drive

# Execution of the BAT file to prepare the environment

The SET_ENV.bat file is ready to use the H drive as base drive, but you can edit the file and set the diver letter to one of your choice.

Simply execute the file and you will be able to call all the .EXE files from wherever you are in the OS.

You can check that everything is going well typing :

# Execution of CWSDPMI to allow Protected DOS Mode

DJGGP creates binaries that requires to go beyond the 640kb memory limitation. You will need to execute the command CWSDPMI.exe to enable
this functionality or your EXE files will crash instantly.

```gcc --version```

# Building using GCC/GXX (assuming Allegro and Mesa lib ready to be used)

For C

```gcc -static -g -o filename.exe filename.c -lalleg -lmesa```

For C++

```gxx -static -g -o filename.exe filename.cc -lalleg -lmesa```

# Building using DIV2.EXE (it executes the full Div Games Studio Environment)

```D.EXE```

# Building for Assembly

All the binaries are ready to be use: LD, TC...

