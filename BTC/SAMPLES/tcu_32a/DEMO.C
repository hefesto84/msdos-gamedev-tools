/*==========================================================================*
 *                                                                          *
 *   DEMO.C  by Karl Keyte                                                  *
 *                                                                          *
 *   Demonstrates the use of the TCU library (by same author)               *
 *                                                                          *
 *==========================================================================*/


#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include <sys\types.h>
#include <usr\tcu.h>            /* Utilities include file */

static TCU_NOTICE  my_notice;
static TCU_FORM    form;        /* Form object */
static     int     time_field;
static     char   *record;

extern TCU_FORM  SAMPLE;        /* Form object to be linked in.  Note that */
                                /* the name is CASE SENSITIVE */
void main (void)
{
   int               name_id,       /* To hold field ID of name form field */
                     pos_x, pos_y,  /* Position for the form */
                     rkey,          /* Key used to exit form */
                     select;        /* Menu selction */
   char              buf[83];       /* Prompt input buffer */
   TCU_MENU          mymenu;        /* Menu object */
   TCU_FIELD_VALUE   fval;          /* Form field value object */
   TCU_FORM_INFO     f_info;        /* Form information structure */

   void far      help (TCU_FORM *, int);     /* Form help function */
   int far       idler (unsigned long);
   int far       keyboard_handler (unsigned short *);
   int far       test_age (TCU_FORM *, int, TCU_FIELD_VALUE *);/* Age verify */
   int far       fn_keys (TCU_FORM *, int, int);               /* Fn. keys */

   char *option_list[] = { "AAdd Customer           =>",   /* Menu options */
                           "DDelete Customer        =>",
                           "QQuery Customer         =>",
                           "PPrint Customer Details",
                           "bAbout this Demo",
                           "GGo back to DOS",
                           NULL};

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Define the menu */

   select = tcu_define_menu (&mymenu,
                         "Customer Menu",
                         tcu_colour_attrib (YELLOW, CYAN),       /* Title */
                         tcu_colour_attrib (BLUE, CYAN),         /* Box */
                         tcu_colour_attrib (WHITE, MAGENTA),     /* Options */
                         tcu_colour_attrib (WHITE, BLUE),        /* Selected */
                         tcu_colour_attrib (LIGHTGRAY, MAGENTA), /* Deselected */
                         TCU_ESC_ESC | TCU_ESC_CNTL_C | TCU_ESC_PGUP |
                                       TCU_ESC_FUNC,
                         TCU_BOX_DOUBLE,
                         option_list,
			 tcu_colour_attrib (YELLOW, MAGENTA));

   if (select != TCU_OK) {
      printf ("Error in defining menu!\n");
      exit (1);
   }

   tcu_load_image_form (&form, &SAMPLE);     /* Load form from image file */

   record = malloc (tcu_form_record_size (&form));  /* Grab a buffer for a record */

   tcu_get_field_id (&form, "name", &name_id);   /* Get name field ID */
   tcu_get_field_id (&form, "time", &time_field); /* Get time field ID */

   tcu_get_form_info (&form, &f_info);         /* Get form information */
   pos_x = 81-f_info.width;                    /* Calculate where to put */
   pos_y = 26-f_info.height;                   /* the form. */

   tcu_set_form_fnkey_fn (&form, fn_keys);     /* Establish fn. key handler */
   tcu_set_form_help (&form, help);            /* Set form help function */
   tcu_set_field_verify (&form, test_age);     /* To verify age field */

   tcu_set_user_key_handler (keyboard_handler);

   tcu_set_menu_option (&mymenu, 4, 0);        /* Disable menu option 4 */

   select = tcu_display_menu (&mymenu, 5, 5);  /* Display the menu */

   buf[2] = '\0';       /* Empty starting string for name prompt */

/* Now get the user's menu selction. Keep getting it until an escape key */
/* is pressed or the 'Go back to DOS' option is selcted (select = 6).    */

   while ((select = tcu_read_menu_selection (&mymenu))> 0 && select < 6) {

      if (select == 5) {   /* ...about this demo... */

         tcu_prepare_notice (&my_notice,
                             "ABOUT THIS DEMO...",
                             tcu_colour_attrib (BLUE, LIGHTGRAY),
	                     tcu_colour_attrib (LIGHTBLUE, LIGHTGRAY),
                             tcu_colour_attrib (BLUE, LIGHTGRAY),
                             TCU_BOX_DOUBLE);

         tcu_notice_text (&my_notice, "");
         tcu_notice_text (&my_notice,
	              "This is a small demonstration to show the use of the");
         tcu_notice_text (&my_notice,
	              "TCU facilities including menus, notices, prompts and");
         tcu_notice_text (&my_notice,
	              "forms.  Full descriptions of the functions are in the");
         tcu_notice_text (&my_notice,
	              "documentation files provided.  For additional support");
         tcu_notice_text (&my_notice,
	              "& queries, contact:");
         tcu_notice_text (&my_notice, "");
         tcu_notice_text (&my_notice, "      Karl Keyte");
         tcu_notice_text (&my_notice, "      KKEYTE@ESOC.BITNET");
         tcu_notice_text (&my_notice, "");

         tcu_display_notice (&my_notice, 20, 14);  /* Put the notice up */

         tcu_clear_notice (&my_notice);            /* Get rid of notice definition */

      } else {   /* ...otherwise get ready for user's input */

         tcu_prepare_notice (&my_notice,
                             "",
                             0,
	                     tcu_colour_attrib (BLACK, GREEN),
                             tcu_colour_attrib (WHITE, GREEN),
                             TCU_BOX_SINGLE);

         tcu_notice_text (&my_notice,
	              "Customer Name :");   /* Prompt bit for input */

         buf[0] = 30;    /* Maximum length of input string */
         tcu_prompt_input (&my_notice,
                           17, 1, buf, tcu_colour_attrib (YELLOW, RED));

         tcu_display_notice (&my_notice,
	                     20, 6+select);   /* Put up prompt & get string */

         tcu_clear_notice (&my_notice);       /* Get rid of prompt definition */

         if ((signed char) buf[1] != -1) {

            fval.v_string = buf+2;             /* Load form name field with */
            tcu_put_field (&form, name_id, &fval); /* the string just obtained */

            tcu_set_field_mode (&form, name_id, TCU_FORM_NOEDIT); /* No changing name! */
            if (select == 3)
               tcu_set_form_mode (&form, TCU_FORM_NOEDIT);
            else
               tcu_set_form_mode (&form, TCU_FORM_EDIT);

            fval.v_int = 0;
            tcu_put_field (&form, time_field, &fval);

            tcu_set_idle_loop (idler);

            tcu_display_form (&form, pos_x, pos_y); /* Display the form and allow */
            tcu_edit_form (&form, 1, &rkey);        /* interactive edits */

            tcu_set_idle_loop (NULL);

            tcu_remove_form (&form);                /* Remove the form from screen */

         }
      }
   }


   tcu_unload_form (&form);       /* Unload the form definition from memory */
   tcu_remove_menu (&mymenu);     /* Remove the menu from the screen */


/* This next section is just to demonstrate the way escape keys work from */
/* menus.  It shows what key was used to leave the menu */


   if (select == 0)
      printf ("Error in displaying menu!\n");
   else
      if (select < 0) {
         select = -select;
         switch (select) {
            case TCU_ESC_ESC:    printf ("User cancelled with ESC key\n");
                                 break;
            case TCU_ESC_PGUP:   printf ("User escaped with PgUp key\n");
                                 break;
            case TCU_ESC_PGDN:   printf ("User escaped with PgDn key\n");
                                 break;
            case TCU_ESC_CLEFT:  printf ("User escaped with <--- key\n");
                                 break;
            case TCU_ESC_CRIGHT: printf ("User escaped with ---> key\n");
                                 break;
            case TCU_ESC_CNTL_C: printf ("User escaped with CNTL/C\n");
                                 break;
            case TCU_ESC_FUNC:   printf ("User escaped with F-%d key\n",
                                         tcu_escape_fkey ());
                                 break;
            case TCU_ESC_USERKEY:printf ("User defined escape key\n");
                                 break;
            default:             printf ("??? Unknown escape key!\n");
                                 break;
         }
      }
}


