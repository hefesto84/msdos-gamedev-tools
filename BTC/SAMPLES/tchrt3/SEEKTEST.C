/*---------------------------------------------------------------------------
 |  Program SEEKTEST.C                                                      |
 |                                                                          |
 |  This program demonstrates the use of TCHRT in timing seek performance   |
 |  of a PC based hard disk drive.  The method used will determine the total|
 |  seek time of the device which includes actual disk seek, controller     |
 |  overhead, and ROM BIOS overhead.  This is a "real world" measurement    |
 |  of disk performance under actual usage conditions.                      |
 |                                                                          |
 |  This program uses both inline generic timers and BIOS interrupt         |
 |  timers to measure disk performance.                                     |
 |                                                                          |
 |  (c)1989 Ryle Design, P.O. Box 22, Mt. Pleasant, Michigan 48804          |
 |                                                                          |
 |  V3.00  Turbo C Shareware Evaluation Version                             |
 ---------------------------------------------------------------------------*/

#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <process.h>

#include "pchrt.h"

union REGS      inregs,outregs;

long unsigned   seek1, seek2, hits1, hits2, seek3, hits3;


void disk_err(int istat)
/*---------------------------------------------------------------------------
 |  This procedure outputs a description of an INT 13h error status, and    |
 |  halts program exection.                                                 |
 |                                                                          |
 |  Globals referenced: none                                                |
 |                                                                          |
 |  Arguments: (int) istat - status returned from INT 13h in AX if carry    |
 |                           flag set.                                      |
 |                                                                          |
 |  Returns  : void                                                         |
 ---------------------------------------------------------------------------*/
{
    switch (istat)
    {
        case 0x00 : return;                                    /* no error */

        case 0x01 : printf("Disk error: Invalid command\n");
                    break;

        case 0x02 : printf("Disk error: Address mark not found\n");
                    break;

        case 0x03 : printf("Disk error: Disk is write-protected\n");
                    break;

        case 0x04 : printf("Disk error: Requested sector not found\n");
                    break;

        case 0x05 : printf("Disk error: Reset failed\n");
                    break;

        case 0x06 : printf("Disk error: Floppy disk removed\n");
                    break;

        case 0x07 : printf("Disk error: Bad parameter table\n");
                    break;

        case 0x08 : printf("Disk error: DMA overrun\n");
                    break;

        case 0x09 : printf("Disk error: DMA crossed 64KB boundary\n");
                    break;

        case 0x0A : printf("Disk error: Bad sector flag set\n");
                    break;

        case 0x0B : printf("Disk error: Bad track flag set\n");
                    break;

        case 0x0C : printf("Disk error: Requested media type not found\n");
                    break;

        case 0x0D : printf("Disk error: Invalid number of sectors on format\n");
                    break;

        case 0x0E : printf("Disk error: Control data address mark detected\n");
                    break;

        case 0x0F : printf("Disk error: DMA arbitration level out of range\n");
                    break;

        case 0x10 : printf("Disk error: Uncorrectable CRC or ECC data error\n");
                    break;

        case 0x11 : printf("Disk warning: ECC corrected data error\n");
                    return;

        case 0x20 : printf("Disk error: Controller failed\n");
                    break;

        case 0x40 : printf("Disk error: Seek failed\n");
                    break;

        case 0x80 : printf("Disk error: Disk has timed out\n");
                    break;

        case 0xAA : printf("Disk error: Drive not ready\n");
                    break;

        case 0xBB : printf("Disk error: Error is undefined\n");
                    break;

        case 0xCC : printf("Disk error: Write fault\n");
                    break;

        case 0xE0 : printf("Disk error: Status register error\n");
                    break;

        case 0xFF : printf("Disk error: Sense operation failed\n");
                    break;

        default   : printf("Unknown INT 13 return status %d\n",istat);
                    break;
    }

    exit(1);

} /* disk_err */


