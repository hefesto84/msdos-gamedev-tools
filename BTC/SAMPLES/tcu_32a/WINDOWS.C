#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <usr\tcu.h>

void main (void)
{
   TCU_WINDOW  my_window1,
               my_window2;
   int         i,
               length;
   char        input_buffer[6];

   tcu_open_window (&my_window1,
                    5, 3,
                    30, 15,
                    "Window I",
                    tcu_colour_attrib (YELLOW, BLUE),
                    tcu_colour_attrib (WHITE, BLUE),
                    tcu_colour_attrib (CYAN, BLUE),
                    TCU_BOX_DOUBLE);

   tcu_open_window (&my_window2,
                    42, 2,
                    25, 20,
                    NULL,
                    tcu_colour_attrib (BLUE, LIGHTGRAY),
                    tcu_colour_attrib (LIGHTGREEN, LIGHTGRAY),
                    tcu_colour_attrib (LIGHTCYAN, LIGHTGRAY),
                    TCU_BOX_DOUBLE);

   tcu_wprintf (&my_window1, "       Control Window\n\n");

   do {

      tcu_wprintf (&my_window1, "Lines to display? ");
      strcpy (input_buffer, "10");
      tcu_wgets (&my_window1, 5, input_buffer, &length);
      tcu_wprintf (&my_window1, "\n");

      if (length == -1)
         if (tcu_get_confirm (20, 20, tcu_colour_attrib (RED, WHITE),
                              tcu_colour_attrib (BLACK, WHITE),
                              "Are you sure you want to quit?")) {
            tcu_wprintf (&my_window1, "\nOooh! You pressed ESCape on me!");
            break;
         }

      for (i=1; i<=atoi(input_buffer); i++)
         tcu_wprintf (&my_window2, "Window 2, Line #%d\n", i);
      tcu_change_colour (&my_window2,
                         tcu_colour_attrib (LIGHTGREEN+BLINK, LIGHTGRAY));
      tcu_wprintf (&my_window2, "---End of display---\n");
      tcu_change_colour (&my_window2,
                         tcu_colour_attrib (LIGHTGREEN, LIGHTGRAY));

   } while (atoi(input_buffer));

   tcu_wprintf (&my_window1, "\n\nPress return to clear the\nwindows");

   while (!kbhit());
   getch();

   tcu_close_window (&my_window1);

   while (!kbhit());
   getch();

   tcu_close_window (&my_window2);

   while (!kbhit());
   getch();
}
