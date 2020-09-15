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

typedef struct{
    BUTTON file;
    int type;
}FILEVIEW;

class X11_Engine
{
private:
    Display *dpy;
    int main_screen;
    Window main_window;
    XEvent main_event;
    std::vector<BUTTON>buttons;
    SHELL *shell; // Encargado de toda la funcionalidad del movimiento
    std::string download_path;
    std::string documents_path;
    std::string desktop_path;
    std::string home_path;

    std::string selected_item;
    bool is_directory;

    std::vector<FILEVIEW>files_in_path;

    std::vector<std::string>_files;
    std::vector<std::string>_directories;

    std::string input_text;
    Window text_box;
    bool created;

public:
    X11_Engine(std::string path);
    X11_Engine();
    ~X11_Engine();

    void init_server();// Inicializa el servidor
    void create_main_window();// Crea la ventana principal
    void start_loop();// Ejecuta la aplicacion
    void create_main_buttons();// Crea los botones principales
    void draw_grid();// Dibuja los limitantes
    void draw_icons_in_buttons();//Dibuja los iconos de los botones principales
    void draw_text_in_buttons();// Dibuja el texto de los botones principales
    void add_icon_to_buttons();// Dibuja los iconos de los botones principales


    Window create_window(int x,int y, int w,int h,XColor bg,XColor bd,bool is_child);// Crea una ventana con el flag si es un ventana padre o hijo
    void map_window_to_display(Window &window);// mapea la ventana con el display
    void map_events_to_window(Window &window,long flags);// mapea los eventos a la ventana
    void create_button(int x,int y, int w, int h,long flags,std::string text);// Funcion encargada de crear botones principales
    void draw_icon(Window &window,int x,int y,BITMAP &bmp);// dibuja un icono dentro de un ventana
    void draw_text(Window &window,std::string text,int x,int y);// dibuja un texto dentro de una ventana
    void draw_line(Window &window,int x1,int y1,int x2, int y2);// dibuja una linea dentro de una ventana
    XColor create_color(std::string color);// Crea un objeto de color basado en un valor HEX
    void change_title(std::string text);// Cambia el titulo de la ventana
    GC create_graphic_context(Window &window);// Crea un contexto grafico para poder definir colores
    BITMAP create_bitmap_object(Window &window,std::string path);// Crea un objeto de tipo BITMAP basado en la ruta
    void event_button_pressed(XEvent &event);// Registra los eventos click de los botones principales
    void create_text_box();
    void change_text();

    //Funciones de los archivos ( Buttones cambiantes )
    void get_files(std::vector<std::string> &files,std::vector<std::string> &directories);
    BUTTON create_button_file(int x,int y, int w, int h,long flags,std::string text);
    void draw_button_file(bool flag);
    void draw_icon_button_file();
    void draw_text_button_file();
    void clear_button_file();
    void file_button_pressed(XEvent &event);

    
};


#endif