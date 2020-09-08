#include "shell.h"
#include "filesapi.h"
#include "pathutilities.h"
#include <iostream>

SHELL::SHELL(){
    navigation = convert_path_to_stack(getenv("HOME"));
}

SHELL::SHELL(std::string initial_path){
    navigation = convert_path_to_stack(initial_path);
}

void SHELL::ls(){
    std::string path = convert_stack_to_path(navigation);
    std::vector<Directory> dirs;
    std::vector<File> files;

    dirs = get_directories_from_path(path);
    files = get_files_from_path(path);

    for (int i = 0; i < dirs.size(); i++)
    {
        std::cout<<"D "<< dirs[i].name<<std::endl;
    }
    
    for (int i = 0; i < files.size(); i++)
    {
        std::cout<<files[i].name<<std::endl;
    }
    
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
void SHELL::cd(std::string path){
    if(!navigation.empty()){
        if(path == ".."){
            navigation.pop_back();
            return;
        }
    }
    if(is_path(path)){
        navigation = convert_path_to_stack(path);
    }else{
        navigation.push_back(path);
    }
}
void SHELL::mkdir(std::string dir_name){
    std::string path = convert_stack_to_path(navigation);
    path+=dir_name;
    make_directory(path);
}

void SHELL::mkHlink(std::string source,std::string link){
    std::string path = convert_stack_to_path(navigation);
    create_hard_link(path+link,path+source);
}

void SHELL::mkSlink(std::string source,std::string link){
    std::string path = convert_stack_to_path(navigation);
    create_symbolic_link(path+link,source);
}