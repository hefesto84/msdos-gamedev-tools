/* ----------------------- editor.c ---------------------- */

#include <stdio.h>
#include <ctype.h>
#include <mem.h>
#include <conio.h>
#include <alloc.h>
#include "twindow.h"
#include "keys.h"

#define TRUE 1
#define FALSE 0
#define TAB 4
#define NEXTTAB (TAB-(x%TAB))
#define LASTTAB ((wwd/TAB)*TAB)
#define PREVTAB (((x-1)%TAB)+1)
#define curr(x,y) (bfptr+(y)*wwd+(x))
#define lineno(y) ((int)(bfptr-topptr)/wwd+(y))

extern int VSG;
int last_x, last_y;
static int wht;
static int wwd;
static int wsz;
static char *topptr;
static char *bfptr;
static char *lstptr;
static int lines;
static char *endptr;
static int blkbeg;
static int blkend;
static int inserting;
static WINDOW *wnd;
static int do_display_text = 1;
/*page*/
/* ---------- local function prototypes ----------- */
void erase_buffer(int *x, int *y);
int lastword(int x, int y);
void last_char(int *x, int *y);
void test_para(int x, int y);
int trailing_spaces(int y);
int first_wordlen(int y);
void paraform(int x, int y);
int blankline(int line);
void delete_word(int x, int y);
void delete_line(int y);
void delete_block(void);
void copy_block(int y);
void move_block(int y);
void mvblock(int y, int moving);
void findlast(void);
void find_end(int *x, int *y);
void carrtn(int *x, int *y, int insert);
void backspace(int *x, int *y);
void fore_word(int *x, int *y, char *bf);
int spaceup(int *x, int *y, char **bf);
void back_word(int *x, int *y, char *bf);
int spacedn(int *x, int *y, char **bf);
void forward(int *x, int *y);
int downward(int *y);
void upward(int *y);
void display_text(void);
void disp_line(int y);
void insert_line(void);
/*page*/
/* ----- Process text entry for a window. ---- */
void text_editor(WINDOW *wnd1, char *bf, int bsize)
{
	char *b, *buff;
	int depart = FALSE, i, c;
	int x, y, svx, svlw, tx, tabctr = 0;

	wnd = wnd1;
	wht = HEIGHT-2;
	wwd = WIDTH-2;
	wsz = wwd * wht;
	topptr = bfptr = bf;
	lines = bsize / wwd;
	endptr = bf + wwd * lines;
	blkbeg = 0;
	blkend = 0;
	inserting = FALSE;
	x = 0;
	y = 0;
	display_text();
	/* --------- read in text from the keyboard ---------- */
	findlast();
	while (TRUE)	{
		last_x = COL + 1 + x;
		last_y = ROW + 1 + y;
		cursor(last_x, last_y);
		buff = curr(x, y);
		if (tabctr)	{
			--tabctr;
			c = ' ';
		}
		else	{
			c = get_char();
			clear_message();
		}
		switch (c)	{
			case '\r':	carrtn(&x, &y, inserting);
						break;
			case DN:	downward(&y);
						break;
			case PGUP:	y = 0;
						for (i = 0; i < wht; i++)
							upward(&y);
						break;
			case PGDN:	y = HEIGHT - 2;
						for (i = 0; i < wht; i++)
							downward(&y);
						y = 0;
						break;
			case '\t':	if (x + NEXTTAB < wwd)	{
							if (inserting)
								tabctr = NEXTTAB;
							else
								x += NEXTTAB;
						}
						else
							carrtn(&x, &y, inserting);
						break;
			case SHIFT_HT:
						if (x < TAB)	{
							upward(&y);
							x = LASTTAB;
						}
						else
							x -= PREVTAB;
						break;
			case CTRL_FWD:
						fore_word(&x, &y, buff);
						break;
			case CTRL_BS:
						back_word(&x, &y, buff);
						break;
			case CTRL_B:
						y = wht - 1;
						break;
			case CTRL_T:
						y = 0;
						break;
			case CTRL_HOME:
						x = y = 0;
						bfptr = topptr;
						display_text();
						break;
			case HOME:	x = 0;
						break;
			case CTRL_END:
						find_end(&x, &y);
						display_text();
						break;
			case END:	last_char(&x, &y);
						break;
			case UP:	upward(&y);
						break;
			case F2:
			case ESC:	depart = TRUE;
						break;
			case '\b':				
			case BS:	if (!(x || y))
							break;
						backspace(&x, &y);
						if (x == wwd - 1)
							last_char(&x, &y);
						if (c == BS)
							break;
						buff = curr(x, y);
			case DEL:	movmem(buff+1, buff, wwd-1-x);
						*(buff+wwd-1-x) = ' ';
						disp_line(y);
						test_para(x+1, y);
						break;
			case ALT_D:	delete_line(y);
						break;
			case CTRL_D:delete_word(x, y);
						test_para(x, y);
						break;
			case INS:	inserting ^= TRUE;
						insert_line();
						break;
			case F3:	erase_buffer(&x, &y);
						break;
			case F4:	paraform(0, y);
						break;
			case F5:	blkbeg = lineno(y) + 1;
						if (blkbeg > blkend)
							blkend = lines;
						display_text();
						break;
			case F6:	blkend = lineno(y) + 1;
						if (blkend < blkbeg)
							blkbeg = 1;
						display_text();
						break;
			case F7:	move_block(y);
						break;
			case F8:	copy_block(y);
						break;
			case F9:	delete_block();
						break;
			case F10:	blkbeg = blkend = 0;
						display_text();
						break;
			case FWD:	forward(&x, &y);
						break;
			default:	if (!isprint(c))
							break;
						if (curr(x, y) == endptr-1 ||
						   (lineno(y)+1 >= lines && inserting
								&& *curr(wwd-2, y) != ' '))	{
							error_message(" End of Buffer ");
							break;
						}
						if (inserting)	{
							buff = curr(x, y);
							movmem(buff, buff + 1, wwd-1-x);
						}
						buff = curr(x, y);
						if (buff < endptr)	{
							if (buff >= lstptr)
								lstptr = buff + 1;
							*buff = c;
							disp_line(y);
						}
						buff = curr(wwd-1, y);
						if (endptr && *buff != ' ')	{
							for (b = buff+1; b < endptr; b++)
								if (*b==' ' && *(b + 1)==' ')
									break;
							movmem(buff+1, buff+2, b-buff-1);
							*(buff+1) = ' ';
							svx = x;
							svlw = lastword(x, y);
							x = wwd-1;
							if (*(buff-1) != ' ')
								back_word(&x, &y, buff);
							tx = x;
							carrtn(&x, &y, TRUE);
							if (svlw)
								x = svx-tx;
							else	{
								x = svx;
								--y;
							}
						}
						forward(&x, &y);
						break;
		}
		if (depart)
			break;
	}
	inserting = FALSE;
	insert_line();
}
/*page*/
/* -------- erase the buffer --------------- */
static void erase_buffer(int *x, int *y)
{
	int c = 0;
	WINDOW *sur;

	sur = establish_window(28, 11, 4, 24);
	set_colors(sur, ALL, RED, YELLOW, BRIGHT);
	display_window(sur);
	wprintf(sur, " Erase text window\n Are you sure? (y/n)");
	while (c != 'y' && c != 'n')	{
		c = get_char();
		c = tolower(c);
		if (c == 'y')	{
			lstptr = bfptr = topptr;
			*x = *y = 0;
			setmem(bfptr, lines * wwd, ' ');
			blkbeg = blkend = 0;
			display_text();
		}
	}
	delete_window(sur);
}

