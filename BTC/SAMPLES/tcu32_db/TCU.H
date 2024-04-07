/*==========================================================================*
 *                                                                          *
 *          Header #include file for Turbo-C Utilities version 3.2          *
 *          ------------------------------------------------------          *
 *                                                                          *
 *                    (c) Copyright Karl Keyte, 1990-91                     *
 *                                                                          *
 *  Karl Keyte                           Phone:  +(49) 6150 2499            *                       *
 *  Gross-Gerauer Strasse 10                                                *
 *  D-6108 Weiterstadt                   e-Mail: KKEYTE@ESOC.BITNET         *
 *  Germany                              Bix:    kkeyte                     *
 *                                                                          *
 *==========================================================================*/


#if     !defined(__TCU__)
#define __TCU__

#define TCU_BOX_SINGLE              1      /* Box bordered by single lines */
#define TCU_BOX_DOUBLE              2      /* Box bordered by double lines */
#define TCU_BOX_BLANK               3      /* Box bordered by spaces       */


#define TCU_ESC_ESC              0x01      /* ESCAPE key exit */
#define TCU_ESC_PGUP             0x02      /* Page Up key exit */
#define TCU_ESC_PGDN             0x04      /* Page Down key exit */
#define TCU_ESC_CLEFT            0x08      /* Cursor left exit */
#define TCU_ESC_CRIGHT           0x10      /* Cursor right exit */
#define TCU_ESC_FUNC             0x20      /* Function key exit */
#define TCU_ESC_CNTL_C           0x40      /* Control-C exit */
#define TCU_ESC_PULLDOWN         0x80      /* Pulldown bar selection */

#define TCU_ESC_USERKEY        0x0100      /* User defined escape key */

#define TCU_ESC_MOUSE_CLICK_L  0xE000      /* Left mouse button click */
#define TCU_ESC_MOUSE_CLICK_R  0xE100      /* Right mouse button click */
#define TCU_USER_DEFINED_SAVE  0xE200      /* User defined accept key */
#define TCU_USER_DEFINED_ESC   0xE300      /* User defined escape key */


#define TCU_MENU_TITLE              1      /* Title */
#define TCU_MENU_BOX                2      /* Surrounding box */
#define TCU_MENU_OPTION             3      /* Option line */
#define TCU_MENU_SELECT             4      /* Selected option line */
#define TCU_MENU_UNAVAIL            5      /* Unavailable option line */


#define TCU_OK                      0      /* Successful TCU service */
#define TCU_ERROR                   1      /* Unsuccessful TCU service */


#define TCU_FORM_EDIT               1      /* Enable edit ability for field */
#define TCU_FORM_NOEDIT             2      /* Disable edit ability for field */
#define TCU_FORM_ECHO               3      /* Echo field when input */
#define TCU_FORM_NOECHO             4      /* No echo on input field */
#define TCU_FORM_ENTER              5      /* Enter the field in edit */
#define TCU_FORM_NOENTER            6      /* Don't enter the field in edit */
#define TCU_FORM_CONFIRM            7      /* Field needs ENTER to confirm */
#define TCU_FORM_NOCONFIRM          8      /* No ENTER needed to confirm */
#define TCU_FORM_ESCS               9      /* Escape keys work in forms */
#define TCU_FORM_NOESCS            10      /* Disable escape keys in forms */

#define TCU_DB_IGNORE_CASE          1      /* Ignore case on DB searches */
#define TCU_DB_NO_IGNORE_CASE       2      /* Don't ignore case */
#define TCU_DB_TRIM_SPACES          3      /* Ignore trailing spaces */
#define TCU_DB_NO_TRIM_SPACES       4      /* Use all spaces in search */


#define TCU_FRM_MAXFIELD          128      /* Maximum fields on form */
#define TCU_MAX_SCREEN_LINES       25      /* Number of lines on screen */


