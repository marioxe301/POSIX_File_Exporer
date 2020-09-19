#include "X11Engine.h"
#include <iostream>
#include <X11/Xutil.h>

#include "constants.h"

X11_Engine::X11_Engine()
{ 
    shell = new SHELL();
    download_path = "/home/mgfe/Documentos/SISO2_Project/rootfld/usr/home/Downloads/";
    documents_path = "/home/mgfe/Documentos/SISO2_Project/rootfld/usr/home/Documents/";
    desktop_path = "/home/mgfe/Documentos/SISO2_Project/rootfld/usr/home/Desktop/";
    home_path = "/home/mgfe/Documentos/SISO2_Project/rootfld/usr/home/";
    created = false;
    click_counter = 0;
}

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
    map_events_to_window(main_window,ExposureMask|KeyPressMask| KeyReleaseMask);
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

    draw_button_file(true);
    change_title(TITLE +shell->get_folder_name());
    create_text_box();
    while (1)
    {
        XNextEvent(dpy,&main_event);
        if(main_event.xany.window == main_window){
            if(main_event.type == Expose){
                draw_icons_in_buttons();
                draw_text_in_buttons();
                draw_grid();
                draw_icon_button_file();
                draw_text_button_file();
                draw_icon_button_file();
                draw_text_button_file();
                change_text();
                change_title(TITLE +shell->get_folder_name());
            }
            if(main_event.type == KeyPress){
                char tmp[2];
                KeySym keysym;
                XLookupString(&main_event.xkey,tmp,2,&keysym,NULL);
                if(tmp[0]== '\b' && !input_text.empty()){
                    input_text.pop_back();
                }else if(tmp[0] == '\000'){
                    created = true;
                    clear_button_file();
                    draw_button_file(created);
                    change_title(TITLE +shell->get_folder_name());
                    created = false;
                }
                else if(tmp[0]!='\b'){ 
                    input_text+=tmp[0]; 
                }
                change_text();
            }
        }
        event_button_pressed(main_event);
        file_button_pressed(main_event);
    }
    XFlush(dpy);
    XCloseDisplay(dpy);
}
void X11_Engine::draw_grid(){
    draw_line(main_window,init_x2,init_y2,DEFAULT_WINDOW_WIDTH,init_y2);
    draw_line(main_window,950,50,950,400);
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

    /*actions buttons*/
    yposition = INITIAL_ACTION_Y+ ACTIONS_MARGIN_VERTICAL;
    create_button(ACTIONS_MARGIN_SIDE+INITIAL_ACTION_X,yposition,ACTIONS_SIZE,ACTIONS_SIZE,ExposureMask|KeyPressMask|ButtonPressMask|ButtonReleaseMask,"New File");
    yposition+= ACTIONS_SIZE + ACTIONS_MARGIN_VERTICAL;
    create_button(ACTIONS_MARGIN_SIDE+INITIAL_ACTION_X,yposition,ACTIONS_SIZE,ACTIONS_SIZE,ExposureMask|KeyPressMask|ButtonPressMask|ButtonReleaseMask,"New Folder");
    yposition+= ACTIONS_SIZE + ACTIONS_MARGIN_VERTICAL;
    create_button(ACTIONS_MARGIN_SIDE+INITIAL_ACTION_X,yposition,ACTIONS_SIZE,ACTIONS_SIZE,ExposureMask|KeyPressMask|ButtonPressMask|ButtonReleaseMask,"Copy");
    yposition+= ACTIONS_SIZE + ACTIONS_MARGIN_VERTICAL;
    create_button(ACTIONS_MARGIN_SIDE+INITIAL_ACTION_X,yposition,ACTIONS_SIZE,ACTIONS_SIZE,ExposureMask|KeyPressMask|ButtonPressMask|ButtonReleaseMask,"Cut");
    yposition+= ACTIONS_SIZE + ACTIONS_MARGIN_VERTICAL;
    create_button(ACTIONS_MARGIN_SIDE+INITIAL_ACTION_X,yposition,ACTIONS_SIZE,ACTIONS_SIZE,ExposureMask|KeyPressMask|ButtonPressMask|ButtonReleaseMask,"Paste");
    yposition+= ACTIONS_SIZE + ACTIONS_MARGIN_VERTICAL;
    create_button(ACTIONS_MARGIN_SIDE+INITIAL_ACTION_X,yposition,ACTIONS_SIZE,ACTIONS_SIZE,ExposureMask|KeyPressMask|ButtonPressMask|ButtonReleaseMask,"Hard Link");
    yposition+= ACTIONS_SIZE + ACTIONS_MARGIN_VERTICAL;
    create_button(ACTIONS_MARGIN_SIDE+INITIAL_ACTION_X,yposition,ACTIONS_SIZE,ACTIONS_SIZE,ExposureMask|KeyPressMask|ButtonPressMask|ButtonReleaseMask,"Symbolic Link");
    yposition+= ACTIONS_SIZE + ACTIONS_MARGIN_VERTICAL;
    create_button(ACTIONS_MARGIN_SIDE+INITIAL_ACTION_X,yposition,ACTIONS_SIZE,ACTIONS_SIZE,ExposureMask|KeyPressMask|ButtonPressMask|ButtonReleaseMask,"Delete");
}

