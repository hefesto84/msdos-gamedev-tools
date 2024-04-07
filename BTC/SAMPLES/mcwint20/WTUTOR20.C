#define USE_LOCAL
#define TRUE           1
#define FALSE          0
#define ENTER          13
#define ESCAPE         27
#define RGTARROW       77
#define LFTARROW       75
#define HIDDEN_CURSOR  8192

#include <stdio.h>
#include <conio.h>
#include <alloc.h>
#include <color.h>
#include <w1.h>
#include <windprot.h>

void         show_initial_screen(void);
void         loop_the_loop(void);
void         display_main_menu(void);
void         display_window_menu(int *);
void         display_misc_menu(int *);
void         show_initwindow(int);
void         show_makewindow(int);
void         show_borders(void);
void         show_removewindow(int);
void         show_titlewindow(int);
void         show_scrollwindow(int);
void         show_specialeffects(int);
void         show_maxwndw(int);
void         show_qwik21(int);
void         show_programming(int);
void         show_ending(void);
void         show_getstring(int);
void         show_getinteger(int);
void         show_getmenu(int);
void         show_wsound(int);
void         show_wsleep(int);
void         show_introduction(int);
void         show_autodemo(void);
int          m_special(int,int*);
void         main(void);

int s_row[14] = {2,2,2,2,2,8,14,20,20,20,20,20,14,8};
int s_col[14] = {6,21,36,51,66,66,66,66,51,36,21,6,6,6};
int n_rows    = 5;
int n_cols    = 10;
int auto_menu = 0;                    /* auto menu flag */
int m_item    = 0;

    char *stra[33] = {
        " WINDOWS and QUICK WRITE UTILITIES FOR C ",
        "               Version 2.0               ",
        "      (C) 87,88 Michael G. Mlachak       ",
        "      (C) 87,88 Brian L. Cassista        ",
        "   Fast - Small - Easy   ",
        "        C H E A P        ",
        "                         ",
        " Supports many modes and ",
        "      many monitors      ",
        " To register a copy send $55 to the       ",
        " above address. Registration includes     ",
        " complete source code and printed manual. ",
        " Libraries are available for Turbo-C,     ",
        " Microsoft 4.0 and 5.0. All memory models ",
        " are supported. If you do not wish to     ",
        " become a registered user libraries for a ",
        " specific model and compiler can be       ",
        " obtained for $10 each. A printed bound   ",
        " manual can also be obtained for $10.     ",
        " Michael G. Mlachak   ",
        " 4318 Stewart Court   ",
        " East Chicago, IN.    ",
        "              46312   ",
        " WORK: (312)-407-5343 ",
        " HOME: (219)-397-8952 ",
        " CIS : 76327,1410     ",
        " Brian L. Cassista    ",
        " 165 South Floyd Lane ",
        " Chicago Hts, IL.     ",
        "              60411   ",
        " WORK: (312)-407-5358 ",
        " HOME: (312)-756-3567 "
    };

    char *strb[2] = {
       " Windows are as easy as 1, 2, 3....... ",
       " The rest of this DEMO is menu driven. ",
    };

    char *main_menu[10] = {
       "General",
       "Windows",
       "Screen",
       "Misc.",
       "Auto Demo",
       "Exit Demo",
       " MC-WINDOWS (TURBO 1.0 DEMO)อออออออออออออ <- -> - Select   Enter-Accept ",
       " ",
       ""
    };

    char *strc[8] = {
      "initwindow",
      " ",
      "FORMAT:   void initwindow (int windowattribute,",
      "                           int clear_screen)",
      "Initwindow initializes  several variables required  by  the",
      "windowing package,  as well as selecting the foreground and",
      "background colors of the initial screen display. Initwindow",
      "must be called before using any of the other functions.    "};

    char *strd[17] = {
      "makewindow",
      " ",
      "FORMAT:  void makewindow (int row,col,rows,cols,wattr,battr,",
      "                                            BORDERS brdrsel);",
      " ",
      "Makewindow puts a new blank window on the display.  The window starts",
      "at the upper left corner (row,col)  and extends for a number of  rows",
      "and columns (rows,cols).   If a border exists, the actual  dimensions",
      "of the TURBO C window will be 2 less than indicated in the makewindow",
      "statement.  The border can be one of the following: (SEE w1.h)",
      " ",
      "   nobrdr     - just window       evensolidbrdr - evenly solid",
      "   blankbrdr  - blank spaces      thinsolidbrdr - thin solid line",
      "   singlebrdr - single line       lhatchbrdr - light hatch",
      "   doublebrdr - double line       mhatchbrdr - medium hatch",
      "   mixedbrdr  - single/double     hhatchbrdr - heavy hatch",
      "   solidbrdr  - solid             userbrdr   - user defined border"};

    char *stre[18] = {
      "removewindow",
      "FORMAT:   void removewindow(void);",
      " ",
      "Removewindow removes the last window",
      "remaining  on  the screen  from  the",
      "makewindow function.",
      "remove_windows",
      "FORMAT:   void remove_windows(int);",
      " ",
      "Remove_windows removes the number of",
      "windows specified. It checks to see ",
      "if there are that amount to remove. ",
      "remove_all_windows",
      "FORMAT: void remove_all_windows(void);",
      " ",
      "Remove_all_windows removes all of   ",
      "the currently displayed windows from",
      "the makewindow function."};

    char *strf[2] = {
      "See if your BIOS gives you flicker",
      "when your screen rolls down next ..."};

    char *strg[19] = {
      " ",
      "    scrollwindow",
      " ",
      " ",
      "FORMAT:  void scrollwindow (byte rowbegin,rowend byte; DIRTYPE dir);",
      "",
      "The BIOS scroll that was used for this call may have worked just fine",
      "for this window.  However, if  your screen just had some bad flicker ",
      "as it was scrolling down, your BIOS is not flicker-free.  To keep the",
      "display flicker-free,  to work on other  video pages  or an EGA, then",
      "you will need this function.  The upward  scroll, used scrollwindow, ",
      "so no flicker was seen then.  It also scrolls partial windows.",
      " ",
      "The direction of the scroll can be any of the following:(SEE w1.h)",
      " ",
      "    'up'   - to scroll up",
      "    'down' - to scroll down",
      " ",
      " "};

    char *strh[7] = {
      "titlewindow",
      "FORMAT: void titlewindow(justify,title,attr);",
      "        enum DIRTYPE justify; char title[80]",
      "        int attr                            ",
      "Titlewindow places a title in the top border",
      "of the current window.  Justify permits left,",
      "center or right justification of the title."};

    char *stri[17] = {
      "There are now two special effects that can",
      "enhance the window display:",
      "   zoomeffect   - emulates the MACINTOSH.",
      "   shadoweffect - places a left or right",
      "              shadow underneath the menu.",
      "These global variables can be placed anywhere",
      "in your program.  zoomeffect is INT while",
      "shadoweffect is of DIRTYPE.",
      "CGA:",
      "The CGA is self-regulating and controls the",
      "zoom rate.",
      "MDA and EGA:",
      "These video cards are quite fast and need a",
      "delay for the effect.  A default value of 11",
      "milliseconds is used in a global INT",
      "named 'zoomdelay' and shouldn't need any",
      "adjustment."};

    char *strj[5] = {
      " The maximum  number  of windows that",
      " may be on the screen at any one time",
      " is   specified   by   the   constant",
      " \"MAXWNDW\".The compiled library uses",
      " this value so do NOT change it......"};

    char *strk[9] = {
      " In addition to windows, there are 13 powerful",
      " Qwik-Write screen functions  you can use:",
      "      qwritelv   qattr      qpage",
      "      qwrite     qattrc     qwritepage",
      "      qwritec    gotorc     cursorchange",
      "      qfillc     qstore               ",
      "      qfill      qrestore                ",
      " In MCWINM20.ARC, compile and run QDEMO20.C to",
      " see all of these features."};

    char *strl[21] = {
      " The functions are used as follows:",
      "",
      "  ....Your include files....",
      "  #include <w1.h> ",
      "  #include <windprot.h> ",
      "  #include <color.h> ",
      "  ...Your variables and functions...",
      "  main(argc,argv)",
      "    {",
      "    initwindow (windowattribute,clear_screen);",
      "    makewindow (row,col,rows,cols,wattr,battr,",
      "                                     brdrsel);",
      "    titlewindow (justify,'the window title',",
      "                                 attribute);",
      "    removewindow();",
      "    }",
      "  end.",
      " ",
      " { Use one removewindow for each makewindow. }",
      " { IMPORTANT: Remember to link in the proper }",
      " {            T1xWIN20.lib library .         }"};

    char *strm[7] = {
      "   If you have any questions or comments, please write to,   ",
      "   or drop a note (CIS:)                                     ",
      "              (SOURCE CODE AVAILABLE for $25)                ",
      "     Michael G. Mlachak               Brian L. Cassista      ",
      "     4318 Stewart Court.              165 South Floyd Lane   ",
      "     East Chicago, IN 46312           Chicago Heights, IL    ",
      "     CIS - 76327,1410                          60411         "};

    char *strn[9] = {
      "get_string",
      "FORMAT:  char *get_string(default_string, row, col, attr,",
      "                          max_str_length, caps_lock,",
      "                          char_types, mask_string, term_str,",
      "                          pad_char, &term_char);",
      " ",
      "Get_string allows you to get string input from the user",
      "with total control of how it appears and terminates as",
      "well as various validity checks."};

    char *stro[10] = {
      "get_integer",
      "FORMAT:  int get_integer(default_integer, row, col, attr,",
      "                          min_int_val, max_int_val",
      "                          prompt_string, term_str,",
      "                          &term_char);",
      " ",
      "Get_integer allows you to get integer input, as would a",
      "calculator (push integers left), from the user with",
      "total control of how it appears and terminates as well",
      "as various validity checks."};

    char *strp[14] = {
      "get_menu",
      "FORMAT:  int get_menu(row, col, rows, cols, attr, inverse_attr,",
      "                       brdr_attr, display_items, term_string",
      "                       spacing, force_vertical, border_type,",
      "                       title_pos, &last_item, clear_window,",
      "                       special_function);",
      " ",
      "Get_menu allows you build a menuing system. It displays",
      "items to the screen in vertical or horizontal fashion",
      "allowing cursor movement to each item. The window depth",
      "is dynamically built based on the number of items and ",
      "the number of columns to be displayed.",
      " ",
      "*** THE MENUS FOR THIS DEMO ARE DONE WITH GET_MENU() ***"};

    char *strq[6] = {
      "wsound",
      "FORMAT:  void wsound(freq, duration);",
      "              unsigned freq, duration",
      " ",
      "Turns the speaker on at the given ",
      "frequency for the specified duration."};

    char *strr[8] = {
      "wsleep",
      "FORMAT:  unsigned wsleep(period);",
      "              unsigned period",
      " ",
      "Suspends a process for the number of",
      "timer ticks specified.",
      " ",
      "*** USED THROUGH OUT THIS DEMO ***"};

    char *intro[16] = {
      " ",
      "The windowing functions in  the file  T1SWIN20.LIB",
      "are  a  set of copyrighted  routines  that  allow ",
      "Turbo C to create incredibly  fast multi-level    ",
      "windows.   Created  under  the  shareware concept,",
      "T1SWIN20.LIB makes use of the quick screen writing",
      "utilities  of Jim H. LeMay -- which were converted",
      "from Turbo Pascal inline to external assembly. The",
      "windows may  be  of any  size  (from 2x2 to screen",
      "limits)  and  color  and  may  have one of several",
      "different  border  styles and colors including  no",
      "border.  They  also   work   in any   column  mode",
      "40/80/etc.  These routines automatically configure",
      "to your video card and  video  mode. All of  these",
      "routines were converted from Turbo Pascal to Turbo",
      "C, (SMALL MODEL) with the authors permission.     "};

    char *window_menu[11] = {
      "initwindow()   ",
      "makewindow()   ",
      "scrollwindow() ",
      "removewindow() ",
      "titlewindow()  ",
      "special effects",
      "window limits  ",
      "sample call    ",
      "ัออออออออออออออออออั",
      "   - select ",
      ""};

    char *misc_menu[8] = {
      "get_string()   ",
      "get_integer()  ",
      "get_menu()     ",
      "wsound()       ",
      "wsleep()       ",
      "ัออออออออออออออออออั",
      "   - select ",
      ""};

