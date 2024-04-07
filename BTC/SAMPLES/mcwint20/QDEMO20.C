#define USE_LOCAL
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dos.h>
#include <conio.h>
#include <w1.h>
#include <windprot.h>
#include <color.h>

#define WAIT 15000
/*===============================================================================*/
/*  Qdemo20.c - Demonstration program for Qwik write utilities  ver 2.0, 1-1-88      */
/*  Demo has been programmed for All Cards and any column mode.  If you want to  */
/*  try 40 column mode, set crtcolumns to 40 in main. This does not set your     */
/*  machine to 40 column mode. This must be done by the DOS mode command.        */
/*                                                                               */
/* AUTHORS: Turbo Pascal version -- (C) 86,87 Jim H. Lemay                       */
/*          Turbo C version -- (C) 1987,1988 Mike Mlachak                             */
/*          Microsoft version -- (C) 1987,1988 Mike Mlachak
/* DATE:    6-1-87                                                               */
/* VERSION: 2.0                                                                  */
/* REVISIONS: Modified source to use new function 2.0 calls 1-1-88
/* COMMENTS: Must be compiled in SMALL model and linked with T1xwin20.lib          */
/*===============================================================================*/

typedef char str80[80];

    int          row, rows, col, cols, ctr, step, rstep, colmax, count,
                 attrib, i, j, crtcols;
    int          hidecursor, oldcursor, fgrnd, bgrnd;
    int          brdrattr, wndwattr;
    char         savedblock[4000], popupblock[4000];
    int          blkrow, blkcol;
    int          crtcolumns;               /*  number of CRT columns  */
    int          tattr;
    int          coll[3], colr[3];
    char         strng[75], numstr[75];
    double       rnum;

    int random(int);


   static str80 data[9] = {
       "1",
       "22",
       "333",
       "Qwik-Write Utilities",
       "Odd  Length",
       "Even  Length",
       "18 characters wide",
       "19 characters width",
       "Margin to Margin width"};