void X11_Engine::add_icon_to_buttons(){
    std::string icons[]={"./img/home.xbm","./img/folder.xbm","./img/desktop.xbm","./img/downloads.xbm","./img/back.xbm","./img/new_file.xbm","./img/new_folder.xbm","./img/copy.xbm","./img/cut.xbm","./img/paste.xbm","./img/hard_link.xbm","./img/sym_link.xbm","./img/delete.xbm"};
    for (int i = 0; i < buttons.size(); i++)
    {
        BITMAP bmp = create_bitmap_object(buttons[i].button,icons[i]);
        buttons[i].icon = bmp;
    }
}

void X11_Engine::draw_icons_in_buttons(){
    for (int i = 0; i < 4; i++)
    {
        draw_icon(buttons[i].button,10,(DEFAULT_ICON_BUTTON_HEIGHT/2)-20,buttons[i].icon);
    }

    draw_icon(buttons[4].button,5,5,buttons[4].icon);

    for (int i = 5; i < buttons.size(); i++)
    {
        draw_icon(buttons[i].button,3,2,buttons[i].icon);
    }
    
}

void X11_Engine::draw_text_in_buttons(){
    for (int i = 0; i < buttons.size()-1; i++)
    {
        draw_text(buttons[i].button,buttons[i].text,50,DEFAULT_ICON_BUTTON_HEIGHT/2);
    }
    
}

