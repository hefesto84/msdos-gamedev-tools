#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <usr\tcu.h>

char               *headings[] = { "RRecords", "FFinance", "LLoad/Store",
                                   "IInvoices", "SStatistics", "OLOGOUT",
                                   NULL };

void main (void)
{
   TCU_MENU            records_m,
                       finance_m,
                       load_m,
                       invoices_m,
                       statistics_m,
                      *mlist[6];
   TCU_FORM            form;
   TCU_FIELD_VALUE     frm_val;
   int                 message,
                       menu,
                       option,
                       r1,r2;
   char                msg[26];
   TCU_PULLDOWN        pd;
   void far            help_handler (int),
        far            p_help_handler (int);

   char               *records_options[] =
             { "PPatient", "SStaff", "DDoctors", "USUppliers", "AAppointment",
               "IInvoice", NULL };

   char               *finance_options[] =
             { "Credits", "Debits", "Insurances", "Unpaid", NULL };

   char               *load_options[] =
             { "Remote data load", "Send data to remote", NULL };

   char               *invoices_options[] =
             { "New", "Browse all", "Find", "Delete", "Print", NULL };

   char               *statistics_options[] =
             { "Patient", "Staff", "Invoices", "Appointments", NULL };

   tcu_set_mouse_mode (1);

   r1 = tcu_define_menu (&records_m,
                         "Records",
                         tcu_colour_attrib (YELLOW, CYAN),       /* Title */
                         tcu_colour_attrib (BLUE, CYAN),         /* Box */
                         tcu_colour_attrib (WHITE, MAGENTA),     /* Options */
                         tcu_colour_attrib (WHITE, BLUE),        /* Selected */
                         tcu_colour_attrib (LIGHTGRAY, MAGENTA), /* Deselected */
                         TCU_ESC_ESC | TCU_ESC_CNTL_C | TCU_ESC_PGUP | TCU_ESC_FUNC,
                         TCU_BOX_DOUBLE,
                         records_options,
		         tcu_colour_attrib (LIGHTRED, MAGENTA));

   r1 = tcu_define_menu (&finance_m,
                         "Finance",
                         tcu_colour_attrib (YELLOW, CYAN),       /* Title */
                         tcu_colour_attrib (BLUE, CYAN),         /* Box */
                         tcu_colour_attrib (WHITE, MAGENTA),     /* Options */
                         tcu_colour_attrib (WHITE, BLUE),        /* Selected */
                         tcu_colour_attrib (LIGHTGRAY, MAGENTA), /* Deselected */
                         TCU_ESC_ESC | TCU_ESC_CNTL_C | TCU_ESC_PGUP | TCU_ESC_FUNC,
                         TCU_BOX_DOUBLE,
                         finance_options,
		         0);

   r1 = tcu_define_menu (&load_m,
                         "Data Load",
                         tcu_colour_attrib (YELLOW, CYAN),       /* Title */
                         tcu_colour_attrib (BLUE, CYAN),         /* Box */
                         tcu_colour_attrib (WHITE, MAGENTA),     /* Options */
                         tcu_colour_attrib (WHITE, BLUE),        /* Selected */
                         tcu_colour_attrib (LIGHTGRAY, MAGENTA), /* Deselected */
                         TCU_ESC_ESC | TCU_ESC_CNTL_C | TCU_ESC_PGUP | TCU_ESC_FUNC,
                         TCU_BOX_DOUBLE,
                         load_options,
		         0);

   r1 = tcu_define_menu (&invoices_m,
                         "Invoices",
                         tcu_colour_attrib (YELLOW, CYAN),       /* Title */
                         tcu_colour_attrib (BLUE, CYAN),         /* Box */
                         tcu_colour_attrib (WHITE, MAGENTA),     /* Options */
                         tcu_colour_attrib (WHITE, BLUE),        /* Selected */
                         tcu_colour_attrib (LIGHTGRAY, MAGENTA), /* Deselected */
                         TCU_ESC_ESC | TCU_ESC_CNTL_C | TCU_ESC_PGUP | TCU_ESC_FUNC,
                         TCU_BOX_DOUBLE,
                         invoices_options,
		         0);

   r1 = tcu_define_menu (&statistics_m,
                         "Statistics",
                         tcu_colour_attrib (YELLOW, CYAN),       /* Title */
                         tcu_colour_attrib (BLUE, CYAN),         /* Box */
                         tcu_colour_attrib (WHITE, MAGENTA),     /* Options */
                         tcu_colour_attrib (WHITE, BLUE),        /* Selected */
                         tcu_colour_attrib (LIGHTGRAY, MAGENTA), /* Deselected */
                         TCU_ESC_ESC | TCU_ESC_CNTL_C | TCU_ESC_PGUP | TCU_ESC_FUNC,
                         TCU_BOX_DOUBLE,
                         statistics_options,
		         0);

   tcu_set_menu_help (&invoices_m, help_handler);
   tcu_set_pulldown_help (&pd, p_help_handler);

   tcu_set_menu_option (&invoices_m, 4, 0);

   mlist[0] = &records_m;
   mlist[1] = &finance_m;
   mlist[2] = &load_m;
   mlist[3] = &invoices_m;
   mlist[4] = &statistics_m;
   mlist[5] = NULL;

   tcu_define_pulldown    (&pd,
                           tcu_colour_attrib (WHITE,BLUE),
                           tcu_colour_attrib (YELLOW,BLUE),
                           tcu_colour_attrib (YELLOW,RED),
                           headings,
		           tcu_colour_attrib (LIGHTGREEN, BLUE),
		           mlist);

   tcu_set_pulldown_help (&pd, p_help_handler);

   tcu_load_form (&form, "pulldown");

   tcu_get_field_id (&form, "menu", &menu);
   tcu_get_field_id (&form, "option", &option);
   tcu_get_field_id (&form, "message", &message);

   tcu_set_field_mode (&form, menu, TCU_FORM_NOENTER);
   tcu_set_field_mode (&form, option, TCU_FORM_NOENTER);
   tcu_set_field_mode (&form, message, TCU_FORM_CONFIRM);

   tcu_display_form (&form, 1, 2);

   do {
      tcu_read_pulldown_selection (&pd, &r1, &r2);
      if (r1) {
         frm_val.v_int = r1;
         tcu_put_field (&form, menu, &frm_val);
         frm_val.v_int = r2;
         tcu_put_field (&form, option, &frm_val);
         if (r2)
            msg[0] = '\0';
         else
            strcpy (msg, "No sub-menu available");
         frm_val.v_string = msg;
         tcu_put_field (&form, message, &frm_val);
      }
   } while (r1);

   tcu_remove_pulldown    (&pd);

   tcu_remove_form (&form);
}