int far test_age (TCU_FORM *frm, int field, TCU_FIELD_VALUE *val)
{
   int                  age_fld,
                        mstatus_id;
   TCU_FIELD_VALUE      local_field;

   tcu_get_field_id (frm, "age", &age_fld);
   if (field != age_fld)
      return (1);

   if (val->v_int == 50) {

      tcu_prepare_notice (&my_notice,
                          "",
                          0,
                          tcu_colour_attrib (YELLOW, RED),
                          tcu_colour_attrib (YELLOW, RED),
                          TCU_BOX_SINGLE);

      tcu_notice_text (&my_notice, "The AGE must be a number from 18 to");
      tcu_notice_text (&my_notice, "120, excluding 50 which is a boring");
      tcu_notice_text (&my_notice, "sort of age!");

      tcu_display_notice (&my_notice, 15, 9);
      tcu_clear_notice (&my_notice);

      return (0);    /* Failed verification */
   }

/* As a little demo., check if the age is less than 16, and if so set the */
/* married status field to NO and disable changes in that field.  If the */
/* age is 16 or over, ensure that the field is editable. */

   tcu_get_field_id (frm, "MSTATUS", &mstatus_id);
   local_field.v_logical = 0;

   if (val->v_int < 16) {
      tcu_put_field (frm, mstatus_id, &local_field);
      tcu_set_field_mode (frm, mstatus_id, TCU_FORM_NOEDIT);
   } else
      tcu_set_field_mode (frm, mstatus_id, TCU_FORM_EDIT);

   return (1);       /* Passed verification */
#pragma warn -par
}
#pragma warn .par