void main(void) {
   oldcursor = cursorchange(HIDDEN_CURSOR);
   show_initial_screen();
   loop_the_loop();
   display_main_menu();
   oldcursor = cursorchange(oldcursor);
}  /* main() */


void show_initial_screen(void) {
   int          loop,
                norm_attr, brdr_attr, hilt_attr;

   if (qseg == 0xB800) {
      norm_attr = wiattr(BLUE, CYAN);
      brdr_attr = wiattr(WHITE, BLUE);
      hilt_attr = wiattr(CYAN, BLUE);
   }
   else {
      norm_attr = wiattr(BLACK, LIGHTGRAY);
      brdr_attr = wiattr(WHITE, LIGHTGRAY);
      hilt_attr = wiattr(WHITE, BLACK);
   }
   initwindow(norm_attr,TRUE);
   zoomeffect   = TRUE;
   shadoweffect = bottomright;
   makewindow(1, 19, 6, 44, hilt_attr, brdr_attr, mixedbrdr);
   for (loop = 0; loop < 4; loop++)
      window_write(-1, stra[loop]);
   shadoweffect = bottomright;
   makewindow(8, 19, 16, 44, hilt_attr, brdr_attr, doublebrdr);
   active_window_row +=4;
   for (loop = 9; loop < 19; loop++)
      window_write(-1, stra[loop]);
   shadoweffect = nodir;
   makewindow(6, 26, 7, 28, hilt_attr, brdr_attr, singlebrdr);
   for (loop = 4; loop < 9; loop++)
      window_write(-1, stra[loop]);
   makewindow(4, 55, 9, 24, hilt_attr, brdr_attr, singlebrdr);
   for (loop = 19; loop < 26; loop++)
      window_write(-1, stra[loop]);
   makewindow(4, 1, 9, 24, hilt_attr, brdr_attr, singlebrdr);
   for (loop = 26; loop < 32; loop++)
      window_write(-1, stra[loop]);
   wsleep(250);
   remove_windows(5);
} /* show_initial_screen */

