/*
 * RDIR.C : A resident directory lister.  This shows how to access DOS from
 *          within a Turbo C resident program.  To completely recompile this
 *          code you must have MASM or a compatible compiler.  Unfortunately
 *          everything but trapping the BIOS Disk service software
 *          interrrupt could be done in Turbo C.  See code commentary.
 *
 *  To activate the program once loaded, press Ctrl and Alt together.
 *  I know this combination gets in the way of SideKick, but I don't use
 *  sidekick <grin> and this is meant as a tutorial anyway.  If you would
 *  like to change this combination see Hot_Combo in the source code.
 *  Next, enter the pattern for the directory search followed either by
 *  Enter or Ctrl-Enter.  Enter will not include directories.  Ctrl-Enter
 *  will include directories.  Directories are displayed in white where
 *  other files are displayed in cyan.  Hidden and system files are included.
 *  The page up and page down keys may be used to see other parts of large
 *  directory listings.
 *
 *  Written by Dean D. McCrory
 *  For Turbo C 1.00
 *  May 14, 1987
 *
 *  Compile with:
 *    masm -mx bioshand.asm;
 *    masm -mx intvid.asm;
 *    tcc -N- rdir.c bioshand.obj intvid.obj
 *
 *  The -N- switch turns stack checking off which is a definite requirement
 *  for writing ISRs.  Have fun <grin>.
 */

#include <dos.h>
#include <dir.h>

#include "intvid.h"

/* function prototypes */
int main (void);
unsigned prgsize (void);
void exit (int);
char far * getdosbusy (void);
void interrupt timer_handler (void);
void do_click (void);
void interrupt special_handler (void);
void interrupt keyboard_handler (void);
extern void interrupt biosdisk_handler (void);
void interrupt break_handler (void);
void list_directory (char);
void set_screen (void);
void get_pattern (void);
void display_entries (void);
void find_entry (void);
void sc_putsa (int, int, char *, int);
void sc_putca (int, int, char, int);
void sc_repvca (int, int, char, int, int);
void sc_rephca (int, int, char, int, int);
void sc_savbox (int, int, int, int, char *);
void sc_resbox (int, int, int, int, char *);
void sc_rptpos (int *, int *);
void sc_setpos (int, int);
char far * sc_cca (int, int);
int getkey ();

/* Define the structure which will be attached to interrupt d0 so we can
   determine if rdir is already loaded */
typedef struct s_rdir_cfg
   {
   char iret;        /* iret first, just in case */
   long * signature; /* signature string */
   } t_rdir_cfg;

/* Suppress some library functions to conserve space */
_setargv () {}
_setenvp () {}

/* defines for various things, in alot of these things we are looking right
   into the BIOS data area. See Peter Norton's Guide to the IBM-PC or
   your Tech. Ref. for an expanation of these data items. */
#define Screen_Ram   ((char far *) 0xb8000000L) + (*(int far *) 0x0000044eL)
#define Display_Page (*(char far *) 0x00000462L)
#define Iret         0xcf     /* for iret in int d1 just in case */
#define Intr         0xd1     /* interrupt for install checking */
#define Timer        0x1c     /* timer interrupt number */
#define Keyboard     0x09     /* keyboard hardware interrupt */
#define Special      0x28     /* special interrupt 28h */
#define Critical     0x24     /* hardware critical interrupt 24h */
#define Break        0x1b     /* bios ctrl-break interrupt */
#define BiosDisk     0x13     /* bios diskette services */
#define NotOk        1        /* already installed return code */
#define Ok           0        /* success return code */
#define Shift_Bits   ((char far *) 0x00000417L)
#define Hot_Combo    ((*Shift_Bits & 12) == 12) /* Our hot key combo */

