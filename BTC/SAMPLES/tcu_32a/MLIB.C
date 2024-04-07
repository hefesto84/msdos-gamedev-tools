/*===========================================================================
 *                                                                          *
 *   Library Merge Procedure                                                *
 *                                                                          *
 *   Written by     : Karl Keyte            Gross-Gerauer Strasse 10        *
 *   Date           : 91.03.16              6108 Weiterstadt                *
 *                                          Germany                         *
 *                                          Tel: +(49) 6150 2499            *
 *                                                                          *
 *==========================================================================*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dir.h>
#include <io.h>
#include <fcntl.h>



int merge_libs (char *mainlib, char *sublib)
{
   int               linenum,
                     modules,
                     nul,
                     old_stderr,
                     old_stdout;
   char              cmd[81],
                     line[81];
   FILE             *inp,
                    *outp1,
                    *outp2;

   if (!strcmp (mainlib, sublib))        /* Don't merge same libraries! */
      return (1);

   if (searchpath ("tlib.exe") == NULL)  /* Die if there's no librarian */
      return (2);

   nul = open ("nul", O_WRONLY);         /* Open the nul device */

   old_stdout = dup (1);                 /* Redirect STDOUT and STDERR to */
   old_stderr = dup (2);                 /* the nul device so that we don't */
   dup2 (nul, 1);                        /* get any messy output */
   dup2 (1, 2);

   strcpy (cmd, "tlib ");
   strcat (cmd, sublib);
   strcat (cmd, ",$lib$lst.$$$");        /* Get a file listing all the */
                                         /* modules in the sub library */
   system (cmd);

   inp = fopen ("$lib$lst.$$$", "r");
   outp1 = fopen ("$mod$ext.$$$", "w");
   outp2 = fopen ("$mod$rep.$$$", "w");

   linenum = 0;
   modules = 0;

   while (fgets (line, 80, inp)) {       /* Scan through the list of modules */
      if (++linenum < 3)                 /* picking out the individual name */
         continue;                       /* and write them to two files */
      if (line[0] == '\t' || line[0] == ' ')  /* which will be used as */
         continue;                       /* response files to the librarian */
      if (strchr (line, '\n'))
         *strchr (line, '\n') = '\0';
      if (strchr (line, '\t'))
         *strchr (line, '\t') = '\0';
      if (strchr (line, ' '))
         *strchr (line, ' ') = '\0';
      if (line[0] == '\0')
         continue;
      modules++;
      if (modules > 1) {
         fprintf (outp1, " &\n*%s", line);
         fprintf (outp2, " &\n-+%s", line);
      } else {
         fprintf (outp1, "*%s", line);
         fprintf (outp2, "-+%s", line);
      }
   }

   fclose (inp);
   fclose (outp1);
   fclose (outp2);

   unlink ("$lib$lst.$$$");            /* Remove the list of modules file */

   strcpy (cmd, "tlib ");
   strcat (cmd, sublib);
   strcat (cmd, " @$mod$ext.$$$");     /* Extract all the modules from the */
                                       /* sub library */
   system (cmd);

   unlink ("$mod$ext.$$$");            /* Remove the extraction response file */

   strcpy (cmd, "tlib ");
   strcat (cmd, mainlib);
   strcat (cmd, " @$mod$rep.$$$");     /* Insert the modules into the main */
                                       /* library */
   system (cmd);

   inp = fopen ("$mod$rep.$$$", "r");

   while (fgets (line, 80, inp)) {     /* Scan through the response file to */
      if (strchr (line, '\n'))         /* get the module names so that they */
         *strchr (line, '\n') = '\0';  /* can be deleted again. */
      if (strchr (line, ' '))
         *strchr (line, ' ') = '\0';
      strcpy (cmd, line+2);
      strcat (cmd, ".obj");
      unlink (cmd);
   }

   fclose (inp);

   unlink ("$mod$rep.$$$");       /* Remove the insertion response file */

   dup2 (old_stdout, 1);          /* Reset STDOUT and STDERR */
   dup2 (old_stderr, 2);

   return (0);                    /* End */
}
