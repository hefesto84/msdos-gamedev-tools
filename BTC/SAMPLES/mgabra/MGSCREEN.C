/*****************************************
                MGSCREEN
*****************************************/

#include "dos.h"
#include "stdlib.h"
#include "mgabra.h"
#define HKEY 0x0439

/*****************************************
                MAKEINFO
*****************************************/

void makeinfo(void)
{
   PLANE info;
   initvideo(1);
   info = createplane(22, 78, 113, " MORE INFORMATION ", "");
   evokeplane(info, 1, 1, 78, " PRESS ANY KEY TO CONTINUE ");
   putplane(info, "\n    Mgscreen, a cheat screen program, was written in Turbo C and linked with");
   putplane(info, "\n the ABRACADABRA! Programmers Resident Utility Library. This library allows");
   putplane(info, "\n you to take any normal Turbo C program and with one function turn it into");
   putplane(info, "\n a memory resident program instantly available at the touch of a hotkey.");
   putplane(info, "\n It just as easily allows you to have your program continue running while");
   putplane(info, "\n it is in background. There is also a rich set of supporting functions");
   putplane(info, "\n that implement various facets of multi-tasking, although only ONE main");
   putplane(info, "\n function is all that is really needed.");
   putplane(info, "\n ");
   putplane(info, "\n    If you are just beginning to write TSRs you have a long road ahead");
   putplane(info, "\n of you. If you have tried but are continually stopped by technical ");
   putplane(info, "\n mysteries hidden deep within DOS then ABRACADABRA! will give you in a ");
   putplane(info, "\n matter of hours the knowledge you have been waiting for.");
   putplane(info, "\n ");
   putplane(info, "\n    Years of research and consulting with many an industry guru has gone");
   putplane(info, "\n into the production of ABRACADABRA!. Mention of their names would be");
   putplane(info, "\n embarrassing because many of the secrets contained in ABRACADABRA! are");
   putplane(info, "\n being fiercely guarded by those who have profited for so many years");
   putplane(info, "\n knowing them. This is no hype, you ARE being kept in the dark.");
   putplane(info, "\n ");
   inkey();
   clearplane(info);
   putplane(info, "\n    ABRACADABRA! is NOT just a cheap set of interrupt routines like so ");
   putplane(info, "\n many off the shelf libraries have been palming off to you as TSR libraries.");
   putplane(info, "\n (You know, the kind you buy and they sit putrifying in your disk drawer.)");
   putplane(info, "\n It is ALL you need, in one compact function. No messy assembly language");
   putplane(info, "\n if you don't want it. No searching through shelves of manuals. No feverish");
   putplane(info, "\n hunt through old issues of DOBS. No thousand cycle write, compile, link");
   putplane(info, "\n to debug your interrupt routines.");
   putplane(info, "\n ");
   putplane(info, "\n    ABRACADABRA! handles for you the orchestration of the multiple ");
   putplane(info, "\n interrupt foundation that is required to write multi-tasking programs.");
   putplane(info, "\n It handles screen I/O as applies to TSRs. It handles the complications");
   putplane(info, "\n of compiled language architecture. It handles everything.");
   putplane(info, "\n ");
   putplane(info, "\n    Now that's the pitch. Here's the offer. I'll send you a copy of");
   putplane(info, "\n the ABRACADABRA! OBJ and header files with a manual explaining the");
   putplane(info, "\n intricacies of TSRs for $29.00. That's completely refundable if you");
   putplane(info, "\n don't like it no questions asked. If you want source code that is ");
   putplane(info, "\n more, $49.00 and includes the manual but is not as easily refundable.");
   putplane(info, "\n I will surely want to know why you don't want it.");
   putplane(info, "\n ");
   inkey();
   clearplane(info);
   putplane(info, "\n    Currently all I have is ABRACADABRA! for Turbo C but the source");
   putplane(info, "\n code is easily modifiable for other languages.");
   putplane(info, "\n ");
   putplane(info, "\n    Just call me at (213) 477-4151 after hours (Hey, I work a full time");
   putplane(info, "\n job) if you want to talk or just leave a message on my answering ");
   putplane(info, "\n machine. OR just send the money and I'll ship. ");
   putplane(info, "\n ");
   putplane(info, "\n    Walt Howard ");
   putplane(info, "\n    12021 Wilshire Blvd. Box 689");
   putplane(info, "\n    Los Angeles, California 90025");
   putplane(info, "\n ");
   putplane(info, "\n    So, before you press the next button, you got to ask yourself");
   putplane(info, "\n one question...");
   putplane(info, "\n ");
   putplane(info, "\n    Do you want to write flashy, reliable, bug free TSRs?");
   putplane(info, "\n ");
   putplane(info, "\n    Well, do ya punk?");
   putplane(info, "\n ");
   inkey();
   hideplane(info);
   clearplane(info);
}

void far tsrexit(void)
{
}

/*****************************************
                MAIN
*****************************************/

main(int argc, char *argv[1])
{                                   
   char vmode;
   if (*argv[1] == '?') 
   {
      makeinfo();
      exit(1);
   }
   intro(9, argv, "Mgscreen", "Control Space", "1.1", "Mgscreen ? For Info\n"
   "Pops Up False Screen So You Can Play Games At Work!", "Walt Howard", 87);
   printf("\nPress Any Key To Continue");
   inkey();
   _AH = 0x0F;		/* Clear Screen */
   geninterrupt(0x10);
   vmode = _AL;
   _AH = 0;
   _AL = vmode;
   geninterrupt(0x10);
   conout("\n\015");
   conout("\n\015 Volume in drive C has no label");
   conout("\n\015 Directory of  C:\\");
   conout("\n\015");
   conout("\n\015BATCH        <DIR>      7-19-87   3:58p");
   conout("\n\015COMM         <DIR>      7-19-87   3:55p");
   conout("\n\015DATAFLEX     <DIR>      7-19-87   3:53p");
   conout("\n\015DOSUTIL      <DIR>      1-01-80  12:00a");
   conout("\n\015DV           <DIR>      1-01-80  12:40a");
   conout("\n\015HTEST        <DIR>      7-19-87   3:52p");
   conout("\n\015FASTBACK     <DIR>      7-23-87   9:44p");
   conout("\n\015AUTOEXEC CC        55   6-15-87   8:39a");
   conout("\n\015COMMAND  COM    23612   7-07-86  12:00p");
   conout("\n\015VDISK    SYS     3307  12-30-85  12:00p");
   conout("\n\015HARDRIVE SYS     8186  12-02-86  12:00p");
   conout("\n\015AUTOEXEC BAT       79   1-01-80  12:40a");
   conout("\n\015TREEINFO NCD      123   1-01-80  12:02a");
   conout("\n\015CBAT               57   8-30-87   4:53p");
   conout("\n\015CONFIG   BAK       34   1-01-80  12:00a");
   conout("\n\015AUTOEXEC BAK       92   8-17-87   8:51p");
   conout("\n\015CONFIG   SYS       51   8-30-87   7:46p");
   conout("\n\015ANSI     SYS     1651  12-30-85  12:00p");
   conout("\n\015       18 File(s)    282624 bytes free");
   conout("\n\015C:\\>");
   tsrset(HKEY, 0, 0, 0, 263, 1);
   while(1){inkey();}
}

/*****************************************
                END OF MGSCREEN
*****************************************/