#define  Box_Row     4     /* row of box */
#define  Box_Col     4     /* column of box */
#define  Box_Hgt     17    /* height of box */
#define  Box_Wdth    70    /* widht of box */
#define  Box_Attr    0x4f  /* attribute of box border */
#define  Box_Incr    14    /* number of columns between start of filenames */
#define  Pat_Size    64    /* size of a pattern string */
#define  Norm_Attr   3     /* attribute of normal files */
#define  Dir_Attr    15    /* attribute of directories */
#define  Name_Size   12    /* size of each file name when displayed */
#define  Per_Screen  5 * (Box_Hgt - 2) /* number of dir entries in box */

/* Defines for key values as returned by getkey () */
#define  Page_Up_Key    329
#define  Page_Dn_Key    337
#define  Escape_Key     27
#define  BackSpace_Key  8
#define  Enter_Key      13
#define  Ctrl_Enter_Key 10
#define  Break_Key      256

char  box_buf[Box_Hgt * Box_Wdth * 2]; /* buffer for saving screen */
int   old_row, old_col;                /* row and col of cursor */
int   current_entry;                   /* last dir entry read */
int   entries_to_skip;                 /* first entry on display */
char  pattern[Pat_Size];               /* pattern for dir searches */
int   attrib;                          /* attrib for dir searches */
struct ffblk ffblk;                    /* file-find block */
int   key, status;                     /* last key, and last ff status */
t_vidreg regs;                         /* global regs structure */
char far * ptr;                        /* general purpose far pointer */
char far * buf_pos;                    /* another of the same */
int   next_line;                       /* used by screen stuff */
int   need_key;                        /* 1 == need to get key in get_pat */

/* Our configuration structure... nothing in it but the signature */
t_rdir_cfg rdir_cfg =
   {
   Iret, (long *) "rdir"
   };

void interrupt (* old_timer) ();    /* previous timer interrupt vector */
void interrupt (* old_keyboard) (); /* previous keyboard int vector */
void interrupt (* old_special) ();  /* previous int 28h vector */
void interrupt (* old_biosdisk) (); /* previous bios disk svc vector */
void interrupt (* old_critical) (); /* previous int 24h vector */
void interrupt (* old_break) ();    /* pervious int 1bh vector */
char far * old_dta;                 /* disk transfer address save area */

static char far * dosbusy_fl;    /* dos maintains this */
char biosbusy_fl = 0;            /* I maintain this */
static int request_fl = 0;       /* 0 - no request
                                    1 - request made
                                    2 - request being serviced
                                  */
int main ()
{
   t_rdir_cfg far * cfg_ptr;

   /* get old configuration information (mayebe) */
   cfg_ptr = (t_rdir_cfg far *) getvect (Intr);

   /* check to see if we are already installed */
   if (*cfg_ptr->signature != *rdir_cfg.signature)
      {
      /* we were not installed so install ourselves */
      old_timer = getvect (Timer);
      old_keyboard = getvect (Keyboard);
      old_special = getvect (Special);
      old_biosdisk = getvect (BiosDisk);
      setvect (Timer, timer_handler);
      setvect (Special, special_handler);
      setvect (BiosDisk, biosdisk_handler);
      setvect (Keyboard, keyboard_handler);
      setvect (Intr, (void interrupt (*) ()) &rdir_cfg);
      dosbusy_fl = getdosbusy ();
      keep (Ok, prgsize ());
      }

   return (NotOk);
}

/* prgsize ()
 *
 * Calculates the program size by looking at __brklvl which is set to
 * the end of initialized and uninitialized data whithin the data segment
 * at program startup.  __brklvl is then changed as memory space is
 * malloc'd.  __brklvl is decremented as malloc'd areas are free'd.
 *
 *   ** This function should work in Tiny, Small, and Meduim models **
 */

unsigned prgsize ()
{
   extern unsigned __brklvl;     /* current top of heap == sbrk (0) */
   extern unsigned _psp;         /* lowest segment address occupied */

   return (_DS + (__brklvl + 15) / 16 - _psp);
}

/* exit ()
 *
 * Rewrite exit for memory conservation.  This exit () does not close files
 * or flush buffers, which is fine in this case because we have no open
 * files or buffers which need to be flushed.
 *
 */
void exit (status)
   int status;
{
   _exit (status);
}