/* ----- see if a word is the last word on the line ------ */
static int lastword(int x, int y)
{
	char *bf = curr(x, y);

	while (x++ < wwd-1)
		if (*bf++ == ' ')
			return 0;
	return 1;
}
/*page*/
/* --- go to last displayable character on the line --- */
static void last_char(int *x, int *y)
{
	char *bf;

	*x = wwd-1;
	bf = curr(0, *y);
	while (*x && *(bf + *x) == ' ')
		--(*x);
	if (*x && *x < wwd - 1)
		(*x)++;
}

/* ----- test to see if paragraph should be reformed ----- */
static void test_para(int x, int y)
{
	int ts, fw;

	if (!scroll_lock() && y < lines)	{
		ts = trailing_spaces(y);
		fw = first_wordlen(y+1);
		if (fw && ts > fw)
			paraform(x, y);
	}
}

/* ---- count the trailing spaces on a line ----- */
static int trailing_spaces(int y)
{
	int x = wwd-1, ct = 0;
	char *bf = curr(0, y);

	while (x >= 0)	{
		if (*(bf + x) != ' ')
			break;
		--x;
		ct++;
	}
	return ct;
}
/* ----- count the length of the first word on a line --- */
static int first_wordlen(int y)
{
	int ct = 0, x = 0;
	char *bf = curr(0, y);

	while (x < wwd-1 && *(bf+x) == ' ')
		x++;
	while (x+ct < wwd-1 && *(bf+x+ct) != ' ')
		ct++;
	return ct;
}


