#include <dos.h>
#include <fstream.h>
#include <conio.h>
#include "graph.hpp"

int main(void)
{
    boxes menu_demo;
    font type_face(8,15);
    fontpointer=&type_face;
    windows main_windows(0,0,639,479,15,1,windows::none);
    main_gui.read("EGA.BIN");
    main_gui.set_mode();
    type_face.load_font("STANDARD.FNT");
    windowpointer=&main_windows;
    main_gui.fill_screen(1);
    windowpointer->text("This is a test....");
    menu_demo.radio_button("Button #1",100,300,200,330);
    menu_demo.radio_button("Button #2",210,300,310,330);

    windows mid_windows(63,30,320,100,12,0,windows::auto_scroll_down);
    windows new_windows(340,110,600,200,11,0,windows::none);
    windows hit_a_key(100,250,330,270,14,0,windows::horizontal_center |
        windows::vertical_center);
    windowpointer=&hit_a_key;
    windowpointer->reset();
    windowpointer->text("Hit any key to exit!");
    windowpointer=&mid_windows;
    windowpointer->reset();
    windowpointer->border(10,2,5);
    windowpointer=&new_windows;
    windowpointer->reset();
    windowpointer->border(13,13,5);
    new_windows.current_y=75;
    while(!kbhit()) {
        for(int i=0; i < 90; i++) {
            if(kbhit())
                break;
            windowpointer=&mid_windows;
            windowpointer->scroll(-1);
            switch(i) {
                case 75:
                windowpointer->text("The Warp Graphics Library!\n",0,0,0);
                break;
                case 60:
                windowpointer->text("Supports Smooth Scrolling\n",0,0,0);
                break;
                case 45:
                windowpointer->text("Overlapping Windows\n",0,0,0);
                break;
                case 30:
                windowpointer->text("and a variety of other\n",0,0,0);
                break;
                case 15:
                windowpointer->text("features.\n",0,0,0);
                break;
            }
            windowpointer=&new_windows;
            windowpointer->scroll(1);
            switch(i) {
                case 15:
                windowpointer->text("Multiple Windows Open\n");
                break;
                case 30:
                windowpointer->text("Extremely fast routines\n");
                break;
                case 45:
                windowpointer->text("All drivers written in\n");
                break;
                case 60:
                windowpointer->text("assembly language\n");
                break;
                case 75:
                windowpointer->text("and C++.\n");
                break;
            }
        }
    }
    main_gui.text_mode();
}
