#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#define __TCUPRE32__
#include <usr\tcu.h>            /* Utilities include file */

void main (void)
{
   int           age_id,        /* To hold field ID of age form field */
                 pos_x, pos_y,  /* Position for the form */
                 rkey;          /* Key used to exit form */
   FORM          form;          /* Form object */
   FIELD_INFO    info;          /* Field information structure */
   FORM_INFO     f_info;        /* Form information structure */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

   set_mouse_mode (1);

   load_form (&form, "sample");              /* Load form from CUF file */

   get_field_id (&form, "age", &age_id);     /* Get age field ID */

   get_form_info (&form, &f_info);           /* Get form information */
   pos_x = 81-f_info.width;                  /* Calculate where to put */
   pos_y = 26-f_info.height;                 /* the form. */

   display_form (&form, pos_x, pos_y);/* Display the form and allow */

   select_field (&form, age_id, &rkey);    /* Select a field */

   remove_form (&form);           /* Remove the form from screen */

   if (rkey >= 0) {
      get_field_info (&form, rkey, &info);
      printf ("User selected field %d (%s)\n", rkey, info.name);
   } else
      printf ("User aborted from field select\n");

   unload_form (&form);           /* Unload the form definition from memory */
}
