/* TCHK 2.1 - Howard Kapustein's Turbo C library        6-6-89      */
/* Copyright (C) 1988,1989 Howard Kapustein.  All rights reserved.  */

/* demotime.c  -  used for testing TCHK time conversions */

#include <howard.h>
#include <timehk.h>

#include <stdio.h>
#include <dos.h>
#include <stdlib.h>
#include <string.h>

void main();

void main()
{
    extern int _argc;
    extern char **_argv;
    void *generic;
    char buff[80], temp[80], temp2[80];
    double dbl, dbl2;
    struct time now, t, t2;
    int stype, dtype;

    if (_argc<3) {
        printf("DemoTime is a demonstration program of the time conversions of TCHK.\n\n");
        printf("Usage:  demotime stype dtype\n\n");
        printf("    demotime will convert the current time from stype format\n");
        printf("    to dtype format. See TCHK.DOC for more details\n");
        exit(1);
    }

    stype = atoi(_argv[1]);
    dtype = atoi(_argv[2]);
    if (stype<1 || stype>24 || dtype<1 || dtype>24) {
        printf("Invalid format (must be 1-24)\n");
        exit(2);
    }

/*  for this demo, I get the current time, convert it to format 21
    (0HH:MM:SS:CC) via a sprintf() and then convert it to format stype
    for the test of time_convert(s,d,stype,dtype).                                      */

    gettime(&now);
    sprintf(buff,"%d:%02d:%02d.%02d",(int)(now.ti_hour),(int)(now.ti_min),(int)(now.ti_sec),(int)(now.ti_hund));
    printf("Current time: %s\nStype %d:  ",buff,stype);
    switch (stype) {
        case 7: { time_convert(buff,&t,21,stype);
                  printf("%d %d %d %d",t.ti_hour,t.ti_min,t.ti_sec,t.ti_hund);
                  generic = (void *) &t;
                  break; }
        case 8:
        case 9:
        case 10:
        case 11:
        case 12: { time_convert(buff,&dbl,21,stype);
                   printf("%lf",dbl);
                   generic = (void *) &dbl;
                   break; }
        default: { time_convert(buff,temp,21,stype);
                   printf("%s",temp);
                   generic = (void *) temp;
                   break; }
    }
    printf("   ->   Dtype %d:  ",dtype);
    switch (dtype) {
        case 7: { time_convert(generic,&t2,stype,dtype);
                  printf("%d %d %d %d",t2.ti_hour,t2.ti_min,t2.ti_sec,t2.ti_hund);
                  break; }
        case 8:
        case 9:
        case 10:
        case 11:
        case 12: { time_convert(generic,&dbl2,stype,dtype);
                   printf("%lf",dbl2);
                   break; }
        default: { time_convert(generic,temp2,stype,dtype);
                   printf("%s",temp2);
                   break; }
    }
    printf("\n");

/* quit */
    exit(0);
}
