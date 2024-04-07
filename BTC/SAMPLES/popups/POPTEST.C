
#include <popups.h>

main()
{

 	init_popup(C80,1,1,80,25,WHITE,BLUE,RED,NO_BORDER);


	explode(ON);
	shrink(ON);
	noise(ON);


	gotoxy(15,3);
	cprintf("This is the main screen.");
	gotoxy(15,4);
	cprintf("Foreground is WHITE, Background is BLUE");
    gotoxy(15,5);
	cprintf("Bordertype is NO_BORDER");
	gotoxy(15,7);
	cprintf("The popup window id number is %d",id_popup());
    gotoxy(15,8);
	cprintf("Maximum windows is set at %d",pop_max());
	gotoxy(15,9);
	cprintf("Number of remaining windows is %d",pop_left());
    gotoxy(15,10);
	cprintf("Popup window buffer size is %d bytes",buf_max());
	gotoxy(15,11);
	cprintf("Number of remaining buffer bytes is %d",buf_left());
    gotoxy(15,12);
	cprintf("This window is %d rows high",high_popup());
	gotoxy(15,13);
	cprintf("This window is %d columns wide",wide_popup());
    gotoxy(15,15);
	cprintf("Strike any key for next window");
	gotoxy(15,19);
	cprintf("This a demo of 'popups.c'");
    gotoxy(15,20);
	cprintf("Version 1.01  01/20/89  Written by Kevin Murphy");

	getch();


	shadow(BR_THICK);

	if (next_popup(5,4,57,15,YELLOW,BROWN,RED,ONE_LINE) != POPUP_ERROR) {
    	gotoxy(2,2);
		cprintf("This is the 1st window");
        gotoxy(2,3);
		cprintf("Bordertype = ONE_LINE");
		tell_info();
    }

	
	if (next_popup(10,6,62,19,YELLOW,RED,WHITE,TWO_LINE) != POPUP_ERROR) {
    	gotoxy(2,2);
		cprintf("This is the 2nd window");
        gotoxy(2,3);
		cprintf("Bordertype = TWO_LINE");
		tell_info();
	}

	shadow(NO_SHADOW);

	if (next_popup(3,3,56,17,GREEN,BLACK,WHITE,THIN_SOLID) != POPUP_ERROR) {
    	gotoxy(2,2);
		cprintf("This is the 3rd window");
		gotoxy(2,3);
		cprintf("Bordertype = THIN_SOLID");
		tell_info();
	}

	if (next_popup(10,2,65,15,RED,BROWN,CYAN,THICK_SOLID) != POPUP_ERROR) {
    	gotoxy(2,2);
		cprintf("This is the 4th window");
		gotoxy(2,3);
		cprintf("Bordertype = THICK_SOLID ");
		tell_info();
	}

	shadow(BR_THICK);

	if (next_popup(17,4,71,21,YELLOW,RED,GREEN,ONE_LINE) != POPUP_ERROR) {
    	gotoxy(2,2);
		cprintf("This is the 5th window");
		gotoxy(2,3);
		cprintf("Bordertype = ONE_LINE");
		tell_info();
	}


	if (next_popup(18,5,64,16,CYAN,BLUE,GREEN,THIN_SOLID) != POPUP_ERROR) {
    	gotoxy(2,2);
		cprintf("This is the 6th window");
		gotoxy(2,3);
		cprintf("Bordertype = THIN_SOLID");
		tell_info();
	}

	if (next_popup(20,7,68,18,YELLOW,BROWN,LIGHTGREEN,TWO_LINE) != POPUP_ERROR) {
    	gotoxy(2,2);
		cprintf("This is the 7th window");
		gotoxy(2,3);
		cprintf("Bordertype = TWO_LINE");
		tell_info();
	}

	shadow(NO_SHADOW);

	if (next_popup(5,5,55,17,YELLOW,LIGHTGRAY,LIGHTGREEN,TWO_LINE) != POPUP_ERROR) {
    	gotoxy(2,2);
		cprintf("This is the 8th window");
		gotoxy(2,3);
		cprintf("Bordertype = TWO_LINE");
		tell_info();
	}


	shadow(BR_THICK);

		if (next_popup(18,9,70,22,YELLOW,RED,GREEN,THICK_SOLID) != POPUP_ERROR) {
    	gotoxy(2,2);
		cprintf("This is the  9th window");
		gotoxy(2,3);
		cprintf("Bordertype = THICK_SOLID");
		gotoxy(2,5);
		cprintf("Window id number = %d",id_popup());
    	gotoxy(2,6);
		cprintf("Windows: Max = %d, Remaining = %d",pop_max(),pop_left());
		gotoxy(2,7);
		cprintf("Buffer bytes: Tot = %d, Remaining = %d",buf_max(),buf_left());
		gotoxy(2,8);
		cprintf("Height = %d rows, Width = %d columns",high_popup(),wide_popup());
    	gotoxy(2,10);
		cprintf("Strike any key for 8th window");
		getch();
		previous_popup();
	}

	gotoxy(2,10);
	cprintf("Strike any key for 7th window         ");
	getch();
	previous_popup();


    gotoxy(2,10);
	cprintf("Strike any key for 6th window         ");
	getch();
	previous_popup();

    gotoxy(2,10);
	cprintf("Strike any key for 5th window         ");
	getch();
	previous_popup();

    gotoxy(2,10);
	cprintf("Strike any key for 4th window         ");
	getch();
	previous_popup();

    gotoxy(2,10);
	cprintf("Strike any key for 3rd window         ");
	getch();
	previous_popup();

    gotoxy(2,10);
	cprintf("Strike any key for 2nd window         ");
	getch();
	previous_popup();

    gotoxy(2,10);
	cprintf("Strike any key for 1st window         ");
	getch();
	previous_popup();

    gotoxy(2,10);
	cprintf("Strike any key for main screen        ");
	getch();
	previous_popup();

	gotoxy(15,15);
	cprintf("Strike any key to exit                 ");
	getch();
	uninit_popup();



}


tell_info()
{

	gotoxy(2,5);
	cprintf("Version %d   Window id number = %d",version_pop(),id_popup());
    gotoxy(2,6);
	cprintf("Windows: Max = %d, Remaining = %d",pop_max(),pop_left());
	gotoxy(2,7);
	cprintf("Buffer bytes: Tot = %d, Remaining = %d",buf_max(),buf_left());
	gotoxy(2,8);
	cprintf("Height = %d rows, Width = %d columns",high_popup(),wide_popup());
    gotoxy(2,10);
	cprintf("Strike any key for next window");
	getch();

}