void test_disk(char disk)
/*---------------------------------------------------------------------------
 |  This function, which contains the actual disk test routines, does the   |
 |  following:                                                              |
 |      1. Seeks the test disk to track 0.                                  |
 |      2. Times 100 calls to seek to track 0.  Since the heads are already |
 |         on track 0, they will not move, and a estimate of the software   |
 |         overhead for each seek call can be made.                         |
 |      3. Times single track seeks to all cylinders (0-1,1-2,2-3,3-4,etc). |
 |         This provides a measurement of single track seek time.           |
 |      4. Seeks from track 0 to all tracks (0-1,0-2,0-3,0-4,etc).  This    |
 |         provides average seek time for the entire disk.                  |
 |      5. The results are reported.                                        |
 |                                                                          |
 |  int86() is used to call the ROM BIOS.  There is some software           |
 |  overhead incurred using this method.                                    |
 |                                                                          |
 |  Generic inline timers are used to measure disk performance.             |
 |                                                                          |
 |  Globals referenced: inregs, outregs.                                    |
 |                                                                          |
 |  Arguments: (char) disk - physical disk # - add to 0x80 for BIOS call.   |
 |                                                                          |
 |  Returns  : void                                                         |
 ---------------------------------------------------------------------------*/
{
    
    int             maxhead,maxcyl,indx;
    unsigned long   seek1,seek2,seek3,hits1,hits2,hits3;
    
    inregs.h.dl = 0x80 + disk;                              /* get disk config */
    inregs.h.ah = 0x08;
    int86(0x13,&inregs,&outregs);
    if (outregs.x.cflag != 0) disk_err(outregs.h.ah);

    maxhead = outregs.h.dh;                                 /* move bits to get */
    maxcyl = ((outregs.h.cl & 0xC0) << 2) + outregs.h.ch;   /* heads & tracks   */

    printf("\nPhysical drive %d shows %d cylinders, %d heads\n\n",disk,maxcyl+1,maxhead+1);

    printf("Starting single track seek test ...\n");

    inregs.h.ah = 0x0C;                                     /* seek command                        */
    inregs.h.ch = 0x00;                                     /* track 0                             */
    inregs.h.cl = 0x01;                                     /* XTs need sector bit set, or no seek */
    inregs.h.dh = 0;                                        /* head 0                              */
    inregs.h.dl = 0x80 + disk;                              /* disk #                              */

    int86(0x13,&inregs,&outregs);                           /* seek to track 0 */
    if (outregs.x.cflag != 0) disk_err(outregs.h.ah);       /* stop on error   */

    for (indx=0; indx<100; indx++)                          /* seek to 0 100 times to get ave overhead */
    {
        t_entry(2);
        int86(0x13,&inregs,&outregs);
        t_exit(2);
    }

    for (indx=1; indx<=maxcyl; indx++)                      /* from zero, single track seek to end of disk */
    {
        inregs.h.ch = indx & 0x00FF;                        /* mask track bits and stuff in cl & ch */
        inregs.h.cl = ((indx & 0x0300) >> 2) + 1;           /* cl sector bit 1 for XTs              */

        t_entry(0);
        int86(0x13,&inregs,&outregs);                       /* seek */
        t_exit(0);

        if (outregs.x.cflag != 0) disk_err(outregs.h.ah);   /* stop on error */

        if ((indx % 100) == 0) printf("%d ",indx);          /* echo to user our progress */
    }


    printf("\nSeeking to all tracks ...\n");

    inregs.h.ch = 0x00;                                     /* back to track 0 for next test */
    inregs.h.cl = 0x01;                                     /* sector bit for XTs            */
    int86(0x13,&inregs,&outregs);                           /* seek                          */
    if (outregs.x.cflag != 0) disk_err(outregs.h.ah);       /* stop on errors                */

    for (indx=1; indx<=maxcyl; indx++)                      /* from track 0, seek to all tracks */
    {
        inregs.h.ch = indx & 0x00FF;                        /* mask tracks bits and stuff in cl & ch */
        inregs.h.cl = ((indx & 0x0300) >> 2) + 1;           /* cl sector bit 1 for XTs               */

        t_entry(1);
        int86(0x13,&inregs,&outregs);                       /* seek */
        t_exit(1);

        if (outregs.x.cflag != 0) disk_err(outregs.h.ah);   /* stop on errors */

        if ((indx % 100) == 0) printf("%d ",indx);          /* echo to user our progress */

        inregs.h.ch = 0x00;                                 /* go back to track 0 for next seek */
        inregs.h.cl = 0x01;
        int86(0x13,&inregs,&outregs);
        if (outregs.x.cflag != 0) disk_err(outregs.h.ah);

    }

    t_ask_timer(0,&hits1,&seek1);                           /* query timers */
    t_ask_timer(1,&hits2,&seek2);
    t_ask_timer(2,&hits3,&seek3);

    printf("\n\nTest of physical disk %d complete.\n",disk);
    printf("Average single track seek ............. %7.3f milliseconds\n",((seek1/hits1)/1000.0) );
    printf("Average seek to all tracks ............ %7.3f milliseconds\n",((seek2/hits2)/1000.0) );
    printf("Estimated software overhead per seek .. %7.3f milliseconds\n\n",((seek3/hits3)/1000.0) );

    t_reset(0);                                             /* reset all timers */
    t_reset(1);
    t_reset(2);

} /* test_disk */


