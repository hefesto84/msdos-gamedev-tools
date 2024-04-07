/*---------------------------------------------------------------------------
 |  Program TESTBIOS.C                                                      |
 |                                                                          |
 |  This program demonstrates the BIOS interrupt timing functionality of    |
 |  TCHRT V3.  The user may select from several BIOS interrupts to          |
 |  "exercise", and appropriate activity is generated.  A BIOS interrupt    |
 |  timer summary is then produced.  MSDOS interrupt 21h is timed in all    |
 |  cases, which will show the user the relationship between operating      |
 |  system and hardware activity.                                           |
 |                                                                          |
 |  (c) 1989 Ryle Design, P.O. Box 22, Mt. Pleasant, MI 48804               |
 |                                                                          |
 |  V3.00  Turbo C Shareware Evaluation Version                             |
 ---------------------------------------------------------------------------*/

#include <stdio.h>
#include <process.h>
#include <conio.h>

#include "pchrt.h"


void do_crt(void)
/*---------------------------------------------------------------------------
 |  This function enables both CRT & MSDOS interrupt timing, generates some |
 |  CRT activity, and displays the BIOS timer report.                       |
 |                                                                          |
 |  Globals referenced: none                                                |
 |                                                                          |
 |  Arguments: void                                                         |
 |                                                                          |
 |  Returns  : void                                                         |
 ---------------------------------------------------------------------------*/
{
    int     indx;

	t_bios_start(CRT10+DOS21);

    for (indx=0; indx<10; indx++) printf("Ryle Design ... Purveyors of Big Science\n");

    t_bios_report(0);

    t_bios_stop();

} /* do_crt */


void do_disk(void)
/*---------------------------------------------------------------------------
 |  This function enables both DISK & MSDOS interrupt timing, generates some|
 |  DISK activity, and displays the BIOS timer report.                      |
 |                                                                          |
 |  Globals referenced: none                                                |
 |                                                                          |
 |  Arguments: void                                                         |
 |                                                                          |
 |  Returns  : void                                                         |
 ---------------------------------------------------------------------------*/
{
    int     indx;
    FILE    *tfile;


    t_bios_start(DISK+DOS21);

    printf("Creating data file ... ");
    tfile = fopen("TEST.XXX","wb");
    printf("complete\n");

    printf("Writing 32k bytes two bytes at a time ... ");
	for (indx=0; indx<16384; indx++) fwrite((int *)indx,1,sizeof(int),tfile);
    printf("complete.\n");

    printf("Closing and erasing file ... ");
    fclose(tfile);
    unlink("TEST.XXX");
    printf("complete.\n");

    t_bios_report(0);

    t_bios_stop();

} /* do_disk */


void do_printer(void)
/*---------------------------------------------------------------------------
 |  This function enables both PRT & MSDOS interrupt timing, generates some |
 |  printer activity, and displays the BIOS timer report.                   |
 |                                                                          |
 |  Globals referenced: none                                                |
 |                                                                          |
 |  Arguments: void                                                         |
 |                                                                          |
 |  Returns  : void                                                         |
 ---------------------------------------------------------------------------*/
{
    int     indx;
	FILE    *pfile;

    printf("Make sure printer is online and ready.  Press any key to continue >> ");
    getch();

    t_bios_start(PRT+DOS21);

    pfile = fopen("PRN","wt");

    for (indx=0; indx<10; indx++) fprintf(pfile,"Ryle Design ... Purveyors of Big Science\n");

    fclose(pfile);

    t_bios_report(0);

    t_bios_stop();

} /* do_printer */


void main(void)
{
    int             indx;
    long unsigned   hits,elapsed;
    char            ts[13];
    
    if (t_start() != TRUE)
    {
        printf("Insufficient heap for TCHRT operation\n");
        exit(0);
    }

    t_entry(0);

    printf("TESTBIOS - TCHRT V3 Demonstration Series\n\n");
    printf("Make sure files 10.INT, 13.INT, 17.INT, and 21.INT are in this directory\n\n");

    do
    {
        printf("0 ... Profile BIOS interrupt 10h (CRT)\n");
        printf("1 ... Profile BIOS interrupt 13h (Disk)\n");
        printf("2 ... Profile BIOS interrupt 17h (Printer)\n");
        printf("3 ... Exit\n");

        printf("Select 0-3 >> ");
        indx = getche();
        indx -= '0';
		printf("\n");

        switch (indx)
        {
			case 0 :    do_crt();     	break;
			case 1 :    do_disk();   	break;
			case 2 :    do_printer();	break;
        }
    }
    while (indx != 3);

    t_exit(0);
    t_ask_timer(0,&hits,&elapsed);
    t_stop();

	printf("TestBios complete.  Elapsed time %s\n",t_cvt_time(elapsed,ts) );

} /* TestBios */