void loop_the_loop(void) {
   int          loop,
                norm_attr, brdr_attr;

   if (qseg == 0xB800) {
      norm_attr = wiattr(MAGENTA, CYAN);
      brdr_attr = wiattr(CYAN, RED);
   }
   else {
      norm_attr = wiattr(WHITE, BLACK);
      brdr_attr = wiattr(WHITE, LIGHTGRAY);
   }
   for(loop = 0; loop <= 13; loop++) {
      makewindow(s_row[loop], s_col[loop], n_rows, n_cols,
                 norm_attr, brdr_attr, mixedbrdr);
      window_printf(-1, "1-WINDOW\n2-WINDOW\n3-WINDOW");
   }
   remove_windows(14);
   for(loop = 13; loop >= 0; loop--) {
      makewindow(s_row[loop], s_col[loop], n_rows, n_cols,
                 norm_attr, brdr_attr, mixedbrdr);
      window_printf(-1, "1-WINDOW\n2-WINDOW\n3-WINDOW");
   }
   remove_windows(14);
   for(loop = 0; loop < 14; loop++) {
      makewindow(s_row[loop], s_col[loop], n_rows, n_cols,
                 norm_attr, brdr_attr, mixedbrdr);
      window_printf(-1,"1-WINDOW\n2-WINDOW\n3-WINDOW");
   }
   makewindow(7, 16, 13, 50, norm_attr, brdr_attr, singlebrdr);
   makewindow(9, 18, 9, 46,  norm_attr, brdr_attr, solidbrdr);
   makewindow(11, 20, 5, 42,  norm_attr, brdr_attr, mhatchbrdr);
   for (loop = 0; loop < 2; loop ++)
      window_write(-1, strb[loop]);
   wsleep(150);
   remove_windows(17);
}  /* loop_the_loop */