#define TCU_FLD_INT                 1      /* Integer (32 bits) */
#define TCU_FLD_FLOAT               2      /* Floating point (64 bits) */
#define TCU_FLD_STRING              3      /* String, ASCIIZ format */
#define TCU_FLD_DATE                4      /* Date (01.01.70 ...) */
#define TCU_FLD_LOGICAL             5      /* True/False, Yes/No, On/Off */
#define TCU_FLD_CHOICE              6      /* Enumerated choice type */
#define TCU_FLD_BUTTON              7      /* Button selection field */


#define TCU_FLD_DAYFIRST            1      /* Dates with day number first */
#define TCU_FLD_MONTHFIRST          2      /* Dates with month number first */
                                           

#define TCU_FLD_TRUEFALSE           1      /* Logicals as TRUE/FALSE */
#define TCU_FLD_YESNO               2      /* Logicals as YES/NO */
#define TCU_FLD_ONOFF               3      /* Logicals as ON/OFF */


#define TCU_FLD_ESCNEXT             1      /* NEXT FIELD field edit escape */
#define TCU_FLD_ESCPREV             2      /* PREVIOUS FIELD escape */
#define TCU_FLD_ESCFIRST            3      /* FIRST FIELD escape */
#define TCU_FLD_ESCLAST             4      /* LAST FIELD escape */
#define TCU_FLD_SELECT              5      /* Field select escape */
#define TCU_FLD_CLICKSELECT         6      /* Mouse click field select */
#define TCU_FLD_ESCESC             -1      /* ESC key escape */
#define TCU_FLD_ESCCNTLC           -2      /* CNTL/C key escape */
#define TCU_FLD_ESCPGUP            -3      /* Page-Up key escape */
#define TCU_FLD_ESCPGDN            -4      /* Page-Down key escape */
#define TCU_FLD_FNKEYSAVE          -5      /* Function key save escape */
#define TCU_FLD_FNKEYESC           -6      /* Function key abort escape */
#define TCU_FLD_BUTTONSAVE         -7      /* Button save escape */
#define TCU_FLD_BUTTONESC          -8      /* Button abort escape */
#define TCU_FLD_USERSAVE           -9      /* User defined accept key */
#define TCU_FLD_USERESC           -10      /* User defined escape key */


typedef struct {
   short               x, y;               /* X and Y coords of current pos */
   short               height,             /* External size of menu */
                       width;              /* External width of menu */
   char               *title;              /* Title of menu */
   unsigned char       title_attrib,       /* Attributes of title */
                       box_attrib,         /* Attributes of surrounding box */
                       option_attrib,      /* Attributes of menu option */
                       select_attrib,      /* Attributes of selected option */
                       unavail_attrib;     /* Attributes of unavailables */
   unsigned char       escape_keys;        /* Keys which allow exit */
   unsigned char       box_type;           /* Type of external box */
   unsigned char       last_selection;     /* Last selected option */
   void far          (*help_fn)(int);      /* User's help function */
   char              **options;            /* Pointer to list of options */
   unsigned char      *cover_zone;         /* Pointer to covered screen */
   unsigned char       short_code;         /* Flags if short code select on */
   unsigned long       option_avail;       /* Option availability flags */
} TCU_MENU;


typedef struct {
   int                 in_use;             /* Flags current use mode */
   char               *cover_zone,
                      *title;
   int                 height,
                       width;
   unsigned char       box_attrib,
                       window_attrib,
                       input_attrib,
                       title_attrib,
                       box_type;
   unsigned char       with_input;
   int                 input_x,
                       input_y;
   char               *input_str;
   char               *txt_line[TCU_MAX_SCREEN_LINES - 2];
   int                 txt_lines;
} TCU_NOTICE;


typedef struct {
   int                 in_use;             /* Flags current use mode */
   char               *cover_zone;
   char               *title;
   int                 cursor_x,
                       cursor_y,
		       height,
                       width,
                       pos_x,
                       pos_y;
   unsigned char       box_attrib,
                       title_attrib,
                       window_attrib,
                       box_type;
} TCU_WINDOW;


