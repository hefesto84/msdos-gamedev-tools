/*===========================================================================
 *                                                                          *
 *   TCU v3.2 Installation Program                                          *
 *                                                                          *
 *   This program is provided to install the TCU system files into user     *
 *   selected directories.  This source code is supplied as part of the     *
 *   package as a demonstration of a TCU application.  It does not use      *
 *   all of the TCU services & facilities, but adequate use of the major    *
 *   functions is made to provide an example of TCU application coding.     *
 *                                                                          *
 *--------------------------------------------------------------------------*
 *                                                                          *
 *   Written by     : Karl Keyte            Gross-Gerauer Strasse 10        *
 *   Date           : 91.03.20              6108 Weiterstadt                *
 *                                          Germany                         *
 *                                          Tel: +(49) 6150 2499            *
 *                                                                          *
 *==========================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <dir.h>
#include <io.h>
#include <usr\tcu.h>


extern TCU_FORM CONFIG;                        /* Linked-in form object */

static unsigned char models[5] = { 1, 0, 0, 1, 0 }; /* Memory models to use */
static int           dirs[5];                  /* Field IDs of the directory */
                                               /* fields in the form */
void main (void)
{
   TCU_WINDOW      screen;
   TCU_NOTICE      notice;
   TCU_FORM        form;
   int             do_cpp,
                   fkey,
                   i, j;
   char            copylib[10],
                   dev[3],
                   dir[65],
                   direct[65],
                   field_name[7],
                  *mmname[] = { "Small", "Compact", "Medium", "Large", "Huge" },
                   tc_include[81],
                   tc_lib[81];
   TCU_FIELD_VALUE val;

   int         far button_handler   (TCU_FORM *, int),
               far form_verify      (TCU_FORM *, int, TCU_FIELD_VALUE *),
               far fn_keys          (TCU_FORM *, int, int);
   int             checkout         (void),
                   create_directory (char *),
                   merge_libs       (char *, char *);

   void            copy_file        (char *, char *),
               far help             (TCU_FORM *, int);

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Open the whole screen as a window to provide a background */

   tcu_open_window (&screen,
                    1, 1,
                    80, 25,
                    "TCU 3.2 Installation",
                    tcu_colour_attrib (RED, LIGHTGRAY),
                    tcu_colour_attrib (WHITE, BLUE),
                    tcu_colour_attrib (BLACK, LIGHTGRAY),
                    TCU_BOX_DOUBLE);


/* Check that the pre-requisites of the installation are OK */

   i = checkout ();           /* 0=OK, 1=Missing Files, 2=TLIB not found */

   if (i) {
      tcu_prepare_notice (&notice, "Installation Error",
                          tcu_colour_attrib (WHITE, LIGHTGRAY),
                          tcu_colour_attrib (RED, LIGHTGRAY),
                          tcu_colour_attrib (WHITE, LIGHTGRAY),
                          TCU_BOX_DOUBLE);
      if (i == 1) {
         tcu_notice_text (&notice,
                      "Some or all of the files required for installation");
         tcu_notice_text (&notice,
                      "of TCU 3.2 are not present in the installation");
         tcu_notice_text (&notice,
                      "directory.  Please re-copy the installation files");
         tcu_notice_text (&notice,
                      "to the installation directory and try again.");
      } else {
         tcu_notice_text (&notice,
                      "The Turbo Librarian, TLIB.EXE, is not found in the");
         tcu_notice_text (&notice,
                      "searchpath.  Please either change the path or copy");
         tcu_notice_text (&notice,
                      "TLIB.EXE temporarily into the installation directory");
      }
      tcu_notice_text (&notice, "");
      tcu_notice_text (&notice, "           -- Press RETURN to continue --");
      tcu_display_notice (&notice, 15, 8);
      tcu_clear_notice (&notice);
      tcu_close_window (&screen);
      return;
   }

   tcu_position_cursor (&screen, 1, 6);
   tcu_wprintf (&screen,
        "               You are in the installation utility for TCU 3.2.\n");
   tcu_wprintf (&screen,
        "               You will be prompted for options required for\n");
   tcu_wprintf (&screen,
        "               the installation.");


/* Check that the installation is really wanted */

   if (!tcu_get_confirm (24, 10, tcu_colour_attrib (BLUE, BLUE),
                         tcu_colour_attrib (WHITE, BLUE),
                         "Do you wish to continue (Y/N)?")) {
      tcu_close_window (&screen);
      return;
   }

   tcu_clear_window (&screen);
   tcu_wprintf (&screen,
            "\n                  Please complete the following information:");


/* Load the linked-in form into the form object */

   tcu_load_image_form (&form, &CONFIG);

   tcu_set_form_fnkey_fn (&form, fn_keys);     /* Function key handler */
   tcu_set_field_verify (&form, form_verify);  /* Field verification handler */
   tcu_set_button_fn (&form, button_handler);  /* Button field handler */
   tcu_set_form_help (&form, help);            /* Help function */


/* Prevent escape keys quitting form entry */

   tcu_set_form_mode (&form, TCU_FORM_NOESCS);


/* Obtain the field IDs for the main installation directories */

   for (i=0; i<5; i++) {
      sprintf (field_name, "DIR_%02d", i+1);
      tcu_get_field_id (&form, field_name, &dirs[i]);
   }


/* Find the current disc */

   val.v_string = dir;
   dir[0] = getdisk () + 'A';


/* Try to locate the file 'tcinst.exe' as a clue to where the main TC root */
/* might be.  If not found, we'll just suggest \TC on the current drive */

   if (!searchpath ("tcinst.exe"))
      strcpy (dir+1, ":\\TC\\");
   else {
      strcpy (dir, searchpath ("tcinst.exe"));
      fnsplit (dir, dev, direct, NULL, NULL);
      fnmerge (dir, dev, direct, NULL, NULL);
   }
   tcu_put_field (&form, tcu_get_field_id (&form, "TC_DIR", NULL), &val);
   dir[0] = getdisk () + 'A';


/* Pre-load the installation directory names */

   strcpy (dir+1, ":\\TCU32\\");
   tcu_put_field (&form, dirs[0], &val);
   tcu_put_field (&form, dirs[3], &val);
   tcu_put_field (&form, dirs[4], &val);
   strcpy (dir+1, ":\\TCU32\\LIB\\");
   tcu_put_field (&form, dirs[1], &val);
   strcpy (dir+1, ":\\TCU32\\EXAMPLES\\");
   tcu_put_field (&form, dirs[2], &val);


/* Show the form and edit.  Only funtion key handler has the ability to */
/* stop the edit as escape keys have been disabled. */

   tcu_display_form (&form, 4, 6);

   tcu_edit_form (&form, 1, &fkey);


/* We can remove the form from the screen now that we've finished with it */

   tcu_remove_form (&form);


/* If an abort was requested, shut down... */

   if (fkey == TCU_FLD_FNKEYESC) {
      tcu_unload_form (&form);
      tcu_close_window (&screen);
      return;
   }


/* Move into real installation phase */

   tcu_clear_window (&screen);
   tcu_wprintf (&screen, "\n Installing...\n\n");


/* Form the directory names for the real TC RTL libraries and the user */
/* include files */

   val.v_string = tc_lib;
   tcu_get_field (&form, tcu_get_field_id (&form, "TC_DIR", NULL), &val);
   if (strchr (tc_lib, ' '))
      *strchr (tc_lib, ' ') = '\0';
   if (tc_lib[strlen (tc_lib) - 1] != '\\')
      strcat (tc_lib, "\\");
   strcpy (tc_include, tc_lib);
   strcat (tc_include, "include\\usr\\");
   strcat (tc_lib, "lib\\");


/* Make sure that the directories are there - will create them if they are */
/* not. */

   create_directory (tc_include);
   create_directory (tc_lib);


/* Check whether installing for TC or TC++ */

   tcu_get_field (&form, tcu_get_field_id (&form, "TC_CPP", NULL), &val);
   do_cpp = val.v_choice.sel;


/* Process files destined for each of the 5 main directories, directory by */
/* directory. */

   for (i=0; i<5; i++) {
      val.v_string = dir;
      tcu_get_field (&form, dirs[i], &val);    /* Get the directory name */
      if (strchr (dir, ' '))
         *strchr (dir, ' ') = '\0';            /* Strip trailing spaces */
      if (!create_directory (dir)) {           /* If can't create directory */
         tcu_change_colour (&screen, tcu_colour_attrib (LIGHTRED, BLUE));
         tcu_wprintf (&screen, " >>> Error installing into %s\n", dir);
         tcu_change_colour (&screen, tcu_colour_attrib (WHITE, BLUE));
         continue;
      }
      switch (i) { /* Now process files according to the directory */
         case 0:   tcu_wprintf (&screen, "    Installing  - Root System\n");
                   copy_file ("aa-read.me!", dir);
                   copy_file ("readme.1st", dir);
                   copy_file ("tcu.h", tc_include);
                   copy_file ("tcupre32.h", tc_include);
                   break;
         case 1:   tcu_wprintf (&screen, "                - Libraries\n");
                   tcu_change_colour (&screen,
                                      tcu_colour_attrib (LIGHTGREEN, BLUE));
                   for (j=0; j<5; j++)
                      if (models[j]) {
                         tcu_wprintf (&screen, "                     o %s\n",
                                      mmname[j]);
                         sprintf (copylib, "tcu_%c.lib", mmname[j][0]);
                         copy_file (copylib, dir);
                      }
                   if (do_cpp == 1)
                      copy_file ("tcu_cpp.lib", dir);
                   tcu_change_colour (&screen,
                                      tcu_colour_attrib (WHITE, BLUE));
                   break;
         case 2:   tcu_get_field (&form,
                                  tcu_get_field_id (&form, "EXAMPLES", NULL),
                                  &val);
                   if (!val.v_logical)
                      break;
                   tcu_wprintf (&screen, "                - Examples/demos\n");
                   copy_file ("*.c", dir);
                   copy_file ("*.cuf", dir);
                   copy_file ("demo.exe", dir);
                   copy_file ("pulldown.exe", dir);
                   copy_file ("select.exe", dir);
                   copy_file ("button.exe", dir);
                   copy_file ("windows.exe", dir);
                   break;
         case 3:   tcu_wprintf (&screen, "                - Utilities\n");
                   copy_file ("tcuinst.exe", dir);
                   copy_file ("fcomp.exe", dir);
                   copy_file ("fshow.exe", dir);
                   break;
         case 4:   tcu_wprintf (&screen, "                - Documents\n\n");
                   copy_file ("*.doc", dir);
                   break;
         default:  break;
      }
   }


/* Check whether the TCU libraries are to be merged with the RTLs */

   tcu_get_field (&form, tcu_get_field_id (&form, "MERGE", NULL), &val);
   if (val.v_logical) {
      tcu_wprintf (&screen, "    Merging libraries into main libraries\n");
      for (i=0; i<5; i++)
         if (models[i]) {
            sprintf (dir, "%sc%c", tc_lib, mmname[i][0]);
            sprintf (direct, "tcu_%c", mmname[i][0]);
            merge_libs (dir, direct);
            if (do_cpp == 1)
               merge_libs (dir, "tcu_cpp");
         }
      sprintf (dir, "del %s*.bak", tc_lib);
      system (dir);
      tcu_wprintf (&screen,
                   "    You may wish to delete the TCU libraries\n");
      tcu_wprintf (&screen,
                   "    now that they have been merged into the RTLs\n\n");
   }

   tcu_wprintf (&screen,
                "\n\n Installation Complete - Press a key to continue");

   while (!kbhit()); getch();


/* End of installation - remove form from memory and close the window */

   tcu_unload_form (&form);
   tcu_close_window (&screen);

   printf ("\nGood luck!\n");
}