/* getdosbusy ()
 *
 * Gets the Dos busy flag through interrupt 34h.  This Dos function returnes
 * the busy flag address in es:bx.  This is an UNDOCUMENTED feature of Dos,
 * however it has worked in Dos versions 2.11 - 3.30 for me.
 */
char far * getdosbusy ()
{
   struct SREGS sregs;        /* segment registers */
   union REGS regs;           /* normal registers */

   regs.h.ah = 0x34;          /* get dos busy flag address (UNDOCUMENTED) */
   intdosx (&regs, &regs, &sregs);
   return (MK_FP (sregs.es, regs.x.bx));
}

/* timer_handler ()
 *
 * This function intercepts the hardware timer interrupt.  It checks the
 * request flag set by the keyboard handler and if set pops the directory
 * function up only if it is currently "safe" to do so.
 */
void interrupt timer_handler ()
{
   static int in_fl = 0;

   /* if the following statement is NOT coded, the 8259 blocks all hardware
      interrupts including the keyboard interrupt.  Since we wait for a key
      in list_directory (), this causes the PC to lock up.  This one took
      a while to figure out */
   outportb (0x20, 0x20);        /* send eoi to 8259 */

   if (! in_fl)
      {
      in_fl = 1;                 /* we are in our ISR */
      if (request_fl == 1)       /* has there been a request for popup? */
         if (! *dosbusy_fl && ! biosbusy_fl)
            list_directory ('T');/* call the directory lister */
         else
            do_click ();         /* click to let user know we are trying */
      in_fl = 0;
      }

   (*old_timer) ();           /* chain to previous timer handler */
   return;                    /* return from ISR */
}

/* do_click ()
 *
 * I guess I got lazy here.  I wanted to just output a short click like
 * sidekick does when it can't pop up because Dos is busy.  At any rate,
 * the function is here if I want to implement that type of thing.
 */
void do_click ()
{
}

/* special_handler ()
 *
 * This interrupt is called from Dos at times when it is "safe" to use Dos
 * functions.  It seems to be called constantly when waiting for keystrokes
 * at the Dos prompt.  Here, we don't have to check the Dos busy flag
 * because it is ALWAYS ok to call Dos from this point.
 */
void interrupt special_handler ()
{
   static int in_fl = 0;

   (*old_special) ();         /* chain to previous int 28 handler */

   if (! in_fl)
      {
      in_fl = 1;                 /* we are in our ISR */
      if (request_fl == 1)       /* see if rdir has been requested */
         list_directory ('S');   /* ok, list the directory */
      in_fl = 0;
      }
   return;
}

/* keyboard_handler ()
 *
 * This is what starts the whole ball rolling!  First we call the old
 * keyboard handler, then check for our hot key.  If our hot key has been
 * pressed, we toggle our internal request flag.  Next, if the request
 * flag is set and it is safe to enter dos, we call the directory lister.
 * If Dos is busy, this request must be handled by either the timer interrupt
 * or the special int 28h interrupt
 */
void interrupt keyboard_handler ()
{
   static int in_fl = 0;

   (*old_keyboard) ();

   if (! in_fl)
      {
      in_fl = 1;
      if (Hot_Combo && request_fl != 2)
         request_fl = ! request_fl;
 
      if (request_fl == 1 && ! *dosbusy_fl && ! biosbusy_fl) 
         list_directory ('K');
      in_fl = 0;
      }

   return;
}

/* critical_handler ()
 *
 * This is only active while in list_directory ().  Its purpose is to avoid
 * the possibility of the user entering a pattern on a floppy drive with
 * no floppy in the drive.  If we did not trap this interrupt, Dos could
 * terminate our TSR which would probably crash the system.
 */
int critical_handler ()
{
   return (0);                /* ignore the error */
}

/* break_handler ()
 *
 * Again, this is only active when in list_directory ().  The purpose is
 * the same as critical_handler () except we are trapping the BIOS Ctrl_Brk
 * key.
 */
void interrupt break_handler ()
{
   return;                    /* ignore the break */
}