typedef struct {
   unsigned char       linecolour,         /* Colour of header line */
                       optioncolour,       /* Colour of possible options */
                       selectcolour;       /* Colour of selected option */
   char              **headings;           /* List of header titles */
   TCU_MENU          **menus;              /* List of menus */
   void far          (*help_fn)(int);      /* User's help function */
   unsigned char       selected,           /* Currently selected menu */
                       use_mode;           /* Current mode of use */
   unsigned char      *cover_zone;         /* Screen contents area */
   unsigned char       short_code;         /* Flags if short code select on */
} TCU_PULLDOWN;


typedef struct {
   unsigned char       sel,                /* Selected choice */
                       max;                /* Maximum choices available */
   char               *str;                /* String of separated choices */
} TCU_CHOICE_TYPE;


typedef union {
   long                v_int;              /* Integer field value */
   double              v_float;            /* Floating field value */
   char               *v_string;           /* String field value */
   unsigned short      v_date;             /* Date value (days -> 01.01.70) */
   unsigned char       v_logical;          /* Logical field value */
   TCU_CHOICE_TYPE     v_choice;           /* Enumerated choice type */
} TCU_FIELD_VALUE;


typedef struct {
   char                   f_name[9];       /* Field name */
   unsigned char          f_type,          /* Type of field */
                          f_size,          /* Width of field on screen */
                          f_decimal,       /* Decimal places if numeric */
                          f_present,       /* Presentation form of data */
                          f_xpos,          /* Window x-coordinate */
                          f_ypos;          /* Window y-coordinate */
   unsigned char          f_usemin,        /* Flags use of minimum range */
                          f_usemax,        /* Flags use of maximum range */
                          f_useval,        /* Flags if initial value used */
                          f_usetmp;        /* Flags if string template used */
   union {
      struct {
         long             f_min,           /* Minimum value */
                          f_max;           /* Maximum value */
      } i;
      struct {
         double           f_min,           /* Minimum value */
                          f_max;           /* Maximum value */
      } f;
      struct {
         unsigned short   f_min,           /* Minimum value */
                          f_max;           /* Maximum value */
      } d;
   } range;
   unsigned char          f_colour,        /* Colour attributes */
                          f_editcolour;    /* Colour during field editing */
   struct {
      unsigned int        a_ronly : 1,     /* Read only flag */
                          a_noecho : 1,    /* No-echo input flag */
                          a_fixtext : 1,   /* Fixed text field flag */
                          a_param : 1,     /* Non-editable variable field */
                          a_confirm : 1;   /* ENTER confirmation needed flag */
   } attrib;
   TCU_FIELD_VALUE        val;             /* Value of field */
   unsigned char          dummy;           /* Marker for start of pointers */
   char                  *f_template;      /* Template for string inputs */
} TCU_FIELD;


typedef struct {
   unsigned int           num_fields;      /* Number of fields */
   unsigned char          colour,          /* Colour attribute */
                          input_colour;    /* Input field colour default */
   char                   title[77];       /* Form title string */
   unsigned char          xpos,            /* x-coordinate on screen */
                          ypos,            /* y-coordinate on screen */
                          height,          /* Height of form */
                          width,           /* Width of form */
                          type,            /* Form surround type */
                          title_attrib,    /* Colour attributes for title */
                          current_field,   /* Current field with focus */
                          use_mode;        /* Flags whether in use/displayed */
   struct {
      unsigned int        ronly : 1,       /* Form is read only */
                          no_esc : 1;      /* No direct escape keys allowed */
   } attrib;
   unsigned char          dummy;           /* Marker for start of pointers */
   TCU_FIELD             *field[TCU_FRM_MAXFIELD]; /* Fields */
   int far              (*verify_fn)
                            (void *, int, TCU_FIELD_VALUE *); /* verify fn */
   void far             (*help_fn)
                            (void *, int);         /* User's help function */
   int far              (*fn_key_fn)
                            (void *, int, int);    /* Function key handler */
   int far              (*button_fn)
                            (void *, int);         /* Button select handler */
   unsigned char         *cover_zone;      /* Screen cover buffer */
} TCU_FORM;


