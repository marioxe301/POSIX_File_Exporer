#include "shell.h"
#include "filesapi.h"
#include "pathutilities.h"
#include <iostream>

SHELL::SHELL(){
    //Nota poner siempre una pleca al final
    navigation = convert_path_to_stack("/home/mgfe/Documentos/SISO2_Project/rootfld/usr/home/");
}

SHELL::SHELL(std::string initial_path){
    navigation = convert_path_to_stack(initial_path);
}

void SHELL::ls(std::vector<std::string>&files,std::vector<std::string>&folder){
    std::string path = convert_stack_to_path(navigation);
    files = get_files_from_path(path);
    folder = get_directories_from_path(path);
}
void SHELL::touch(std::string filename){
    std::string path = convert_stack_to_path(navigation);
    path+= filename;
    create_file(path);
}
void SHELL::rm_file(std::string filename){
    std::string path = convert_stack_to_path(navigation);
    path+= filename;
    remove_file(path);
}
void SHELL::rm_dir(std::string dirname){
    std::string path = convert_stack_to_path(navigation);
    path+= dirname;
    remove_directory(path);
}
void SHELL::cd(std::string path){
    if(!navigation.empty()){
        if(path == ".."){
            if(navigation.back() == "rootfld"){return;}
            navigation.pop_back();
            return;
        }
    }
    if(is_path(path)){
        navigation = convert_path_to_stack(path);
        return;
    }else{
        navigation.push_back(path);
        return;
    }
}
void SHELL::mkdir(std::string dir_name){
    std::string path = convert_stack_to_path(navigation);
    path+=dir_name;
    make_directory(path);
}

void SHELL::mkHlink(std::string source,std::string link){
    create_hard_link(link,source);
}

void SHELL::mkSlink(std::string source,std::string link){
    create_symbolic_link(link,source);
}

std::string SHELL::get_folder_name(){
    return navigation.back();
}

void SHELL::open(std::string filename){
    std::string path = convert_stack_to_path(navigation);
    open_file(path+filename);
}

std::string SHELL::get_absolute_path(){
    std::string path = convert_stack_to_path(navigation);
    return path;
}

void SHELL::cp_file(std::string from,std::string to){
    copy_file(from,to);
}
void SHELL::cp_folder(std::string from,std::string to){
    copy_directory(from,to);
}
void SHELL::mv_file(std::string from,std::string to){
    move_file(from,to);
}
void SHELL::mv_folder(std::string from,std::string to){
    move_directory(from,to);
}