/* list_directory ()
 *
 * This is the actual routine which lists the directory.  It can be called
 * by either the keyboard_handler (), timer_handler (), or the
 * special_handler ().  It does not care which.  While looking at this code
 * you may notice the heavy use of global variables instead of autos; this
 * is becuase we my be popped up from somewhere in DOS... we are using the
 * default (current) stack and we don't know how much there is, therefore
 * we should use as little as possible!
 */
void list_directory (type)
   char type;
{
   request_fl = 2;            /* currently servicing the request */
   need_key = 1;

   /* save and set critical error handler */
   old_critical = getvect (Critical);
   harderr (critical_handler);

   /* save and set ctrl-break handler */
   old_break = getvect (Break);
   setvect (Break, break_handler);

   /* save and set the disk transfer address */
   old_dta = getdta ();
   setdta ((char far *) &ffblk);

   /* eat any type-ahead keys */
   while (bioskey (1))
      bioskey (0);

   set_screen ();

   /* FOR DEBUGGING ONLY */
   sc_putca (Box_Row + 1, Box_Col + Box_Wdth - 1, type, Box_Attr);

   while (get_pattern (), key != Escape_Key && key != Break_Key)
      {
      attrib = (key == Enter_Key ? FA_HIDDEN | FA_SYSTEM : FA_HIDDEN |
         FA_SYSTEM | FA_DIREC);
      entries_to_skip = 0;    /* start at beginning of dir list */
      current_entry = -1;     /* have to do a find first */
      display_entries ();
      need_key = 0;           /* for next time into get_pattern */

      do
         {
         key = getkey ();
         switch (key)
            {
            case Page_Up_Key:
               if (entries_to_skip - Per_Screen >= 0)
                  {
                  current_entry = -1;
                  entries_to_skip -= Per_Screen;
                  display_entries ();
                  }
               break;

            case Page_Dn_Key:
               if (status == 0)
                  {
                  entries_to_skip += Per_Screen;
                  display_entries ();
                  }
               break;

            case Escape_Key:
               need_key = 1;
               break;

            case Break_Key:
               need_key = 1;
               break;
            }
         }
      while (key == Page_Dn_Key || key == Page_Up_Key);
      }

   /* restore critical error handler, break handler and dta address */
   setvect (Critical, old_critical);
   setvect (Break, old_break);
   setdta (old_dta);

   /* restore the screen, and cursor position */
   sc_resbox (Box_Row, Box_Col, Box_Hgt, Box_Wdth, box_buf);
   sc_setpos (old_row, old_col);

   /* request has been filled, and now no request is active */
   request_fl = 0;            /* no request made */
}

/* set_screen ()
 *
 * This function sets up the initial screen for list_directory ()
 */
void set_screen ()
{
   /* save area on screen */
   sc_savbox (Box_Row, Box_Col, Box_Hgt, Box_Wdth, box_buf);
   sc_rptpos (&old_row, &old_col);

   /* draw our box */
   sc_repvca (Box_Row + 1, Box_Col, '\xb3', Box_Hgt - 2,  Box_Attr);
   sc_repvca (Box_Row + 1, Box_Col + Box_Wdth - 1, '\xb3', Box_Hgt - 2,
      Box_Attr);
   sc_rephca (Box_Row + Box_Hgt - 1, Box_Col + 1, '\xc4', Box_Wdth - 2,
      Box_Attr);
   sc_putca (Box_Row, Box_Col, '\xda', Box_Attr);
   sc_putca (Box_Row, Box_Col + Box_Wdth - 1, '\xbf', Box_Attr);
   sc_putca (Box_Row + Box_Hgt - 1, Box_Col, '\xc0', Box_Attr);
   sc_putca (Box_Row + Box_Hgt - 1, Box_Col + Box_Wdth - 1, '\xd9', Box_Attr);
}

/* get_pattern ()
 *
 * This gets the pattern from the user.  The result is put in the global
 * variable pattern.  The only editing key is backspace.
 */
