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
    add_icon_to_buttons();
    while (1)
    {
        XNextEvent(dpy,&main_event);
        if(main_event.xany.window == main_window){
            if(main_event.type == Expose){
                draw_grid();
                /*draw_text_in_buttons();
                draw_icons_in_buttons();*/
            }
        }
        event_button_pressed(main_event);
    }
    XFlush(dpy);
    XCloseDisplay(dpy);
}
void X11_Engine::draw_grid(){
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
    GC gc = create_graphic_context(button);

    BUTTON btn;
    btn.button = button;
    btn.text = text;
    btn.gc = gc;
    buttons.push_back(btn);
}

void X11_Engine::draw_icon(Window &window,int x,int y,BITMAP &bmp){
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
    create_button(0,yposition,DEFAULT_ICON_BUTTON_WIDTH,DEFAULT_ICON_BUTTON_HEIGHT,ExposureMask|KeyPressMask|ButtonPressMask|ButtonReleaseMask,"Home");
    yposition+=DEFAULT_ICON_BUTTON_HEIGHT;
    create_button(0,yposition,DEFAULT_ICON_BUTTON_WIDTH,DEFAULT_ICON_BUTTON_HEIGHT,ExposureMask|KeyPressMask|ButtonPressMask|ButtonReleaseMask,"Documents");
    yposition+=DEFAULT_ICON_BUTTON_HEIGHT;
    create_button(0,yposition,DEFAULT_ICON_BUTTON_WIDTH,DEFAULT_ICON_BUTTON_HEIGHT,ExposureMask|KeyPressMask|ButtonPressMask|ButtonReleaseMask,"Desktop");
    yposition+=DEFAULT_ICON_BUTTON_HEIGHT;
    create_button(0,yposition,DEFAULT_ICON_BUTTON_WIDTH,DEFAULT_ICON_BUTTON_HEIGHT-2,ExposureMask|KeyPressMask|ButtonPressMask|ButtonReleaseMask,"Downloads");

    /*BACK AND FRONT BUTTONS*/
    create_button(init_x2+10,3,BACK_BUTTON_WIDTH,BACK_BUTTON_HEIGHT,ExposureMask|KeyPressMask|ButtonPressMask|ButtonReleaseMask,"Back");
    create_button(170+BACK_BUTTON_WIDTH,3,BACK_BUTTON_WIDTH,BACK_BUTTON_HEIGHT,ExposureMask|KeyPressMask|ButtonPressMask|ButtonReleaseMask,"Front");
}

void X11_Engine::add_icon_to_buttons(){
    std::string icons[]={"./img/home.xbm","./img/folder.xbm","./img/desktop.xbm","./img/downloads.xbm","./img/back.xbm","./img/front.xbm"};
    for (int i = 0; i < buttons.size(); i++)
    {
        BITMAP bmp = create_bitmap_object(buttons[i].button,icons[i]);
        buttons[i].icon = bmp;
    }
}

void X11_Engine::draw_icons_in_buttons(){
    for (int i = 0; i < buttons.size()-2; i++)
    {
        draw_icon(buttons[i].button,10,(DEFAULT_ICON_BUTTON_HEIGHT/2)-20,buttons[i].icon);
    }

    draw_icon(buttons[4].button,5,5,buttons[4].icon);
    draw_icon(buttons[5].button,10,5,buttons[5].icon);

}

void X11_Engine::draw_text_in_buttons(){
    for (int i = 0; i < buttons.size()-2; i++)
    {
        draw_text(buttons[i].button,buttons[i].text,50,DEFAULT_ICON_BUTTON_HEIGHT/2);
    }
    
}

void X11_Engine::change_button_color(BUTTON &button){
    XColor color = create_color(PRESSED_BUTTON_COLOR);
    XSetWindowBackground(dpy,button.button,color.pixel);
}
void X11_Engine::change_default_button_color(BUTTON &button){
    XColor color = create_color(BUTTON_COLOR);
    XSetWindowBackground(dpy,button.button,color.pixel);
}

void X11_Engine::event_button_pressed(XEvent &event){
    for (int i = 0; i < buttons.size(); i++)
    {   
        if(event.xany.window == buttons[i].button){
            if(event.type == Expose){
                draw_icons_in_buttons();
                draw_text_in_buttons();
            }
            if(event.type == ButtonPress){
                if(event.xbutton.button == 1){
                    change_button_color(buttons[i]);
                    std::cout<<"P"<<std::endl;
                }
            }
            if(event.type == ButtonRelease){
                if(event.xbutton.button == 1){
                    change_button_color(buttons[i]);
                    std::cout<<"R"<<std::endl;
                }
            }
        }
    }
    
}