void X11_Engine::event_button_pressed(XEvent &event){
    for (int i = 0; i < 5; i++)
    {   
        if(event.xany.window == buttons[i].button){
            if(event.type == ButtonPress){
                if(event.xbutton.button == Button1){
                    std::cout<<buttons[i].text + " Pressed"<<std::endl;
                    created = true;
                }
            }
            if(event.type == ButtonRelease){
                if(event.xbutton.button == Button1){
                    if(buttons[i].text == "Home"){
                        shell->cd(home_path);
                    }else if(buttons[i].text == "Documents"){
                        shell->cd(documents_path);
                    }else if(buttons[i].text == "Desktop"){
                        shell->cd(desktop_path);
                    }else if(buttons[i].text == "Downloads"){
                        shell->cd(download_path);
                    }else if(buttons[i].text == "Back"){
                        shell->cd("..");
                    }

                    std::cout<<buttons[i].text + " Released"<<std::endl;
                    clear_button_file();
                    draw_button_file(created);
                    change_title(TITLE +shell->get_folder_name());
                
                    created = false;
                }
            }
        }
    }
    /*Botones de Acciones*/
    for (int i = 5; i < buttons.size(); i++)
    {
        if(event.xany.window == buttons[i].button){
            if(event.type == ButtonPress){
                if(event.xbutton.button == Button1){
                    created = true;
                    std::cout<<buttons[i].text + " Pressed"<<std::endl;
                }
            }
            
            if(event.type == ButtonRelease){
                if(event.xbutton.button == Button1){
                    if(buttons[i].text == "New File" && !input_text.empty()){
                        shell->touch(input_text);
                        input_text="";
                        change_text();
                    }else if(buttons[i].text == "New Folder" && !input_text.empty()){
                        shell->mkdir(input_text);
                        input_text="";
                        change_text();
                    }else if(buttons[i].text == "Delete" && !selected_item.empty()){
                        if(is_directory){
                            shell->rm_dir(selected_item);
                        }else{
                            shell->rm_file(selected_item);
                        }
                    }else if((buttons[i].text == "Copy") && !selected_item.empty()){
                        from_path = shell->get_absolute_path() + selected_item;
                        std::cout<< "From_path :"+from_path<<std::endl;
                        is_copy = true;
                        is_cut = false;
                    }else if(buttons[i].text == "Cut" && !selected_item.empty()){
                        from_path = shell->get_absolute_path() + selected_item;
                        std::cout<< "From_path :"+from_path<<std::endl;
                        is_copy = false;
                        is_cut = true;
                    }else if(buttons[i].text == "Paste" && !selected_item.empty() && !from_path.empty()){
                        if(is_directory && is_copy){

                            to_path = shell->get_absolute_path()+"copia_f";
                            shell->cp_folder(from_path,to_path);
                        }else if(is_directory && is_cut){
                            to_path = shell->get_absolute_path() + selected_item;
                            shell->mv_folder(from_path,to_path);
                        }else if( !is_directory && is_copy){
                            to_path = shell->get_absolute_path()+ selected_item;
                            shell->cp_file(from_path,to_path);
                        }else if(!is_directory && is_cut){
                            to_path = shell->get_absolute_path()+selected_item;
                            shell->mv_file(from_path,to_path);
                        }
                        std::cout<< "to_path :"+to_path<<std::endl;
                    }else if(buttons[i].text == "Hard Link" && !selected_item.empty() && !input_text.empty() && !from_path.empty()){
                        if(is_directory){
                            std::cout<<"No se puede realizar links duros en Carpeta"<<std::endl;
                        }else{
                            to_path = shell->get_absolute_path() + input_text;
                            shell->mkHlink(from_path,to_path);
                        }
                    }else if(buttons[i].text == "Symbolic Link" && !selected_item.empty() && !input_text.empty() && !from_path.empty()){
                        to_path = shell->get_absolute_path()+ input_text;
                        shell->mkSlink(from_path,to_path);
                    }
                    clear_button_file();
                    draw_button_file(created);
                    std::cout<<buttons[i].text + " Released"<<std::endl;
                    created = false;
                    input_text.empty();
                }
            }
        }
    }
    
    
}
void X11_Engine::create_text_box(){
    XColor color = create_color(BUTTON_COLOR);
    XColor b_color = create_color(BUTTON_BORDER_COLOR);
    text_box = create_window(init_x2 + MARGIN_FILE + BACK_BUTTON_WIDTH+ MARGIN_FILE +3,3,TEXT_BOX_WIDTH,TEXT_BOX_HEIGTH,color,b_color,true);
    map_events_to_window(text_box,ExposureMask|KeyPressMask| KeyReleaseMask);
    map_window_to_display(text_box);
}

void X11_Engine::change_text(){
    XClearWindow(dpy,text_box);
    draw_text(text_box,TEXTBOX_NAME+input_text,10,25);
}

/*
    Funciones referentes a todo lo relacionado con lo visual
    de los archivos
*/
void X11_Engine::get_files(std::vector<std::string> &files,std::vector<std::string> &directories){
    shell->ls(files,directories);
}

