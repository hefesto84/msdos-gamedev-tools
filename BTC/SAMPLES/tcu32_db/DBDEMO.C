#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <usr\tcu.h>


static TCU_DB            db;


void main (void)
{
   int               rkey;
   char             *keys[] = { "LName" , "FName" , "Date_Reg" , NULL };
   TCU_FORM          form;
   TCU_FORM_INFO     finfo;
   TCU_WINDOW        win;

   int  far          button_handler (TCU_FORM *, int);
   void far          help_handler (TCU_FORM *, int);

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Open the form object file */
/* ------------------------- */

   if (tcu_load_form (&form, "dbform") != TCU_OK) {
      printf ("DBDEMO: Error opening form 'dbform.cfo'\n");
      exit (1);
   }


/* Set up the button and help handlers for the form */
/* ------------------------------------------------ */

   tcu_set_button_fn (&form, button_handler);
   tcu_set_form_help (&form, help_handler);


/* Try to open existing database, else try to create new one */
/* --------------------------------------------------------- */

   if (tcu_db_open (&db, &form, "demo_db", 2) == TCU_ERROR)
      if (tcu_db_create (&db, &form, "demo_db", 2, keys) == TCU_ERROR) {
         tcu_unload_form (&form);
         printf ("DBDEMO: Cannot open or create database\n");
         exit (1);
      }


/* Set the search options for the database */
/* --------------------------------------- */

   tcu_db_set_search_mode (&db, TCU_DB_IGNORE_CASE);
   tcu_db_set_search_indices (&db, 2);


/* Open window as background */
/* ------------------------- */

   tcu_open_window (&win, 1, 1, 80, 25, "",
                    tcu_colour_attrib (LIGHTGREEN, BLUE),
                    tcu_colour_attrib (0, BLUE),
                    tcu_colour_attrib (0, BLUE),
                    TCU_BOX_SINGLE);


/* Get form info. and use it to display the form in the middle at the top */
/* ---------------------------------------------------------------------- */

   tcu_get_form_info (&form, &finfo);
   tcu_display_form (&form, (81-finfo.width)/2, 2);


/* All the work is now done by the button handler function.  Edit the form */
/* and quit only when selected.  Start at the first database record.       */
/* ----------------------------------------------------------------------- */

   tcu_db_first (&db);
   tcu_db_read (&db);

   tcu_set_form_mode (&form, TCU_FORM_NOESCS);
   do
      tcu_edit_form (&form, 1, &rkey);
   while (rkey != TCU_FLD_BUTTONESC);
   

/* Finished!  Can remove database and form structures and terminate */
/* ---------------------------------------------------------------- */

   tcu_db_close (&db);
   tcu_unload_form (&form);
   tcu_close_window (&win);

   exit (0);
}



