/* TCHK 2.1 - Howard Kapustein's Turbo C library        6-6-89      */
/* Copyright (C) 1988,1989 Howard Kapustein.  All rights reserved.  */

/* demonum.c  -  used for testing TCHK number (math/accounting) functions */

#include <howard.h>
#include  <mathhk.h>
#include <finance.h>
#include <stdio.h>
#include <dos.h>             /* for argc/argv */

void main();

void main()
{
    extern int _argc;
    extern char **_argv;
    double d=123456.78901, cost=10000.0, salvage=4000.0;
    int i=2, life=10, period=_argc;

    printf("round %lf to %d places: %lf\n",d,i,round(d,i));
    printf("frac %lf: %lf\n\n",d,frac(d));

    printf("Cost: %lf    Salvage: %lf    Life: %d    Period: %d\n\n",cost,salvage,life,period);
    printf("straight line: %lf\n",SLD(cost,salvage,life));
    printf("sum year digits: %lf\n",SYD(cost,salvage,life,period));
    printf("double decline: %lf\n",DDB(cost,life,period));
    printf("Dep 1: %lf\n",depreciation(cost,salvage,life,period,1));
    printf("Dep 2: %lf\n",depreciation(cost,salvage,life,period,2));
    printf("Dep 3: %lf\n\n",depreciation(cost,salvage,life,period,3));
    printf("Accum Dep 1: %lf\n",accum_dep(cost,salvage,life,period+1,1));
    printf("Accum Dep 2: %lf\n",accum_dep(cost,salvage,life,period+1,2));
    printf("Accum Dep 3: %lf\n",accum_dep(cost,salvage,life,period+1,3));
}
