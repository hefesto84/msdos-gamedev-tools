     ______   ___    ___
    /\  _  \ /\_ \  /\_ \
    \ \ \L\ \\//\ \ \//\ \      __     __   _ __   ___ 
     \ \  __ \ \ \ \  \ \ \   /'__`\ /'_ `\/\`'__\/ __`\
      \ \ \/\ \ \_\ \_ \_\ \_/\  __//\ \L\ \ \ \//\ \L\ \
       \ \_\ \_\/\____\/\____\ \____\ \____ \ \_\\ \____/
	\/_/\/_/\/____/\/____/\/____/\/___L\ \/_/ \/___/
				       /\____/
				       \_/__/     Version 3.12


		A game programming library.

	      By Shawn Hargreaves, 1994/1999.

		See the AUTHORS file for a
	       complete list of contributors.



#include <std_disclaimer.h>

   "I do not accept responsibility for any effects, adverse or otherwise, 
    that this code may have on you, your computer, your sanity, your dog, 
    and anything else that you can think of. Use it at your own risk."



======================================
============ Introduction ============
======================================

   Allegro is a library of functions for use in computer games, written in a 
   mixture of C and assembly language. This is version 3.12, for the djgpp 
   compiler: see the NEWS file for a list of differences from the previous 
   release. There are also ports to win32 using DirectX, and to X-Windows: 
   see http://www.talula.demon.co.uk/allegro/ for more information about 
   these versions. A wide range of extension packages and add-on modules are 
   also available, which can be found in the "Library Extensions" section of 
   the Allegro site, http://www.talula.demon.co.uk/allegro/.

   According to the Oxford Companion to Music, Allegro is the Italian for 
   "quick, lively, bright". It is also a recursive acronym which stands for 
   "Allegro Low Level Game Routines".



