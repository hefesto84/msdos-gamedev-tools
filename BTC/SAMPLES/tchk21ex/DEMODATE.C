/* TCHK 2.1 - Howard Kapustein's Turbo C library        6-6-89      */
/* Copyright (C) 1988,1989 Howard Kapustein.  All rights reserved.  */

/* demodate.c  -  used for testing TCHK date conversions */

#include <howard.h>
#include <datehk.h>
#include <dateconv.h>
#include <stdio.h>
#include <dos.h>
#include <stdlib.h>
#include <string.h>

void main(void);

void main(void)
{
    extern int _argc;
    extern char **_argv;
    extern char *Days[];
    FILE *fp;
    struct date today;
/*    char *c, buff[80], *c2, temp[20];*/
    char US[12],Euro[12],Jap[12], *us,*euro,*jap, Sho[80],Ful[80], *shortd,*full;
    double cal,calcent,julA,julB,julE;
    struct ddate dd, *DD;

    if ((_argc<2) || ((fp = fopen(_argv[1],"w")) == NULL)) {
        printf("DemoDate is a demonstration program for the date conversions of TCHK.\n\n");
        printf("Usage:  demodate filename.ext\n\n");
        printf("    demodate will write to the file you specify a small chart\n");
        printf("    of possible date conversions. Cross index the date formats\n");
        printf("    you wish to convert. If the entry is 0 (zero) there is no\n");
        printf("    conversion presently available. Otherwise today's date will\n");
        printf("    appear in the format specified in the To column.\n\n");
        printf("In the works is a generic date conversion function that can convert\n");
        printf("over 40 different date formats quickly and easily. See TCHK.DOC for\n");
        printf("more details.");
        exit(1);
    }



    getdate(&today);
    fprintf(fp,"DATEHK\n\n");
    fprintf(fp,"      To   Greg-USA  Greg-Eur  Greg-Jap  Calendar    CalCent     Jul-E       Jul-A       Jul-B       dd-> mon  day  year\n");
    fprintf(fp,"From\n");
/* USA*/
    sprintf(US,"%02d-%02d-%02d",today.da_mon,today.da_day,today.da_year-1900);
    sprintf(Euro,"%02d-%02d-%02d",today.da_day,today.da_mon,today.da_year-1900);
    sprintf(Jap,"%02d-%02d-%02d",today.da_year-1900,today.da_mon,today.da_day);
    cal = GregtoCal(US);
    calcent = GregtoCalCent(US);
    julE = julA = julB = 0;
    dd.dday = dd.dmon = dd.dyear = 0;
fprintf(fp,"Greg-USA   %s  %s  %s  %10.2f  %10.2f  %10.2f  %10.2f  %10.2f       %02d   %02d   %d\n",US,Euro,Jap,cal,calcent,julE,julA,julB,dd.dmon,dd.dday,dd.dyear);
    cal = GregEurotoCal(Euro);
    calcent = GregEurotoCalCent(Euro);
fprintf(fp,"Greg-Euro  %s  %s  %s  %10.2f  %10.2f  %10.2f  %10.2f  %10.2f       %02d   %02d   %d\n",US,Euro,Jap,cal,calcent,julE,julA,julB,dd.dmon,dd.dday,dd.dyear);
    cal = GregJaptoCal(Jap);
    calcent = GregJaptoCalCent(Jap);
fprintf(fp,"Greg-Jap   %s  %s  %s  %10.2f  %10.2f  %10.2f  %10.2f  %10.2f       %02d   %02d   %d\n",US,Euro,Jap,cal,calcent,julE,julA,julB,dd.dmon,dd.dday,dd.dyear);
    us = CaltoGreg(cal);
    strcpy(US,us);
    euro = CaltoGregEuro(cal);
    strcpy(Euro,euro);
    jap = CaltoGregJap(cal);
    strcpy(Jap,jap);
    julE = CaltoJul(cal);
    julA = CaltoJulA(cal);
    julB = CaltoJulB(cal);
    calcent = CaltoCalCent(cal);
fprintf(fp,"Calendar   %s  %s  %s  %10.2f  %10.2f  %10.2f  %10.2f  %10.2f       %02d   %02d   %d\n",US,Euro,Jap,cal,calcent,julE,julA,julB,dd.dmon,dd.dday,dd.dyear);
    cal = CalCenttoCal(calcent);
fprintf(fp,"Cal-Cent   %s  %s  %s  %10.2f  %10.2f  %10.2f  %10.2f  %10.2f       %02d   %02d   %d\n",US,Euro,Jap,cal,calcent,julE,julA,julB,dd.dmon,dd.dday,dd.dyear);
    us = JultoGreg(julE);
    strcpy(US,us);
    euro = JultoGregEuro(julE);
    strcpy(Euro,euro);
    jap = JultoGregJap(julE);
    strcpy(Jap,jap);
    cal = JultoCal(julE);
    calcent = JultoCalCent(julE);
    DD = Jultoddate(julE);
    dd = *DD;
fprintf(fp,"Jul-E      %s  %s  %s  %10.2f  %10.2f  %10.2f  %10.2f  %10.2f       %02d   %02d   %d\n",US,Euro,Jap,cal,calcent,julE,julA,julB,dd.dmon,dd.dday,dd.dyear);
    julA = CaltoJulA(cal);
    us = JulAtoGreg(julA);
    strcpy(US,us);
    euro = JulAtoGregEuro(julA);
    strcpy(Euro,euro);
    jap = JulAtoGregJap(julA);
    strcpy(Jap,jap);
    cal = JulAtoCal(julA);
    calcent = JulAtoCalCent(julA);
    DD = JulAtoddate(julA);
    dd = *DD;
fprintf(fp,"Jul-A      %s  %s  %s  %10.2f  %10.2f  %10.2f  %10.2f  %10.2f       %02d   %02d   %d\n",US,Euro,Jap,cal,calcent,julE,julA,julB,dd.dmon,dd.dday,dd.dyear);
    julB = CaltoJulB(cal);
    us = JulBtoGreg(julB);
    strcpy(US,us);
    euro = JulBtoGregEuro(julB);
    strcpy(Euro,euro);
    jap = JulBtoGregJap(julB);
    strcpy(Jap,jap);
    cal = JulBtoCal(julB);
    calcent = JulBtoCalCent(julB);
    DD = JulBtoddate(julB);
    dd = *DD;
fprintf(fp,"Jul-B      %s  %s  %s  %10.2f  %10.2f  %10.2f  %10.2f  %10.2f       %02d   %02d   %d\n",US,Euro,Jap,cal,calcent,julE,julA,julB,dd.dmon,dd.dday,dd.dyear);
    julB = 0.0;
    julA = 0.0;
    julE = 0.0;
    cal = 0;
    calcent = 0;
    us = CaltoGreg(cal);
    strcpy(US,us);
    euro = CaltoGregEuro(cal);
    strcpy(Euro,euro);
    jap = CaltoGregJap(cal);
    strcpy(Jap,jap);
fprintf(fp,"ddate      %s  %s  %s  %10.2f  %10.2f  %10.2f  %10.2f  %10.2f       %02d   %02d   %d\n",US,Euro,Jap,cal,calcent,julE,julA,julB,dd.dmon,dd.dday,dd.dyear);

julE = CaltoJul(GregtoCal(us));
fprintf(fp,"\nDay of week: %d   =   %s\n",dayofweek(julE),Days[dayofweek(julE)]);

fprintf(fp,"       dd->mo dd->da dd->yr    short        full\n");
    shortd = ddatetoshort(&dd);
    strcpy(Sho,shortd);
    full = ddatetofull(&dd);
    strcpy(Ful,full);
fprintf(fp,"ddate:   %2d     %2d     %2d       %s   %s\n",dd.dmon,dd.dday,dd.dyear,Sho,Ful);
    DD = shorttoddate(shortd);
    dd = *DD;
    strcpy(Ful,"");
fprintf(fp,"short:   %2d     %2d     %2d       %s   %s\n",dd.dmon,dd.dday,dd.dyear,Sho,Ful);
    DD = fulltoddate(full);
    dd = *DD;
    strcpy(Sho,"");
    strcpy(Ful,full);
fprintf(fp,"full:    %2d     %2d     %2d       %s   %s\n",dd.dmon,dd.dday,dd.dyear,Sho,Ful);


/* quit */
    fclose(fp);
    exit(0);
}

/*  Yes, I know, this program is a memory waster. Between most
    fprintfs I should free() some memory before using the variable
    again. This is only a demo. When you use the functions, make
    sure you free your allocated memory.                            */