void far help_handler (int option)
{
   TCU_NOTICE        notice;

   tcu_prepare_notice (&notice,
                       "Help",
                       tcu_colour_attrib (BLUE, LIGHTGRAY),
                       tcu_colour_attrib (LIGHTBLUE, LIGHTGRAY),
                       tcu_colour_attrib (BLUE, LIGHTGRAY),
                       TCU_BOX_SINGLE);

   tcu_notice_text (&notice, "Sorry, no help defined currently");
   tcu_notice_text (&notice, "for option %d.", option);

   tcu_display_notice (&notice, 25, 20);

   tcu_clear_notice (&notice);
}

void far p_help_handler (int option)
{
   TCU_NOTICE       notice;

   tcu_prepare_notice (&notice,
                       "HELP",
                       tcu_colour_attrib (BLUE, LIGHTGRAY),
                       tcu_colour_attrib (LIGHTBLUE, LIGHTGRAY),
                       tcu_colour_attrib (BLUE, LIGHTGRAY),
                       TCU_BOX_DOUBLE);

   tcu_notice_text (&notice,
                "Sorry, no help defined currently");
   tcu_notice_text (&notice,
                "for option %s.", headings[option-1]);

   tcu_display_notice (&notice,
                       23, 20);

   tcu_clear_notice (&notice);
}
