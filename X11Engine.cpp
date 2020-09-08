#include "X11Engine.h"
#include <iostream>
#include <X11/Xutil.h>

#include "constants.h"

X11_Engine::X11_Engine(){ shell = new SHELL();}

X11_Engine::X11_Engine(std::string path){ shell = new SHELL(path);}

X11_Engine::~X11_Engine(){ free(shell); }

void X11_Engine::init_server(){
    dpy = XOpenDisplay(NULL);
    if(dpy==NULL){
        std::cout<<"Error al abrir Display"<<std::endl;
    }
    main_screen = DefaultScreen(dpy);
}
Window X11_Engine::create_window(int x,int y, int w,int h,XColor bg,XColor bd,bool is_child){
    if(!is_child){
        return XCreateSimpleWindow(dpy,RootWindow(dpy,main_screen),x,y,w,h,1,bd.pixel,bg.pixel);
    }
    return XCreateSimpleWindow(dpy,main_window,x,y,w,h,1,bd.pixel,bg.pixel);
}
void X11_Engine::create_main_window(){
   
    XColor color = create_color(WINDOW_COLOR);
    main_window = create_window(20,20,DEFAULT_WINDOW_WIDTH,DEFAULT_WINDOW_HEIGHT,color,color,false);
    map_events_to_window(main_window,ExposureMask|KeyPressMask);
    map_window_to_display(main_window);
    change_title(TITLE);
    
}

void X11_Engine::map_events_to_window(Window &window,long flags){
    XSelectInput(dpy,window,flags);
}

void X11_Engine::map_window_to_display(Window &window){
    XMapWindow(dpy,window);
}

void X11_Engine::start_loop(){
    init_server();
    create_main_window();
    create_main_buttons();
    while (1)
    {
        XNextEvent(dpy,&main_event);
        if(main_event.type = Expose){
            draw_grid();
            draw_text_buttons();
            draw_icon(buttons[0].button,20,10);
        }
    }
    XFlush(dpy);
    XCloseDisplay(dpy);
}
void X11_Engine::draw_grid(){
    draw_line(main_window,init_x,init_y,init_x,DEFAULT_WINDOW_HEIGHT);
    draw_line(main_window,init_x2,init_y2,DEFAULT_WINDOW_WIDTH,init_y2);
}

void X11_Engine::draw_line(Window &window,int x1,int y1,int x2, int y2){
    GC gc = create_graphic_context(window);
    XColor color = create_color(LINE_COLOR);
    XSetForeground(dpy,gc,color.pixel);
    XDrawLine(dpy,window,gc,x1,y1,x2,y2);
}

void X11_Engine::create_button(int x,int y, int w, int h,long flags,std::string text){
    XColor color = create_color(BUTTON_COLOR);
    XColor b_color = create_color(BUTTON_BORDER_COLOR);
    Window button = create_window(x,y,w,h,color,b_color,true);
    map_events_to_window(button,flags);
    map_window_to_display(button);

    BUTTON btn;
    btn.button = button;
    btn.text = text;
    buttons.push_back(btn);
}

void X11_Engine::draw_icon(Window &window,int x,int y){
    BITMAP bmp = create_bitmap_object(window,"./img/file.xbm");
    XColor c = create_color(BUTTON_COLOR);
    XSetForeground(dpy,bmp.gc,bmp.color.pixel);
    XSetBackground(dpy,bmp.gc,c.pixel);
    XCopyPlane(dpy,bmp.bitmap,window,bmp.gc,0,0,bmp.bmp_w,bmp.bmp_h,x,y,1);
}
void X11_Engine::draw_text(Window &window,std::string text,int x,int y){
    XFontStruct *font = XLoadQueryFont(dpy,"fixed");
    GC gc = create_graphic_context(window);
    XColor color = create_color("#ffffff");
    XSetForeground(dpy,gc,color.pixel);
    XSetFont(dpy,DefaultGC(dpy,main_screen),font->fid);
    XDrawString(dpy,window,gc,x,y,text.c_str(),text.size());
}

XColor X11_Engine::create_color(std::string color){
    XColor c;
    XParseColor(dpy,DefaultColormap(dpy,main_screen),color.c_str(),&c);
    XAllocColor(dpy,DefaultColormap(dpy,main_screen),&c);
    return c;
}

void X11_Engine::change_title(std::string text){
    XStoreName(dpy,main_window,text.c_str());
}

GC X11_Engine::create_graphic_context(Window &window){
    return XCreateGC(dpy,window,0,NULL);
}

BITMAP X11_Engine::create_bitmap_object(Window &window,std::string path){
    BITMAP bmp;
    XReadBitmapFile(dpy,window,path.c_str(),&bmp.bmp_w,&bmp.bmp_h,&bmp.bitmap,&bmp.hp_x,&bmp.hp_y);
    GC gc = create_graphic_context(window);
    XColor color = create_color("#ffffff");
    bmp.gc = gc;
    bmp.color = color;
    return bmp;
}

void X11_Engine::clear_screen(Window&window){
    XClearWindow(dpy,window);
}

void X11_Engine::create_main_buttons(){
    int yposition = 0;
    create_button(0,yposition,DEFAULT_ICON_BUTTON_WIDTH,DEFAULT_ICON_BUTTON_HEIGHT,ExposureMask|KeyPressMask,"Home");
    yposition+=DEFAULT_ICON_BUTTON_HEIGHT;
    create_button(0,yposition,DEFAULT_ICON_BUTTON_WIDTH,DEFAULT_ICON_BUTTON_HEIGHT,ExposureMask|KeyPressMask,"Documents");
    yposition+=DEFAULT_ICON_BUTTON_HEIGHT;
    create_button(0,yposition,DEFAULT_ICON_BUTTON_WIDTH,DEFAULT_ICON_BUTTON_HEIGHT,ExposureMask|KeyPressMask,"Desktop");
    yposition+=DEFAULT_ICON_BUTTON_HEIGHT;
    create_button(0,yposition,DEFAULT_ICON_BUTTON_WIDTH,DEFAULT_ICON_BUTTON_HEIGHT-2,ExposureMask|KeyPressMask,"Downloads");

    /*BACK AND FRONT BUTTONS*/
    create_button(init_x2+10,3,BACK_BUTTON_WIDTH,BACK_BUTTON_HEIGHT,ExposureMask|KeyPressMask,"Back");
    create_button(170+BACK_BUTTON_WIDTH,3,BACK_BUTTON_WIDTH,BACK_BUTTON_HEIGHT,ExposureMask|KeyPressMask,"Front");
}

void X11_Engine::draw_text_buttons(){
    for (int i = 0; i < buttons.size()-2; i++)
    {
        draw_text(buttons[i].button,buttons[i].text,20,DEFAULT_ICON_BUTTON_WIDTH/2);
    }
    
}