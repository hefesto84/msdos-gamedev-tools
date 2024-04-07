README.TXT for mouse routines

Michael Chen    mchen@groucho.cs.psu.edu
7/26/1992

INTRODUCTION

   Hello out there!  If you have downloaded this file, either you are curious
to see routines that use a mouse, or you are in the same predicament I was in
less than a week ago.
   I wanted to find some way use the mouse from within my Turbo C++ programs.
But there are no built-in routines, so I began scavenging the Internet ftp
sites for something suitable.
   I ended up with two candidates, MOUCLIB and MMOUSE.  Neither really seemed
to fill the bill.  MOUCLIB needed TASM to build itself, which I don't have; 
MMOUSE worked, but seemed a little inelegant and sluggish.  I sighed and dug
around some more for documentation on the interrupt calls.
   Lo and behold, I found INTER31, the interrupt list to end all interrupt
lists!  Kudos to Ralf Brown for this gem.  (Of course, I'm strapped for disk
space, so I left it on my Sun account and grep'd for the mouse calls.)

SUMMARY (brief)

   A day later, MOUSE was born!  It's fast, and it uses the interrupt calls
from Ralf Brown's list.  (A copy has been included in the file MOUSE.TXT.)
In its current state, it merely provides a nice way to use the mouse.  I have
not made a .DOC file, because in its current state the MOUSE.TXT file is
documentation enough.  I haven't bothered to include the source for the same
reason --- it's boring, and the only nifty thing is the .HPP include file for
C++, which I'm including.
   In future uploads, I hope to get an event handler going, and provide the
usual extra mouse cursors, etc.  I have not included an interface for Turbo
Pascal, basically because the last version I used was 3.0, and I have no idea
what TP is like these days.  If anyone makes one, please send me a copy so I
can include it in the next post.
   If anyone has wishes, suggestions, etc., please e-mail or write me, and
tell me what you're using it for. (Address below)

RIGHTS & WHATNOT

   I'm no lawyer, so this is going to be really informal.
   Being a relatively poor college student, I hate not being able to use a
program just because I can't afford it.  So, following the Golden Rule, you
don't actually HAVE to pay anything to use this.  I'd rather not stifle any
poor soul's creative energies for a lousy buck.
   The above assumes that you, in turn, will not charge fees to use programs
incorporating this code.  If you do want to charge people, please "register"
by telling me who you are, what you're making, and including $25 US.  (I'm
assuming that anyone who is actually going to make money isn't going to miss
such a paltry sum.)
   If you include this code in a program to be released to others, please
mention my name somewhere, and contact me, so I can find out if people will
really use this.
   Donations are always welcome if you can afford them, of course... :)

DISCLAIMER

   I take no responsibility for any damage, incidental or otherwise, which
results from the use of this code.  (It's just a bunch of interrupt calls to
the mouse driver!) :)
   This software has been tested on my 386SX/25 with 2 megs of RAM, a Trident
SVGA card w/512K, and a 40MB IDE hard drive.  (Pretty hurting for development
systems, I know.)  It hasn't caused any problems, and I don't expect any.
From personal experience, mouse bugs are usually the driver's fault, so just
try upgrading your mouse driver first.

ADDRESSES (until August 14, 1992)

	461 East Beaver Avenue
	State College, PA 16801-5633 USA

	mchen@groucho.cs.psu.edu	(primary)
	mchen@cs.psu.edu		(if for some reason the above
					 address is unreachable)

Enjoy!

--- Michael Chen
    7/26/1992