void display_main_menu(void) {
   int          m_item = 0,
                norm_attr, hilt_attr, brdr_attr,
                done = FALSE;
   char         *m_tstring;

   if (qseg == 0xB800) {
      norm_attr = wiattr(YELLOW, BLUE);
      hilt_attr = wiattr(WHITE, RED);
      brdr_attr = wiattr(WHITE, BLUE);
   }
   else {
      norm_attr = wiattr(WHITE, BLACK);
      hilt_attr = wiattr(BLACK, LIGHTGRAY);
      brdr_attr = wiattr(WHITE, BLACK);
   }
   zoomeffect = FALSE;
   m_tstring = (char *)calloc(1, (2 * sizeof(char)));
   qfill(1, 1, 25, 80, hilt_attr, (unsigned char)' ');
   do {
      if (get_menu(1, 1, 3, 80, norm_attr, hilt_attr, brdr_attr,
                   main_menu, m_tstring, 3, FALSE, doublebrdr,
                   topleft, &m_item, FALSE, m_special) == ESCAPE) {
         done = FALSE;
         removewindow();
      }
      else {
         switch (m_item) {
            case 0  : /* General */
               show_introduction(FALSE);
               removewindow();
            break;
            case 1  : /* Windows */
               display_window_menu(&m_item);
               removewindow();
            break;
            case 2  : /* Screen */
               shadoweffect = bottomright;
               zoomeffect   = TRUE;
               show_qwik21(FALSE);
               while (getch() != ENTER);
               removewindow();
               shadoweffect = nodir;
               zoomeffect   = FALSE;
            break;
            case 3  : /* Misc. */
               display_misc_menu(&m_item);
               removewindow();
            break;
            case 4  : /* Auto Demo */
               show_autodemo();
               remove_all_windows();
               zoomeffect   = FALSE;
               shadoweffect = nodir;
            break;
            case 5  : /* Exit Demo */
               shadoweffect = bottomright;
               zoomeffect   = TRUE;
               show_ending();
               wsleep(150);
               removewindow();
               shadoweffect = nodir;
               zoomeffect   = FALSE;
               done = TRUE;
               remove_all_windows();
            break;
            default :
            break;
         } /* switch (m_item) */
      }
   } while(!done);
   free(m_tstring);
} /* display_main_menu() */

void         show_introduction(int auto_demo) {
   int         index,
               norm_attr, brdr_attr;

   if (qseg == 0xB800) {
      norm_attr = wiattr(BLACK, GREEN);
      brdr_attr = wiattr(LIGHTGREEN, GREEN);
   }
   else {
      norm_attr = wiattr(BLACK, LIGHTGRAY);
      brdr_attr = wiattr(WHITE, LIGHTGRAY);
   }
   shadoweffect = bottomright;
   zoomeffect = TRUE;
   makewindow(4, 1, 19, 52, norm_attr, brdr_attr, mixedbrdr);
   for (index = 0; index <= 15; index++)
      window_write(-1, intro[index]);
   if (!auto_demo)
      titlewindow(bottomcenter, " Enter - Returns to main menu ", -1);
   shadoweffect = nodir;
   if (!auto_demo) {
      while ( getch() != ENTER );
      removewindow();
   }
   zoomeffect = FALSE;
}  /* show_introduction() */

void         display_window_menu(int *main_menu_item) {
   int         done=FALSE,
               w_tchar,
               norm_attr, hilt_attr, brdr_attr,
               w_item = 0;
   char        w_tstring[3];

   if (qseg == 0xB800) {
      norm_attr = wiattr(YELLOW, BLUE);
      hilt_attr = wiattr(LIGHTBLUE, RED);
      brdr_attr = wiattr(WHITE, BLUE);
   }
   else {
      norm_attr = wiattr(LIGHTGRAY, BLACK);
      hilt_attr = wiattr(WHITE, BLACK);
      brdr_attr = wiattr(WHITE, BLACK);
   }
   sprintf(w_tstring,"%c%c", RGTARROW, LFTARROW);
   do {
      shadoweffect = bottomright;
      w_tchar = get_menu(3, 13, 10, 20, norm_attr, hilt_attr, brdr_attr,
                   window_menu, w_tstring, 0, TRUE, mixedbrdr,
                   topcenter, &w_item, TRUE, m_special);
      if ( (w_tchar != ESCAPE)&&(w_tchar != RGTARROW)&&(w_tchar != LFTARROW) ) {
         switch (w_item) {
            case 0  :  /* initwindow() */
               show_initwindow(FALSE);
               while (getch() != ENTER);
               removewindow();
            break;
            case 1  :  /* makewindow() */
               show_makewindow(FALSE);
               while (getch() != ENTER);
               makewindow(18, 22, 4, 45, norm_attr, brdr_attr, mixedbrdr);
               window_write(-1, "Here are some sample window borders. To");
               window_write(-1, "see how they are removed see removewindow.");
               shadoweffect = nodir;
               show_borders();
               wsleep(150);
               remove_windows(10);
               shadoweffect = bottomright;
               remove_windows(2);
            break;
            case 2  :  /* scrollwindow() */
               show_scrollwindow(FALSE);
               while (getch() != ENTER);
               removewindow();
            break;
            case 3  :  /* removewindow() */
               shadoweffect = nodir;
               show_removewindow(FALSE);
               while (getch() != ENTER);
               removewindow();
               makewindow(18, 22, 3, 35, norm_attr, brdr_attr, mixedbrdr);
               window_write(-1, "Here are some sample windows.");
               show_borders();
               wsleep(150);
               makewindow(21, 12, 3, 40, norm_attr, brdr_attr, mixedbrdr);
               window_write(-1, "Don't blink they might disappear.");
               wsleep(150);
               remove_windows(15);
               shadoweffect = bottomright;
            break;
            case 4  :  /* titlewindow() */
               show_titlewindow(FALSE);
               while (getch() != ENTER);
               removewindow();
            break;
            case 5 :  /* special effects */
              shadoweffect = nodir;
              show_specialeffects(FALSE);
              while(getch() != ENTER);
              remove_windows(3);
              shadoweffect = bottomright;
            break;
            case 6 :  /* window limits */
               show_maxwndw(FALSE);
               while (getch() != ENTER);
               removewindow();
            break;
            case 7 :  /* sample call */
               show_programming(FALSE);
               while (getch() != ENTER);
               removewindow();
            break;
            default :
            break;
         }  /* switch(w_item) */
      }
      else {
         if (w_tchar ==  RGTARROW)
            (*main_menu_item)++;
         else
            if (w_tchar ==  LFTARROW)
               (*main_menu_item)--;
         done = TRUE;
      }
   } while(!done);
   shadoweffect = nodir;
}  /* display_window_menu() */

