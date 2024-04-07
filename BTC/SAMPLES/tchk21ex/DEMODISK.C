/* TCHK 2.1 - Howard Kapustein's Turbo C library        6-6-89      */
/* Copyright (C) 1988,1989 Howard Kapustein.  All rights reserved.  */

/* demodisk.c  -  used for testing TCHK disktype() function */

#include <ibm.h>
#include <dos.h>
#include <stdio.h>

void main();

void main()
{
    byte id, drives;

    printf("DemoDisk.c  -  a demonstration of disktype() from TCHK\n\n");

    for (drives=1; drives<26; drives++)     /* drives A - Z */
        if ((id = disktype(drives)) != DISK_INVALID)    /* valid drive? */
            printf("Drive %c: has an id byte of %X\n",64+drives,id);

    printf("\nSee ibm.h for explanations of the id byte\n");
}