/* FUNCTION KEY HANDLER */

int far fn_keys (TCU_FORM *form, int field, int key)
{
   switch (key) {
      case 9:      tcu_save_environment ();
                   if (tcu_get_confirm (24, 11,
                                        tcu_colour_attrib (WHITE, MAGENTA),
                                        tcu_colour_attrib (WHITE, MAGENTA),
                                        "Do you really want to quit (Y/N)?")) {
                      tcu_restore_environment ();
                      return (2);    /* 2 = Escape request */
                   } else {
                      tcu_restore_environment ();
                      return (0);    /* Ignore... */
                   }
      case 10:     return (1);       /* 1 = Save request */
      default:     tcu_warnbeep();
                   return (0);       /* 0 = Ignore */
   }
#pragma warn -par
}
#pragma warn .par



/* FIELD VERIFICATION FUNCTION */

int far form_verify (TCU_FORM *form, int field, TCU_FIELD_VALUE *val)
{
   int               i;
   char              new[27];
   TCU_FIELD_VALUE   new_val;

   new_val.v_string = new;

   for (i=0; i<5; i++)                         /* Make sure that each of the */
      if (field == dirs[i]) {                  /* 5 main directories are */
         strcpy (new, val->v_string);          /* terminated with a '\' and */
         if (strchr (new, ' '))                /* with no trailing spaces. */
            *strchr (new, ' ') = '\0';
         if (new[strlen (new) - 1] != '\\')
            strcat (new, "\\");
         strupr (new);                            /* Force to upper case */
         tcu_put_field (form, dirs[i], &new_val); /* Write the field back */
         if (i)                                /* If any but the root, then */
            return (1);                        /* finish with verification OK */
         else
            break;
      }

   if (i)                 /* If not one of the directories then return OK */
      return (1);

   tcu_put_field (form, dirs[3], &new_val); /* Copy root directory to utils */
   tcu_put_field (form, dirs[4], &new_val); /* and documentation directories */

   strcat (new, "LIB\\");
   tcu_put_field (form, dirs[1], &new_val); /* Add LIB to root for libraries */

   new[strlen (new) - 4] = '\0';
   strcat (new, "EXAMPLES\\");
   tcu_put_field (form, dirs[2], &new_val); /* Add EXAMPLES to root for the */
                                            /* examples */
   return (1);            /* Passed verification! */
}



