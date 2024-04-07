/*************************
 *	MOUSE.C          *
 *************************/

#include <dos.h>
#include "mouse.h"

/*  Function 0.    Mouse Reset     */
int reset_mouse(void)
{
	union REGS ireg, oreg;

	ireg.x.ax = 0;
	int86(0x33, &ireg, &oreg);

	return( (oreg.x.ax == 0)? 0: 1 );
}

/*  Use the mouse if there is one */
void init_mouse(void)
{
	extern int mu_install;

    if ( reset_mouse() ) {
        show_mouse();
		mu_install = 1;
	}
}

/*  Function 0.     Get mouse button number    */
int get_button_num(void)
{
	union REGS ireg, oreg;

	ireg.x.ax = 0;
	int86(0x33, &ireg, &oreg);

	return( ( (oreg.x.bx==3)||(oreg.x.bx==2) )? oreg.x.bx: 0 );
}


/*  Function 1.    Show mouse cursor   */
void show_mouse(void)
{
	union REGS ireg;

	ireg.x.ax = 1;
	int86(0x33, &ireg, &ireg);
}


/* Function 2.     Hide mouse cursor   */
void hide_mouse(void)
{
	union REGS ireg;

	ireg.x.ax = 2;
	int86(0x33, &ireg, &ireg);
}


/* Function 3. Check which button(s) is(are) pressed  */
int which_pressed(MOUSE *mouse)
{
	union REGS ireg, oreg;

	ireg.x.ax = 3;
	int86(0x33, &ireg, &oreg);

	mouse->x = oreg.x.cx;
	mouse->y = oreg.x.dx;
	return(oreg.x.bx);
}


/* Function 3.   Get position of cursor in graphics  */
void get_xy(MOUSE *grmouse)
{
	union REGS ireg, oreg;

	ireg.x.ax = 3;
	int86(0x33, &ireg, &oreg);

	grmouse->x = oreg.x.cx;
	grmouse->y = oreg.x.dx;
}


/* Function 3.   Get position of cursor in text  */
void get_t_xy(MOUSE *txtmouse)
{
	union REGS ireg, oreg;

	ireg.x.ax = 3;
	int86(0x33, &ireg, &oreg);

	txtmouse->x = oreg.x.cx/8 + 1;
	txtmouse->y = oreg.x.dx/8 + 1;
}


/* Function 4.   Set position of mouse cursor in graphics  */
void set_xy(int x, int y)
{
	union REGS ireg;

	ireg.x.ax = 4;
	ireg.x.cx = x;
	ireg.x.dx = y;
	int86(0x33, &ireg, &ireg);
}


/* Function 4.   Set position of mouse cursor in text  */
void set_t_xy(int Tx, int Ty)
{
	int x, y;

	x = (Tx-1) * 8;
	y = (Ty-1) * 8;
    set_xy(x, y);
}


/* Function 5.   Get button press information  */
int pressed_status(MOUSE *mouse, int button)
{
	union REGS ireg, oreg;

	ireg.x.ax = 5;
	ireg.x.bx = button;
	int86(0x33, &ireg, &oreg);

	mouse->x = oreg.x.cx;
	mouse->y = oreg.x.dx;
	mouse->but = oreg.x.ax;
	return(oreg.x.bx);
}

int click_button(MOUSE *mouse, int button)
{
	/* 0: left_b, 1: right_b, 2: middle_b */
	int mask[] = {-1, 0, 1, -1, 2};

    return( ((which_pressed(mouse)==button)&&
        (pressed_status(mouse, mask[button])==1))? 1: 0);
}


/* Function 6.   Get button release information  */
int released_status(MOUSE *mouse, int button)
{
	union REGS ireg, oreg;

	ireg.x.ax = 6;
	ireg.x.bx = button;
	int86(0x33, &ireg, &oreg);

	mouse->x = oreg.x.cx;
	mouse->y = oreg.x.dx;
	mouse->but = oreg.x.ax;
	return(oreg.x.bx);
}


/* Function 7.  Set the range of horizontal cursor position */
void set_x_range(int min, int max)
{
	union REGS ireg;

	ireg.x.ax = 7;
	ireg.x.cx = min;
	ireg.x.dx = max;
	int86(0x33, &ireg, &ireg);
}


/* Function 8.  Set the range of vertical cursor position */
void set_y_range(int min, int max)
{
	union REGS ireg;

	ireg.x.ax = 8;
	ireg.x.cx = min;
	ireg.x.dx = max;
	int86(0x33, &ireg, &ireg);
}


