/* TCHK 2.1 - Howard Kapustein's Turbo C library        6-6-89      */
/* Copyright (C) 1988,1989 Howard Kapustein.  All rights reserved.  */

/* demoadv.c  -  used for testing TCHK advanced date conversions */

#include <howard.h>
#include <datehk.h>
#include <dateadv.h>

#include <stdio.h>
#include <dos.h>
#include <stdlib.h>
#include <string.h>

void main(void);

void main(void)
{
    extern int _argc;
    extern char **_argv;
    char buff[80], temp[80], temp2[80];
    double dbl, dbl2;
    struct date today, d, d2;
    struct ddate dd, dd2;
    int stype, dtype;

    if (_argc<3) {
        printf("DemoAdv is a demonstration program of the advanced date conversions of TCHK.\n\n");
        printf("Usage:  demoadv stype dtype\n\n");
        printf("    demoadv will convert today's date from stype format\n");
        printf("    to dtype format. See TCHK.DOC for more details\n");
        exit(1);
    }

    stype = atoi(_argv[1]);
    dtype = atoi(_argv[2]);
    if (stype<1 || stype>43 || dtype<1 || dtype>43) {
        printf("Invalid format (must be 1-43)\n");
        exit(2);
    }

/*  for this demo, I get today's date, convert it to format 1 (MM-DD-YY)
    via a sprintf() and then convert it to format stype for the test of
    date_convert(s,d,stype,dtype).                                      */

    getdate(&today);
    sprintf(buff,"%2d-%2d-%2d",today.da_mon,today.da_day,today.da_year-1900);
    printf("Today's date: %s     Stype %d:  ",buff,stype);
    switch (stype) {
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:  { date_convert(buff,&dbl,1,stype);
                   printf("%lf",dbl);
                   switch (dtype) {
                       case 4:
                       case 5:
                       case 6:
                       case 7:
                       case 8:  { date_convert(&dbl,&dbl2,1,dtype);  break; }
                       case 9:  { date_convert(&dbl,&d2,1,dtype);    break; }
                       case 10: { date_convert(&dbl,&dd2,1,dtype);   break; }
                       default: { date_convert(&dbl,temp2,1,dtype); break; }
                   }
                   break;
                 }
        case 9:  { date_convert(buff,&d,1,stype);
                   printf("%d %d %d",d.da_day,d.da_mon,d.da_year);
                   switch (dtype) {
                       case 4:
                       case 5:
                       case 6:
                       case 7:
                       case 8:  { date_convert(&d,&dbl2,1,dtype);  break; }
                       case 9:  { date_convert(&d,&d2,1,dtype);    break; }
                       case 10: { date_convert(&d,&dd2,1,dtype);   break; }
                       default: { date_convert(&d,temp2,1,dtype); break; }
                   }
                   break;
                 }
        case 10: { date_convert(buff,&dd,1,stype);
                   printf("%d %d %d",dd.dday,dd.dmon,dd.dyear);
                   switch (dtype) {
                       case 4:
                       case 5:
                       case 6:
                       case 7:
                       case 8:  { date_convert(&dd,&dbl2,1,dtype);  break; }
                       case 9:  { date_convert(&dd,&d2,1,dtype);    break; }
                       case 10: { date_convert(&dd,&dd2,1,dtype);   break; }
                       default: { date_convert(&dd,temp2,1,dtype); break; }
                   }
                   break;
                 }
        default: { date_convert(buff,temp,1,stype);
                   printf("%s",temp);
                   switch (dtype) {
                       case 4:
                       case 5:
                       case 6:
                       case 7:
                       case 8:  { date_convert(temp,&dbl2,1,dtype);  break; }
                       case 9:  { date_convert(temp,&d2,1,dtype);    break; }
                       case 10: { date_convert(temp,&dd2,1,dtype);   break; }
                       default: { date_convert(temp,temp2,1,dtype); break; }
                   }
                   break;
                 }
    }
    printf("   ->   Dtype %d:  ",dtype);
    switch (dtype) {
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:  { printf("%lf",dbl2);  break; }
        case 9:  { printf("%d %d %d",d2.da_day,d2.da_mon,d2.da_year);  break; }
        case 10: { printf("%d %d %d",dd2.dday,dd2.dmon,dd2.dyear);  break; }
        default: { printf("%s",temp2);  break; }
    }
    printf("\n");

/* quit */
    exit(0);
}