void         display_misc_menu(int *main_menu_item) {
   int         done=FALSE,
               term_char,
               default_int=123,
               norm_attr, hilt_attr, brdr_attr, spec_attr,
               msc_tchar,
               msc_item = 0;
   char        msc_tstring[3],
               *default_str, *term_str;

   if (qseg == 0xB800) {
      norm_attr = wiattr(YELLOW, BLUE);
      hilt_attr = wiattr(LIGHTBLUE, RED);
      brdr_attr = wiattr(WHITE, BLUE);
      spec_attr = wiattr(GREEN, RED);
   }
   else {
      norm_attr = wiattr(LIGHTGRAY, BLACK);
      hilt_attr = wiattr(WHITE, BLACK);
      brdr_attr = wiattr(WHITE, BLACK);
      spec_attr = wiattr(WHITE, LIGHTGRAY);
   }
   sprintf(msc_tstring,"%c%c", RGTARROW, LFTARROW);
   do {
      shadoweffect = bottomright;
      msc_tchar = get_menu(3, 32, 7, 20, norm_attr, hilt_attr, brdr_attr,
                   misc_menu, msc_tstring, 0, TRUE, mixedbrdr,
                   topcenter, &msc_item, TRUE, m_special);
      if ( (msc_tchar != ESCAPE)&&(msc_tchar != RGTARROW)&&(msc_tchar != LFTARROW) ) {
         switch (msc_item) {
            case 0  :  /* get_string() */
               show_getstring(FALSE);
               while (getch() != ENTER);
               default_str  = (char *)calloc(1, (25 * sizeof(char)));
               term_str     = (char *)calloc(1, sizeof(char));
               makewindow(5, 5, 5, 30, hilt_attr, brdr_attr, mhatchbrdr);
               titlewindow(topcenter, " Enter your name : ", -1);
               get_string(default_str, 7, 7, spec_attr, 25, TRUE,
                           3, TRUE, term_str, (char)'_', &term_char);
               titlewindow(bottomcenter, " You just used get_string ", -1);
               wsleep(150);
               remove_windows(2);
               free(default_str);
               free(term_str);
            break;
            case 1  :  /* get_integer() */
               show_getinteger(FALSE);
               while (getch() != ENTER);
               term_str = (char *)calloc(1, sizeof(char));
               makewindow(15, 35, 3, 31, hilt_attr, brdr_attr, thinsolidbrdr);
               get_integer(default_int, 16, 37, spec_attr, 0, 32000,
                            "Enter an integer : ", term_str, &term_char);
               titlewindow(bottomcenter, " You just used get_integer ", -1);
               wsleep(150);
               remove_windows(2);
               free(term_str);
            break;
            case 2  :  /* get_menu() */
               show_getmenu(FALSE);
               while (getch() != ENTER);
               removewindow();
            break;
            case 3  :  /* wsound() */
               show_wsound(FALSE);
               while (getch() != ENTER);
               removewindow();
            break;
            case 4  :  /* wsleep() */
               show_wsleep(FALSE);
               while (getch() != ENTER);
               removewindow();
            break;
            default :
            break;
         }  /* switch(msc_item) */
      }
      else {
         if (msc_tchar ==  RGTARROW)
            (*main_menu_item)++;
         else
            if (msc_tchar ==  LFTARROW)
               (*main_menu_item)--;
         done = TRUE;
      }
   } while(!done);
   shadoweffect = nodir;
}  /* display_misc_menu() */

void       show_autodemo(void) {
   int     default_int=123, spec_attr, hilt_attr, brdr_attr, term_char;
   char   *default_str, *term_str;

   if (qseg == 0xB800) {
      hilt_attr = wiattr(LIGHTBLUE, RED);
      brdr_attr = wiattr(WHITE, BLUE);
      spec_attr = wiattr(GREEN, RED);
   }
   else {
      hilt_attr = wiattr(WHITE, BLACK);
      brdr_attr = wiattr(WHITE, BLACK);
      spec_attr = wiattr(WHITE, LIGHTGRAY);
   }
   show_introduction(TRUE);
   wsleep(250);

   show_initwindow(TRUE);
   wsleep(150);

   show_makewindow(TRUE);
   wsleep(250);
   shadoweffect = nodir;
   show_borders();
   wsleep(100);

   show_removewindow(TRUE);
   wsleep(200);
   remove_windows(13);
   shadoweffect = bottomright;

   show_scrollwindow(TRUE);
   wsleep(200);

   show_titlewindow(TRUE);
   wsleep(150);

   shadoweffect = nodir;
   show_specialeffects(TRUE);
   wsleep(200);
   shadoweffect = bottomright;

   show_maxwndw(TRUE);
   wsleep(150);

   zoomeffect = TRUE;
   show_qwik21(TRUE);
   wsleep(150);
   zoomeffect = FALSE;

   show_programming(TRUE);
   wsleep(150);

   show_getstring(TRUE);
   wsleep(150);
   default_str  = (char *)calloc(1, (25 * sizeof(char)));
   term_str     = (char *)calloc(1, sizeof(char));
   makewindow(5, 5, 5, 30, hilt_attr, brdr_attr, mhatchbrdr);
   titlewindow(topcenter, " Enter your name : ", -1);
   get_string(default_str, 7, 7, spec_attr, 25, TRUE,
               3, TRUE, term_str, (char)'_', &term_char);
   titlewindow(bottomcenter, " You just used get_string ", -1);
   wsleep(150);
   free(default_str);

   show_getinteger(TRUE);
   wsleep(150);
   term_str = (char *)calloc(1, sizeof(char));
   makewindow(15, 35, 3, 31, hilt_attr, brdr_attr, thinsolidbrdr);
   get_integer(default_int, 16, 37, spec_attr, 0, 32000,
                "Enter an integer : ", term_str, &term_char);
   titlewindow(bottomcenter, " You just used get_integer ", -1);
   wsleep(150);
   free(term_str);

   show_getmenu(TRUE);
   wsleep(175);

   show_wsound(TRUE);
   wsleep(100);

   show_wsleep(TRUE);
   wsleep(100);
}  /* show_autodemo() */