void get_pattern ()
{
   static int pos;
   static char * ptr;

   for (pos = Box_Row + 1; pos < Box_Row + Box_Hgt - 1; pos++)
      sc_rephca (pos, Box_Col + 1, ' ', Box_Wdth - 2, Norm_Attr);
   sc_rephca (Box_Row, Box_Col + 1, ' ', Box_Wdth - 2, Box_Attr);
   pos = Box_Col + 2;
   ptr = pattern;
   *ptr = '\0';
   do
      {
      sc_setpos (Box_Row, pos);
      if (need_key)
         key = getkey ();
      need_key = 1;
      switch (key)
         {
         case BackSpace_Key:
            if (pos > Box_Col + 2)
               {
               pos--;
               sc_putca (Box_Row, pos, ' ', Box_Attr);
               *--ptr = '\0';
               }
            break;

         case Escape_Key:
         case Enter_Key:
         case Ctrl_Enter_Key:
            break;

         default:
            if (key >= 32 && key <= 127 && pos < Pat_Size + Box_Col + 2)
               {
               sc_putca (Box_Row, pos, key, Box_Attr);
               pos++;
               *ptr++ = key;
               *ptr = '\0';
               }
            break;
         }
      }
   while (key != Escape_Key && key != Enter_Key && key != Ctrl_Enter_Key &&
      key != Break_Key);

   /* if they didn't type anything, or the last character was a directory
      separator, or a drive specifier, make the pattern *.* */
   if (pattern[0] == '\0' || *(--ptr) == '\\' || *ptr == '/' || *ptr == ':')
      strcat (pattern, "*.*");
}

/* display_entries ()
 *
 * Displays or redisplays the directory entries.
 */
void display_entries ()
{
   static int entries_to_display;
   static int row;
   static int col;
   static int attr;
   static int len; 

   entries_to_display = Per_Screen;
   row = Box_Row + 1;
   col = Box_Col + 1;

   /* First skip to the start of the current page */
   while (current_entry < entries_to_skip)
      find_entry ();

   /* For every possible entry on the screen: either display the filename
      or display blanks */
   while (entries_to_display--)
      {
      if (! status)
         {
         attr = Norm_Attr;
         if (ffblk.ff_attrib & FA_DIREC)
            attr = Dir_Attr;
         sc_putsa (row, col, ffblk.ff_name, attr);
         len = strlen (ffblk.ff_name);
         sc_rephca (row, col + len, ' ', Name_Size - len, attr);
         find_entry ();
         }
      else
         sc_rephca (row, col, ' ', Name_Size, Norm_Attr);
      if (++row > Box_Row + Box_Hgt - 2)
         col += Box_Incr, row = Box_Row + 1;
      }
}

/* find_entry ()
 *
 * Finds the next entry based on the current entry.  If current entry is
 * -1 then a findfirst () is assumed to be needed, else a findnext () is
 * executed.
 */
void find_entry ()
{
   if (current_entry == -1)
      status = findfirst (pattern, &ffblk, attrib);
   else
      status = findnext (&ffblk);
   current_entry++;
}

/* The following functions are used to write to the screen.  They do write
   directly to screen RAM so they will cause "snow" on some CGA systems */

/* sc_putsa ()
 *
 * Write a string with the given attribute at row, col
 */
void sc_putsa (row, col, string, attr)
   int row;
   int col;
   register char * string;
   int attr;
{
   /* calculate pointer to screen RAM */
   ptr = sc_cca (row, col);

   /* write each of the characters in string to the screen RAM */
   while (*string)
      {
      *ptr++ = *string++;           /* write the character */
      *ptr++ = attr;                /* write the attribute */
      }
}

/* sc_putca ()
 *
 * Write a single character with the specified attribute, at row, col
 */
void sc_putca (row, col, ch, attr)
   int row;
   int col;
   char ch;
   int attr;
{
   /* calculate pointer to screen RAM */
   ptr = sc_cca (row, col);

   *ptr++ = ch;
   *ptr = attr;
}