typedef struct {
   char                   name[9];         /* Field name */
   unsigned char          type,            /* Type of field */
                          size,            /* Width of field on screen */
                          decimal,         /* Decimal places if numeric */
                          present,         /* Presentation form of data */
                          xpos,            /* Window x-coordinate */
                          ypos,            /* Window y-coordinate */
                          usemin,          /* Flags use of minimum range */
                          usemax,          /* Flags use of maximum range */
                          useval,          /* Flags if initial value used */
                          usetmp;          /* Flags if string template used */
   union {
      struct {
         long             min,             /* Minimum value */
                          max;             /* Maximum value */
      } i;
      struct {
         double           min,             /* Minimum value */
                          max;             /* Maximum value */
      } f;
      struct {
         unsigned short   min,             /* Minimum value */
                          max;             /* Maximum value */
      } d;
   } range;
   unsigned char          colour,          /* Colour attributes */
                          editcolour;      /* Colour during field editing */
   struct {
      unsigned int        ronly : 1,       /* Read only flag */
                          noecho : 1,      /* No-echo input flag */
                          fixtext : 1,     /* Fixed text field flag */
                          param : 1,       /* Non-editable variable field */
                          confirm : 1;     /* ENTER confirmation needed flag */
   } attr;
   TCU_FIELD_VALUE        val;             /* Value of field */
} TCU_FIELD_INFO;


typedef struct {
   unsigned int           num_fields;      /* Number of fields */
   char                  *title;           /* Title string of form */
   unsigned char          text_colour,     /* Colour attribute */
                          field_colour,    /* Input field colour default */
                          title_colour,    /* Title string colour */
                          xpos,            /* x-coordinate on screen */
                          ypos,            /* y-coordinate on screen */
                          height,          /* Height of form */
                          width,           /* Width of form */
                          box_type,        /* Form surround type */
                          mode,            /* Flags whether in use/displayed */
                          verify_fn,       /* Verification function active */
                          help_fn,         /* Help function active */
                          fn_key_fn,       /* 1 if fn. key handler active */
                          button_fn;       /* 1 if button select handler */
   struct {
      unsigned int        ronly : 1,       /* Form is read only */
                          no_esc : 1;      /* Escape keys disabled */
   } attr;
} TCU_FORM_INFO;


typedef struct {
   long            db_next,                /* Next free database slot */
                   db_current;             /* Current location of ptr */
   TCU_FORM       *form_ref,               /* Pointer to registered form */
                  *temp_form;              /* Copy of above during form edit */
   char           *formbuf,                /* Pointer to field buffer */
                  *index;                  /* Pointer to b-tree index */
   int             db_file,                /* Database file handle */
                   db_recsize;             /* Size of form record */
   unsigned short  db_idxdir;              /* Sort direction for keys */
   struct {                                /* String search attributes */
      unsigned     rtrim : 1,              /* Ignore trailing blanks */
                   ignorecase : 1;         /* Ignore case */
   } s_attr;
   unsigned char   db_duplicates,          /* Number of keys to check */
                   ix_fields[16][2],       /* Index field ID and type */
                   number_of_keys,         /* Total number of fields in key */
                   search_keys,            /* Number of fields to look for */
                   use_mode;               /* Flags current in-use mode */
} TCU_DB;



/* External objects */

extern TCU_FORM *_TCU_UPDATE_form;



/* Add prefix to function prototypes for C++ compilation */

#ifdef __cplusplus
#define c_cpp extern "C"
#else
#define c_cpp
#endif