void         show_scrollwindow(int auto_demo) {
   int         index,
               norm_attr, spec_attr;

   if (qseg == 0xB800) {
      norm_attr = wiattr(BLACK, BROWN);
      spec_attr = wiattr(LIGHTMAGENTA, BLUE);
   }
   else {
      norm_attr = wiattr(BLACK, LIGHTGRAY);
      spec_attr = wiattr(WHITE, LIGHTGRAY);
   }
   /*  -- ScrollWindow description --  */
     if (!auto_demo) {
        show_makewindow(FALSE);
        titlewindow(clearbottom, "", -1);
     }
     zoomeffect = TRUE;
     shadoweffect = bottomright;
     makewindow(11, 22, 4, 40, norm_attr, norm_attr, mixedbrdr);
     for (index = 0; index <= 1; index++ )
       window_write(-1, strf[index]);
     wsleep(100);
     removewindow();
     shadoweffect = nodir;

     gotorc(active_window_row, active_window_col);
     for (index = 0; index <= 18; index++)
        window_bios_scroll(8,5,76,22,spec_attr);

     for (index = 0; index <= 18; index++) {
        scrollwindow(1, 19, up);
        qwrite(active_window_row + 18, active_window_col, -1, strg[index]);
     }
     if (!auto_demo)
        titlewindow(bottomcenter, " Enter - Returns to sub menu ", -1);
     zoomeffect = FALSE;
}  /* show_scrollwindow() */

void         show_makewindow(int auto_demo) {
   int         index,
               norm_attr, brdr_attr, spec_attr, spc1_attr, spc2_attr;

   if (qseg == 0xB800) {
      norm_attr = wiattr(LIGHTMAGENTA, BLUE);
      brdr_attr = wiattr(LIGHTMAGENTA, BLUE);
      spec_attr = wiattr(YELLOW, RED);
      spc1_attr = wiattr(LIGHTRED, BLUE);
      spc2_attr = wiattr(YELLOW, BLUE);
   }
   else {
      norm_attr = wiattr(BLACK, LIGHTGRAY);
      brdr_attr = wiattr(WHITE, LIGHTGRAY);
      spec_attr = norm_attr;
      spc1_attr = norm_attr;
      spc2_attr = norm_attr;
   }
   zoomeffect = TRUE;
   makewindow(4, 7, 21, 71, norm_attr, brdr_attr, solidbrdr);
   window_write(spec_attr, strd[0]);
   for (index = 1; index <= 3; index++ )
     window_write(spc1_attr, strd[index]);
   for (index = 4; index <= 16; index++ )
     window_write(spc2_attr, strd[index]);
   if (!auto_demo)
      titlewindow(bottomcenter, " Enter - Returns to sub menu ", -1);
   zoomeffect = FALSE;
}  /* show_makewindow() */

void         show_borders(void) {
   int         index,
               norm_attr, brdr_attr;

   if (qseg == 0xB800) {
      norm_attr = wiattr(YELLOW, GREEN);
      brdr_attr = wiattr(YELLOW, GREEN);
   }
   else {
      norm_attr = wiattr(LIGHTGRAY, BLACK);
      brdr_attr = wiattr(WHITE, BLACK);
   }
   /*  -- Show different borders --  */
   for (index = 1; index <= 5; index++)
     makewindow(2 + 2 * index, 13 + 3 * index, 5, 10, norm_attr,
                brdr_attr, (enum BORDERS)index);
   for (index = 6; index < 11; index++)
     makewindow(24 - 2 * index, 24 + 3 * index, 5, 10,
                norm_attr, brdr_attr, (enum BORDERS)index);
}  /* show_borders() */

void         show_initwindow(int auto_demo) {
   int         index,
               norm_attr, spec_attr, spc1_attr, spc2_attr;

   if (qseg == 0xB800) {
      norm_attr = wiattr(YELLOW, RED);
      spec_attr = wiattr(BLUE, MAGENTA);
      spc1_attr = wiattr(CYAN, RED);
      spc2_attr = wiattr(WHITE, RED);
   }
   else {
      norm_attr = wiattr(BLACK, LIGHTGRAY);
      spec_attr = norm_attr;
      spc1_attr = norm_attr;
      spc2_attr = norm_attr;
   }
   zoomeffect = TRUE;
   makewindow(11, 15, 12, 63, norm_attr, norm_attr, singlebrdr);
   window_write(spec_attr, strc[0]);
   for (index = 1; index <= 3; index++ )
     window_write(spc1_attr, strc[index]);
   for (index = 4; index <= 7; index++ )
     window_write(spc2_attr, strc[index]);
   if (!auto_demo)
      titlewindow(bottomcenter, " Enter - Returns to sub menu ", -1);
   zoomeffect = FALSE;
}  /* show_initwindow() */