/* Function 9.  Set mouse graphic cursor block  */
void set_graphic_cursor(int x, int y, unsigned int far *pattern)
{
	union REGS ireg;
	struct SREGS isreg;

	ireg.x.ax = 9;
	ireg.x.bx = x;
	ireg.x.cx = y;
	ireg.x.dx = FP_OFF(pattern);
	isreg.es  = FP_SEG(pattern);
	int86x(0x33, &ireg, &ireg, &isreg);
}


/* Function 10.  Set mouse text cursor   */
void set_text_cursor(int type, int screen_mask, int cursor_mask)
{
	union REGS ireg;

	ireg.x.ax = 10;
	ireg.x.bx = type;
	ireg.x.cx = screen_mask;   /* 選擇游標顯示的模式 */
	ireg.x.dx = cursor_mask;   /* 輸入游標的起始線 或screen-mask 的值*/
	int86(0x33, &ireg, &ireg); /* 輸入游標的終止線 或cursor-mask 的值*/
}


#if !defined(NOCURSOR)

unsigned Cursor[][32]= {
   {
   /*  Cursor[0] ----->  Hand  */
   /*  Screen Mask  */
     0xE1FF,      /*  1110000111111111   */
     0xE1FF,	  /*  1110000111111111   */
     0xE1FF,	  /*  1110000111111111   */
     0xE1FF,      /*  1110000111111111   */
     0xE1FF,	  /*  1110000111111111   */
     0xE000,      /*  1110000000000000   */
     0xE000,	  /*  1110000000000000   */
     0xE000,      /*  1110000000000000   */
     0x0000,      /*  0000000000000000   */
     0x0000,      /*  0000000000000000   */
     0x0000,      /*  0000000000000000   */
     0x0000,      /*  0000000000000000   */
     0x0000,      /*  0000000000000000   */
     0x0000,      /*  0000000000000000   */
     0x0000,      /*  0000000000000000   */
     0x0000,      /*  0000000000000000   */

   /*  Cursor Mask  */
     0x1E00,      /*  0001111000000000   */
     0x1200,      /*  0001001000000000   */
     0x1200,  	  /*  0001001000000000   */
     0x1200,      /*  0001001000000000   */
     0x1200,      /*  0001001000000000   */
     0x13FF,      /*  0001001111111111   */
     0x1249,      /*  0001001001001001   */
     0x1249,      /*  0001001001001001   */
     0xF249,      /*  1111001001001001   */
     0x9001,      /*  1001000000000001   */
     0x9001,	  /*  1001000000000001   */
     0x9001,      /*  1001000000000001   */
     0x8001,      /*  1000000000000001   */
     0x8001,      /*  1000000000000001   */
     0x8001,      /*  1000000000000001   */
     0xFFFF       /*  1111111111111111   */
   },

   {
   /*  Cursor[1] -----> Arrow  */
   /*  Screen Mask  */
     0x3FFF,      /*  0011111111111111   */
     0x1FFF,	  /*  0001111111111111   */
     0x0FFF,      /*  0000111111111111   */
     0x07FF,      /*  0000011111111111   */
     0x03FF,      /*  0000001111111111   */
     0x01FF,      /*  0000000111111111   */
     0x00FF,	  /*  0000000011111111   */
     0x007F,      /*  0000000001111111   */
     0x003F,      /*  0000000000111111   */
     0x001F,	  /*  0000000000011111   */
     0x01FF,      /*  0000000111111111   */
     0x10FF,      /*  0001000011111111   */
     0x30FF,      /*  0011000011111111   */
     0xF87F,      /*  1111100001111111   */
     0xF87F,      /*  1111100001111111   */
     0xFC3F,      /*  1111110000111111   */

   /*  Cursor Mask  */
     0x0000, 	  /*  0000000000000000   */
     0x4000, 	  /*  0100000000000000   */
     0x6000,      /*  0110000000000000   */
     0x7000,      /*  0111000000000000   */
     0x7800,      /*  0111100000000000   */
     0x7C00,      /*  0111110000000000   */
     0x7E00,	  /*  0111111000000000   */
     0x7F00,      /*  0111111100000000   */
     0x7F80,      /*  0111111110000000   */
     0x7FC0,      /*  0111111111000000   */
     0x6C00,      /*  0110110000000000   */
     0x4600,      /*  0100011000000000   */
     0x0600,      /*  0000011000000000   */
     0x0300,      /*  0000001100000000   */
     0x0300,      /*  0000001100000000   */
     0x0180       /*  0000000110000000   */
   },

   {
   /*  Cursor[2] -----> Inverse Arrow  */
   /*  Screen Mask  */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,	  /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,	  /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,	  /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,	  /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */

   /*  Cursor Mask  */
     0x0000, 	  /*  0000000000000000   */
     0x4000, 	  /*  0100000000000000   */
     0x6000,      /*  0110000000000000   */
     0x7000,      /*  0111000000000000   */
     0x7800,      /*  0111100000000000   */
     0x7C00,      /*  0111110000000000   */
     0x7E00,	  /*  0111111000000000   */
     0x7F00,      /*  0111111100000000   */
     0x7F80,      /*  0111111110000000   */
     0x7FC0,      /*  0111111111000000   */
     0x6C00,      /*  0110110000000000   */
     0x4600,      /*  0100011000000000   */
     0x0600,      /*  0000011000000000   */
     0x0300,      /*  0000001100000000   */
     0x0300,      /*  0000001100000000   */
     0x0180       /*  0000000110000000   */
   },

   {
   /*  Cursor[3] -----> Cross  */
   /*  Screen Mask  */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,	  /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,	  /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,	  /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,	  /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */

   /*  Cursor Mask  */
     0x0200, 	  /*  0000001000000000   */
     0x0200, 	  /*  0000001000000000   */
     0x0200,      /*  0000001000000000   */
     0x0200,      /*  0000001000000000   */
     0x0200,      /*  0000001000000000   */
     0x0000,      /*  0000000000000000   */
     0xF8F8,	  /*  1111100011111000   */
     0x0000,      /*  0000000000000000   */
     0x0200,      /*  0000001000000000   */
     0x0200,      /*  0000001000000000   */
     0x0200,      /*  0000001000000000   */
     0x0200,      /*  0000001000000000   */
     0x0200,      /*  0000001000000000   */
     0x0000,      /*  0000000000000000   */
     0x0000,      /*  0000000000000000   */
     0x0000       /*  0000000000000000   */
   },

   {
   /*  Cursor[4] -----> Pencil */
   /*  Screen Mask  */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,	  /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,	  /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,	  /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,	  /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */

   /*  Cursor Mask  */
     0x0000, 	  /*  0000000000000000   */
     0x0038, 	  /*  0000000000111000   */
     0x0044,      /*  0000000001000100   */
     0x00A2,      /*  0000000010100010   */
     0x0132,      /*  0000000100110010   */
     0x025A,      /*  0000001001011010   */
     0x04A4,	  /*  0000010010100100   */
     0x0948,      /*  0000100101001000   */
     0x1A90,      /*  0001101010010000   */
     0x1D20,      /*  0001110100100000   */
     0x2640,      /*  0010010101000000   */
     0x2380,      /*  0010001110000000   */
     0x2300,      /*  0010001100000000   */
     0x7C00,      /*  0111110000000000   */
     0x6000,      /*  0110000000000000   */
     0x0000       /*  0000000000000000   */
   },

   {
   /*  Cursor[5] -----> FILL   */
   /*  Screen Mask  */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,	  /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,	  /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,	  /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,	  /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */

   /*  Cursor Mask  */
     0x0080, 	  /*  0000000010000000   */
     0x0180, 	  /*  0000000110000000   */
     0x0280,      /*  0000001010000000   */
     0x0440,      /*  0000010001000000   */
     0x0820,      /*  0000100000100000   */
     0x1010,      /*  0001000000010000   */
     0x2008,	  /*  0010000000001000   */
     0x4004,      /*  0100000000000100   */
     0xFFFE,      /*  1111111111111110   */
     0x9552,      /*  1001010101010010   */
     0x8AAC,      /*  1000101010101100   */
     0x8558,      /*  1000010101011000   */
     0x82B0,      /*  1000001010110000   */
     0x8160,      /*  1000000101100000   */
     0x80C0,      /*  1000000011000000   */
     0x8000       /*  1000000000000000   */
     },

    /* Cursor[6] ----> Sand Counter */
    /* Screen Mask */
    {
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,	  /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,	  /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,	  /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,	  /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */
     0xFFFF,      /*  1111111111111111   */

     /* Cursor Mask */
     0xFFFF,      /*  1111111111111111   */
     0x2004,      /*  0010000000000100   */
     0x2004,      /*  0010000000000100   */
     0x2AAC,      /*  0010101010101100   */
     0x3554,      /*  0011010101010100   */
     0x1AA8,      /*  0001101010101000   */
     0x0D50,      /*  0000110101010000   */
     0x06A0,      /*  0000011010100000   */
     0x0890,      /*  0000100010010000   */
     0x1088,      /*  0001000010001000   */
     0x2084,      /*  0010000010000100   */
     0x2084,      /*  0010000010000100   */
     0x21C4,      /*  0010000111000100   */
     0x3FFC,      /*  0011111111111100   */
     0xFFFF,      /*  1111111111111111   */
     0x0000       /*  0000000000000000   */
   }
 };

#endif
