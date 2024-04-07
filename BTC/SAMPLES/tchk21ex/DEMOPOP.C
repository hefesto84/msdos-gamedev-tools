/* TCHK 2.1 - Howard Kapustein's Turbo C library        6-6-89      */
/* Copyright (C) 1988,1989 Howard Kapustein.  All rights reserved.  */

/* demopop.c  - used for testing TCHK popup menus */

#include <menuhk.h>
#include <video.h>
#include <color.h>
#include <howard.h>
#include <stdio.h>
#include <string.h>

void main()
{
    extern int _argc, popuperrno;
    char *cmd[]={""," Protocol",""," Xmodem"," Ymodem"," Zmodem"," Disabled"," Other",""};
    char cmdflag[] = { STATICTEXT,STATICTEXT,STATICTEXT,ENABLED,ENABLED,ENABLED,DISABLED,ENABLED,STATICTEXT };
    char fr[] = {'É','Í','»','º','¼','Í','È','º','\0','¹','Ì'};
    int k, cmdkey[] = { -1,-1,-1, 1,1,1, -1, -1, -1};
    struct popup_header *ph;

    ph = popup_alloc(8,4,25,14,fr,"Upload",CENTER,cmd,cmdkey,cmdflag,
                   LWHITE|B_BLUE, LRED, YELLOW, LRED, LBLUE|B_WHITE, CYAN,
                   BLACK|B_CYAN, LGREEN, 1,
                   CASEINDEP|ERASEMENU|DISABLENOHILITE|WRAPAROUND|ESCQUIT);
    if (ph == NULL)
        printf("ph == NULL  (popuperrno = %d)\n",popuperrno);
    else {
        do {
            k = popup_get(ph);
            gotohv(60,22);
            printf("k = %2d",k);
        } while (k != 0);
        popup_free(ph);
    }
}