void far help (TCU_FORM *form, int field)
{
   tcu_prepare_notice (&my_notice,
                       "HELP",
                       tcu_colour_attrib (BLUE, GREEN),
                       tcu_colour_attrib (WHITE, GREEN),
                       tcu_colour_attrib (WHITE, GREEN),
                       TCU_BOX_DOUBLE);

   tcu_notice_text (&my_notice, "Field : %03d", field);
   tcu_notice_text (&my_notice, "");
   tcu_notice_text (&my_notice, "Please complete the form displayed and");
   tcu_notice_text (&my_notice, "finish your entry with:");
   tcu_notice_text (&my_notice, "");
   tcu_notice_text (&my_notice, "    ESC   : To abort the entry");
   tcu_notice_text (&my_notice, " or PgUp  : To complete the entry");
   tcu_notice_text (&my_notice, "");
   tcu_notice_text (&my_notice, "      PRESS RETURN TO CONTINUE");

   tcu_display_notice (&my_notice, 19, 6);
   tcu_clear_notice (&my_notice);
#pragma warn -par
}
#pragma warn .par


/* Function key handler */

int far fn_keys (TCU_FORM *f, int field, int key)
{
   if (key == 11) {
      FILE *of;
      of = fopen ("demo.dat", "wb");
      tcu_write_formrec (f, record);
      fwrite (record, 155, 1, of);
      fclose (of);
      return (3);
   }

   if (key == 12) {
      FILE *inf;
      inf = fopen ("demo.dat", "rb");
      fread (record, 155, 1, inf);
      fclose (inf);
      tcu_read_formrec (f, record);
      return (3);
   }

   tcu_prepare_notice (&my_notice,
                       "",
                       0,
                       tcu_colour_attrib (WHITE, GREEN),   /* Reject all function */
                       tcu_colour_attrib (WHITE, GREEN),   /* keys. */
                       TCU_BOX_DOUBLE);

   tcu_notice_text (&my_notice,
                "You just pressed F-%d on field %d which is undefined!",
                key, field);
   tcu_display_notice (&my_notice, 15, 12);
   tcu_clear_notice (&my_notice);

   return (0);          /* 0 means continue as if no key was pressed */
#pragma warn -par
}
#pragma warn +par


int far idler (unsigned long ticks)
{
   time_t               timer;
   char                 ch_time[6];
   TCU_FIELD_VALUE      val;

   if (!(ticks % 100)) {
      tcu_save_environment ();
      time (&timer);
      strncpy (ch_time, ctime (&timer) + 11, 5);
      ch_time[5] = '\0';
      val.v_string = ch_time;
      tcu_put_field (_TCU_UPDATE_form, time_field, &val);
      tcu_restore_environment ();
   }
   return (0);      
}


/* Change ALT = into accept character and '~' into '?' */

int far keyboard_handler (unsigned short *scancode)
{
   if (*scancode == 0x8300)
      return (1);
   if ((*scancode & 0x00FF) == '~')
      *scancode = '?';
   return (0);
}