==================================
============ Features ============
==================================

   Supports VGA mode 13h, mode-X (twenty three tweaked VGA resolutions plus 
   unchained 640x400 Xtended mode), and SVGA modes with 8, 15, 16, 24, and 
   32 bit color depths, taking full advantage of VBE 2.0 linear framebuffers 
   and the VBE/AF hardware accelerator API if they are available. Additional 
   video hardware support is available from the FreeBE/AF project 
   (http://www.talula.demon.co.uk/freebe/).

   Drawing functions including putpixel, getpixel, lines, rectangles, flat 
   shaded, gouraud shaded, and texture mapped polygons, circles, floodfill, 
   bezier splines, patterned fills, masked, run length encoded, and compiled 
   sprites, blitting, bitmap scaling and rotation, translucency/lighting, 
   and text output with proportional fonts. Supports clipping, and can draw 
   directly to the screen or to memory bitmaps of any size.

   Hardware scrolling, mode-X split screens, and palette manipulation.

   FLI/FLC animation player.

   Plays background MIDI music and up to 64 simultaneous sound effects, and 
   can record sample waveforms and MIDI input. Samples can be looped 
   (forwards, backwards, or bidirectionally), and the volume, pan, pitch, 
   etc, can be adjusted while they are playing. The MIDI player responds to 
   note on, note off, main volume, pan, pitch bend, and program change 
   messages, using the General MIDI patch set and drum mappings. Currently 
   supports Adlib, SB, SB Pro, SB16, AWE32, MPU-401, ESS AudioDrive, Ensoniq 
   Soundscape, and software wavetable MIDI.

   Easy access to the mouse, keyboard, joystick, and high resolution timer 
   interrupts, including a vertical retrace interrupt simulator.

   Routines for reading and writing LZSS compressed files.

   Multi-object data files and a grabber utility.

   Math functions including fixed point arithmetic, lookup table trig, and 
   3d vector/matrix manipulation.

   GUI dialog manager and file selector.



===================================
============ Copyright ============
===================================

   Allegro is gift-ware. It was created by a number of people working in 
   cooperation, and is given to you freely as a gift. You may use, modify, 
   redistribute, and generally hack it about in any way you like, and you do 
   not have to give us anything in return. However, if you like this product 
   you are encouraged to thank us by making a return gift to the Allegro 
   community. This could be by writing an add-on package, providing a useful 
   bug report, making an improvement to the library, or perhaps just 
   releasing the sources of your program so that other people can learn from 
   them. If you redistribute parts of this code or make a game using it, it 
   would be nice if you mentioned Allegro somewhere in the credits, but you 
   are not required to do this. We trust you not to abuse our generosity.



============================================
============ Supported hardware ============
============================================

   The bare minimum you need to use Allegro is a 386 with a VGA graphics 
   card, but a 486 is strongly recommended. To get into SVGA modes you will 
   need a compatible SVGA card, which means something that has a working 
   VESA or VBE/AF driver.

   Ideally you should use VBE/AF, because it allows Allegro to use hardware 
   acceleration functions to speed up the drawing. The FreeBE/AF project 
   (http://www.talula.demon.co.uk/freebe/) provides a number of free VBE/AF 
   drivers (volunteers to write more are always welcome!), and accelerated 
   drivers for a large number of cards are available commercially as part of 
   the SciTech Display Doctor package (http://www.scitechsoft.com/).

   If you have a VBE 2.0 or VBE 3.0 driver you are probably fine just using 
   that, although unlike VBE/AF it won't provide any hardware acceleration. 
   If you have an older VESA BIOS implementation (eg. VESA 1.2), beware. For 
   one thing, everything will be much slower if Allegro can't use the sexy 
   VBE 2.0 features. For another, I could go on all day telling horror 
   stories about the buggy and generally just pathetic VESA implementations 
   that I've come across. If you are having trouble with the SVGA modes, try 
   getting a copy of the SciTech Display Doctor and see if that clears 
   things up (it probably will: SciTech usually get these things right).

   Note that the native SVGA chipset drivers from Allegro 3.0 and earlier 
   have been removed. These are still available as an optional add-on 
   package from the same sites as Allegro, but are not needed any more 
   because you can get the same code in a more flexible format as part of 
   the FreeBE/AF project.

   On the sound front, Allegro supports sample playback on the SB (mono), 
   the SB Pro (stereo), the SB16, the ESS AudioDrive, and the Ensoniq 
   Soundscape. It has MIDI drivers for the OPL2 FM synth (Adlib and SB 
   cards), the OPL3 (Adlib Gold, SB Pro-II and above), the pair of OPL2 
   chips found in the SB Pro-I, the AWE32 EMU8000 chip, the raw SB MIDI 
   output, and the MPU-401 interface, plus it can emulate a wavetable MIDI 
   synth in software, running on top of any of the supported digital 
   soundcards. The Creative Labs SB PCI-64 and PCI-128 cards are actually 
   based on the Ensoniq chipset, so they can be used with the Soundscape and 
   MPU-401 drivers. You'll have to manually select the MPU, though, because 
   it won't be autodetected. Actually, quite a lot of wavetable boards 
   emulate the MPU, so give this a try and see if it works on your card. If 
   you feel like coming up with drivers for any other hardware, they would 
   be much appreciated.

   Audio recording is supported for all SB cards, but only in unidirectional 
   mode, ie. you cannot simultaneously record and playback samples. MIDI 
   input is provided by the MPU-401 and SB MIDI drivers, but there are some 
   restrictions on this. The SB MIDI interface cannot be used at the same 
   time as the digital sound system, and the MPU will only work when there 
   is an IRQ free for it to use (this will be true if you have an SB16 or 
   greater, or if no SB-type digital driver is installed, or if your MIDI 
   interface uses a different IRQ to the SB).

   You may notice that this release contains some code for building a Linux 
   version, but don't bother trying this: it won't work! A _lot_ more work 
   is needed before Allegro will be usable under Linux. See the "work in 
   progress" section of the Allegro website for the latest information about 
   the Linux port.



============================================
============ Installing Allegro ============
============================================

   To conserve space I decided to make this a source-only distribution, so 
   you will have to compile Allegro before you can use it. To do this you 
   should:

   - Go to wherever you want to put your copy of Allegro (your main djgpp 
     directory would be fine, but you can put it somewhere else if you 
     prefer), and unzip everything. Allegro contains several subdirectories, 
     so you must specify the -d flag to pkunzip.

   - If you are using PGCC, uncomment the definition of PGCC at the top of 
     the makefile, or set the environment variable "PGCC=1".

   - Type "cd allegro", followed by "make". Then go do something 
     interesting while everything compiles. If all goes according to plan 
     you will end up with a bunch of test programs, some tools like the 
     grabber, and the library itself, liballeg.a.

     If you have any trouble with the build, look at faq.txt for the 
     solutions to some of the more common problems.

   - If you want to use the sound routines or a non-US keyboard layout, it 
     is a good idea to set up an allegro.cfg file: see below.

   - If you want to read the Allegro documentation with the standalone Info 
     viewer, edit the file djgpp\info\dir, and in the menu section add the 
     lines:

	 * Allegro: (allegro.inf).
		 The Allegro game programming library

   - If you want to read the Allegro documentation with the Rhide online 
     help system, go to the "Help / Syntax help / Files to search" menu, and 
     add "allegro" after the existing "libc" entry (separated by a space).

   - If you want to create the HTML documentation as one large allegro.html 
     file rather than splitting it into sections, edit docs\allegro._tx, 
     remove the @multiplefiles statement from line 8, and run make again.

   - Once the build is finished you can recover some disk space by running 
     "make compress" (which uses the DJP or UPX programs to compress the 
     executable files), and/or "make clean" (to get rid of all the temporary 
     files and HTML format documentation).

   To use Allegro in your programs you should:

   - Put the following line at the beginning of all C or C++ files that use 
     Allegro:

	 #include <allegro.h>

   - If you compile from the command line or with a makefile, add '-lalleg' 
     to the end of the gcc command, eg:

	 gcc foo.c -o foo.exe -lalleg

   - If you are using Rhide, go to the Options/Libraries menu, type 'alleg' 
     into the first empty space, and make sure the box next to it is checked.

   See allegro.txt for details of how to use the Allegro functions, and how 
   to build a debug version of the library.



=======================================
============ Configuration ============
=======================================

   When Allegro initialises the keyboard and sound routines it reads 
   information about your hardware from a file called allegro.cfg or 
   sound.cfg. If this file doesn't exist it will autodetect (ie. guess :-) 
   You can write your config file by hand with a text editor, or you can use 
   the setup utility program.

   Normally setup.exe and allegro.cfg will go in the same directory as the 
   Allegro program they are controlling. This is fine for the end user, but 
   it can be a pain for a programmer using Allegro because you may have 
   several programs in different directories and want to use a single 
   allegro.cfg for all of them. If this is the case you can set the 
   environment variable ALLEGRO to the directory containing your 
   allegro.cfg, and Allegro will look there if there is no allegro.cfg in 
   the current directory.

   The mapping tables used to store different keyboard layouts are stored in 
   a file called keyboard.dat. This must either be located in the same 
   directory as your Allegro program, or in the directory pointed to by the 
   ALLEGRO environment variable. If you want to support different 
   international keyboard layouts, you must distribute a copy of 
   keyboard.dat along with your program.

   Various translations of things like the system error messages are stored 
   in a file called language.dat. This must either be located in the same 
   directory as your Allegro program, or in the directory pointed to by the 
   ALLEGRO environment variable. If you want to support non-English versions 
   of these strings, you must distribute a copy of language.dat along with 
   your program.

   See allegro.txt for details of the config file format.



================================================
============ Notes for the musician ============
================================================

   The OPL2 synth chip can provide either nine voice polyphony or six voices 
   plus five drum channels. How to make music sound good on the OPL2 is left 
   as an exercise for the reader :-) On an SB Pro or above you will have 
   eighteen voices, or fifteen plus drums. Allegro decides whether to use 
   drum mode individually for each MIDI file you play, based on whether it 
   contains any drum sounds or not. If you have an orchestral piece with 
   just the odd cymbal crash, you might be better removing the drums 
   altogether as that will let Allegro use the non-drum mode and give you an 
   extra three notes polyphony.

   When Allegro is playing a MIDI file in looped mode, it jumps back to the 
   start of the file when it reaches the end of the piece. To control the 
   exact loop point, you may need to insert a dummy marker event such as a 
   controller message on an unused channel.

   All the OPL chips have very limited stereo capabilities. On an OPL2, 
   everything is of course played in mono. On the SB Pro-I, sounds can only 
   be panned hard left or right. With the OPL3 chip in the SB Pro-II and 
   above, they can be panned left, right, or centre. I could use two voices 
   per note to provide more flexible panning, but that would reduce the 
   available polyphony and I don't want to do that. So don't try to move 
   sounds around the stereo image with streams of pan controller messages, 
   because they will jerk horribly. It is also worth thinking out the 
   panning of each channel so that the music will sound ok on both SB Pro-I 
   and OPL3 cards. If you want a sound panned left or right, use a pan value 
   less than 48 or greater than 80. If you want it centred, use a pan value 
   between 48 and 80, but put it slightly to one side of the exactly central 
   64 to control which speaker will be used if the central panning isn't 
   possible.

   The DIGMID wavetable driver uses standard GUS format .pat files, and you 
   will need a collection of such instruments before you can use it. This 
   can either be in the standard GUS format (a set of .pat files and a 
   default.cfg index), or a patches.dat file as produced by the pat2dat 
   utility. You can also use pat2dat to convert AWE32 SoundFont banks into 
   the patches.dat format, and if you list some MIDI files on the command 
   line it will filter the sample set to only include the instruments that 
   are actually used by those tunes, so it can be useful for getting rid of 
   unused instruments when you are preparing to distribute a game. See the 
   Allegro website for some links to suitable sample sets.

   The DIGMID driver normally only loads the patches needed for each song 
   when the tune is first played. This reduces the memory usage, but can 
   result in a longish delay the first time you play each MIDI file. If you 
   prefer to load the entire patch set in one go, call the 
   load_midi_patches() function.

   The CPU sample mixing code can support between 1 and 64 voices, going up 
   in powers of two (ie. either 1, 2, 4, 8, 16, 32, or 64 channels). By 
   default it provides 8 digital voices, or 8 digital plus 24 MIDI voices (a 
   total of 32) if the DIGMID driver is in use. But the more voices, the 
   lower the output volume and quality, so you may wish to change this by 
   calling the reserve_voices() function or setting the digi_voices and 
   midi_voices parameters in allegro.cfg.



======================================
============ Contact info ============
======================================

   The latest version of Allegro can always be found on the Allegro 
   homepage, http://www.talula.demon.co.uk/allegro/.

   There are three mailing lists for Allegro-related discussion, each with a 
   slightly different purpose.

   [AL] - Allegro - allegro@canvaslink.com
      This list is for any kind of discussion about Allegro, questions about 
      Allegro, reports of problems with Allegro, suggestions for new 
      features, announcements of programs written using Allegro, etc. 
      General rule: if it has to do with Allegro, you can post it here. If 
      not, go somewhere else (for example comp.os.msdos.djgpp, comp.lang.c, 
      or the AGP list).

   [AGP] - Allegro Games Programming - agp@canvaslink.com
      This list is for discussion of the more general kinds of issue that 
      may arise while writing a game using Allegro, but that are not 
      specifically related to Allegro itself. For example if you wanted to 
      talk about algorithms for scrolling a tile map, that would be 
      off-topic in the main Allegro list, but appropriate to discuss here.

   [AD] - Allegro Developers - conductors@canvaslink.com
      This list is for the people currently working on Allegro, who use it to
      coordinate their efforts. You can use this address if you need to 
      contact the developers directly, for example to submit some new code 
      that you have written or to report a bug. Unlike the other lists, we 
      will be really rude to people who post inappropriate material here, so 
      please don't do that! Do not send tech-support questions to this list. 
      Don't post bug reports here unless you are 100% certain they aren't 
      your fault (if you are in any doubt, use the main Allegro list 
      instead, which is read by most of the same people who are subscribed 
      here). The Allegro development is a completely open process, and 
      everyone is welcome to drop by, have a listen, and start contributing 
      code patches. This list is for working rather than talking, though, so 
      please don't do anything that might get in our way.

   To subscribe to one of the lists, write to listserv@canvaslink.com with 
   the text "subscribe {list} myname" in the body of your message, where 
   {list} is one of allegro, agp, or conductors.

   To remove yourself from a list, write to listserv@canvaslink.com with the 
   text "unsubscribe {list}" in the body of your message, where {list} is 
   one of allegro, agp, or conductors.

   Note that the address for subscribing and unsubscribing is NOT the same 
   as the regular address for posting messages! Please remember to send your 
   administration requests to the listserv, rather than to the list itself.

   To send a message to one of the lists, write to allegro@canvaslink.com, 
   agp@canvaslink.com, or conductors@canvaslink.com. You don't need to be 
   subscribed to these lists before you can post to them, but it is a good 
   idea to subscribe in order to see the replies. It is ok to post code 
   contributions to the developers list without being subscribed, though, as 
   long as you mention this in your mail so we will know to reply by private 
   email. Before posting tech-support questions to the Allegro list, please 
   take a moment to read the guidelines in help.txt.

   You can search through the mailing list archives using the engine on 
   http://www.canvaslink.com/allegro/search.htm.

   Note: please don't send messages in HTML format. The increased size 
   places an unnecessary load on the server, and many subscribers have a 
   hard time reading these posts.

   Note: please do not crosspost between these lists. Choose the most 
   appropriate one for your message, and then send it only to that list.

   Note: please don't send large binary attachments to any of the lists. 
   Upload your files to a website and then post the URL, or if you can't do 
   that, post an annoucement asking people to write to you privately, and 
   then send the file by individual email to whoever responds.

   My personal address is shawn@talula.demon.co.uk, but please try to avoid 
   mailing me personally about anything other than personal issues. Use the 
   main Allegro list for questions about Allegro, and the developers list 
   for sending code contributions. I read all the lists, and by using them 
   you are likely to get a response even if I'm not around or am busy with 
   something else at the moment.

   For the ever-shrinking minority of people without net access, my snail 
   address is Flat 1, 71 Croham Road, Croydon, Surrey, England, CR2 7HG. For 
   the even smaller minority of people who can't get access to a postal 
   service, find your way to South Croydon railway station and head off down 
   Croham Road, away from the Croham Arms pub. Just after the turnoff to 
   Castlemaine Avenue, it is the first house on the left.