/* ------------ form a paragraph -------------- */
static void paraform(int x, int y)
{
	char *cp1, *cp2, *cpend, *svcp;
	int x1;

	if (blankline(lineno(y)+1))
		return;
	if (!blkbeg)	{
		blkbeg = blkend = lineno(y)+1;
		blkend++;
		while (blkend < lines)	{
			if (blankline(blkend))
				break;
			blkend++;
		}
		--blkend;
	}
	if (lineno(y) != blkbeg-1)
		x = 0;
	x1 = x;
	cp1 = cp2 = topptr + (blkbeg - 1) * wwd + x;
	cpend = topptr + blkend * wwd;
	while (cp2 < cpend)	{
		while (*cp2 == ' ' && cp2 < cpend)
			cp2++;
		if (cp2 == cpend)
			break;
		/* at a word */
		while (*cp2 != ' ' && cp2 < cpend)	{
			if (x1 >= wwd - 1)	{
				/* wrap the word */
				svcp = cp1 + (wwd - x1);
				while (*--cp1 != ' ')	{
					*cp1 = ' ';
					--cp2;
				}
				x1 = 0;
				blkbeg++;
				cp1 = svcp;
			}
			*cp1++ = *cp2++;
			x1++;
		}
		if (cp2 < cpend)	{
			*cp1++ = ' ';
			x1++;
		}
	}
	while (cp1 < cpend)
		*cp1++ = ' ';
 	blkbeg++;
 	if (blkbeg <= blkend)
		delete_block();
	blkbeg = blkend = 0;
	display_text();
	findlast();
}
/*page*/
/* ------- test for a blank line ---------- */
static int blankline(int line)
{
	char *cp;
	int x;

	cp = topptr + (line-1) * wwd;
	for (x = 0; x < wwd; x++)
		if (*(cp + x) != ' ')
			break;
	return (x == wwd);
}

/* ------------- delete a word -------------- */
static void delete_word(int x, int y)
{
	int wct = 0;
	char *cp1, *cp2;

	cp1 = cp2 = curr(x, y);
	if (*cp2 == ' ')
		while (*cp2 == ' ' && x + wct < wwd)	{
			wct++;
			cp2++;
		}
	else	{
		while (*cp2 != ' ' && x + wct < wwd)	{
			wct++;
			cp2++;
		}
		while (*cp2 == ' ' && x + wct < wwd)	{
			wct++;
			cp2++;
		}
	}
	movmem(cp2, cp1, wwd - x - wct);
	setmem(cp1 + wwd - x - wct, wct, ' ');
	display_text();
	findlast();
}
/* ----------- delete a line --------------- */
static void delete_line(int y)
{
	char *cp1, *cp2;
	int len;

	cp1 = bfptr + y * wwd;
	cp2 = cp1 + wwd;
	if (cp1 < lstptr)	{
		len = endptr - cp2;
		movmem(cp2, cp1, len);
		lstptr -= wwd;
		setmem(endptr - wwd, wwd, ' ');
		display_text();
	}
}

/* ----------- delete a block ------------- */
static void delete_block()
{
	char *cp1, *cp2;
	int len;

	if (!blkbeg || !blkend)	{
		putchar(7);
		return;
	}
	cp1 = topptr + blkend * wwd;
	cp2 = topptr + (blkbeg - 1) * wwd;
	len = endptr - cp1;
	movmem(cp1, cp2, len);
	setmem(cp2 + len, endptr - (cp2 + len), ' ');
	blkbeg = blkend = 0;
	lstptr -= (cp1 - cp2);
	display_text();
}
/*page*/
/* ------- move and copy text blocks -------- */
static void mvblock(int y, int moving)
{
	char *cp1, *cp2, *hd;
	int len;
	if (!blkbeg || !blkend)	{
		putch(BELL);
		return;
	}
	if (lineno(y) >= blkbeg-1 && lineno(y) <= blkend-1)	{
		error_message("Can't move/copy a block into itself");
		return;
	}
	len = (blkend - blkbeg + 1) * wwd;
	if ((hd = malloc(len)) == 0)
		return;
	cp1 = topptr + (blkbeg-1) * wwd;
	movmem(cp1, hd, len);
	cp2 = topptr + lineno(y) * wwd;
	if (moving)	{
		if (lineno(y) > blkbeg-1)
			cp2 -= len;
		do_display_text = 0;
		delete_block();
		do_display_text = 1;
	}
	if (cp2+len <= endptr)	{
		movmem(cp2, cp2 + len, endptr - cp2 - len);
		movmem(hd, cp2, len);
	}
	free(hd);
	blkbeg = blkend = 0;
	display_text();
}
/* ------------- copy a block ---------------- */
static void copy_block(int y)
{
	mvblock(y, FALSE);
	findlast();
}

/* --------- move a block ------------ */
static void move_block(int y)
{
	mvblock(y, TRUE);
}