/* BUTTON FIELD HANDLER */

int far button_handler (TCU_FORM *form, int field)
{
   int           choice,
                 mmodels;
   char         *menu_options[] = { "Sû Small" , "C  Compact", "M  Medium",
                                    "Lû Large" , "H  Huge", NULL };
   TCU_MENU      model_menu;


   tcu_save_environment ();    /* Save the environment because we're going */
                               /* to alter the screen here */

   tcu_get_field_id (form, "MMODELS", &mmodels);  /* Get button field ID */

   if (field == mmodels) {     /* Should be because it's the only button! */

      tcu_define_menu (&model_menu, "Memory Models", /* Create the menu */
                       tcu_colour_attrib (BLUE, LIGHTGRAY),
                       tcu_colour_attrib (BLACK, LIGHTGRAY),
                       tcu_colour_attrib (BLACK, LIGHTGRAY),
                       tcu_colour_attrib (BLACK, GREEN),
                       tcu_colour_attrib (GREEN, BLACK),
                       TCU_ESC_ESC,
                       TCU_BOX_SINGLE,
                       menu_options,
                       tcu_colour_attrib (RED, LIGHTGRAY));


/* Display the menu on the screen */

      tcu_display_menu (&model_menu, 55, 16);


/* Get selections until ESC is pressed */

      while ((choice=tcu_read_menu_selection (&model_menu)) > 0) {
         models[choice-1] ^= 1;                /* Toggle selection */
         if (models[choice-1])                 /* Set the selection marker */
            menu_options[choice-1][1] = 'û';   /* in the option line in the */
         else                                  /* menu. */
            menu_options[choice-1][1] = ' ';
         tcu_set_menu_option (&model_menu, choice-1, 1); /* Force display now */
      }


/* Clear the menu from the screen */

      tcu_remove_menu (&model_menu);
   }

   tcu_restore_environment ();                 /* Return the environment */

   return (0);
}



