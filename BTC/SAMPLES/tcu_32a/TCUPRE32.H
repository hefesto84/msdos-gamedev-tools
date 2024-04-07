/*==========================================================================*
 *                                                                          *
 *  This file is included automatically bu the main TCU.H file when the     *
 *  symbol __TCUPRE32__ is set to request compatibility with versions of    *
 *  of TCU prior to 3.2.                                                    *
 *                                                                          *
 *                    (c) Copyright Karl Keyte, 1990-91                     *
 *                                                                          *
 *  Karl Keyte                           Phone:  +(49) 6150 2499            *                       *
 *  Gross-Gerauer Strasse 10                                                *
 *  D-6108 Weiterstadt                   e-Mail: KKEYTE@ESOC.BITNET         *
 *  Germany                              Bix:    kkeyte                     *
 *                                                                          *
 *==========================================================================*/



#define BOX_SINGLE        TCU_BOX_SINGLE
#define BOX_DOUBLE        TCU_BOX_DOUBLE
#define BOX_BLANK         TCU_BOX_BLANK


#define ESC_ESC           TCU_ESC_ESC
#define ESC_PGUP          TCU_ESC_PGUP
#define ESC_PGDN          TCU_ESC_PGDN           
#define ESC_CLEFT         TCU_ESC_CLEFT
#define ESC_CRIGHT        TCU_ESC_CRIGHT
#define ESC_FUNC          TCU_ESC_FUNC
#define ESC_CNTL_C        TCU_ESC_CNTL_C
#define ESC_MOUSE_CLICK_L TCU_ESC_MOUSE_CLICK_L
#define ESC_MOUSE_CLICK_R TCU_ESC_MOUSE_CLICK_R


#define MENU_TITLE        TCU_MENU_TITLE         
#define MENU_BOX          TCU_MENU_BOX           
#define MENU_OPTION       TCU_MENU_OPTION        
#define MENU_SELECT       TCU_MENU_SELECT        
#define MENU_UNAVAIL      TCU_MENU_UNAVAIL       


#define FORM_EDIT         TCU_FORM_EDIT         
#define FORM_NOEDIT       TCU_FORM_NOEDIT       
#define FORM_ECHO         TCU_FORM_ECHO         
#define FORM_NOECHO       TCU_FORM_NOECHO       
#define FORM_ENTER        TCU_FORM_ENTER        
#define FORM_NOENTER      TCU_FORM_NOENTER      
#define FORM_CONFIRM      TCU_FORM_CONFIRM      
#define FORM_NOCONFIRM    TCU_FORM_NOCONFIRM    
#define FORM_ESCS         TCU_FORM_ESCS         
#define FORM_NOESCS       TCU_FORM_NOESCS       


#define FRM_MAXFIELD      TCU_FRM_MAXFIELD
#define MAX_SCREEN_LINES  TCU_MAX_SCREEN_LINES


#define FLD_INT           TCU_FLD_INT           
#define FLD_FLOAT         TCU_FLD_FLOAT         
#define FLD_STRING        TCU_FLD_STRING        
#define FLD_DATE          TCU_FLD_DATE          
#define FLD_LOGICAL       TCU_FLD_LOGICAL       
#define FLD_CHOICE        TCU_FLD_CHOICE        
#define FLD_BUTTON        TCU_FLD_BUTTON        


#define FLD_DAYFIRST      TCU_FLD_DAYFIRST      
#define FLD_MONTHFIRST    TCU_FLD_MONTHFIRST    


#define FLD_TRUEFALSE     TCU_FLD_TRUEFALSE     
#define FLD_YESNO         TCU_FLD_YESNO         
#define FLD_ONOFF         TCU_FLD_ONOFF         


