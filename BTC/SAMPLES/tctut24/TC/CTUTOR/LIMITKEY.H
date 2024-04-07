/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* A header file for procedure limitchar(char *s, char key, int ekey)  */
/* which ensures that only a set of prescribed key presses will be     */
/* accepted by the program. The function limitchar(...) has three      */
/* arguments, one to declare the set of acceptable characters as a set */
/* of char, the second the variable name for the entered character     */
/* and the third an integer value indicating whether the keypress      */
/* involved an Escape Sequence (27;nn) or an Extended Code (0;nn).     */
/*                                                                     */
/* LIMITKEY.H                  R Shaw            29.3.93               */
/*_____________________________________________________________________*/

#include <conio.h>
#include <stdio.h>

limitchar(char *s, char *pkey, int *pekey)
{
  char  chr;

  do
  {
    *pekey = 0;
    *pkey  = getch();
    if (*pkey == 0)
      {
	*pekey = 1;
	*pkey  = getch();
        break;
      }
    if (*pkey == 27)
      {
	*pekey = 1;
	*pkey  = getch();
        break;
      }
  }
  while ((chr = strchr(s,*pkey)) == NULL);
  putch(*pkey);
}