/*--------------------------------------------------------------------------*/
/*                                                                          */
/*                            Database Services                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/


c_cpp int  tcu_db_at_bof               (TCU_DB *);
c_cpp int  tcu_db_at_eof               (TCU_DB *);
c_cpp int  tcu_db_close                (TCU_DB *);
c_cpp int  tcu_db_create               (TCU_DB *, TCU_FORM *, char *, int,
                                        char **);
c_cpp int  tcu_db_delete               (TCU_DB *);
c_cpp void tcu_db_end_form_edit        (TCU_DB *);
c_cpp int  tcu_db_find                 (TCU_DB *, TCU_FIELD_VALUE *);
c_cpp int  tcu_db_first                (TCU_DB *);
c_cpp int  tcu_db_last                 (TCU_DB *);
c_cpp int  tcu_db_next                 (TCU_DB *);
c_cpp int  tcu_db_open                 (TCU_DB *, TCU_FORM *, char *, int);
c_cpp int  tcu_db_previous             (TCU_DB *);
c_cpp int  tcu_db_read                 (TCU_DB *);
c_cpp int  tcu_db_read_index_field     (TCU_DB *, int, TCU_FIELD_VALUE *);
c_cpp long tcu_db_record_count         (TCU_DB *);
c_cpp int  tcu_db_remove               (char *);
c_cpp int  tcu_db_rewrite              (TCU_DB *);
c_cpp int  tcu_db_save                 (TCU_DB *);
c_cpp int  tcu_db_search               (TCU_DB *, TCU_FIELD_VALUE *);
c_cpp int  tcu_db_set_search_indices   (TCU_DB *, int);
c_cpp int  tcu_db_set_search_mode      (TCU_DB *, int);
c_cpp void tcu_db_start_form_edit      (TCU_DB *);
c_cpp int  tcu_db_write                (TCU_DB *);



/*--------------------------------------------------------------------------*/
/*                                                                          */
/*                  Pop-Up & Pulldown Menu Services                         */
/*                                                                          */
/*--------------------------------------------------------------------------*/


c_cpp int tcu_change_menu_attribs      (TCU_MENU *, int, unsigned char);
c_cpp int tcu_change_menu_escapes      (TCU_MENU *, unsigned char);
c_cpp int tcu_clear_menu_in_pulldown   (TCU_PULLDOWN *);
c_cpp int tcu_define_menu              (TCU_MENU *, char *, unsigned char,
                                        unsigned char, unsigned char,
                                        unsigned char, unsigned char,
                                        unsigned char, unsigned char, char **,
                                        unsigned char);
c_cpp int tcu_define_pulldown          (TCU_PULLDOWN *, unsigned char,
                                        unsigned char, unsigned char, char **,
                                        unsigned char, TCU_MENU **);
c_cpp int tcu_display_menu             (TCU_MENU *, int, int);
c_cpp int tcu_display_pulldown_header  (TCU_PULLDOWN *);
c_cpp int tcu_escape_fkey              (void);
c_cpp int tcu_new_pulldown_cover       (TCU_PULLDOWN *);
c_cpp int tcu_read_menu_selection      (TCU_MENU *);
c_cpp int tcu_read_pulldown_selection  (TCU_PULLDOWN *, int *, int *);
c_cpp int tcu_remove_menu              (TCU_MENU *);
c_cpp int tcu_remove_pulldown          (TCU_PULLDOWN *);
c_cpp int tcu_set_menu_help            (TCU_MENU *, void far (*)(int));
c_cpp int tcu_set_menu_option          (TCU_MENU *, int, int);
c_cpp int tcu_set_pulldown_help        (TCU_PULLDOWN *, void far (*)(int));



