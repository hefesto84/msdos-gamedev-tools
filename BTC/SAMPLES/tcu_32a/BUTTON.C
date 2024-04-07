#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <usr\tcu.h>


void main (void)
{
   int               esc_key,
                     x_pos,
                     y_pos;
   TCU_FORM          form;
   TCU_FORM_INFO     info;
   int far           button_handler (TCU_FORM *, int);

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

   if (tcu_load_form (&form, "button") == TCU_ERROR) {  /* Load the form from */
      printf ("Error opening form file\n");         /* the CFO file */
      exit (1);
   }

   tcu_get_form_info (&form, &info);       /* Calculate where to put the */
   x_pos = (80-info.width)/2+1;            /* form to get it in the middle */
   y_pos = (25-info.height)/2+1;           /* of the screen */

   tcu_set_button_fn (&form, button_handler);  /* Set up the button handler */

   tcu_display_form (&form, x_pos, y_pos);     /* Get form on screen */

/* For this one we only want to accept button selections, so we keep */
/* looping around 'edit_form' until we get one.  Using 0 as the start */
/* field keeps the field focus on the last selected field.  This way */
/* the form stays exactly as it was until one of the buttons has been */
/* selected. */

   do {
      tcu_edit_form (&form, 0, &esc_key);
      if (esc_key == TCU_FLD_BUTTONESC)
         if (!tcu_get_confirm (25, 10, tcu_colour_attrib (WHITE, RED),
                               tcu_colour_attrib (WHITE, RED),
                               "QUIT: Are you sure? (Y/N)"))
            esc_key = TCU_FLD_ESCESC;
   } while (esc_key != TCU_FLD_BUTTONESC && esc_key != TCU_FLD_BUTTONSAVE);

   tcu_remove_form (&form);                /* Remove the form from the screen */
   tcu_unload_form (&form);                /* ...and from memory */

   if (esc_key == TCU_FLD_BUTTONESC)       /* Show which button was selected */
      printf ("QUIT selected\n");
   else
      printf ("ACCEPT selected\n");
}


int far button_handler (TCU_FORM *form, int field)
{
   if (field == tcu_get_field_id (form, "quit", NULL))  /* Is it QUIT button? */
      return (2);                          /* Return saying ESCape form */
   else
      return (1);                          /* Otherwise, save & escape */
}