/* HELP FUNCTION */

void far help (TCU_FORM *form, int field)
{
   TCU_NOTICE         notice;


/* Get the notice ready */

   tcu_prepare_notice (&notice, "Help!",
                       tcu_colour_attrib (WHITE, MAGENTA),
                       tcu_colour_attrib (WHITE, MAGENTA),
                       tcu_colour_attrib (WHITE, MAGENTA),
                       TCU_BOX_SINGLE);


/* Write the appropriate text into the notice for the selected field */

   if (field == tcu_get_field_id (form, "DIR_01", NULL)) {
      tcu_notice_text (&notice, "The TCU directory is the root directory for");
      tcu_notice_text (&notice, "TCU files.  Other subdirectories are normally");
      tcu_notice_text (&notice, "created under this root directory.");
   } else

   if (field == tcu_get_field_id (form, "DIR_02", NULL)) {
      tcu_notice_text (&notice, "This is where the TCU libraries will be copied.");
      tcu_notice_text (&notice, "If you select merged libraries then these may be");
      tcu_notice_text (&notice, "deleted if you wish.");
   } else

   if (field == tcu_get_field_id (form, "DIR_03", NULL)) {
      tcu_notice_text (&notice, "The TCU example programs will be loaded into");
      tcu_notice_text (&notice, "this directory if you have selected installation");
      tcu_notice_text (&notice, "of the examples.");
   } else

   if (field == tcu_get_field_id (form, "DIR_04", NULL)) {
      tcu_notice_text (&notice, "The TCU utilities such as the forms compiler and");
      tcu_notice_text (&notice, "form viewer will be copied into this directory.");
      tcu_notice_text (&notice, "You may wish to put the utilities in a general");
      tcu_notice_text (&notice, "executables directory.");
   } else

   if (field == tcu_get_field_id (form, "DIR_05", NULL)) {
      tcu_notice_text (&notice, "The documentation for TCU and the forms compiler");
      tcu_notice_text (&notice, "will be copied into this directory.");
   } else

   if (field == tcu_get_field_id (form, "MERGE", NULL)) {
      tcu_notice_text (&notice, "If you select merged libraries, the TCU libraries");
      tcu_notice_text (&notice, "will be merged into the standard C run-time");
      tcu_notice_text (&notice, "libraries enabling you to link with TCU without");
      tcu_notice_text (&notice, "specifying additional libraries.");
   } else

   if (field == tcu_get_field_id (form, "EXAMPLES", NULL)) {
      tcu_notice_text (&notice, "If you request the loading of examples, they will");
      tcu_notice_text (&notice, "be copied into the directory you have specified");
      tcu_notice_text (&notice, "for examples.");
   } else

   if (field == tcu_get_field_id (form, "TC_DIR", NULL)) {
      tcu_notice_text (&notice, "This is the Root directory of Turbo C / C++. It");
      tcu_notice_text (&notice, "is normally called TC, TCP or BC.  It is assumed");
      tcu_notice_text (&notice, "that the Turbo libraries are in a subdirectory");
      tcu_notice_text (&notice, "called LIB under this directory.");
   } else

   if (field == tcu_get_field_id (form, "TC_CPP", NULL)) {
      tcu_notice_text (&notice, "This identifies whether TCU is used with Turbo");
      tcu_notice_text (&notice, "C v2.0 or for Turbo/Borland C++.  It is used to");
      tcu_notice_text (&notice, "merge the special library TCU_CPP when used with");
      tcu_notice_text (&notice, "Turbo C 2.0");
   } else

   if (field == tcu_get_field_id (form, "MMODELS", NULL)) {
      tcu_notice_text (&notice, "Select this with '=' or the mouse to obtain a");
      tcu_notice_text (&notice, "menu of memory models.  Only libraries for those");
      tcu_notice_text (&notice, "selected memory models will be loaded and");
      tcu_notice_text (&notice, "merged.");
   } else {

      tcu_notice_text (&notice, "Sorry, no help available!");
   }


/* Finish off notice, display, clear and exit */

   tcu_notice_text (&notice, "");
   tcu_notice_text (&notice, "          -- Press RETURN to continue --");
   tcu_display_notice (&notice, 16, 17);

   tcu_clear_notice (&notice);
}