void test_disk_bios(char disk)
/*---------------------------------------------------------------------------
 |  This function, which contains the actual disk test routines, does the   |
 |  following:                                                              |
 |      1. Seeks the test disk to track 0.                                  |
 |      2. Times 100 calls to seek to track 0.  Since the heads are already |
 |         on track 0, they will not move, and a estimate of the software   |
 |         overhead for each seek call can be made.                         |
 |      3. Times single track seeks to all cylinders (0-1,1-2,2-3,3-4,etc). |
 |         This provides a measurement of single track seek time.           |
 |      4. Seeks from track 0 to all tracks (0-1,0-2,0-3,0-4,etc).  This    |
 |         provides average seek time for the entire disk.                  |
 |      5. The results are reported.                                        |
 |                                                                          |
 |  int86() is used to call the ROM BIOS.  There is some software           |
 |  overhead incurred using this method.                                    |
 |                                                                          |
 |  BIOS interrupt timers are used to measure disk performance.             |
 |                                                                          |
 |  Globals referenced: inregs, outregs.                                    |
 |                                                                          |
 |  Arguments: (char) disk - physical disk # - add to 0x80 for BIOS call.   |
 |                                                                          |
 |  Returns  : void                                                         |
 ---------------------------------------------------------------------------*/
{
    
    int             maxhead,maxcyl,indx;
    unsigned long   seek1,seek2,seek3,hits1,hits2,hits3;
    
    t_bios_start(DISK);
    
    inregs.h.dl = 0x80 + disk;                              /* get disk config */
    inregs.h.ah = 0x08;
    int86(0x13,&inregs,&outregs);
    if (outregs.x.cflag != 0) disk_err(outregs.h.ah);

    maxhead = outregs.h.dh;                                 /* move bits to get */
    maxcyl = ((outregs.h.cl & 0xC0) << 2) + outregs.h.ch;   /* heads & tracks   */

    printf("\nPhysical drive %d shows %d cylinders, %d heads\n\n",disk,maxcyl+1,maxhead+1);

    printf("Starting single track seek test ...\n");

    inregs.h.ah = 0x0C;                                     /* seek command                        */
    inregs.h.ch = 0x00;                                     /* track 0                             */
    inregs.h.cl = 0x01;                                     /* XTs need sector bit set, or no seek */
    inregs.h.dh = 0;                                        /* head 0                              */
    inregs.h.dl = 0x80 + disk;                              /* disk #                              */

    int86(0x13,&inregs,&outregs);                           /* seek to track 0 */
    if (outregs.x.cflag != 0) disk_err(outregs.h.ah);       /* stop on error   */

    for (indx=0; indx<100; indx++)                          /* seek to 0 100 times to get ave overhead */
        int86(0x13,&inregs,&outregs);

    t_bios_ask(DISK,0x0C,&hits1,&seek1);
    t_bios_reset(DISK,0x0C);

    for (indx=1; indx<=maxcyl; indx++)                      /* from zero, single track seek to end of disk */
    {
        inregs.h.ch = indx & 0x00FF;                        /* mask track bits and stuff in cl & ch */
        inregs.h.cl = ((indx & 0x0300) >> 2) + 1;           /* cl sector bit 1 for XTs              */

        int86(0x13,&inregs,&outregs);                       /* seek */

        if (outregs.x.cflag != 0) disk_err(outregs.h.ah);   /* stop on error */

        if ((indx % 100) == 0) printf("%d ",indx);          /* echo to user our progress */
    }

    t_bios_ask(DISK,0x0C,&hits2,&seek2);

    printf("\nSeeking to all tracks ...\n");

    inregs.h.ch = 0x00;                                     /* back to track 0 for next test */
    inregs.h.cl = 0x01;                                     /* sector bit for XTs            */
    int86(0x13,&inregs,&outregs);                           /* seek                          */
    if (outregs.x.cflag != 0) disk_err(outregs.h.ah);       /* stop on errors                */

    t_bios_reset(DISK,0x0C);
    for (indx=1; indx<=maxcyl; indx++)                      /* from track 0, seek to all tracks */
    {
        inregs.h.ch = indx & 0x00FF;                        /* mask tracks bits and stuff in cl & ch */
        inregs.h.cl = ((indx & 0x0300) >> 2) + 1;           /* cl sector bit 1 for XTs               */

        int86(0x13,&inregs,&outregs);                       /* seek */

        if (outregs.x.cflag != 0) disk_err(outregs.h.ah);   /* stop on errors */

        if ((indx % 100) == 0) printf("%d ",indx);          /* echo to user our progress */

        inregs.h.ch = 0x00;                                 /* go back to track 0 for next seek */
        inregs.h.cl = 0x01;
        int86(0x13,&inregs,&outregs);
        if (outregs.x.cflag != 0) disk_err(outregs.h.ah);

    }
    t_bios_ask(DISK,0x0C,&hits3,&seek3);


    printf("\n\nTest of physical disk %d complete.\n",disk);
    printf("Average single track seek ............. %7.3f milliseconds\n",((seek2/hits2)/1000.0) );
    printf("Average seek to all tracks ............ %7.3f milliseconds\n",((seek3/hits3)/1000.0) );
    printf("Estimated software overhead per seek .. %7.3f milliseconds\n\n",((seek1/hits1)/1000.0) );

    t_bios_stop();

} /* test_disk_bios */