/* sc_repvca ()
 *
 * Repeat the given character length times with the specified attribute
 * starting from row, col in a vertical direction.
 */
void sc_repvca (row, col, ch, length, attr)
   int row;
   int col;
   char ch;
   int attr;
{
   /* calculate pointer to screen RAM */
   ptr = sc_cca (row, col);
   next_line = 80 * 2 - 2;

   while (length--)
      {
      *ptr++ = ch;            /* write the character */
      *ptr++ = attr;          /* write the attribute */
      ptr += next_line;       /* move to the next line */
      }
}

/* sc_rephca ()
 *
 * Repeat the given character length times with the specified attribute
 * starting from row, col in a horizontal direction.
 */
void sc_rephca (row, col, ch, length, attr)
   int row;
   int col;
   char ch;
   int length;
   int attr;
{
   /* calculate pointer to screen RAM */
   ptr = sc_cca (row, col);

   while (length--)
      {
      *ptr++ = ch;        /* write the character */
      *ptr++ = attr;      /* write the attribute */
      }
}

/* sc_savbox ()
 *
 * Saves the screen image into buf.  Row, col specify the upper left hand
 * corner of the box, height and width specify the number of rows and
 * columns to save.
 */
void sc_savbox (row, col, height, width, buf)
   int row;
   int col;
   int height;
   int width;
   char * buf;
{
   /* calculate pointer to screen RAM */
   ptr = sc_cca (row, col);

   buf_pos = (char far *) buf;
   width *= 2;

   while (height--)          /* while we still have rows to do */
      {
      movedata (FP_SEG (ptr), FP_OFF (ptr), FP_SEG (buf_pos), FP_OFF
         (buf_pos), width);
      buf_pos += width;
      ptr += 160;
      }
}

/* sc_resbox ()
 *
 * Performs the opposite function of sc_savbox ()
 */
void sc_resbox (row, col, height, width, buf)
   int row;
   int col;
   int height;
   int width;
   char * buf;
{
   ptr = sc_cca (row, col);
   width *= 2;
   buf_pos = (char far *) buf;

   while (height--)
      {
      movedata (FP_SEG (buf_pos), FP_OFF (buf_pos), FP_SEG (ptr), FP_OFF
         (ptr), width);
      buf_pos += width;
      ptr += 160;
      }
}

/* sc_rptpos ()
 *
 * Reports the current cursor row and column values.  These values are
 * placed in the integers which are pointed at by row and col.   I had to
 * use my own intvid () routine here instead of int86 because int86 reserves
 * some space on the stack for temporary vars.  It also assumes that the
 * current ss (stack segment) is the same as ds (data segment).  This is
 * not true in this case, so they end up writing to random places in memory
 * and usually crash the machine.  Be carefull with using library routines
 * within an ISR.
 */
void sc_rptpos (row, col)
   register int * row;
   register int * col;
{
   regs.h.ah = 3;
   regs.h.bh = Display_Page;
   intvid (&regs, 1); 
   *row = regs.h.dh;
   *col = regs.h.dl;
}


/* sc_setpos ()
 *
 * Sets the cursor positon at row, col.
 */
void sc_setpos (row, col)
   register int row;
   register int col;
{
   regs.h.ah = 2;
   regs.h.dh = row;
   regs.h.dl = col;
   regs.h.bh = Display_Page;
   intvid (&regs, 0); 
}


/* sc_cca ()
 *
 * Calculates a far pointer into screen RAM based on row, col.
 */
char far * sc_cca (row, col)
   int row;
   int col;
{
   return (Screen_Ram + row * 160 + col * 2);
}

/* getkey ()
 *
 * Gets a single key from the keyboard (waits for one if necessary).  If the
 * key is a special key (low byte is 0) the returned value is 256 plus
 * the scan code (high byte).  If it is normal key, just the ASCII code is
 * returned.
 */
int getkey ()
{
   static unsigned key;

   key = bioskey (0);
   if (! (key & 0xff))
      key = (key >> 8) | 256;
   else
      key = key & 0xff;
   
   return (key);
}

