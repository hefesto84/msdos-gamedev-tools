/************************************************************************/
/************************************************************************//*


	              QLIB DEMONSTRATION PROGRAM

     This program exercises the QLIB functions for TUBBO C (the name
     "TURBO C" is the property of BORLAND INTERNATIONAL).

     To run this program, specify QDEMO.PRJ as the project file and
     ajust your compiler options to a LARGE MEMORY MODEL.

     To become a registered QLIB user, see the comments at the end
     of this file.

     For additional information, run the QWELCOME.COM program.
                                                                        */
/************************************************************************/
/************************************************************************/



/************************************************************************/
/***************  beginning of QDEMO declaration block  *****************/
/************************************************************************/

/*  declaration for the strings to be processed by qlib variables	*/
typedef 	unsigned 	char	string[79];

/*  declaration for the prompt structure, see comments below for more	*/
/*  information on the qlib prompt conventions.				*/
typedef struct	prompt_structure
		{
		string	tag;
		string	value;
		int	tagx;
		int 	tagy;
		int	valx;
		int	valy;
		int	vall;
		char	tagf;
		char	tagb;
		char	valf;
		char	valb;
		int	valr;
		string  valt;
		string  vale;
		char	prmx;
		string  mhea;
		string  mtra;
		string  stid;
		int	poid;
		} prompt_record;

