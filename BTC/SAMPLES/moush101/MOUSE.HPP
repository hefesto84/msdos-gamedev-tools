/*

 MYMOUSE.HPP

 Header file for Microsoft Mouse routines for Turbo C++

 Michael Chen	7/26/1992

 Do not create instance until after graphics mode

*/


#ifndef __MOUSE_HPP
#define __MOUSE_HPP

/****************************************************************************

 Include files

****************************************************************************/

extern "C" {
#include "mouse.h"
}


/****************************************************************************

 Defines, bit masks and flags

****************************************************************************/


/****************************************************************************

 Classes, typedefs and related info

****************************************************************************/

class Mouse
{
public:

  Mouse()			{ StartMouse(); }
  ~Mouse()			{ EndMouse();	}

  int init()			{ return InitMouseDriver(); }
  int reset()			{ return ResetMouse(); }
  int disable_driver()		{ return DisableMouseDriver(); }
  int enable_driver()		{ return EnableMouseDriver(); }

  void show()			{ ShowMouseCursor(); }
  void hide()			{ HideMouseCursor(); }

  int status()			{ return GetMouseStatus(); }

  int buttons()			{ return NumberOfButtons; }
  int present()			{ return DriverPresent; }
  int x()			{ return MouseX; }
  int y()			{ return MouseY; }
  int button()			{ return MouseButton; }
  int left()			{ return (MouseButton & S_LEFT_BUTTON); }
  int right()			{ return (MouseButton & S_RIGHT_BUTTON); }
  int middle()			{ return (MouseButton & S_MIDDLE_BUTTON); }
  int which()			{ return WhichButton; }
  int pressed()			{ return MousePressed; }
  int where_x() 		{ return MouseWhereX; }
  int where_y()			{ return MouseWhereY; }
  int moved_x()			{ return MouseMovedX; }
  int moved_y()			{ return MouseMovedY; }
  MouseHandler last_handler()	{ return LastMouseHandler; }
  int x_speed()			{ return MouseXSpeed; }
  int y_speed()			{ return MouseYSpeed; }
  int double_threshold()	{ return MouseDoubleThreshold; }

  void position(int x,int y)	{ SetMousePosition(x,y); }

  int check_down(int b)		{ return GetMouseDown(b); }
  int check_up(int b)		{ return GetMouseUp(b); }
  int check_moved()		{ return GetMouseMove(); }
  void clear_button(int b)	{ GetMouseDown(b); GetMouseUp(b); }
  void clear_moved()		{ GetMouseMove(); }
  void clear_all()		{ clear_button(LEFT_BUTTON);
				  clear_button(RIGHT_BUTTON);
				  clear_button(MIDDLE_BUTTON);
				  clear_moved(); }
  void xrange(int l,int h)	{ SetMouseXRange(l,h); }
  void yrange(int l,int h)	{ SetMouseYRange(l,h); }
  void update_region
    (int x0, int y0,
     int x1, int y1)		{ SetUpdateRegion(x0,y0,x1,y1); }

  void cursor(MouseCursor mc)	{ SetMouseCursor(mc); }
  int large_cursor
    (LargeMouseCursor c)	{ return SetLargeMouseCursor(c); }

  void set_handler
    (MouseHandler mh,int mask)	{ SetMouseHandler(mh,mask); }
  void exchange_handler
    (MouseHandler mh,int mask)	{ ExchangeMouseHandler(mh,mask); }

  int set_alt_handler
    (MouseHandler mh,int mask)	{ return SetAltMouseHandler(mh,mask); }
  MouseHandler get_alt_handler
    (int mask)			{ return GetAltMouseHandler(mask); }

  void lightpen(int f)		{ SetLightPenEmulation(f); }

  void mickey_ratio
    (int xr, int yr)		{ SetMickeyRatio(xr,yr); }

  int state_size()		{ return GetDriverStateSize(); }
  void far* get_state()		{ return GetDriverState(); }
  void set_state(void far* fp)	{ SetDriverState(fp); }

  void set_sensitivity
    (int xs, int ys, int dt)	{ SetMouseSensitivity(xs,ys,dt); }
  void get_sensitivity()	{ GetMouseSensitivity(); }

  void set_page(int p)		{ SetMouseDisplayPage(p); }
  int page()			{ return GetMouseDisplayPage(); }

};


#endif
