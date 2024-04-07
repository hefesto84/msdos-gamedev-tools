/*      Mouse.H       */

#define __MOUSE__	1

#define LEFT_B		1
#define RIGHT_B		2
#define MIDDLE_B	4

typedef struct {
        int x;
        int y;
        int but;
} MOUSE;

int  reset_mouse(void);
void init_mouse(void);
int  get_button_num(void);
void show_mouse(void);
void hide_mouse(void);
int  which_pressed(MOUSE *);
void get_xy(MOUSE *);
void get_t_xy(MOUSE *);
void set_xy(int, int);
void set_t_xy(int, int);
int  pressed_status(MOUSE *, int);
int  released_status(MOUSE *, int);
void set_x_range(int, int);
void set_y_range(int, int);
void set_graphic_cursor(int, int, unsigned int far *);
void set_text_cursor(int, int, int);
int  click_button(MOUSE *, int);

/***************** not frequently used functions ****************
int  Motion(int *, int *);
void Event_Set(unsigned int, void far *);
void Lightpen_Emu_On(void);
void Lightpen_Emu_Off(void);
void Set_M2P_Ratio(int, int);
void Block_Of_Hide(int, int, int, int);
void Double_Speed(int);
void far *Swap_Event_Set(unsigned int *, void far *);
int  Get_Storage_Req();
void Save_Mouse_State(void *);
void Restore_Mouse_State(void *);
***************************************************************/

