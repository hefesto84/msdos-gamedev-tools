#include <stdio.h>
#include <dos.h>
 
static	char	SW = 0;		/* DOS switch character, either '-' or '/' */

int
getswitch()
{
   if (SW == 0) {
      /* get SW using dos call 0x37 */
      _AX = 0x3700;
      geninterrupt(0x21);
      SW = _DL;
   }
   return( SW & 0xFF );
}
