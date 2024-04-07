/*--------------------------------------------------------------------------*
 | Author:                                                                  |
 | Sherif El-Kassas        .       :.                                       |
 | EB dept           \_____o__/ __________                                  |
 | Eindhoven U of Tec       .. /                                            |
 | The Netherlands            /             Email: elkassas@eb.ele.tue.nl   |
 *--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*
 | The author shall not be liable to the user for any direct, indirect      |
 | or consequential loss arising from the use of, or inability to use,      |
 | any program or file howsoever caused.  No warranty is given that the     |
 | programs will work under all circumstances.                              |
 *--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*
 |   SAVEINTS is supplied as is with no warranty, expressed or implied.     |
 |   It is not copyrighted, either.                                         |
 |   Usage:  SAVEINTS <interrupt file name>                                 |
 |   Compile with: TCC -mh saveints.c                                       |
 *--------------------------------------------------------------------------*/


#if defined(__TINY__) || defined(__SMALL__) || defined(__MEDIUM__)
#error --Compile with LARGE data model
#else

#include <stdio.h>

extern void _restorezero(void);

main(int ac, char **av){
  FILE     *file;
  char far *ints=0L;

  if (ac > 1)
    if ( (file=fopen(*++av, "wb")) != NULL){
      _restorezero();
      fwrite(ints, 1, 1024, file);
      fclose(file);
    }
    else printf("Can't create %s\n", *av);
  else printf("Invalid number of parameters\n");
}

#endif
