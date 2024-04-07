class windows;
extern windows *windowpointer;

class gui {
friend class point;
friend class line;
friend class windows;
friend class boxes;
friend class font;
    unsigned char *screen_buffer[4];
    unsigned char far *graph_driver;        // stores the graphics driver file
    unsigned int graph_driver_pointer;
    // pointer to the current position in the graphics driver

    void find_func(void);
    // locates the next function in the graphics driver

    void (far *fill_screen_ptr)(unsigned char color);
    // fills the screen with the specified color

    void (far *set_mode_ptr)(void);
    // sets the graphics mode

    void (far *out_bitmask_ptr)(unsigned int x, unsigned int y, unsigned char color,
        unsigned char *bitmask, unsigned int height, unsigned char text);
    // used for outputting characters to the screen in a graphics mode

    void (far *point_put_ptr)(unsigned int x, unsigned int y, unsigned char color);
    // outputs a pixel to the screen

    void (far *line_put_ptr)(unsigned int x1, unsigned int y1, unsigned int x2,
        unsigned int y2, unsigned char color);
    // draws a line on the screen

    void (far *horizontal_line_ptr)(unsigned int x1, unsigned int y1,
        unsigned int x2, unsigned char color);
    // draws a horizontal line on the screen

    void (far *vertical_line_ptr)(unsigned int y1, unsigned int x1,
        unsigned int y2, unsigned char color);
    // draws a vertical line on the screen

    void (far *filled_box_ptr)(unsigned int x1, unsigned int y1, unsigned int x2,
        unsigned int y2, unsigned char color);
    // draws a filled box on the screen

    void (far *scroll_up_ptr)(unsigned int x1, unsigned int y1, unsigned int x2,
        unsigned int y2, unsigned char color, unsigned int lines);
    // scrolls the screen up

    void (far *scroll_down_ptr)(unsigned int x1, unsigned int y1, unsigned int x2,
        unsigned int y2, unsigned char color, unsigned int lines);

    void (far *save_full_screen)(unsigned char *buffer[4]);

    void (far *restore_full_screen)(unsigned char *buffer[4]);

    unsigned int (far *screen_mem)(void);

public:
    int (far *set_fli_mode)(void);
    // attempts to set mode 13 and returns the value 0 if unsuccessful

    void (far *highlight_window)(unsigned int x1, unsigned int y1,
        unsigned int x2, unsigned int y2);
    // highlights a window

    int (far *set_gif_mode)();

    void (far *output_raster)(unsigned char *raster_data, unsigned int length);

    unsigned long (far *window_memory)(unsigned int x1, unsigned int y1,
        unsigned int x2, unsigned int y2);

    void (far *save_window)(unsigned char *window_buffer[4], unsigned int x1,
        unsigned int y1, unsigned int x2, unsigned int y2);

    void (far *write_window)(unsigned char *window_buffer[4], unsigned int x1,
        unsigned int y1, unsigned int x2, unsigned int y2);

    void read(char *path);
    // loads the graphics driver file

    void fill_screen(unsigned char color);
    // fills the screen with the specified color

    void set_mode(void);
    // sets the video mode

    void text_mode(void);
    // sets text mode

    void scroll(unsigned int x1, unsigned int y1, unsigned int x2,
        unsigned int y2, unsigned char color, int lines);
    // scrolls the screen

    void save_screen(void);
    void restore_screen(void);
};

extern gui main_gui;

class point {
public:
    void put(unsigned int x, unsigned int y, unsigned char color);
};

class line : public point {
public:
    void put(unsigned int x1, unsigned int y1, unsigned int x2,
        unsigned int y2, unsigned char color);
};


class boxes : public line {
public:
    void check(unsigned int x, unsigned int y);
    void uncheck(unsigned int x, unsigned int y);
    void box(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2,
        unsigned char color);
    void check_box(char *text, unsigned char parameter,
        unsigned int x,
        unsigned int y,
        unsigned char color);
    void filled_box(unsigned int x1, unsigned int y1, unsigned int x2,
        unsigned int y2, unsigned char fillcolor);
    void radio_button(char *text, unsigned int x1, unsigned int y1,
        unsigned int x2, unsigned int y2);
};


class windows : public boxes {
// defines a windows on the screen
public:
    enum opts {
        none=               0,
        vertical_center=    0x0001,
        horizontal_center=  0x0002,
        auto_scroll_up=     0x0004,
        auto_scroll_down=   0x0008
    };
private:
    enum opts formatting_options;
public:
    unsigned int cursor;
    unsigned char border_color1;
    unsigned char border_color2;
    unsigned int border_width;
    unsigned int x_indent;
    unsigned int y_indent;
    unsigned int x1;
    unsigned int y1;
    unsigned int x2;
    unsigned int y2;
    unsigned int current_x;
    unsigned int current_y;
    unsigned char current_color;
    unsigned char background_color;
    unsigned char *windows_buffer[4];   // saved windows data
    void reset(void);
    void put_cursor(unsigned int x=windowpointer->current_x,
        unsigned int y=windowpointer->current_y,
        unsigned char color=windowpointer->current_color);
    void remove_cursor(unsigned int x=windowpointer->current_x,
        unsigned int y=windowpointer->current_y,
        unsigned char background=windowpointer->background_color);
    void text(unsigned char text, unsigned int andwith=0,
        unsigned int x=windowpointer->current_x,
        unsigned int y=windowpointer->current_y,
        unsigned char color=windowpointer->current_color,
        unsigned char background=windowpointer->background_color);
    void text(char *text, unsigned int andwith=0,
        unsigned int x=windowpointer->current_x,
        unsigned int y=windowpointer->current_y,
        unsigned char color=windowpointer->current_color,
        unsigned char background=windowpointer->background_color);
    windows(void);
    windows(unsigned int xx1, unsigned int yy1, unsigned int xx2,
        unsigned int yy2, unsigned char color=7, unsigned char background=0,
        enum opts options=none);
    void set_window(unsigned int xx1, unsigned int yy1, unsigned int xx2,
        unsigned int yy2, unsigned char color=7, unsigned char background=0,
        enum opts options=none);
    void border(unsigned char color=15, unsigned char color2=8,
        unsigned int width=3);
    void scroll(int lines);
    void save(void);
    void restore(void);
    void setf(enum opts options);
    void unsetf(enum opts options);
};


class font : public boxes {
    char name[12];
    char bitmap[256][16];
public:
    void put_char(unsigned char text, unsigned int x, unsigned int y,
        unsigned char color, unsigned int andwith, unsigned char background);
    int width, height;
    font(unsigned int x, unsigned int y);
    void load_font(char *text);
    friend class windows;
};

extern font *fontpointer;