void         show_removewindow(int auto_demo) {
   int         index,
               norm_attr, spec_attr, spc1_attr, spc2_attr;

   if (qseg == 0xB800) {
      norm_attr = wiattr(BLACK, BROWN);
      spec_attr = wiattr(WHITE, MAGENTA);
      spc1_attr = wiattr(YELLOW, BROWN);
      spc2_attr = wiattr(WHITE, BROWN);
   }
   else {
      norm_attr = wiattr(BLACK, LIGHTGRAY);
      spec_attr = norm_attr;
      spc1_attr = norm_attr;
      spc2_attr = norm_attr;
   }
   zoomeffect = TRUE;
   makewindow(4, 1, 8, 40, norm_attr, norm_attr, doublebrdr);
   window_write(spec_attr, stre[0]);
   window_write(spc1_attr, stre[1]);
   for (index = 2; index <= 5; index++ )
     window_write(spc2_attr, stre[index]);
   makewindow(8, 42, 8, 38, norm_attr, norm_attr, doublebrdr);
   window_write(spec_attr, stre[6]);
   window_write(spc1_attr, stre[7]);
   for (index = 8; index <= 11; index++ )
     window_write(spc2_attr, stre[index]);
   makewindow(13, 1, 8, 40, norm_attr, norm_attr, doublebrdr);
   window_write(spec_attr, stre[12]);
   window_write(spc1_attr, stre[13]);
   for (index = 14; index <= 17; index++ )
     window_write(spc2_attr, stre[index]);
   if (!auto_demo) {
      makewindow(19, 45, 3, 31, norm_attr, norm_attr, hhatchbrdr);
      window_write(spec_attr, "Enter - Returns to sub menu");
   }
   zoomeffect = FALSE;
}  /* show_removewindow() */

void         show_titlewindow(int auto_demo) {
   int         index,
               norm_attr, spec_attr, spc1_attr, spc2_attr;

   if (qseg == 0xB800) {
      norm_attr = wiattr(RED, LIGHTGRAY);
      spec_attr = wiattr(LIGHTGREEN, BLUE);
      spc1_attr = wiattr(BLACK, LIGHTGRAY);
      spc2_attr = wiattr(GREEN, LIGHTGRAY);
   }
   else {
      norm_attr = wiattr(BLACK, LIGHTGRAY);
      spec_attr = norm_attr;
      spc1_attr = norm_attr;
      spc2_attr = norm_attr;
   }
   zoomeffect = TRUE;
   makewindow(11, 16, 10, 49, norm_attr, norm_attr, mhatchbrdr);
   window_write(spec_attr, strh[0]);
   window_write(spc1_attr, strh[1]);
   window_write(spc1_attr, strh[2]);
   for (index = 3; index <= 6; index++ )
     window_write(spc2_attr, strh[index]);
   if (!auto_demo)
      titlewindow(bottomcenter, " Enter - Returns to sub menu ", -1);
   zoomeffect = FALSE;
}  /* show_titlewindow() */

void         show_specialeffects(int auto_demo) {
   int         index,
               norm_attr, brdr_attr, spec_attr;

   if (qseg == 0xB800) {
      norm_attr = wiattr(BLACK, GREEN);
      brdr_attr = wiattr(WHITE, GREEN);
      spec_attr = wiattr(LIGHTRED, CYAN);
   }
   else {
      norm_attr = wiattr(BLACK, LIGHTGRAY);
      brdr_attr = wiattr(WHITE, LIGHTGRAY);
      spec_attr = wiattr(LIGHTGRAY, BLACK);
   }
   /*  -- Special effects description --  */
   zoomeffect = TRUE;
   makewindow(4, 1, 10, 49, norm_attr, brdr_attr, doublebrdr);
   titlewindow(topcenter, " Special Effects ", -1);
   active_window_col++;
   for (index = 0; index <= 7; index++)
      window_write(-1, stri[index]);

   makewindow(14, 25, 11, 49, norm_attr, brdr_attr, doublebrdr);
   titlewindow(topcenter, " Special Effects Cont. ... ", -1);
   active_window_col++;
   for (index = 8; index <= 16; index++)
      if ( (index != 8) && (index != 11) )
         window_write(-1, stri[index]);
      else
         window_write(brdr_attr, stri[index]);
   if (!auto_demo) {
      makewindow(8, 51, 3, 29, spec_attr, spec_attr, hhatchbrdr);
      window_write(-1, "Enter - Returns to sub menu");
   }
   zoomeffect = FALSE;
} /* show_specialeffects() */

void         show_maxwndw(int auto_demo) {
   int          index,
                norm_attr;

   if (qseg == 0xB800)
      norm_attr = wiattr(BLACK, BROWN);
   else
      norm_attr = wiattr(BLACK, LIGHTGRAY);
   /*  -- MaxWndw constant --  */
   makewindow(17, 9, 7, 40, norm_attr, norm_attr, singlebrdr);
   titlewindow(topcenter, " WINDOW LIMITS ", -1);
   if (!auto_demo)
      titlewindow(bottomcenter, " Enter - Returns to sub menu ", -1);
   for (index = 0; index <= 4; index++)
      window_write(-1, strj[index]);
} /* show_maxwndw() */

void         show_qwik21(int auto_demo) {
   int         index,
               norm_attr, spec_attr;

   if (qseg == 0xB800) {
      norm_attr = wiattr(YELLOW, RED);
      spec_attr = wiattr(WHITE, RED);
   }
   else {
      norm_attr = wiattr(BLACK, LIGHTGRAY);
      spec_attr = wiattr(WHITE, LIGHTGRAY);
   }
   /*  -- QWIK21.INC procedures --  */
   makewindow(8, 20, 13, 51, norm_attr, norm_attr, evensolidbrdr);
   titlewindow(topcenter, " Utility Functions ", -1);
   if (!auto_demo)
      titlewindow(bottomcenter, " Enter - returns to main menu ", -1);
   active_window_row++;
   for (index = 0; index <= 8; index++)
      if ( (index > 1) && (index < 7) )
         window_write(spec_attr, strk[index]);
      else
         window_write(-1, strk[index]);
} /* show_qwik21() */

