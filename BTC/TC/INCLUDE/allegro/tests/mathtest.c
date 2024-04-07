/*         ______   ___    ___ 
 *        /\  _  \ /\_ \  /\_ \ 
 *        \ \ \L\ \\//\ \ \//\ \      __     __   _ __   ___ 
 *         \ \  __ \ \ \ \  \ \ \   /'__`\ /'_ `\/\`'__\/ __`\
 *          \ \ \/\ \ \_\ \_ \_\ \_/\  __//\ \L\ \ \ \//\ \L\ \
 *           \ \_\ \_\/\____\/\____\ \____\ \____ \ \_\\ \____/
 *            \/_/\/_/\/____/\/____/\/____/\/___L\ \/_/ \/___/
 *                                           /\____/
 *                                           \_/__/
 *
 *      Fixed point maths test program for the Allegro library.
 *
 *      By Shawn Hargreaves.
 *
 *      See readme.txt for copyright information.
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "allegro.h"


#define SIN    1
#define COS    2
#define TAN    3
#define ASIN   4
#define ACOS   5
#define ATAN   6
#define SQRT   7

#define ADD    1
#define SUB    2
#define MUL    3
#define DIV    4

char calc_str[80];

fixed calc_value;
int calc_operation;
int calc_clear_flag;

extern DIALOG calculator[];

#define CALC_STR  3



void redraw(DIALOG *d)
{
   show_mouse(NULL);

   SEND_MESSAGE(calculator+CALC_STR, MSG_DRAW, 0);

   if (d)
      SEND_MESSAGE(d, MSG_DRAW, 0);

   show_mouse(screen);
}



void reset_calc()
{
   calc_value = 0;
   calc_operation = 0;
   calc_clear_flag = FALSE;
}



fixed get_calc_value()
{
   double d = atof(calc_str);
   return ftofix(d);
}



void set_calc_value(fixed v)
{
   int c, l;
   int has_dot;
   char b[80];
   double d = fixtof(v);

   sprintf(b, "%.8f", d);

   l = 0;
   has_dot = FALSE;

   for (c=0; b[c]; c++) {
      if ((b[c] >= '0') && (b[c] <= '9')) {
	 l++;
	 if (l > 8) {
	    b[c] = 0;
	    break;
	 }
      }
      else
	 if (b[c] == '.')
	    has_dot = TRUE;
   }

   if (has_dot) {
      for (c=c-1; c>=0; c--) {
	 if (b[c] == '0')
	    b[c] = 0;
	 else {
	    if (b[c] == '.') 
	       b[c] = 0;
	    break;
	 }
      }
   }

   if (errno)
      sprintf(calc_str, "-E- (%s)", b);
   else
      strcpy(calc_str, b);

   errno = 0;
}



int right_text_proc(int msg, DIALOG *d, int c)
{
   int len;

   if (msg == MSG_DRAW) {
      len = strlen(d->dp);
      text_mode(d->bg);
      textout(screen, font, d->dp, d->x+d->w-len*8, d->y, d->fg);
      rectfill(screen, d->x, d->y, d->x+d->w-len*8-1, d->y+d->h, d->bg); 
   }

   return D_O_K;
}



int input_proc(int msg, DIALOG *d, int c)
{
   int c1, c2;
   char *s = d->dp;
   int ret = d_button_proc(msg, d, c);

   if (ret == D_CLOSE) {
      if ((calc_clear_flag) && (s[0] != '+'))
	 strcpy(calc_str, "0");
      else if (strncmp(calc_str, "-E-", 3) == 0) {
	 redraw(d);
	 return D_O_K;
      }

      calc_clear_flag = FALSE;

      if (s[0] == '.') {
	 for (c1=0; calc_str[c1]; c1++) 
	    if (calc_str[c1] == '.')
	       break;

	 if (!calc_str[c1])
	    strcat(calc_str, ".");
      }
      else if (s[0] == '+') {
	 if (calc_str[0] == '-')
	    memmove(calc_str, calc_str+1, strlen(calc_str+1)+1);
	 else {
	    memmove(calc_str+1, calc_str, strlen(calc_str)+1);
	    calc_str[0] = '-';
	 }
      }
      else {
	 if (strcmp(calc_str, "0") == 0)
	    calc_str[0] = 0;
	 else if (strcmp(calc_str, "-0") == 0) 
	    strcpy(calc_str, "-");

	 c2 = 0;
	 for (c1=0; calc_str[c1]; c1++)
	    if ((calc_str[c1] >= '0') && (calc_str[c1] <= '9'))
	       c2++;

	 if (c2 < 8)
	    strcat(calc_str, s);
      }

      redraw(d);
      return D_O_K;
   }

   return ret;
}



int unary_operator(int msg, DIALOG *d, int c)
{
   fixed x;
   int ret = d_button_proc(msg, d, c);

   if (ret == D_CLOSE) {
      x = get_calc_value();

      switch (d->d1) {

	 case SIN:
	    x = fsin(x);
	    break;

	 case COS:
	    x = fcos(x);
	    break;

	 case TAN:
	    x = ftan(x);
	    break;

	 case ASIN:
	    x = fasin(x);
	    break;

	 case ACOS:
	    x = facos(x);
	    break;

	 case ATAN:
	    x = fatan(x);
	    break;

	 case SQRT:
	    x = fsqrt(x);
	    break;
      }

      set_calc_value(x);
      calc_clear_flag = TRUE;

      redraw(d);
      return D_O_K;
   }

   return ret;
}



int work_out()
{
   fixed x;

   x = get_calc_value();

   switch (calc_operation) {

      case ADD:
	 x = fadd(calc_value, x);
	 break;

      case SUB:
	 x = fsub(calc_value, x);
	 break;

      case MUL:
	 x = fmul(calc_value, x);
	 break;

      case DIV:
	 x = fdiv(calc_value, x);
	 break;
   }

   set_calc_value(x);
   reset_calc();
   calc_clear_flag = TRUE;

   redraw(NULL);
   return D_O_K;
}



int equals_proc(int msg, DIALOG *d, int c)
{
   int ret = d_button_proc(msg, d, c);

   if (ret == D_CLOSE) { 
      ret = work_out();
      redraw(d);
   }

   return ret;
}



int binary_operator(int msg, DIALOG *d, int c)
{
   int ret = d_button_proc(msg, d, c);

   if (ret == D_CLOSE) {
      work_out();

      calc_value = get_calc_value();
      calc_operation = d->d1;
      calc_clear_flag = TRUE;

      redraw(d);
      return D_O_K;
   }

   return ret;
}



int clearer()
{
   reset_calc();
   strcpy(calc_str, "0");
   redraw(NULL);
   return D_O_K;
}



int clear_proc(int msg, DIALOG *d, int c)
{
   int ret = d_button_proc(msg, d, c);

   if (ret == D_CLOSE) {
      ret = clearer();
      redraw(d);
   }

   return ret;
}



DIALOG calculator[] =
{
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)     (d2)           (dp)        (dp2) (dp3) */
   { d_shadow_box_proc, 0,    0,    192,  172,  255,  8,    0,    0,       0,       0,             NULL,       NULL, NULL  },
   { d_box_proc,        8,    10,   176,  20,   255,  0,    0,    0,       0,       0,             NULL,       NULL, NULL  },
   { d_box_proc,        10,   12,   172,  16,   0,    255,  0,    0,       0,       0,             NULL,       NULL, NULL  },
   { right_text_proc,   24,   16,   144,  8,    0,    255,  0,    0,       0,       0,             calc_str,   NULL, NULL  },

   { unary_operator,    16,   48,   48,   12,   255,  0,    0,    D_EXIT,  SIN,     0,             "sin",      NULL, NULL  },
   { unary_operator,    72,   48,   48,   12,   255,  0,    0,    D_EXIT,  COS,     0,             "cos",      NULL, NULL  },
   { unary_operator,    128,  48,   48,   12,   255,  0,    0,    D_EXIT,  TAN,     0,             "tan",      NULL, NULL  },

   { unary_operator,    16,   64,   48,   12,   255,  0,    0,    D_EXIT,  ASIN,    0,             "asin",     NULL, NULL  },
   { unary_operator,    72,   64,   48,   12,   255,  0,    0,    D_EXIT,  ACOS,    0,             "acos",     NULL, NULL  },
   { unary_operator,    128,  64,   48,   12,   255,  0,    0,    D_EXIT,  ATAN,    0,             "atan",     NULL, NULL  },

   { input_proc,        8,    88,   32,   12,   255,  0,    '7',  D_EXIT,  0,       0,             "7",        NULL, NULL  },
   { input_proc,        44,   88,   32,   12,   255,  0,    '8',  D_EXIT,  0,       0,             "8",        NULL, NULL  },
   { input_proc,        80,   88,   32,   12,   255,  0,    '9',  D_EXIT,  0,       0,             "9",        NULL, NULL  },
   { binary_operator,   116,  88,   32,   12,   255,  0,    '/',  D_EXIT,  DIV,     0,             "/",        NULL, NULL  },
   { clear_proc,        152,  88,   32,   12,   255,  0,    'c',  D_EXIT,  0,       0,             "C",        NULL, NULL  },
   { d_keyboard_proc,   0,    0,    0,    0,    0,    0,    0,    0,       KEY_DEL, KEY_BACKSPACE, clearer,    NULL, NULL  },

   { input_proc,        8,    108,  32,   12,   255,  0,    '4',  D_EXIT,  0,       0,             "4",        NULL, NULL  },
   { input_proc,        44,   108,  32,   12,   255,  0,    '5',  D_EXIT,  0,       0,             "5",        NULL, NULL  },
   { input_proc,        80,   108,  32,   12,   255,  0,    '6',  D_EXIT,  0,       0,             "6",        NULL, NULL  },
   { binary_operator,   116,  108,  32,   12,   255,  0,    '*',  D_EXIT,  MUL,     0,             "*",        NULL, NULL  },
   { d_button_proc,     152,  108,  32,   12,   255,  0,    27,   D_EXIT,  0,       0,             "off",      NULL, NULL  },

   { input_proc,        8,    128,  32,   12,   255,  0,    '1',  D_EXIT,  0,       0,             "1",        NULL, NULL  },
   { input_proc,        44,   128,  32,   12,   255,  0,    '2',  D_EXIT,  0,       0,             "2",        NULL, NULL  },
   { input_proc,        80,   128,  32,   12,   255,  0,    '3',  D_EXIT,  0,       0,             "3",        NULL, NULL  },
   { binary_operator,   116,  128,  32,   12,   255,  0,    '-',  D_EXIT,  SUB,     0,             "-",        NULL, NULL  },
   { unary_operator,    152,  128,  32,   12,   255,  0,    0,    D_EXIT,  SQRT,    0,             "sqr",      NULL, NULL  },

   { input_proc,        8,    148,  32,   12,   255,  0,    '0',  D_EXIT,  0,       0,             "0",        NULL, NULL  },
   { input_proc,        44,   148,  32,   12,   255,  0,    '.',  D_EXIT,  0,       0,             ".",        NULL, NULL  },
   { input_proc,        80,   148,  32,   12,   255,  0,    0,    D_EXIT,  0,       0,             "+/-",      NULL, NULL  },
   { binary_operator,   116,  148,  32,   12,   255,  0,    '+',  D_EXIT,  ADD,     0,             "+",        NULL, NULL  },
   { equals_proc,       152,  148,  32,   12,   255,  0,    '=',  D_EXIT,  0,       0,             "=",        NULL, NULL  },
   { d_keyboard_proc,   0,    0,    0,    0,    0,    0,    13,   0,       0,       0,             work_out,   NULL, NULL  },

   { NULL,              0,    0,    0,    0,    0,    0,    0,    0,       0,       0,             NULL,       NULL, NULL  }
};



int main()
{
   allegro_init();
   install_mouse();
   install_keyboard();
   install_timer();
   set_gfx_mode(GFX_VGA, 320, 200, 0, 0);
   set_pallete(desktop_pallete);

   textout(screen, font, "Angles are binary, 0-255", 0, 0, 255);
   reset_calc();
   strcpy(calc_str, "0");
   errno = 0;

   centre_dialog(calculator);
   do_dialog(calculator, -1);

   return 0;
}