static str80 vers = "Qwik-Write Utilities (Version 2.0)";
void main(void)
{
/*  --- Set up data ---  */
  qinit(); /*  << <<  Required intializing statement !!  */
  crtcolumns = 80;
  crtcols = crtcolumns;
  hidecursor = 8192;

/*  --- Initial screen ---  */
  oldcursor = cursorchange(hidecursor);
  qfill(1, 1, 25, crtcols, (BLUE << 4) + WHITE, ' '); /*   Clear Screen  */
  qwritec(11, 1, crtcols, (BLUE << 4) + YELLOW, vers);
  qwritec(13, 1, crtcols, -1, "Your screen is about to explode.");
  qwritec(14, 1, crtcols, -1, "Hold on to your seat ...");
  wsleep(6*18);

/*  --- Explosion of Boxes ---  */
  qfill(11, 1, 4, crtcols, -1, ' '); /*   Clear Lines  */
  qattr(1, 1, 25, crtcols, LIGHTGRAY << 4); /*   Change screen attribute  */
  ctr = crtcols / 2;
  for (step = 2; step <= ctr - 2; step++) {
     if (step > 24)
        rstep = 12;
     else
        rstep = step >> 1;
     for (count = 1; count <= 20; count++) {
        row = 13 - rstep + random(rstep + 2);
        rows = rstep;
        cols = rstep + rstep + (rstep >> 2);
        if (step <= 24)
           col = ctr - cols + random(cols + 1);
        else
           col = ctr - 1 - step + random(step + step - 22);
      fgrnd = random(16);
      bgrnd = random(8);
      if (bgrnd == fgrnd)
         fgrnd = fgrnd + 1;
      attrib = (bgrnd << 4) + fgrnd;
      qfill(row,col,row,cols,attrib,'²');
    }  /* for count = 1*/
  } /* for step = 2,..etc */

  qfillc(10, 1, crtcols, 6, 34, RED << 4, ' ');
  qfillc(11, 1, crtcols, 4, 30, BROWN << 4, ' ');
  tattr = (RED << 4) + YELLOW;
  qwritec(12, 1, crtcols, tattr, vers);
  qwritec(13, 1, crtcols, tattr, "Turbo-C (1.0)");

  /*  --- Save Screen for Page Demo ---  */
  if (maxpage > 0)
  {
    qstore(1, 1, 25, crtcols, savedblock);
    qwritepage(1);
    qrestore(1, 1, 25, crtcols, savedblock);
    qwritepage(0);
  }
  /*  --- End of Save Screen ---  */
  wsleep(4*18);
  tattr = (BLUE << 4) + WHITE;
  qwritec(6, 1, crtcols, tattr, " qwrite will write with new attributes ");
  qwritec(7, 1, crtcols, tattr, " that you specify direct to the screen.");
  wsleep(5*18);
  qwritec(18, 1, crtcols, -1, " qwrite will also use existing attributes ");
  qwritec(19, 1, crtcols, -1, "    when you do not even know or care.    ");
                        /*  highlight the word 'existing'  */
  qattrc(18, 6, crtcols + 5, 1, 10, (LIGHTRED << 4) + WHITE);
  wsleep(6*18);
  qwritec(21, 1, crtcols, tattr, " Say Goodbye to this screen. ");
  wsleep(4*18);
  /*  --- Disintigrate Screen ---  */

  for (i = 1; i <=7500; i++) {
    row = random(25) +1;
    col = random(crtcols) + 1;
    qfill(row,col,1,1,BLACK, ' ');
  }

/*  --- Qwrite with Reals Demo ---  */
  qfill(1, 1, 25, crtcols, YELLOW, ' '); /*   Clear Screen  */
  qwritec(2, 1, crtcols, -1, "qwrite with TURBO-C's sprintf will write");
  qwritec(3, 1, crtcols, -1, "reals and integers faster:");
  wsleep(6*18);
  rnum = 1.2345678901;
  for (col = 0; col <= (crtcols / 20) - 1; col++)  {
     for (row = 5; row <= 24; row++) {
        rnum = rnum + 20.0;
        sprintf(numstr,"%e",rnum);
        qwrite(row, col * 20 + 4, -1, numstr);
     }
  }
  wsleep(7*18);

/*  --- Centering Demo ---  */
  qfill(1, 1, 25, crtcols, LIGHTGRAY << 4, ' '); /*   Clear Screen  */
  qwritec(2, 1, crtcols, -1, "qwritec  will automatically");
  qwritec(3, 1, crtcols, -1, "center your data ...");
  qwritec(4, 1, crtcols, -1, "(ODD breaks are shifted to the left.)");
  wsleep(6*18);

  /*  - Set up columns for varying column modes -  */
  coll[1] = 1;
  colr[1] = crtcols;
  if (crtcols < 80) {
     coll[0] = coll[1];
     coll[2] = crtcols / 2;
     colr[0] = colr[1];
     colr[2] = crtcols / 2;
  }
  else {
     coll[0] = 3;
     colr[0] = 26;
     coll[2] = crtcols - 14;
     colr[2] = crtcols - 14;
  }

  qwritec(7, coll[0], colr[0], -1, "between margins ...");
  qbox(8, (coll[0] + (colr[0] >> 1)) - 14, 15, 26, WHITE, LIGHTGRAY, doublebrdr);
  wsleep(4*18);
  for (row = 11; row <= 19; row++)
     qwritec(row, coll[0], colr[0], -1, data[row - 11]);
  wsleep(4*18);

  qwritec(7, coll[1], colr[1], -1, "between two columns ...");
  qfillc(9, coll[1], colr[1], 13, 24, YELLOW, ' '); /*   Clear window  */
  for (row = 9; row <= 21; row++)
     qwritec(row, coll[1], colr[1], -1, "><"); /*   Show two columns   */
  wsleep(4*18);
  for (row = 11; row <= 19; row++)
     qwritec(row, coll[1], colr[1], LIGHTRED, data[row - 11]);
  wsleep(4*18);

  qwritec(7, coll[2], colr[2], -1, "or on a center line ...");
  qfillc(8, coll[2], colr[2], 15, 27, LIGHTGRAY << 4, ' '); /*   Clear window  */
  for (row = 9; row <= 21; row++) /*   Show center line   */
     qwritec(row, coll[2], colr[2], (LIGHTGRAY << 4) + WHITE, "|");
  wsleep(4*18);
  for (row = 11; row <= 19; row++)
     qwritec(row, coll[2], colr[2], -1, data[row - 11]);
  wsleep(7*18);

/*  --- Qfill Demo ---  */
  qfill(1, 1, 25, crtcols, WHITE, ' '); /*   Clear Screen  */
  qwritec(2, 1, crtcols, -1, "qfill as well as qattr can fill");
  qwritec(3, 1, crtcols, -1, "your screen in several ways.");
  wsleep(5*18);

  qwritec(7, 1, crtcols, -1, "by rows ...");
  wsleep(4*18);
  for (row = 9; row <= 24; row++)
     qfill(row, 2, 1, crtcols - 2, 9 + row, (char)(row + 56));
  wsleep(5*18);

  qfill(7, 1, 19, crtcols, WHITE, ' '); /*   Clear Lines  */
  qwritec(7, 1, crtcols, -1, "by columns ...");
  wsleep(4*18);
  for (col = 2; col <= crtcols - 2; col++)
     qfill(9, col, 16, 1, 16 + col, (char)(col + 63));
  wsleep(5*18);

  qfill(7, 1, 19, crtcols, WHITE, ' '); /*   Clear Lines  */
  qwritec(7, 1, crtcols, -1, "or by row-by-column blocks ...");
  wsleep(4*18);
  qfill(9, 2, 16, crtcols - 2, (BLUE << 4) + YELLOW, '!');
  wsleep(6*18);

/*  --- Qbox demo ---  */
  qfill(1, 1, 25, crtcols, LIGHTGRAY << 4, ' '); /*   Clear Screen  */
  qwritec(2, 1, crtcols, -1, "qbox is an application procedure made");
  qwritec(3, 1, crtcols, -1, "from qwrite and qfill.  Together they");
  qwritec(4, 1, crtcols, -1, "can make windows with borders easy.");
  wsleep(5*18);
  qwritec(14, 1, crtcols, -1, "How about 1000 of them? ... ");
  wsleep(4*18);
  colmax = crtcols - 21;
  for (i = 1; i <= 1000; i++) {
     row = random(10) + 6;
     col = random(colmax) + 2;
     brdrattr = random(128);
     wndwattr = random(128);
     qbox(row, col, 10, 20, brdrattr, wndwattr, doublebrdr);
  }
  wsleep(7*18);

/*  --- Block Transfer and PopUp Demo ---  */
  qfill(1, 1, 25, crtcols, YELLOW, '?'); /*   Clear Screen  */
  qfillc(10, 1, crtcols, 6, 40, BROWN << 4, ' '); /*   Clear Block  */
  qwritec(11, 1, crtcols, -1, "qstore will save and restore");
  qwritec(12, 1, crtcols, -1, "Row-by-Column blocks on your display.");
  qwritec(13, 1, crtcols, -1, "It is so fast, I have to slow it down");
  qwritec(14, 1, crtcols, -1, "so you can see it.");
  wsleep(6*18);
  blkrow = 8;
  blkcol = (crtcols / 2) - 9;
  qstore(blkrow, blkcol, 10, 20, savedblock);
  /*  --- Make a Pop Up Menu ---  */
  qbox(blkrow, blkcol, 10, 20, (BLUE << 4) + YELLOW, (BLUE << 4) + BROWN, doublebrdr);
  qwritec(blkrow + 4, blkcol, blkcol + 20, -1, "Pop Up");
  qwritec(blkrow + 5, blkcol, blkcol + 20, -1, "Menu");
  /*  --- End of Pop Up Menu ---  */
  qstore(blkrow, blkcol, 10, 20, popupblock);
  wsleep(3*18);
  colmax = crtcols - 20;
  for (i = 1; i <= 30; i++) {
    for (j = 0; j <=WAIT; j++);
     qrestore(blkrow, blkcol, 10, 20, savedblock);
     blkrow = random(15) + 1;
     blkcol = random(colmax) + 1;
     qstore(blkrow, blkcol, 10, 20, savedblock);
     qrestore(blkrow, blkcol, 10, 20, popupblock);
  }

/*  --- Page Demo ---  */
  if (maxpage > 0) {
     qpage(1);
     qwritepage(1);
     tattr = (BLUE << 4) + YELLOW;
     qwritec(20, 1, crtcols, tattr, " Remember this page?  ");
     qwritec(21, 1, crtcols, tattr, " It wasn't destroyed, but saved using ");
     qwritec(22, 1, crtcols, tattr, " qstore/qrestore and placed on a new page. ");
     wsleep(6*18);
     qwritepage(0);
     qpage(0);
  }

/*  --- Attribute Demo ---  */
  qfill(1, 1, 25, crtcols, (GREEN << 4) + GREEN, ' '); /*   Clear Screen  */
  tattr = (GREEN << 4) + WHITE;
  qwritec(2, 1, crtcols, tattr, "Qwik-Write Utilities is hiding strings on");
  qwritec(3, 1, crtcols, tattr, "your screen ...");
  cols = crtcols / 20;
  if (qwait == 0)
     tattr = 0;
  else
     tattr = (GREEN << 4) + GREEN;
  for (col = 0; col <= cols - 1; col++)
     for (row = 5; row <= 20; row++)
        qwrite(row, 20 * col + 1, tattr, data[3]);
  wsleep(3*18);

  qfill(2, 1, 2, crtcols, -1, ' '); /*   Clear Lines  */
  tattr = (GREEN << 4) + BLACK;
  qwritec(2, 1, crtcols, tattr, "qattr can show them -");
  qwritec(3, 1, crtcols, tattr, "by merely changing the attribute!");
  wsleep(5*18);

  /*  --- Try using Turbo's color procedures this time ---  */
  qattr(5, 1, 16, crtcols, tattr); /*   Reveal Data  */
  wsleep(3*18);

  qfill(2, 1, 2, crtcols, -1, ' '); /*   Clear Lines  */
  qwritec(2, 1, crtcols, tattr, "Or even just emphasize what's seen ...");
  for (i = 1; i <= 100; i++) {
    row = random(16) + 5;
    col = random(cols) * 20 + 1;
    qattr(row, col, 1, 20, 46);
    for (j=0; j<=WAIT-7500; j++);
    qattr(row, col, 1, 20, 32);
  }
  for (i = 1; i <= cols; i++) /*   Emphasize Data  */
    qattr(5 * i, (i - 1) * 20 + 1, 1, 20, (LIGHTGREEN << 4) + YELLOW);
  tattr = wiattr(YELLOW,GREEN);
  qwritec(22, 1, crtcols, tattr,
           " TURBO-C Version (C) 1987,1988 Michael G. Mlachak & Brian L. Cassista");
  gotorc(24, 1);
  oldcursor = cursorchange(oldcursor);
}

int random(seed)
{
  int hold;

     hold= rand();
     if (hold > 127)
       hold = hold % 127;
     hold = hold % seed;
   return(hold);
}