void         show_programming(int auto_demo) {
   int         index,
               norm_attr;

   if (qseg == 0xB800)
      norm_attr = wiattr(YELLOW, MAGENTA);
   else
      norm_attr = wiattr(WHITE, LIGHTGRAY);
   /*  -- Programming for WINDOW33.INC --  */
   zoomeffect = TRUE;
   makewindow(1, 25, 24, 50, norm_attr, norm_attr, mhatchbrdr);
   titlewindow(topcenter, " Sample ", -1);
   if (!auto_demo)
      titlewindow(bottomcenter, " Enter - returns to sub menu ", -1);
   for (index = 0; index <= 20; index++)
      window_write(-1, strl[index]);
   zoomeffect = FALSE;
} /* show_programming() */

void         show_ending(void) {
   int         index,
               norm_attr, spec_attr;

   if (qseg == 0xB800) {
      norm_attr = wiattr(BLACK, BROWN);
      spec_attr = wiattr(WHITE, BROWN);
   }
   else {
      norm_attr = wiattr(BLACK, LIGHTGRAY);
      spec_attr = wiattr(WHITE, LIGHTGRAY);
   }
   /*  -- Editor for WINDOW33.INC --  */
   makewindow(8, 10, 11, 65, norm_attr, norm_attr, hhatchbrdr);
   titlewindow(topcenter, " Thats All Folks !! ", -1);
   active_window_col++;
   active_window_row++;
   for (index = 0; index <= 6; index++)
      window_write(spec_attr, strm[index]);
}  /* show_editor */

void       show_getstring(int auto_demo) {
   int         index,
               norm_attr, spec_attr;

   if (qseg == 0xB800) {
      norm_attr = wiattr(LIGHTBLUE, MAGENTA);
      spec_attr = wiattr(YELLOW, RED);
   }
   else {
      norm_attr = wiattr(WHITE, BLACK);
      spec_attr = wiattr(WHITE, LIGHTGRAY);
   }
   zoomeffect = TRUE;
   makewindow(9, 7, 11, 65, norm_attr, norm_attr, mixedbrdr);
   titlewindow(topcenter, " GET_STRING ", spec_attr);
   if (!auto_demo)
      titlewindow(bottomcenter, " Enter - returns to sub menu ", -1);
   active_window_col++;
   for (index = 0; index <= 8; index++)
      window_write(-1, strn[index]);
   zoomeffect = FALSE;
}  /* show_getstring() */

void       show_getinteger(int auto_demo) {
   int         index,
               norm_attr;

   if (qseg == 0xB800)
      norm_attr = wiattr(YELLOW, BLUE);
   else
      norm_attr = wiattr(WHITE, LIGHTGRAY);
   zoomeffect = TRUE;
   makewindow(4, 3, 12, 61, norm_attr, norm_attr, lhatchbrdr);
   titlewindow(topcenter, " GET_INTEGER ", -1);
   if (!auto_demo)
      titlewindow(bottomcenter, " Enter - returns to sub menu ", -1);
   active_window_col++;
   for (index = 0; index <= 9; index++)
      window_write(-1, stro[index]);
   zoomeffect = FALSE;
}  /* show_getinteger() */

void       show_getmenu(int auto_demo) {
   int         index,
               norm_attr, brdr_attr;

   if (qseg == 0xB800) {
      norm_attr = wiattr(YELLOW, GREEN);
      brdr_attr = wiattr(WHITE, GREEN);
   }
   else {
      norm_attr = wiattr(WHITE, LIGHTGRAY);
      brdr_attr = wiattr(WHITE, BLACK);
   }
   zoomeffect = TRUE;
   makewindow(6, 5, 16, 68, norm_attr, brdr_attr, singlebrdr);
   titlewindow(topcenter, " GET_MENU ", -1);
   if (!auto_demo)
      titlewindow(bottomcenter, " Enter - returns to sub menu ", -1);
   active_window_col++;
   for (index = 0; index <= 12; index++)
      window_write(-1, strp[index]);
   window_write(brdr_attr, strp[index]);
   zoomeffect = FALSE;
}  /* show_getmenu() */

void       show_wsound(int auto_demo) {
   int         index,
               norm_attr;

   if (qseg == 0xB800)
      norm_attr = wiattr(LIGHTRED, CYAN);
   else
      norm_attr = wiattr(LIGHTGRAY, BLACK);
   zoomeffect = TRUE;
   makewindow(6, 12, 8, 45, norm_attr, norm_attr, doublebrdr);
   titlewindow(topcenter, " WSOUND ", -1);
   if (!auto_demo)
      titlewindow(bottomcenter, " Enter - returns to sub menu ", -1);
   active_window_col++;
   for (index = 0; index <= 5; index++)
      window_write(-1, strq[index]);
   zoomeffect = FALSE;
}  /* show_wsound() */

void       show_wsleep(int auto_demo) {
   int         index,
               norm_attr, spec_attr;

   if (qseg == 0xB800)  {
      norm_attr = wiattr(WHITE, RED);
      spec_attr = wiattr(YELLOW, RED);
   }
   else {
      norm_attr = wiattr(LIGHTGRAY, BLACK);
      spec_attr = wiattr(WHITE, BLACK);
   }
   zoomeffect = TRUE;
   makewindow(5, 20, 10, 40, norm_attr, norm_attr, doublebrdr);
   titlewindow(topcenter, " WSLEEP ", -1);
   if (!auto_demo)
      titlewindow(bottomcenter, " Enter - returns to sub menu ", -1);
   active_window_col++;
   for (index = 0; index <= 6; index++)
      window_write(-1, strr[index]);
   window_write(spec_attr, strr[index]);
   zoomeffect = FALSE;
}  /* show_wsleep() */

int m_special(int chr, int *finished) {
   if ( (chr == RGTARROW) || (chr == LFTARROW) )
      *finished = TRUE;
   else
      *finished = FALSE;
   return(chr);
} /* m_special() */