#define FLD_ESCNEXT       TCU_FLD_ESCNEXT
#define FLD_ESCPREV       TCU_FLD_ESCPREV       
#define FLD_ESCFIRST      TCU_FLD_ESCFIRST      
#define FLD_ESCLAST       TCU_FLD_ESCLAST       
#define FLD_SELECT        TCU_FLD_SELECT        
#define FLD_CLICKSELECT   TCU_FLD_CLICKSELECT   
#define FLD_ESCESC        TCU_FLD_ESCESC        
#define FLD_ESCCNTLC      TCU_FLD_ESCCNTLC      
#define FLD_ESCPGUP       TCU_FLD_ESCPGUP       
#define FLD_ESCPGDN       TCU_FLD_ESCPGDN       
#define FLD_FNKEYSAVE     TCU_FLD_FNKEYSAVE     
#define FLD_FNKEYESC      TCU_FLD_FNKEYESC      
#define FLD_BUTTONSAVE    TCU_FLD_BUTTONSAVE    
#define FLD_BUTTONESC     TCU_FLD_BUTTONESC     


typedef TCU_MENU          MENU;
typedef TCU_NOTICE        NOTICE;
typedef TCU_WINDOW        WINDOW;
typedef TCU_PULLDOWN      PULLDOWN;
typedef TCU_CHOICE_TYPE   CHOICE_TYPE;
typedef TCU_FIELD_VALUE   FIELD_VALUE;
typedef TCU_FIELD         FIELD;
typedef TCU_FORM          FORM;
typedef TCU_FIELD_INFO    FIELD_INFO;
typedef TCU_FORM_INFO     FORM_INFO;


/* External objects */

#define _UPDATE_form      _TCU_UPDATE_form


#define change_colour             tcu_change_colour
#define change_menu_attribs       tcu_change_menu_attribs
#define change_menu_attribs       tcu_change_menu_attribs
#define change_menu_escapes       tcu_change_menu_escapes
#define clear_menu_in_pulldown    tcu_clear_menu_in_pulldown
#define clear_notice              tcu_clear_notice
#define clear_window              tcu_clear_window
#define close_window              tcu_close_window
#define colour_attrib             tcu_colour_attrib
#define date_string               tcu_date_string
#define date_value                tcu_date_value
#define define_menu               tcu_define_menu
#define define_pulldown           tcu_define_pulldown
#define display_form              tcu_display_form
#define display_menu              tcu_display_menu
#define display_notice            tcu_display_notice
#define display_pulldown_header   tcu_display_pulldown_header
#define edit_form                 tcu_edit_form
#define escape_fkey               tcu_escape_fkey
#define get_confirm               tcu_get_confirm
#define get_field                 tcu_get_field
#define get_field_choice_string   tcu_get_field_choice_string
#define get_field_id              tcu_get_field_id
#define get_field_info            tcu_get_field_info
#define get_form_info             tcu_get_form_info
#define load_form                 tcu_load_form
#define load_image_form           tcu_load_image_form
#define new_pulldown_cover        tcu_new_pulldown_cover
#define notice_text               tcu_notice_text
#define open_window               tcu_open_window
#define position_cursor           tcu_position_cursor
#define prepare_notice            tcu_prepare_notice
#define prompt_input              tcu_prompt_input
#define put_field                 tcu_put_field
#define read_menu_selection       tcu_read_menu_selection
#define read_pulldown_selection   tcu_read_pulldown_selection
#define remove_form               tcu_remove_form
#define remove_menu               tcu_remove_menu
#define remove_pulldown           tcu_remove_pulldown
#define select_field              tcu_select_field
#define set_button_fn             tcu_set_button_fn
#define set_field_attrib          tcu_set_field_attrib
#define set_field_mode            tcu_set_field_mode
#define set_field_verify          tcu_set_field_verify
#define set_form_fnkey_fn         tcu_set_form_fnkey_fn
#define set_form_help             tcu_set_form_help
#define set_form_mode             tcu_set_form_mode
#define set_menu_help             tcu_set_menu_help
#define set_menu_option           tcu_set_menu_option
#define set_mouse_mode            tcu_set_mouse_mode
#define set_pulldown_help         tcu_set_pulldown_help
#define unload_form               tcu_unload_form
#define wgets                     tcu_wgets
#define wprintf                   tcu_wprintf
