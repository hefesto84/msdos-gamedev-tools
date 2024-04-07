/*
	popups.c
	a multi level pop up window module

*/


#include <stdio.h>
#include <conio.h>


#define VERSION 101

#define FALSE 0
#define TRUE 1

#define SPACE 0x20
#define POPERR -1

#define DEFAULT_WAIT 0x0019
#define DEFAULT_TONE 0x0100
#define DEFAULT_STEP 0x0100

#define NO_SHADOW 0
#define TOP_LEFT_SHADOW 1
#define TOP_RIGHT_SHADOW 2
#define BOTTOM_LEFT_SHADOW 3
#define BOTTOM_RIGHT_SHADOW 4
#define TOP_LEFT_WIDE 6
#define TOP_RIGHT_WIDE 7
#define BOTTOM_LEFT_WIDE 8
#define BOTTOM_RIGHT_WIDE 9



#define NO_BORDER 0
#define SINGLE_BORDER 1
#define DOUBLE_BORDER 2
#define THIN_BORDER 3
#define THICK_BORDER 4

#define OLD_SCREEN 4000    /* 4000 for 80 column    2000 for 40 column */
#define FULL_SCREEN 4000   /* 4000 for 80 column    2000 for 40 column */
#define MAX_WINDOW 25      /* max windows even if buffer not full */
#define MAX_SCREENS 5      /* max number of FULL_SCREENS to buffer (1 - 8) */
#define BUF_SIZE FULL_SCREEN * MAX_SCREENS
#define PARAMS 9

static char scrbuf[BUF_SIZE];  /* storage buffer for previous window */
static char *scrbufptr;         /* pointer to last window in buffer */
static char oldscr[OLD_SCREEN];
static char explode_w,shrink_w,shadow_w,noise_w,shadow_edge;
static unsigned char init[1] = { 0 };
static unsigned char bord[8] = { NO_BORDER,' ',' ',' ',' ',' ',' ',' ' };
static int param[PARAMS*(MAX_WINDOW+1)];
static unsigned int explode_wait,shrink_wait;
static int popup_mode,pop_id,shadow_attr;
static int rn,rw;
static unsigned int tone,tone_step;
static struct text_info initial_info;

/*************************************************************************/

init_popup(mode,left,top,right,bottom,foreground,background,border,bordertype)
int mode,left,top,right,bottom,foreground,background,border,bordertype;
{
	int pptr;

		if (init[0] != 0)
        	return POPERR;

		init[0] = 1;

		shadow_w = NO_SHADOW;
		shadow_attr = BLACK;
		noise_w = explode_w = shrink_w = FALSE;
        explode_wait = shrink_wait = DEFAULT_WAIT;
		tone = DEFAULT_TONE;
		tone_step = DEFAULT_STEP;

		gettextinfo(&initial_info);

		if (initial_info.currmode < 2)
    		gettext(1,1,40,25,oldscr);
		else
			gettext(1,1,80,25,oldscr);

		if (mode < 2) {
			rn = 40;
			rw = 38;
		}
		else {
			rn = 80;
			rw = 78;
		}
		popup_mode = mode;
		textmode(mode);
 		scrbufptr = &scrbuf[0];
		pop_id = 0;

		param[0] = left;
    	param[1] = top;
    	param[2] = right;
    	param[3] = bottom;
    	param[4] = foreground;
    	param[5] = background;
    	param[6] = border;
		param[7] = bordertype;
		param[8] = NO_SHADOW;

		delay(explode_wait);

    	build_window();
		return pop_id;

}

/*************************************************************************/

uninit_popup()
{

	if (pop_id)
        return POPERR;
	abandon_popup();

}

/*************************************************************************/

abandon_popup()
{

	if (!init[0])
        return POPERR;


	textmode(initial_info.currmode);

	if (initial_info.currmode < 2)
    	puttext(1,1,40,25,oldscr);
	else
		puttext(1,1,80,25,oldscr);

	window(initial_info.winleft,initial_info.wintop,initial_info.winright,initial_info.winbottom);
	textattr(initial_info.attribute);
	gotoxy(initial_info.curx,initial_info.cury);

}

/****************************************************************************/

version_pop()
{
	return VERSION;
}

/****************************************************************************/

shrink(tog)
char tog;
{
	shrink_w = tog;
}

/****************************************************************************/

explode(tog)
char tog;
{
	explode_w = tog;
}

/****************************************************************************/