/* Make sure directory exists, creating it if not */

int create_directory (char *d)
{
   char             dev[3],
                    dir[65],
                    path[81],
                   *scan;

   strcpy (path, d);

   fnsplit (path, dev, dir, NULL, NULL);

   strcpy (path, dev);
   strcat (path, "\\");

   scan = strtok (dir, "\\");
   while (scan) {
      strcat (path, scan);
      if (access (path, 0))
         if (mkdir (path))
            return (0);
      strcat (path, "\\");
      scan = strtok (NULL, "\\");
   }

   return (1);
}



/* Copy a file to a specified directory */

void copy_file (char *file, char *dir)
{
   char            cmd[81];

   sprintf (cmd, "copy %s %s%s/v >nul", file, dir, file);
   system (cmd);
}



/* Check the installation can succeed. */

int checkout (void)
{
   char           *flist[] = { "aa-read.me!",  "tcu.doc",      "fcomp.doc",
                               "tcu.h",        "tcupre32.h",   "tcu_cpp.lib",
                               "tcu_s.lib",    "tcu_c.lib",    "tcu_m.lib",
                               "tcu_l.lib",    "tcu_h.lib",    "demo.c",
                               "demo.exe",     "sample.cuf",   "pulldown.c",
                               "pulldown.exe", "pulldown.cuf", "select.c",
                               "select.exe",   "windows.c",    "windows.exe",
                               "fshow.exe",    "fcomp.exe",    "button.c",
                               "button.exe",   "button.cuf",   "tcuinst.c",
                               "mlib.c",       "tcuinst.exe",  "config.cuf",
                               "readme.1st",

                                NULL };
   int             i;

   for (i=0; flist[i]; i++)        /* Check that all the files exist in */
      if (access (flist[i], 0))    /* the directory */
         return (1);

   if (!searchpath ("tlib.exe"))   /* Check that we can get at TLIB when we */
      return (2);                  /* need it. */

   return (0);
}
