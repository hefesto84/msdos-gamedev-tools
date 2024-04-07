/*
tchelp.c
by Edward V Dong, 08 June 1987

-----------------------------------------------------------------------
 tchelp.c is based on about.c, originally written for DeSmet C, but
 converted to Turbo C version 1.0.  The following is the original
 discussion on about.c
-----------------------------------------------------------------------
   Usage:           about  <name>  [<catfile>]

   Examples:        about calloc
                    about *
                    about * b:c.cat
                    about fourier b:math.dat

    Description:    For each line in <catfile> beginning as ":<name>",
                    successive lines are displayed until the next line
                    begins with ":";  if <name> is *, all entries are
                    displayed;  <catfile> defaults to "about.dat".

    Purpose:        The file about.dat contains synopses of C functions,
                    thus making "about" an elementary "help facility".

                    However, <catfile> may be any list of :<name>'s of
                    interest, with each name on a separate line, and
                    each followed by its respective text to be displayed.

    Limitations:    A simple linear search is used, so probably no more
                    than a few hundred names can be handled usefully.

    Language:       C (DeSmet 2.4)

    Author:         R. E. Sawyer
                    3620 Spencer St. 30, Torrance, CA 90503
                    1 Jan 85
-----------------------------------------------------------------------
*/

#include <stdio.h>

#define CATFILE "tc.dat"
#define DEFCHAR ':'
#define BUFLEN 82

FILE *fin;

main(ac, av)
    int ac;
    char *av[];
    {
    char *catfile, *more;
    char buf[BUFLEN];
    char *print_text();
    int showall;
    int done;

    showall = 0;
    done = 0;

    if ((ac < 2) || (ac > 3))
        {
        printf("\nUsage:    tchelp <name>  [<catfile>]\n");
        printf("\nFor each line in <catfile> beginning as \":<name>\",");
        printf("\nsuccessive lines are displayed until the next line");
        printf("\nbegins with \":\";  if <name> is *, all entries are");
        printf("\ndisplayed;  <catfile> defaults to \"tc.dat\".\n");
        printf("\nbased on original by R.E.Sawyer (about.c) revised");
        printf("\nby Edward V. Dong, 8 June 1987, for Turbo C.\n");
        exit();
        }
    else if (ac == 2)
        {
        if ((fin = fopen(catfile = CATFILE, "rb")) == 0)
            {
            printf("\n---File %s not found", catfile);
            exit();
            }
        }
    else if (ac ==3)
        {
        if ((fin = fopen(catfile = av[2], "rb")) == 0)
            {
            printf("\n---File %s not found", catfile);
            exit();
            }
        }
    if (*av[1] == '*')
        showall = 1;
    printf("\n(Catalogue file:  %s)\n\n", catfile);
    more = fgets(buf, BUFLEN - 1, fin);
    while (more!=NULL)
        {
        if ((buf[0] == DEFCHAR)
            && ((comp(av[1], buf + 1) == 0) || showall))
            {
            more = print_text(buf);
            if (!showall)
                {
                done = 1;
                break;
                }
            }
        else
            more = fgets(buf, BUFLEN - 1, fin);
        }
    if ((done != 1) && !showall)
        printf("\"%s\" is not catalogued\n\n", av[1]);
    fclose(fin);
    }
    
int comp(s, t)
    char *s, *t;
    {
    int i;

    for (i = 0; (t[i] > ' ') && (t[i] <= '~'); ++i)
        ;
    t[i] = '\0';
    return strcmp(s, t);
    }

#include <string.h>
char *print_text(buf)
    char buf[];
    {
    char *more;
    printf("%s:\n", &buf[1]);
    while (((more = fgets(buf, BUFLEN - 1, fin)) != NULL)
        && (buf[0] != DEFCHAR))
        {
        if (strrchr(buf,10)!=NULL) *strrchr(buf,10) = 0;
        puts(buf);
        }
    return more;
    }