shadow(shade)
char shade;
{
	shadow_w = shade;
}

/****************************************************************************/

shadow_color(sattr)
int sattr;
{
	shadow_attr = sattr;
}



/****************************************************************************/

explode_delay(del)
unsigned int del;
{
	if (!del)
		return explode_wait;
	else
		explode_wait = del;
}

/****************************************************************************/

shrink_delay(del)
unsigned int del;
{
	if (!del)
		return shrink_wait;
	else
		shrink_wait = del;
}

/****************************************************************************/

noise(tog)
char tog;
{
	noise_w = tog;
}

/****************************************************************************/

noise_tone(stone)
unsigned int stone;
{
	if (!stone)
		return tone;
	else
		tone = stone;
}

/****************************************************************************/

noise_step(stone)
unsigned int stone;
{
	if (!stone)
		return tone_step;
	else
		tone_step = stone;
}

/*************************************************************************/

id_popup()
{
	return pop_id;
}

/*************************************************************************/

pop_max()
{
	return MAX_WINDOW;
}

/*************************************************************************/

pop_left()
{
	return (MAX_WINDOW - pop_id);
}

/*************************************************************************/

buf_max()
{
	return BUF_SIZE;
}

/*************************************************************************/

buf_left()
{
	return (BUF_SIZE-(scrbufptr - &scrbuf[0]));
}

/*************************************************************************/

high_popup()
{
	int pptr,high;

	pptr = (pop_id*PARAMS);
	high = ((param[pptr+3] - param[pptr+1]) +1);
	if (param[pptr+7] != NO_BORDER)
		high -= 2;
    return high;
}

/*************************************************************************/

wide_popup()
{
	int pptr,wide;

	pptr = (pop_id*PARAMS);
	wide = ((param[pptr+2] - param[pptr]) +1);
	if (param[pptr+7] == THICK_BORDER)
		wide -= 4;
    else if (param[pptr+7] != NO_BORDER)
		wide -= 2;

    return wide;
}

/*************************************************************************/

next_popup(left,top,right,bottom,foreground,background,border,bordertype)
int left,top,right,bottom,foreground,background,border,bordertype;
{
	int pptr,offx,offy,ox,oy;

	if (shadow_w) {
		shadow_edge = FALSE;

		if (shadow_w > BOTTOM_RIGHT_SHADOW) {
        	if ((left < 3) || (right > rw))
				shadow_edge = TRUE;
		}

		else {
        	if ((left == 1) || (right == rn))
				shadow_edge = TRUE;
		}

        if ((top == 1) || (bottom == 25))
			shadow_edge = TRUE;

   	}

	if (shadow_w && !shadow_edge) {
        if (shadow_w > BOTTOM_RIGHT_SHADOW) {
    		offx = 5;
			offy = 3;
			ox = 2;
			oy = 1;
		}
        else {
  			offx = offy = 3;
			ox = oy = 1;
		}
	}
    else {
		offx = offy = 1;
		ox = oy = 0;
	}

	if ((scrbufptr+(2*(((right-left)+offx)*((bottom-top)+offy)))) >= &scrbuf[BUF_SIZE])
		return POPERR;

    if (pop_id == MAX_WINDOW)
		return POPERR;

	++pop_id;

	pptr = (pop_id*PARAMS);
	param[pptr] = left;
    param[pptr+1] = top;
    param[pptr+2] = right;
    param[pptr+3] = bottom;
    param[pptr+4] = foreground;
    param[pptr+5] = background;
    param[pptr+6] = border;
	param[pptr+7] = bordertype;
	if (shadow_edge)
      	param[pptr+8] = NO_SHADOW;
	else
		param[pptr+8] = shadow_w;

	gettext((left-ox),(top-oy),(right+ox),(bottom+oy),scrbufptr);
	scrbufptr = (scrbufptr+(2*(((right-left)+offx)*((bottom-top)+offy))));
    build_window();
    return pop_id;

}

/*************************************************************************/

