/* ---------------- poems.c ----------------- */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "twindow.h"
#include "keys.h"

/* -------- local prototypes ----------- */
void get_poem(int s);
int ht (char **tb);
int wd (char **tb);
char *titles [] = {
	" 1: TELL ALL THE TRUTH BUT TELL IT SLANT ",
	" 2: AFTER LONG SILENCE ",
	" 3: A MAN SAID TO THE UNIVERSE ",
	" 4: FLYING CROOKED ",
	" 5: THE IDLE LIFE I LEAD ",0
};
WINDOW *pno [] = {0, 0, 0, 0, 0};
static int x [] = {20, 15, 29, 10, 17};
static int y [] = {5, 10, 13, 18, 6};
static int wcl [] [2] = {	{BLUE, WHITE},
							{MAGENTA, WHITE},
							{RED, WHITE},
							{GREEN, WHITE},
							{AQUA, WHITE}	};
char *poem1 [] = {
	"Tell all the truth but tell it slant -",
	"Success in Circuit lies",
	"Too bright for our infirm Delight",
	"The Truth's superb surprise",
	"",
	"As Lightning to the Children eased",
	"With explanation kind",
	"The Truth must dazzle gradually",
	"Or every man be blind -",
	"                Emily Dickenson",
	0
};

char *poem2 [] = {
	"Speech after long silence; it is right,",
	"All other lovers being estranged or dead,",
	"Unfriendly lamplight hid under its shade,",
	"The curtains drawn upon unfriendly night,",
	"That we descant and yet again descant",
	"Upon the supreme theme of Art and Song:",
	"Bodily decrepitude is wisdom; young",
	"We loved each other and were ignorant.",
	"              William Butler Yeats",
	0
};

char *poem3 [] = {
	"A man said to the universe:",
	"\"Sir, I exist!\"",
	"\"However,\" replied the universe,",
	"\"The fact has not created in me",
	"A sense of obligation.\"",
	"               Stephen Crane",
	0
};

char *poem4 [] = {
	"The butterfly, a cabbage-white,",
	"(His honest idiocy of flight)",
	"Will never now, it is too late,",
	"Master the art of flying straight,",
	"Yet has - who knows so well as I? -",
	"A just sense of how not to fly:",
	"He lurches here and there by guess",
	"And God and hope and hopelessness.",
	"Even the aerobatic swift",
	"Has not his flying-crooked gift.",
	"              Robert Graves",
	0
};
/*page*/
char *poem5 [] = {
	"The idle life I lead",
	"Is like a pleasant sleep,",
	"Wherein I rest and heed",
	"The dreams that by me sweep.",
	"",
	"And still of all my dreams",
	"In turn so swiftly past,",
	"Each in its fancy seems,",
	"A nobler than the last.",
	"",
	"And every eve I say,",
	"Noting my step in bliss,",
	"That I have known no day",
	"In all my life like this.",
	"         Robert Bridges",
	0
};
char **poem [] = {poem1,poem2,poem3,poem4,poem5,0};

void poems()
{
	int s = 0, i, c;
	WINDOW *mn;
	char **cp;

	cursor(0, 25);
	mn = establish_window(0, 0, 7, 45);
	set_title(mn, " Select A Poem ");
	set_colors(mn, ALL, BLUE, GREEN, BRIGHT);
	set_colors(mn, ACCENT, GREEN, WHITE, BRIGHT);
	display_window(mn);
	cp = titles;
	while (*cp)
		wprintf(mn, "\n%s", *cp++);
	while (1)	{
		set_help("poemmenu", 40, 10);
		s = get_selection(mn, s+1, "12345");
		if (s == 0)
			break;
		if (s == FWD || s == BS)	{
			s = 0;
			continue;
		}
		hide_window(mn);
		get_poem(--s);
		c = 0;
		set_help("poems   ", 5, 15);
		while (c != ESC)	{
			c = get_char();
			switch (c)	{
				case FWD:	rmove_window(pno[s], 1, 0);
							break;
				case BS:	rmove_window(pno[s], -1, 0);
							break;
				case UP:	rmove_window(pno[s], 0, -1);
							break;
				case DN:	rmove_window(pno[s], 0, 1);
							break;
				case DEL:	delete_window(pno[s]);
							pno[s] = NULL;
							break;
				case '+':	forefront(pno[s]);
							break;
				case '-':	rear_window(pno[s]);
				default:	break;
			}
			if (c > '0' && c < '6')
				get_poem(s = c - '1');
		}
		forefront(mn);
		display_window(mn);
	}
	close_all();
	for (i = 0; i < 5; i++)
		pno[i] = NULL;
}
/*page*/
/* --- activate a poem by number ---- */
static void get_poem(int s)
{
	char **cp;
	static int lastp = -1;
	if (lastp != -1)
		set_intensity(pno[lastp], DIM);
	lastp = s;
	if (pno [s])
		set_intensity(pno[s], BRIGHT);
	else	{
		pno [s] = establish_window
			(x[s], y[s], ht(poem[s]), wd(poem[s]));
		set_title(pno[s], titles[s]);
		set_colors(pno[s],ALL,wcl[s][0],wcl[s][1],BRIGHT);
		set_border(pno[s], 1);
		display_window(pno[s]);
		cp = poem[s];
		while (*cp)
			wprintf(pno[s], "\n %s", *cp++);
	}
}
/* ------- compute height of a window display table ---- */
static int ht(char **tb)
{
	int h = 0;
	while (*(tb + h++))	;
	return h + 3;
}
/* ------- compute width of a window display table ------- */
static int wd(char **tb)
{
	int w = 0;
	while (*tb)		{
		w = max(w, strlen(*tb));
		tb++;
	}
	return w + 4;
}
