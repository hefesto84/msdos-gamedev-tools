/* TCHK 2.1 - Howard Kapustein's Turbo C library        6-6-89      */
/* Copyright (C) 1988,1989 Howard Kapustein.  All rights reserved.  */

/* demopars.c  -  used for testing TCHK parsing functions */

#include <howard.h>
#include <video.h>
#include <color.h>
#include <filehk.h>
#include <keycode.h>
#include <keyboard.h>
#include <string.h>
#include <stringhk.h>
#include <alloc.h>

void main();

void main()
{
    struct filespec *fs;
    char temp[80], temp2[80];
    int k, argk[] = { ESC };

    do {
        cls();
        textattr(LCYAN);
        cputs("Input a filespec (ESC to end):");
        textattr(LRED);
        strfill(temp,' ',59);
        k = getget(32,1,temp,60,"X",1,argk,TRIMALL|INSERTMODE);
        if (k == ESC)
            break;
        fs = parsefilename(temp);
        gotoxy(1,2);
        textattr(YELLOW);
        cprintf("Parsed -> %c    [%s]    [%s]\r\n",fs->drive,fs->path,fs->filename);
        expandfilespec(temp,temp2);
        cprintf("Expanded -> [%s]\r\n",temp2);
        textattr(LRED);
        cputs("Press any key to continue");
        inkey(TRUE);
        free(fs);
    } while (TRUE);
}
