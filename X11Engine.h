#ifndef X11ENGINE_H
#define X11ENGINE_H
#include <X11/Xlib.h>
#include <vector>
#include "shell.h"
#include <string>

typedef struct
{
    Pixmap bitmap;
    unsigned int bmp_w;
    unsigned int bmp_h;
    int hp_x;
    int hp_y;
    GC gc;
    XColor color;

}BITMAP;

typedef struct{
    Window button;
    std::string text;
    BITMAP icon;
}BUTTON;

class X11_Engine
{
private:
    Display *dpy;
    int main_screen;
    Window main_window;
    XEvent main_event;
    std::vector<BUTTON>buttons;
    std::vector<Window>inputs;
    SHELL *shell;
public:
    X11_Engine(std::string path);
    X11_Engine();
    ~X11_Engine();

    void init_server();//D
    void create_main_window();//D
    void start_loop();//D
    void create_main_buttons();//D
    void draw_grid();//D
    void clear_screen(Window&window);//D
    void draw_main_icons_buttons();//P
    void draw_text_buttons();//P
    void add_icon_to_button(Window &window,std::string path);//P


    Window create_window(int x,int y, int w,int h,XColor bg,XColor bd,bool is_child);//D
    void map_window_to_display(Window &window);//D
    void map_events_to_window(Window &window,long flags);//D
    void create_button(int x,int y, int w, int h,long flags,std::string text);//D
    void draw_icon(Window &window,int x,int y);//P
    void draw_text(Window &window,std::string text,int x,int y);//D
    void draw_line(Window &window,int x1,int y1,int x2, int y2);//D
    XColor create_color(std::string color);//D
    void change_title(std::string text);//D
    GC create_graphic_context(Window &window);//D
    BITMAP create_bitmap_object(Window &window,std::string path);//D
};


#endif