/************************************************************************/
/*                 beginning of mainline demo program		        */
/************************************************************************/
main()
{
/*  initialize some color attributes to be used in the demo		*/
unsigned	char	red            = 4;
unsigned	char	white_on_blue  = 31;
unsigned	char	black_on_white = 112;
unsigned	char	bright_white   = 15;
unsigned	char	blinking_white = 143;
unsigned	char	blinking_blue  = 129;
unsigned	char	cyan_on_blue   = 27;

/*  declare a variable "anystring" to be of type "string"		*/
string		anystring;

/*  declare a variable "q" to be of type "prompt_record"		*/
prompt_record	q;

/*  declare some general integer variables				*/
int index;
int intvar;
int returnkey = 13;
int f1key = 0x013b;
int f2key = 0x013c;
int f3key = 0x013d;

char charvar;

/*  set up and print welcome screen, see the functions below for comments */
/*  and notes on individual functions used here...                        */
	qclrscr(bright_white);
	qdrawbox(0, 7, 79, 12,
		"[ QLIB DEMONSTRATION PROGRAM ]",
		"[ HIT          TO COMTINUE ]",
		white_on_blue,
		bright_white,
		bright_white);
	qsetatt(1,8,78,11,cyan_on_blue);
	qsnap("<RETURN>", 32, 12, blinking_white);
	qcenter_line("A set of easy to use, flexible, consistent, well-documented", 9, white_on_blue);
	qcenter_line("functions for TURBO C", 10, white_on_blue);
	qgotoxy(0,25);
	while(! (intvar==returnkey) )
		{
		intvar=qreadkbd();
		}
/*  end of welcome screen...                                            */

/*           beginning of commented code                                */

/*  the following line uses the qclrscr function to clear the screen 	*/
/*  and set the default attribute to bright white (15d).		*/	
        qclrscr(bright_white);

/*  the following line positions the cursor at column 0, row 0		*/
	qgotoxy(0, 0);

/*  set up variables for printf  					*/
	strcpy(anystring,"this is an output line");
	printf("We are going to output some characters using printf - hit a key when ready...\n");
	charvar = getch();

	qclrscr(bright_white);
	qgotoxy(0, 0);
	for (index=1;index<25;++index)  printf("%s\n", anystring);

/*  the following line uses the qgotoxy function to "hide" the cursor, */
/*  the technique recommended by MICROSOFT.                            */
	qgotoxy(0, 25);

/*  the following line uses the qsnap function to print the literal	*/
/*  at column 0, row 24.  The literal will be printed in blinking cyan  */
/*  on blue.								*/
	qsnap("hit <return> to go on...", 0, 24, cyan_on_blue + 0x80);

	charvar = getch();
	qclrscr(bright_white);
	qcenter_line("Did you notice that I turned the cursor off?", 10, bright_white);
	qcenter_line("Now we do twice that with qlib calls - hit a key when ready...", 12, bright_white);
	charvar = getch();
	qclrscr(bright_white);

/*  the following line uses the qrembox function to clear the screen.	*/
	qrembox(0, 24, 79, 24);

	for (index=0;index<24;++index)  qsnap(anystring, 00, index, bright_white);
	for (index=0;index<24;++index)  qsnap(anystring, 50, index, bright_white);
	qrembox(0, 9, 79, 10);

/*  the following line uses the qcenter function to center the literal	*/
/*  on line 9 of an 80 column screen.  It will be centered in bright 	*/
/*  white (15d).							*/
	qcenter_line("That was it!!!", 9, bright_white);

	qcenter_line("just hit any key to continue", 10, blinking_white);
	charvar = getch();
	qclrscr(bright_white);

/*  the following line sets the default attribute from column 0, row 11,*/
/*  to column 79, row 13, to white on blue.				*/
	qsetatt(0,11,79,13,white_on_blue);

	qcenter_line("ok - now for some boxes...  hit a key when ready", 12, white_on_blue);
	charvar = getch();
	qclrscr(bright_white);

/*  the following line draws a box on the screen from column 17, row 0, */
/*  to column 79, row 5.  The box characters will be bright white.	*/
/*  The first literal will be centered between column 17 and column 79	*/
/*  on row 0 and will be black on white.  The second literal will be	*/
/*  centered between column 17 and column 79 on row 5 and will be cyan	*/
/*  on blue.  The area inside the box will be cleared.			*/								
        qdrawbox(17, 00, 79, 05, "[ THIS IS ANOTHER BOX ]", "[ BOTTOM TITLE ]", bright_white, black_on_white, cyan_on_blue);

	qdrawbox(00, 00, 15, 05, "[ HELLO ]", "", bright_white, black_on_white, 0);
        qdrawbox(00, 07, 45, 11, "[ THIS IS A SQUISHY BOX ]", "[ DIMENSIONS ARE 0,7,45,11 ]", white_on_blue, black_on_white, black_on_white);
        qdrawbox(47, 07, 79, 23, "[ THIS IS A LONG BOX ]", "[ THIS IS LINE 23 ]", 13, 14, 15);
	qdrawbox(00, 13, 05, 23, "", "", 11, 15, 15);
	qdrawbox(06, 13, 10, 23, "", "", cyan_on_blue, 15, 15);
	qdrawbox(11, 13, 45, 23, "", "", 14, 15, 15);
	qsnap("hit <return>...", 0, 24, blinking_white);
	charvar = getch();
	qclrscr(bright_white);
	qclrscr(15);

/*  The following line clears out the keyboard buffer.			*/
	qdrain_keyboard();

	charvar = 0;
	qcenter_line("keyboard status display", 0, 15);
	qcenter_line("please play with <ALT>, <CTRL>, <R-SHFT>, <L-SHFT>,", 5, 15);
	qcenter_line("<CAPS>, <INSERT>, <NUM>, and <SCROLL>", 6, 15);
	qcenter_line("press <return> to quit...", 10, 15);
	qgotoxy(0,25);
	intvar = 0;
	while(intvar != returnkey)
		{
		while (!kbhit())
/*  			The following line displays the key board	*/
/*  			status (toggles) on line 24.			*/
			qkbd_status_line(24);

/*  The following line calls the qreadkbd function.  Upon return, the	*/
/*  high byte of "intvar" will contain a one if a special key has been  */
/*  pressed (function keys, arrow keys, etc).  If a special key has not */
/*  been pressed, the high byte of "intvar" will contain a zero.  In    */
/*  either case, the low byte of "intvar" will contain the ascii code   */
/*  representing the keystroke.  The QLIB functions qhi(intvar), and    */
/*  qlo(intvar) can be used to segregate the bytes into unsigned char   */
/*  variables if need be.						*/
		intvar = qreadkbd();

/*  The loop exit conditions are intvar==13, which translates to hitting*/
/*  the <return key>.                  					*/
		}
        qdrain_keyboard();
        qclrscr(cyan_on_blue);
	qdrawbox(0, 9, 79, 11, "", "", 14, 22, 0);
	qcenter_line("What about prompts?  Hit any key to continue...", 10, 23);
	charvar = getch();
 	qclrscr(bright_white);

/*----------------------------------------------------------------------*/
/*  The following segment of code intializes a "prompt record" in	*/
/*  preparation for a call to the qprompt function.			*/
/*                                                                      */
/*    A "prompt" is considered to have the following elements:          */
/*                                                                      */
/*       (Elements included in release 3)                               */
/*            tag   -  A literal describing the data to be collected    */
/*            value -  The data to be collected                         */
/*            tagx  -  The column where the tag is to be displayed      */
/*            tagy  -  The row where the tag is to be displayed         */
/*            tagf  -  The foreground color to be used when displaying  */
/*                     the tag                                          */
/*            tagb  -  The background color to be used where displaying */
/*                     the tag                                          */
/*            valx  -  The column where data collection is to begin     */
/*            valy  -  The row where data collection is to occur        */
/*            vall  -  The maximum number of characters to be collected */
/*            valf  -  The foreground color of characters entered       */
/*            valb  -  The background color of characters entered       */
/*                                                                      */
/*       (Elements not included in release 3)                           */
/*            valr  -  The edit rules mask for entered characters       */
/*            valt  -  The external table to be used for validation     */
/*            vale  -  The external table to be used for expansion      */
/*            mhea  -  The message header string                        */
/*            mtra  -  The message trailer string                       */
/*            stid  -  The destination LAN station                      */
/*            poid  -  The destination port                             */
/*            prmx  -  The exit conditions mask                         */
/*----------------------------------------------------------------------*/
		
/*  The following line initializes the prompt tag                    	*/
	strcpy(q.tag, "Type a string for me to center or <return> to quit:  ");

/*  The following line intializes the column where the prompt tag is to */
/*  be displayed.							*/
	q.tagx = 0;

/*  The following line initializes the row where the prompt tag is to   */
/*  be displayed.							*/
	q.tagy = 5;

/*  The following line sets the foreground color for the prompt tag.	*/
	q.tagf = 15;

/*  The following line sets the background color for the prompt tag.	*/
	q.tagb = 0;

/*  The following line sets the first column where the string is to be	*/
/*  collected.								*/
	q.valx = q.tagx + strlen(q.tag);

/*  The following line sets the row where the string is to be collected */
	q.valy = q.tagy;

/*  The following line sets the maximum length of the string to be 	*/
/*  collected.								*/
	q.vall = 25;

/*  The following lines sets the foreground color for the string as it  */
/*  is entered.								*/
	q.valf = 7;

/*  The following lines sets the background color for the string as it  */
/*  is entered.								*/
	q.valb = 1;

/*  The following initializes the data itself				*/
	strcpy(q.value, "      ");

	qcenter_line("THE QLIB FUNCTIONS ACT TOGETHER TO CREATE PROMPTS!", 16, bright_white);
	while (strlen(q.value) != 0)
		{

/*		The following line sends the prompt record to the prompt*/
/*		function.  On return, the element "value" will contain  */
/*		what was entered at the keyboard.  			*/

		qprompt(&q);
		qrembox(0, 0, 79, 0);
		qcenter_line(q.value, 0, 15);	
		}

        qclrscr(bright_white);
	qcenter_line("Let's play with the          keys", 12, 15);
	qsnap("FUNCTION", 43, 12, cyan_on_blue + 0x80);
	qcenter_line("Please hit the F9 key to continue...", 14, 15);
	qgotoxy(0,25);
	intvar = 0;
	while(! (intvar==0x0143) ) /* 0x0143 is how qreadkbd() returns F9 */
		intvar = qreadkbd();
	qcenter_line("THE QLIB FUNCTIONS ACT TOGETHER TO CREATE MENUS!!!!", 5, bright_white);
	while(! (intvar==f3key) )
		{
		qdrawbox(19,10,59,18,"[ MAIN MENU ]","[ QDEMO FUNCTIONS ]", cyan_on_blue, black_on_white, red);
		qcenter_line("F1           Do this           ", 13, bright_white);
		qcenter_line("F2           Do the other thing", 14, bright_white);
		qcenter_line("F3           Quit              ", 15, bright_white);
		qcenter_line("PRESS F1, F2, or F3 ", 23, bright_white);
		qrembox(0,24,79,24);
		qdrain_keyboard();
		intvar=qreadkbd();
		qcenter_line("   INVALID CHOICE   ", 23, blinking_white);
		if (intvar==f1key)
			{
			qcenter_line("   You selected F1  ", 23, blinking_white);
			qcenter_line("F1           Do this           ", 13, black_on_white);
			}
		if (intvar==f2key)
			{
			qcenter_line("   You selected F2  ", 23, blinking_white);
 			qcenter_line("F2           Do the other thing", 14, black_on_white);
			}
		qcenter_line("Hit <RETURN> to continue", 24, bright_white);
		if (intvar != f3key) while(! (intvar==returnkey)) intvar = qreadkbd();
                }
        qclrscr(bright_white);
	for (index=0;index<=24;++index)
		qcenter_line("Thank you  Thank you  Thank you  Thank you", index, index);
	qdrawbox(10, 8, 70, 12, "[ QLIB DEMONSTRATION PROGRAM ]", "[ THAT'S ALL FOLKS! ]", bright_white, bright_white, bright_white);
	qcenter_line("Become a registered user today!", 10, bright_white);
	qgotoxy(0,23); }
