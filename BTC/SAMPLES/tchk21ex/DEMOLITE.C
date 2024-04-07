/* TCHK 2.1 - Howard Kapustein's Turbo C library        6-6-89      */
/* Copyright (C) 1988,1989 Howard Kapustein.  All rights reserved.  */

/* demolite.c  - used for testing TCHK litebar menus */

#include <menuhk.h>
#include <video.h>
#include <color.h>
#include <howard.h>
#include <stdio.h>
#include <string.h>
#include <keycode.h>
/*
coords        1        2        3         4          cmd #
    0    5    0    5   0    5   0    5    0
     Dial    Add   moVe                             0  2  4
     Manual  Edit  Kill                             1  3  5
*/

void main()
{
    extern int _argc, litebarerrno;
    char *cmd[]={" Dial ", " Manual ", " Add ", " Edit ", " moVe ", " Kill "};
    char cmdflag[] = { ENABLED,ENABLED,ENABLED,ENABLED,ENABLED,ENABLED };
    char *msg[] = { NULL, NULL, NULL, NULL, NULL, NULL };
    int cmdx[] = { 0, 0, 8, 8,14,14};
    int cmdy[] = { 0, 1, 0, 1, 0, 1 };
    int cmdleft[] = { 4, 5, 0, 1, 2, 3 };
    int cmdright[]= { 2, 3, 4, 5, 0, 1 };
    int cmdup[] =   { 1, 0, 3, 2, 5, 4 };
    int cmddown[] = { 1, 0, 3, 2, 5, 4 };
    int k, cmdkey[] = { 1, 1, 1, 1, 3, 1 };
    int argq[] = { ALT_Q };
    struct litebar_header *lh;

    lh = litebar_alloc(1,10,80,11,NULL,NULL,NONE,6,cmd,cmdleft,cmdright,
                       cmdup,cmddown,cmdkey,cmdflag,cmdx,cmdy,msg,1,1,1,
                       argq, BLACK, BLACK, YELLOW, LRED, LBLUE|B_WHITE,
                       CYAN, BLACK|B_CYAN, LGREEN, LWHITE|B_RED, 1,
                       CASEINDEP|ERASEMENU|ESCQUIT|FREEMENU);
    if (lh == NULL)
        printf("lh == NULL    (litebarerrno = %d)\n",litebarerrno);
    else {
        cls();
        do {
            k = litebar_get(lh);
            gotohv(60,22);
            printf("k = %4d",k);
        } while (k != 0);
    }           /* note: litebar_free() not needed because of FREEMENU flag */
}