previous_popup()
{
	int s_top,s_bottom,s_left,s_right,s_wide,s_high,xadd,x,y,sx,sy,offs;
	char buf[FULL_SCREEN];
	int pptr;
	int wattribute;
	int	bufptr,lastptr;
	unsigned int beep;
	char v_shrunk,h_shrunk;

	if (!pop_id)
		return POPERR;

    window(1,1,rn,25);

	pptr = (pop_id*PARAMS);
	if (param[pptr+8])  {
        if (param[pptr+8] > BOTTOM_RIGHT_SHADOW) {
			sx = 2;
			sy = 1;
		}
        else
			sx = sy = 1;

	}
	else
		sx = sy = 0;

	offs = (2*(((param[pptr+2]-param[pptr])+((sx*2)+1))*((param[pptr+3]-param[pptr+1])+((sy*2)+1))));
    scrbufptr = (scrbufptr-offs);

	if (shrink_w) {
		wattribute = (param[pptr+6] + (param[pptr+5] * 0x10));
        bufptr = 1;
		while (bufptr < offs) {
			buf[bufptr] = wattribute;
			bufptr += 2;
		}

		x = (param[pptr+3] - param[pptr+1]);
		y = (param[pptr+2] - param[pptr]);
		if ((y < 12) || (x < 8))
			goto tosm;

		xadd = 0;
		if (y <= x)
			y = (x+1);
		while (y > x) {
			y = (y-x);
			++xadd;
		}
		if (y >= (x/2))
			++xadd;

    	s_top = (param[pptr+1] +1);
		s_bottom = (param[pptr+3] -1);

    	s_left = (param[pptr] + xadd);
		s_right = (param[pptr+2] - xadd);

    	s_wide = ((s_right - s_left) * 2);
		s_high = ((s_wide + 2) * (s_bottom - s_top));

		h_shrunk = v_shrunk = FALSE;

		if (noise_w) {
			beep = tone;
			sound(beep);
        }

		if (bord[0] != param[pptr+7]) {
			check_border(pptr);
		}

		while (TRUE) {
			buf[0] = bord[4];
    		bufptr = 2;
			while (bufptr < s_wide) {
				buf[bufptr] = bord[1];
				bufptr += 2;
			}
			buf[bufptr] = bord[5];
			bufptr += 2;

            if (param[pptr+7] == THICK_BORDER) {
				while (bufptr < s_high) {
					buf[bufptr] = buf[bufptr+2] = bord[3];
		    		bufptr += 4;
					lastptr = (bufptr-6);
					while (bufptr < (lastptr+s_wide)) {
    					buf[bufptr] = SPACE;
						bufptr += 2;
					}
        			buf[bufptr] = buf[bufptr+2] = bord[3];
					bufptr += 4;
        		}
			}
			else {
                while (bufptr < s_high) {
					buf[bufptr] = bord[3];
					bufptr += 2;
					lastptr = (bufptr-2);
		        	while (bufptr < (lastptr+s_wide)) {
    					buf[bufptr] = SPACE;
						bufptr += 2;
					}
        			buf[bufptr] = bord[3];
					bufptr += 2;
        		}
			}


			buf[bufptr] = bord[6];
    		bufptr += 2;
			while (bufptr < (s_wide+s_high)) {
				buf[bufptr] = bord[2];
				bufptr += 2;
			}
			buf[bufptr] = bord[7];

			puttext((param[pptr]-sx),(param[pptr+1]-sy),(param[pptr+2]+sx),(param[pptr+3]+sy),scrbufptr);

    		puttext(s_left,s_top,s_right,s_bottom,buf);

			if ((v_shrunk) && (h_shrunk))
				break;

			if (!v_shrunk) {
    			++s_top;
				--s_bottom;
            	if ((s_top + 5) >= s_bottom)
					v_shrunk = TRUE;
			}
			if (!h_shrunk) {
				s_left += xadd;
				s_right -= xadd;
            	if ((s_left + 9) >= s_right)
					h_shrunk = TRUE;
			}

			s_wide = ((s_right - s_left) * 2);
			s_high = ((s_wide + 2) * (s_bottom - s_top));

			if (noise_w) {
    			beep += tone_step;
				sound(beep);
			}

			delay(shrink_wait);

			if (noise_w) {
    			beep += tone_step;
				sound(beep);
			}


		}


	}   /* end of if shrink_w */

tosm:

	if (noise_w && !shrink_w)  {
		sound(tone);
		delay(shrink_wait);
        delay(shrink_wait);

	}

	puttext((param[pptr]-sx),(param[pptr+1]-sy),(param[pptr+2]+sx),(param[pptr+3]+sy),scrbufptr);


	--pop_id;
	pptr = (pop_id*PARAMS);

	if (!(param[pptr+7]))
    	window(param[pptr],param[pptr+1],param[pptr+2],param[pptr+3]);
	else if (param[pptr+7] == THICK_BORDER)
    	window((param[pptr]+2),(param[pptr+1]+1),(param[pptr+2]-2),(param[pptr+3]-1));
    else
		window((param[pptr]+1),(param[pptr+1]+1),(param[pptr+2]-1),(param[pptr+3]-1));
    textcolor(param[pptr+4]);
	textbackground(param[pptr+5]);

	if (noise_w)
		nosound();

	return pop_id;

}