/*---------------------  end of program  ----------------------------------*/
/*
        In addition to complete documentation, QLIB registrants will
        receive new releases of QLIB for one year mailed automatically
        with no further action required on your part.  New releases
        (at least one is guaranteed) will be sent on low density (360K)
        diskettes.  Documentation will be included on the diskette
        and with laser-jet quality hard copy.	

        The registration process involves sending your name, address,
        Compuserve ID (if appropriate), and $35 to:

                      Lisa T. Vass
                      5311 Blvd East # 3
                      West New York, New Jersey  07093

        Non domestic registrants must include an additional $20.
*/

/***************************************************************************/
/*                                                                         */
/*   below is a sample of the documentation sent to qlib registrants       */
/*                                                                         */
/***************************************************************************//*
qsnap function:

	usage:  void qsnap (unsigned char string[79],
			    unsigned int x,
                            unsigned int y,
                            unsigned char attribute);
        location:           qlib.lib
        related functions:  void qget (unsigned char string[79],
                                       unsigned int x,
                                       unsigned int y,
                                       unsigned int length);
        description:        Qsnap provides no-snow, direct screen output.
        		    the assumed screen dimensions are 0 - 79
        		    columns (the "X" variable) and 0 - 24 rows
                            (the "Y" variable).
                            Qget returns a string of specified length
                            from column x, row y.  Terminating the
                            string with an ASCII zero for any subsequent
                            string processing is the programmer's
                            responsibility.
	returns:            Nothing.  No error checking.  Integer variables
                            are used to permit access to "invisible" rows
                            and columns.  This usage, however, is not
                            supported or recommended, other than for hiding
                            the cursor (column 0, row 25) */
/*-------------------------------------------------------------------------*/