void main(void)
{
    int     indx;
    int     numdisk;
    char    atom;

    t_start();                                              /* start TCHRT */

    printf("SeekTest.  TCHRT V3 Demonstration Series\n");
    printf("(c)1989 Ryle Design, P.O. Box 22, Mt. Pleasant, Michigan 48804\n\n");

    printf("Checking equipment ... ");

    inregs.h.ah = 0x08;
    inregs.h.dl = 0x80;
    int86(0x13,&inregs,&outregs);                           /* get available physical disks */
    if (outregs.x.cflag != 0)
    {
        printf("There are no hard disks on this system!\n\n");  /* exit on error */
        printf("SeekTest complete\n");
        exit(0);
    }

    numdisk = outregs.h.dl;                                 /* DL has total disks on controller */
    printf("%d physical hard disk(s) found\n\n",numdisk);

    printf("*** WARNING -- Do not proceed unless the test disk is backed up!\n\n");     /* A word of advice ... */

    do
    {
        for (indx=0; indx<numdisk; indx++)
        {
            printf("%d ... Test disk %d\n",indx,indx);
        }
        printf("%d ... Exit SeekTest\n",numdisk);

        do
        {
            printf("Select disk or exit (0-%d) >> ",numdisk);
            atom = getche();
            atom -= '0';
            printf("\n");
        }
        while ( (atom <0) || (atom > numdisk) );

        if (atom == numdisk)
        {
            t_stop();                                       /* shut down TCHRT before exit */
            printf("\nSeekTest complete");
            exit(0);
        }

        do
        {
            printf("\nUse inline or BIOS interrupt timers?\n");
            printf("0 ... Use inline timers\n");
            printf("1 ... Use BIOS interrupt timers\n");
            printf("Select 0 or 1 >> ");
			indx = getche();
			indx -= '0';
        }
		while ( (indx > 1) || (indx < 0) );

		if (indx == 0)
			test_disk(atom);
        else
			test_disk_bios(atom);

    }
    while (atom != numdisk);

} /* main */