/*************************************************************************/

build_window()
{

	int s_top,s_bottom,s_left,s_right,s_wide,s_high,x,y,xadd;
	char buf[FULL_SCREEN];
	char ntop[160];
	int pptr;
	int wattribute;
	int wide,high;
	int	bufptr,lastptr;
	unsigned int beep;

    window(1,1,rn,25);

	pptr = (pop_id*PARAMS);
	wide = ((param[pptr+2] - param[pptr]) * 2);
	high = ((wide + 2) * (param[pptr+3] - param[pptr+1]));
	wattribute = (param[pptr+6] + (param[pptr+5] * 0x10));

	if (bord[0] != param[pptr+7]) {
		check_border(pptr);
	}

	bufptr = 1;
	while (bufptr < FULL_SCREEN) {
		buf[bufptr] = wattribute;
		bufptr += 2;
	}


  	if (!explode_w) {
    	if (noise_w)
			sound(tone);
		goto tosmall;
  	}

	x = (param[pptr+3] - param[pptr+1]);
	y = (param[pptr+2] - param[pptr]);
	if ((y < 12) || (x < 8))
		goto tosmall;

    s_top = (param[pptr+1] + ((x/2)-1));
	s_bottom = (param[pptr+3] - ((x/2)-1));

    s_left = (param[pptr] + ((y/2)-4));
	s_right = (param[pptr+2] - ((y/2)-4));

    s_wide = ((s_right - s_left) * 2);
	s_high = ((s_wide + 2) * (s_bottom - s_top));

	xadd = ((y / x)+1);

	beep = tone;

	while (TRUE) {
		buf[0] = bord[4];
    	bufptr = 2;
		while (bufptr < s_wide) {
			buf[bufptr] = bord[1];
			bufptr += 2;
		}
		buf[bufptr] = bord[5];
		bufptr += 2;
		while (bufptr < s_high) {
     		if (param[pptr+7] == THICK_BORDER) {
				buf[bufptr] = buf[bufptr+2] = bord[3];
		    	bufptr += 4;
				lastptr = (bufptr-6);
				while (bufptr < (lastptr+s_wide)) {
    				buf[bufptr] = SPACE;
					bufptr += 2;
				}
        		buf[bufptr] = buf[bufptr+2] = bord[3];
				bufptr += 4;
        	}
			else {
				buf[bufptr] = bord[3];
				bufptr += 2;
				lastptr = (bufptr-2);
				while (bufptr < (lastptr+s_wide)) {
    				buf[bufptr] = SPACE;
					bufptr += 2;
				}
        		buf[bufptr] = bord[3];
				bufptr += 2;
        	}
		}
		buf[bufptr] = bord[6];
    	bufptr += 2;
		while (bufptr < (s_wide+s_high)) {
			buf[bufptr] = bord[2];
			bufptr += 2;
		}
		buf[bufptr] = bord[7];
    	puttext(s_left,s_top,s_right,s_bottom,buf);

		--s_top;
		++s_bottom;
		s_left -= xadd;
		s_right += xadd;

    	if (s_top <= param[pptr+1])
			s_top = param[pptr+1];
    	if (s_bottom >= param[pptr+3])
			s_bottom = param[pptr+3];
		if (s_left <= param[pptr])
			s_left = param[pptr];
		if (s_right >= param[pptr+2])
			s_right = param[pptr+2];

		s_wide = ((s_right - s_left) * 2);
		s_high = ((s_wide + 2) * (s_bottom - s_top));

		if (noise_w) {
    		beep += tone_step;
			sound(beep);
		}

		delay(explode_wait);

		if (noise_w) {
    		beep += tone_step;
			sound(beep);
		}

		if ((s_top == param[pptr+1]) && (s_bottom == param[pptr+3]) && (s_left == param[pptr]) && (s_right == param[pptr+2]))
			break;

	}   /* end of while (TRUE) */


tosmall:

	buf[0] = bord[4];
    bufptr = 2;
	while (bufptr < wide) {
		buf[bufptr] = bord[1];
		bufptr += 2;
	}
	buf[bufptr] = bord[5];
	bufptr += 2;
	while (bufptr < high) {
     	if (param[pptr+7] == THICK_BORDER) {
			buf[bufptr] = buf[bufptr+2] = bord[3];
		    bufptr += 4;
			lastptr = (bufptr-6);
			while (bufptr < (lastptr+wide)) {
    			buf[bufptr] = SPACE;
				bufptr += 2;
			}
        	buf[bufptr] = buf[bufptr+2] = bord[3];
			bufptr += 4;
        }
		else {
			buf[bufptr] = bord[3];
			bufptr += 2;
			lastptr = (bufptr-2);
			while (bufptr < (lastptr+wide)) {
    			buf[bufptr] = SPACE;
				bufptr += 2;
			}
        	buf[bufptr] = bord[3];
			bufptr += 2;
        }
	}
	buf[bufptr] = bord[6];
    bufptr += 2;
	while (bufptr < (wide+high)) {
		buf[bufptr] = bord[2];
		bufptr += 2;
	}
	buf[bufptr] = bord[7];
    puttext(param[pptr],param[pptr+1],param[pptr+2],param[pptr+3],buf);

	if (param[pptr+8]) {
		bufptr = 0;
		while (bufptr < 160) {
			buf[bufptr] = 'Û';
        	++bufptr;
			buf[bufptr] = shadow_attr;
			++bufptr;
		}

     	switch(param[pptr+8]) {
			case TOP_LEFT_WIDE:
    			puttext((param[pptr]-2),(param[pptr+1]-1),(param[pptr+2]-2),(param[pptr+1]-1),buf);
            	puttext((param[pptr]-2),(param[pptr+1]-1),(param[pptr]-1),(param[pptr+3]-1),buf);
                break;
			case TOP_RIGHT_WIDE:
                puttext((param[pptr]+2),(param[pptr+1]-1),(param[pptr+2]+2),(param[pptr+1]-1),buf);
            	puttext((param[pptr+2]+1),(param[pptr+1]-1),(param[pptr+2]+2),(param[pptr+3]-1),buf);
                break;
			case BOTTOM_LEFT_WIDE:
                puttext((param[pptr]-2),(param[pptr+3]+1),(param[pptr+2]-2),(param[pptr+3]+1),buf);
            	puttext((param[pptr]-2),(param[pptr+1]+1),(param[pptr]-1),(param[pptr+3]+1),buf);
                break;
			case BOTTOM_RIGHT_WIDE:
                puttext((param[pptr]+2),(param[pptr+3]+1),(param[pptr+2]+2),(param[pptr+3]+1),buf);
            	puttext((param[pptr+2]+1),(param[pptr+1]+1),(param[pptr+2]+2),(param[pptr+3]+1),buf);
                break;
         	case TOP_LEFT_SHADOW:
                gettext((param[pptr]-1),(param[pptr+1]-1),(param[pptr+2]-1),(param[pptr+1]-1),ntop);
        		bufptr = 0;
				while (bufptr < 160) {
					ntop[bufptr] = 'Ü';
        			++bufptr;
					ntop[bufptr] = ((ntop[bufptr] & 0x70) | shadow_attr);
					++bufptr;
				}
    			puttext((param[pptr]-1),(param[pptr+1]-1),(param[pptr+2]-1),(param[pptr+1]-1),ntop);
            	puttext((param[pptr]-1),(param[pptr+1]),(param[pptr]-1),(param[pptr+3]-1),buf);
				gettext((param[pptr]-1),(param[pptr+3]),(param[pptr]-1),(param[pptr+3]),ntop);
                ntop[0] = 'ß';
				ntop[1] = ((ntop[1] & 0x70) | shadow_attr);
                puttext((param[pptr]-1),(param[pptr+3]),(param[pptr]-1),(param[pptr+3]),ntop);
                break;
			case TOP_RIGHT_SHADOW:
                gettext((param[pptr]+1),(param[pptr+1]-1),(param[pptr+2]+1),(param[pptr+1]-1),ntop);
                bufptr = 0;
				while (bufptr < 160) {
					ntop[bufptr] = 'Ü';
        			++bufptr;
					ntop[bufptr] = ((ntop[bufptr] & 0x70) | shadow_attr);
					++bufptr;
				}
                puttext((param[pptr]+1),(param[pptr+1]-1),(param[pptr+2]+1),(param[pptr+1]-1),ntop);
            	puttext((param[pptr+2]+1),(param[pptr+1]),(param[pptr+2]+1),(param[pptr+3]-1),buf);
				gettext((param[pptr+2]+1),(param[pptr+3]),(param[pptr+2]+1),(param[pptr+3]),ntop);
                ntop[0] = 'ß';
				ntop[1] = ((ntop[1] & 0x70) | shadow_attr);
                puttext((param[pptr+2]+1),(param[pptr+3]),(param[pptr+2]+1),(param[pptr+3]),ntop);
                break;
			case BOTTOM_LEFT_SHADOW:
				gettext((param[pptr]-1),(param[pptr+3]+1),(param[pptr+2]-1),(param[pptr+3]+1),ntop);
                bufptr = 0;
				while (bufptr < 160) {
					ntop[bufptr] = 'ß';
        			++bufptr;
					ntop[bufptr] = ((ntop[bufptr] & 0x70) | shadow_attr);
					++bufptr;
				}
                puttext((param[pptr]-1),(param[pptr+3]+1),(param[pptr+2]-1),(param[pptr+3]+1),ntop);
            	puttext((param[pptr]-1),(param[pptr+1]+1),(param[pptr]-1),(param[pptr+3]),buf);
				gettext((param[pptr]-1),(param[pptr+1]),(param[pptr]-1),(param[pptr+1]),ntop);
                ntop[0] = 'Ü';
				ntop[1] = ((ntop[1] & 0x70) | shadow_attr);
				puttext((param[pptr]-1),(param[pptr+1]),(param[pptr]-1),(param[pptr+1]),ntop);
                break;
			case BOTTOM_RIGHT_SHADOW:
				gettext((param[pptr]+1),(param[pptr+3]+1),(param[pptr+2]+1),(param[pptr+3]+1),ntop);
                bufptr = 0;
				while (bufptr < 160) {
					ntop[bufptr] = 'ß';
        			++bufptr;
					ntop[bufptr] = ((ntop[bufptr] & 0x70) | shadow_attr);
					++bufptr;
				}
                puttext((param[pptr]+1),(param[pptr+3]+1),(param[pptr+2]+1),(param[pptr+3]+1),ntop);
				puttext((param[pptr+2]+1),(param[pptr+1]+1),(param[pptr+2]+1),(param[pptr+3]),buf);
                gettext((param[pptr+2]+1),(param[pptr+1]),(param[pptr+2]+1),(param[pptr+1]),ntop);
                ntop[0] = 'Ü';
				ntop[1] = ((ntop[1] & 0x70) | shadow_attr);
            	puttext((param[pptr+2]+1),(param[pptr+1]),(param[pptr+2]+1),(param[pptr+1]),ntop);
                break;
		}

	}


    if (!(param[pptr+7]))
    	window(param[pptr],param[pptr+1],param[pptr+2],param[pptr+3]);
	else if (param[pptr+7] == THICK_BORDER)
    	window((param[pptr]+2),(param[pptr+1]+1),(param[pptr+2]-2),(param[pptr+3]-1));
    else
		window((param[pptr]+1),(param[pptr+1]+1),(param[pptr+2]-1),(param[pptr+3]-1));

    textcolor(param[pptr+4]);
	textbackground(param[pptr+5]);

	if (noise_w)
		nosound();

}

/*************************************************************************/

check_border(bptr)
int bptr;
{

		bord[0] = param[bptr+7];
		switch(param[bptr+7]) {
			case NO_BORDER:
				bord[1] = bord[2] = bord[3] = bord[4] = bord[5] = bord[6] = bord[7] = ' ';
				break;
			case SINGLE_BORDER:
				bord[1] = bord[2] = 'Ä';
				bord[3] = '³';
				bord[4] = 'Ú';
				bord[5] = '¿';
				bord[6] = 'À';
				bord[7] = 'Ù';
				break;
    		case DOUBLE_BORDER:
				bord[1] = bord[2] = 'Í';
				bord[3] = 'º';
				bord[4] = 'É';
				bord[5] = '»';
				bord[6] = 'È';
				bord[7] = '¼';
				break;
    		case THIN_BORDER:
				bord[1] = 'ß';
				bord[2] = 'Ü';
				bord[3] = bord[4] = bord[5] = bord[6] = bord[7] = 'Û';
				break;
        	case THICK_BORDER:
				bord[1] = bord[2] = bord[3] = bord[4] = bord[5] = bord[6] = bord[7] = 'Û';
				break;
    	}

}