BUTTON X11_Engine::create_button_file(int x,int y, int w, int h,long flags,std::string text){
    XColor color = create_color(BUTTON_COLOR);
    XColor b_color = create_color(BUTTON_BORDER_COLOR);
    Window button = create_window(x,y,w,h,color,b_color,true);
    map_events_to_window(button,flags);
    map_window_to_display(button);

    BUTTON btn;
    btn.button = button;
    btn.text = text;
    return btn;
}
void X11_Engine::draw_button_file(bool flag){
    // No dibujaria si no es falso
    if(!flag){return;}
    
    int control = 0;
    int x = INITIAL_X_FILE + MARGIN_FILE;
    int y = INITIAL_Y_FILE + MARGIN_FILE;
    
    get_files(_files,_directories);

    /*Remover duplicados*/
    for (int i = 0; i < _directories.size(); i++)
    {
        std::string tmp = _directories[i];
        for(int j=0;j<_files.size(); j++){
            if(tmp == _files[j]){
                _files.erase(_files.begin()+j);
            }
        }
    }
    
    FILEVIEW fv;
    for (int i = 0; i < _directories.size(); i++)
    {
        if(control == 7){
            y+= FILE_SIZE + MARGIN_FILE;
            x = INITIAL_X_FILE + MARGIN_FILE;
            control = 0;
        }
        fv.file = create_button_file(x,y,FILE_SIZE,FILE_SIZE,ExposureMask|KeyPressMask|ButtonPressMask|ButtonReleaseMask,_directories[i]);
        fv.type = 0;
        fv.file.icon = create_bitmap_object(fv.file.button,"./img/folder.xbm");
        files_in_path.push_back(fv);
        x+= FILE_SIZE + MARGIN_FILE;
        control++;
    }

    for (int i = 0; i < _files.size(); i++)
    {
        if( control == 8){
            y+= FILE_SIZE + MARGIN_FILE;
            x = INITIAL_X_FILE + MARGIN_FILE;
            control = 0;
        }
        fv.file = create_button_file(x,y,FILE_SIZE,FILE_SIZE,ExposureMask|KeyPressMask|ButtonPressMask|ButtonReleaseMask,_files[i]);
        fv.type = 1;
        fv.file.icon = create_bitmap_object(fv.file.button,"./img/file.xbm");
        files_in_path.push_back(fv);
        x+= FILE_SIZE + MARGIN_FILE;
        control++;

    }
    
}

void X11_Engine::draw_icon_button_file(){
    if(files_in_path.empty()){return;}
    for (int i = 0; i < files_in_path.size(); i++)
    {
        draw_icon(files_in_path[i].file.button,20,10,files_in_path[i].file.icon);
    }
    
}
void X11_Engine::draw_text_button_file(){
    if(files_in_path.empty()){return;}
    for (int i = 0; i < files_in_path.size(); i++)
    {
        draw_text(files_in_path[i].file.button,files_in_path[i].file.text,10,70);
    }
}

void X11_Engine::clear_button_file(){
    for (int i = 0; i < files_in_path.size(); i++)
    {
        XDestroyWindow(dpy,files_in_path[i].file.button);
    }

    files_in_path.clear();
}

void X11_Engine::file_button_pressed(XEvent &event){
    for (int i = 0; i < files_in_path.size(); i++)
    {
        if(files_in_path[i].type == 0){
            if(event.xany.window == files_in_path[i].file.button){
                if(event.type == ButtonPress){
                    if(event.xbutton.button == Button1){
                        std::cout<<files_in_path[i].file.text+" Pressed"<<std::endl;
                        created = true;
                    }

                }
                if(event.type == ButtonRelease){
                    if(event.xbutton.button == Button1){
                        shell->cd(files_in_path[i].file.text);
                        std::cout<<files_in_path[i].file.text+" Released"<<std::endl;
                        clear_button_file();
                        draw_button_file(created);
                        change_title(TITLE +shell->get_folder_name());
                        created = false;
                    }
                    if(event.xbutton.button == Button3){
                        selected_item = files_in_path[i].file.text;
                        std::cout<<files_in_path[i].file.text+" Selected"<<std::endl;
                        is_directory = true;
                    }
                }
            }
        }else{
            if(event.xany.window == files_in_path[i].file.button){
                if(event.type == ButtonPress){
                    if(event.xbutton.button == Button3){ 
                    }
                    if(event.xbutton.button == Button1){
                        click_counter++;
                    }
                }
                if(event.type == ButtonRelease){
                    if(event.xbutton.button == Button3){
                        selected_item = files_in_path[i].file.text;
                        std::cout<<files_in_path[i].file.text+" Selected"<<std::endl;
                        is_directory = false;
                    }
                    if(event.xbutton.button == Button1){
                        if(click_counter >= 2){
                            selected_item = files_in_path[i].file.text;
                            std::cout<<"Opening "+files_in_path[i].file.text<<std::endl;
                            shell->open(selected_item);
                            click_counter = 0;
                        }
                    }
                }
            }
        }
    }
    
}