/*--------------------------------------------------------------------------*/
/*                                                                          */
/*                       Prompt & Notice Utilities                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/


c_cpp int tcu_clear_notice             (TCU_NOTICE *);
c_cpp int tcu_display_notice           (TCU_NOTICE *, int, int);
c_cpp int tcu_get_confirm              (int, int, unsigned char, unsigned char,
                                        char *, ...);
c_cpp int tcu_notice_text              (TCU_NOTICE *, char *, ...);
c_cpp int tcu_prepare_notice           (TCU_NOTICE *, char *, unsigned char,
                                        unsigned char, unsigned char,
                                        unsigned char);
c_cpp int tcu_prompt_input             (TCU_NOTICE *, int, int, char *,
                                        unsigned char);



/*--------------------------------------------------------------------------*/
/*                                                                          */
/*                           Screen/Form Utilities                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/


c_cpp int tcu_clear_form_fields        (TCU_FORM *);
c_cpp int tcu_display_form             (TCU_FORM *, int, int);
c_cpp int tcu_edit_form                (TCU_FORM *, int, int *);
c_cpp int tcu_form_record_size         (TCU_FORM *);
c_cpp int tcu_get_field                (TCU_FORM *, int, TCU_FIELD_VALUE *);
c_cpp int tcu_get_field_choice_string  (TCU_FORM *, int, char *);
c_cpp int tcu_get_field_id             (TCU_FORM *, char *, int *);
c_cpp int tcu_get_field_info           (TCU_FORM *, int, TCU_FIELD_INFO *);
c_cpp int tcu_get_form_info            (TCU_FORM *, TCU_FORM_INFO *);
c_cpp int tcu_load_form                (TCU_FORM *, char *);
c_cpp int tcu_load_image_form          (TCU_FORM *, TCU_FORM *);
c_cpp int tcu_put_field                (TCU_FORM *, int, TCU_FIELD_VALUE *);
c_cpp int tcu_read_formrec             (TCU_FORM *, char *);
c_cpp int tcu_remove_form              (TCU_FORM *);
c_cpp int tcu_select_field             (TCU_FORM *, int, int *);
c_cpp int tcu_set_button_fn            (TCU_FORM *, int far (*)(TCU_FORM *,
                                                                int));
c_cpp int tcu_set_field_attrib         (TCU_FORM *, int, unsigned char);
c_cpp int tcu_set_field_mode           (TCU_FORM *, int, int);
c_cpp int tcu_set_field_verify         (TCU_FORM *, int far (*)
                                                    (TCU_FORM *, int,
                                                     TCU_FIELD_VALUE *));
c_cpp int tcu_set_form_fnkey_fn        (TCU_FORM *, int far (*)
                                                    (TCU_FORM *, int, int));
c_cpp int tcu_set_form_help            (TCU_FORM *, void far (*)
                                                    (TCU_FORM *, int));
c_cpp int tcu_set_form_mode            (TCU_FORM *, int);
c_cpp int tcu_unload_form              (TCU_FORM *);
c_cpp int tcu_write_formrec            (TCU_FORM *, char *);



/*--------------------------------------------------------------------------*/
/*                                                                          */
/*                          Windowing Utilities                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/


c_cpp int tcu_change_colour            (TCU_WINDOW *, unsigned char);
c_cpp int tcu_clear_window             (TCU_WINDOW *);
c_cpp int tcu_close_window             (TCU_WINDOW *);
c_cpp int tcu_open_window              (TCU_WINDOW *, int, int, int, int,
                                        char *, unsigned char, unsigned char,
                                        unsigned char, unsigned char);
c_cpp int tcu_position_cursor          (TCU_WINDOW *, int, int);
c_cpp int tcu_wprintf                  (TCU_WINDOW *, char *, ...);
c_cpp int tcu_wgets                    (TCU_WINDOW *, int, char *, int *);



/*--------------------------------------------------------------------------*/
/*                                                                          */
/*                            General Utilities                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/


c_cpp unsigned char  tcu_colour_attrib        (int, int);
c_cpp char          *tcu_date_string          (unsigned short, unsigned char);
c_cpp unsigned short tcu_date_value           (char *, unsigned char);
c_cpp unsigned short tcu_get_user_keypress    (void);
c_cpp unsigned long  tcu_hash_value           (char *, unsigned long);
c_cpp void           tcu_restore_environment  (void);
c_cpp void           tcu_save_environment     (void);
c_cpp int            tcu_set_idle_loop        (int far (*)(unsigned long));
c_cpp int            tcu_set_mouse_mode       (unsigned char);
c_cpp int            tcu_set_user_key_handler (int far (*)(unsigned short *));
c_cpp void           tcu_warnbeep             (void);



/* If TCU compatibility mode is selected to be compatible with pre version */
/* 3.2 then we must include the 'tcupre32.h' file here to alias the right */
/* bits.  Don't rely on this always being here... it won't be! */

#if      defined(__TCUPRE32__)
#include <usr\tcupre32.h>
#endif

#endif