int far button_handler (TCU_FORM *form, int button)
{
   int                ret_val,
                      stat;
   TCU_NOTICE         nt;
   unsigned char      bcol,
                      ncol;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Save the environment so we can use notices relative to the whole screen */
/* ----------------------------------------------------------------------- */

   tcu_save_environment ();
   tcu_db_start_form_edit (&db);


/* If QUIT is selected, check that the user is really sure */
/* ------------------------------------------------------- */

   if (button == tcu_get_field_id (form, "B_Quit", NULL))
      if (tcu_get_confirm (22, 12,
                           tcu_colour_attrib (WHITE,MAGENTA),
                           tcu_colour_attrib (WHITE,MAGENTA),
                           "Are you sure you want to quit (Y/N)?"))
         ret_val = 2;
      else
         ret_val = 3;

   bcol = tcu_colour_attrib (WHITE,BLUE);
   ncol = tcu_colour_attrib (YELLOW,BLUE);

   if (button == tcu_get_field_id (form, "B_First", NULL) ||
       button == tcu_get_field_id (form, "B_Last", NULL)) {
      if (button == tcu_get_field_id (form, "B_First", NULL))
         stat = tcu_db_first (&db);
      else
         stat = tcu_db_last (&db);
      if (stat == TCU_ERROR) {
         tcu_prepare_notice (&nt, "Warning", bcol, bcol, ncol,
                             TCU_BOX_SINGLE);
         tcu_notice_text (&nt, "Database is empty!");
         tcu_display_notice (&nt, 29, 22);
         tcu_clear_notice (&nt);
         ret_val = 3;
      } else {
         tcu_db_read (&db);
         ret_val = 1;
      }
   }

   if (button == tcu_get_field_id (form, "B_Next", NULL))
      if (tcu_db_next (&db) == TCU_ERROR) {
         tcu_prepare_notice (&nt, "Warning", bcol, bcol, ncol,
                             TCU_BOX_SINGLE);
         tcu_notice_text (&nt, "END OF DATABASE");
         tcu_display_notice (&nt, 30, 22);
         tcu_clear_notice (&nt);
         tcu_db_last (&db);
         ret_val = 3;
      } else {
         tcu_db_read (&db);
         ret_val = 1;
      }

   if (button == tcu_get_field_id (form, "B_Prev", NULL))
      if (tcu_db_previous (&db) == TCU_ERROR) {
         tcu_prepare_notice (&nt, "Warning", bcol, bcol, ncol,
                             TCU_BOX_SINGLE);
         tcu_notice_text (&nt, "START OF DATABASE");
         tcu_display_notice (&nt, 29, 22);
         tcu_clear_notice (&nt);
         tcu_db_first (&db);
         ret_val = 3;
      } else {
         tcu_db_read (&db);
         ret_val = 1;
      }

   if (button == tcu_get_field_id (form, "B_Add", NULL))
      if (tcu_db_write (&db) == TCU_ERROR) {
         tcu_prepare_notice (&nt, "Warning", bcol, bcol, ncol,
                             TCU_BOX_SINGLE);
         tcu_notice_text (&nt, "DUPLICATES EXISTING RECORD");
         tcu_display_notice (&nt, 25, 22);
         tcu_clear_notice (&nt);
         ret_val = 3;
      } else {
         tcu_prepare_notice (&nt, "New Record", bcol, bcol, ncol,
                             TCU_BOX_SINGLE);
         tcu_notice_text (&nt, "RECORD ADDED, %d RECORDS",
                          tcu_db_record_count (&db));
         tcu_display_notice (&nt, 26, 22);
         tcu_clear_notice (&nt);
         ret_val = 1;
      }

   if (button == tcu_get_field_id (form, "B_Delete", NULL))
      if (tcu_db_delete (&db) == TCU_ERROR) {
         tcu_prepare_notice (&nt, "Warning", bcol, bcol, ncol,
                             TCU_BOX_SINGLE);
         tcu_notice_text (&nt, "NO RECORD TO DELETE");
         tcu_display_notice (&nt, 28, 22);
         tcu_clear_notice (&nt);
         ret_val = 3;
      } else {
         if (tcu_db_at_eof (&db))
            tcu_db_last (&db);
         if (!tcu_db_record_count (&db))
            tcu_clear_form_fields (form);
         else
            tcu_db_read (&db);
         ret_val = 1;
      }

   if (button == tcu_get_field_id (form, "B_Search", NULL)) {
      if (tcu_db_search (&db, NULL) == TCU_ERROR) {
         tcu_prepare_notice (&nt, "Warning", bcol, bcol, ncol,
                             TCU_BOX_SINGLE);
         tcu_notice_text (&nt, "NO SUCH RECORD FOUND");
         tcu_display_notice (&nt, 28, 22);
         tcu_clear_notice (&nt);
         ret_val = 3;
      } else {
         tcu_db_read (&db);
         ret_val = 1;
      }
   }

   if (button == tcu_get_field_id (form, "B_Update", NULL))
      if (tcu_db_rewrite (&db) == TCU_ERROR) {
         tcu_prepare_notice (&nt, "Warning", bcol, bcol, ncol,
                             TCU_BOX_SINGLE);
         tcu_notice_text (&nt, "RECORD DOES NOT EXIST");
         tcu_display_notice (&nt, 28, 22);
         tcu_clear_notice (&nt);
         ret_val = 3;
      } else
         ret_val = 1;

   tcu_db_end_form_edit (&db);
   tcu_restore_environment ();
   return (ret_val);
}



void far help_handler (TCU_FORM *form, int field)
{
   TCU_NOTICE           nt;
   long                 recs;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

   tcu_prepare_notice (&nt, "HELP!",
                       tcu_colour_attrib (WHITE, BLUE),
                       tcu_colour_attrib (WHITE, BLUE),
                       tcu_colour_attrib (YELLOW, BLUE),
                       TCU_BOX_SINGLE);
   tcu_notice_text (&nt, "Use the usual form editing facilities to complete");
   tcu_notice_text (&nt, "form entries. Use the mouse (or keyboard & =) to");
   tcu_notice_text (&nt, "perform the appropriate function:");
   tcu_notice_text (&nt, "");
   tcu_notice_text (&nt, "    FIRST     : Move to first record");
   tcu_notice_text (&nt, "    LAST      : Move to last record");
   tcu_notice_text (&nt, "    NEXT      : Move to next record");
   tcu_notice_text (&nt, "    PREVIOUS  : Move to previous record");
   tcu_notice_text (&nt, "    SEARCH    : Search for matching name");
   tcu_notice_text (&nt, "    ADD       : Add new record");
   tcu_notice_text (&nt, "    DELETE    : Delete displayed record");
   tcu_notice_text (&nt, "    UPDATE    : Change an existing record");
   tcu_notice_text (&nt, "    QUIT      : Exit the demonstration");
   tcu_notice_text (&nt, "");
   recs = tcu_db_record_count (&db);
   tcu_notice_text (&nt, "There %s currently %ld record%s in the database",
                    (recs == 1)? "is" : "are", recs, (recs == 1)? "" : "s");
   tcu_display_notice (&nt, 14, 8);
   tcu_clear_notice (&nt);
#pragma warn -par
}
#pragma warn .par