/* ------- find the last character in the buffer -------- */
static void findlast()
{
	register char *lp = endptr - 1;
	register char *tp = topptr;

	while (lp > tp && (*lp == ' ' || *lp == '\0'))	{
		if (*lp == '\0')
			*lp = ' ';
		--lp;
	}
	if (*lp != ' ')
		lp++;
	lstptr = lp;
}

/* ------- go to the end of the data in the buffer ------- */
static void find_end(int *x, int *y)
{
	int ct;

	bfptr = lstptr;
	ct = (lstptr - topptr) % wsz;
	bfptr -= ct;
	if (bfptr + wsz > endptr)
		bfptr = endptr - wsz;
	*y = (ct / wwd);
	*x = 0;
	downward(y);
}
/*page*/
/* -------- carriage return -------- */
static void carrtn(int *x, int *y, int insert)
{
	int insct;
	char *cp, *nl;
	int ctl = 2;

	cp = curr(*x, *y);
	nl = cp + ((cp - topptr) % wwd);
	if (lineno(*y) + 2 < lines)
		if (insert && nl < endptr)	{
			insct = wwd - *x;
			while (ctl--)	{
				if (endptr > cp + insct)	{
					movmem(cp, cp+insct, endptr-insct-cp);
					setmem(cp, insct, ' ');
				}
				else if (ctl == 1)
					setmem(cp, endptr - cp, ' ');
				cp += insct * 2;
				insct = *x;
			}
		}
	*x = 0;
	downward(y);
	if (insert)	{
		test_para(*x, *y);
		display_text();
	}
	if (lineno(*y) + 2 < lines)
		if (insert)
			if ((lstptr + wwd) <= endptr)
				if (lstptr > curr(*x, *y))
					lstptr += wwd;
}
/*page*/
/* ------- move the buffer offset back one position ------ */
static void backspace(int *x, int *y)
{
	if (*x == 0)	{
		if (*y)
			*x = wwd - 1;
		upward(y);
	}
	else
		--(*x);
}

/* -------- move the buffer offset forward one word ------ */
static void fore_word(int *x, int *y, char *bf)
{
	while (*bf != ' ')	{
		if (spaceup(x, y, &bf) == 0)
			return;
		if (*x == 0)
			break;
	}
	while (*bf == ' ')
		if (spaceup(x, y, &bf) == 0)
			return;
}

static int spaceup(int *x, int *y, char **bf)
{
	if (*bf == lstptr)
		return 0;
	(*bf)++;
	forward(x, y);
	return 1;
}
/*page*/
/* ------- move the buffer offset backward one word ------ */
static void back_word(int *x, int *y, char *bf)
{
	spacedn(x, y, &bf);
	while (*bf == ' ')
		if (spacedn(x, y, &bf) == 0)
			return;
	while (*bf != ' ')	{
		if (*x == 0)
			return;
		if (spacedn(x, y, &bf) == 0)
			return;
	}
	spaceup(x, y, &bf);
}

static int spacedn(int *x, int *y, char **bf)
{
	if (*bf == topptr)
		return 0;
	--(*bf);
	backspace(x, y);
	return 1;
}


/* ----- move the buffer offset forward one position ----- */
static void forward(int *x, int *y)
{
	int ww = wwd;

	(*x)++;
	if (*x == ww)	{
		downward(y);
		*x = 0;
	}
}
/*page*/
/* ------- move the buffer offset down one position ------ */
static int downward(int *y)
{
	if (*y < wht - 1)	{
		(*y)++;
		return 1;
	}
	else if ((bfptr + wsz) < endptr)	{
		bfptr += wwd;
		scroll(wnd, UP);
		disp_line(wht-1);
		return 1;
	}
	return 0;
}

/* -------- move the buffer offset up one position ------ */
static void upward(int *y)
{
	if (*y)
		--(*y);
	else if ((topptr + wwd) <= bfptr)	{
		bfptr -= wwd;
		scroll(wnd, DN);
		disp_line(0);
	}
}

/* ---- display all the lines in a window ------ */
static void display_text()
{
	int y = 0;

	if (do_display_text)
		while (y < wht)
			disp_line(y++);
}
/*page*/
/* ---------- Display a line -------- */
static void disp_line(int y)
{
	int x = 0, atr = WNORMAL;

	if (blkbeg || blkend)
		if (lineno(y) >= blkbeg-1)
			if (lineno(y) <= blkend-1)
				atr = WACCENT;
	while (x < wwd)	{
		displ(wnd, x+1, y+1, *(bfptr+y * wwd+x), atr);
		x++;
	}
}

/* ---------- set insert/exchange cursor shape ----------- */
static void insert_line()
{
	set_cursor_type(inserting ? 0x0106 : 0x0607);
}

