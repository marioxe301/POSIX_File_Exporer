#include "filesapi.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <experimental/filesystem>

void create_file(std::string filename){
    int fd;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    if(creat(filename.c_str(),mode)!=-1){
        std::cout<< "Archivo Creado"<<std::endl;
        return;
    }
    std::cout<< "Fallo al crear el Archivo "<<std::endl;
}

void make_directory(std::string directory){
    mode_t mode = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ;
    if(mkdir(directory.c_str(),mode)!= -1){
         std::cout<< "Carpeta Creada"<<std::endl;
        return;
    }
    std::cout<< "Fallo al crear Carpeta "<<std::endl;
}

void remove_file(std::string filename){
    if(remove(filename.c_str())!=-1){
         std::cout<< "Archivo Removido: "<<filename<<std::endl;
        return;
    }
    std::cout<< "Fallo al eliminar Archivo"<<std::endl;
}
void remove_directory(std::string directory){
    std::experimental::filesystem::remove_all(directory);
    std::cout<< "Carpeta Removida: "<<directory<<std::endl;
}
void create_hard_link(std::string link_name,std::string source){
    if(link(source.c_str(),link_name.c_str())!=-1){
         std::cout<< "Link Duro Creado"<<std::endl;
        return;
    }
    std::cout<< "Fallo al crear Link Duro "<<std::endl;
}
void create_symbolic_link(std:: string link_name, std::string source){
    if(symlink(source.c_str(),link_name.c_str())!= -1){
         std::cout<< "Link Simbolico Creado"<<std::endl;
        return;
    }
    std::cout<< "Fallo al crear Link Symbolico "<<std::endl;
}
void open_file(std::string filename){
    char *program = new char[ filename.size() + 5];
    strcpy(program,"code ");
    strcat(program,filename.c_str());

    system(program);
}

std::vector<std::string> get_files_from_path(std::string path){
    std::vector<std::string> files;
    DIR * dir_pointer = opendir(path.c_str());
    struct dirent *dp;
    while((dp = readdir(dir_pointer)) != NULL){
        if(dp->d_name[0]!='.'){
            if(dp->d_type == DT_REG ){
                std::string _name(dp->d_name);
                files.push_back(_name);
            }
        }
    }
    closedir(dir_pointer);
    return files;
}
std::vector<std::string> get_directories_from_path(std::string path){
    std::vector<std::string> directories;
    DIR * dir_pointer = opendir(path.c_str());
    struct dirent *dp;
    while((dp = readdir(dir_pointer)) != NULL){
        if(dp->d_name[0]!='.'){
            if(dp->d_type == DT_DIR){
                std::string _name(dp->d_name);
                directories.push_back(_name);
            }
        }
    }
    closedir(dir_pointer);
    return directories;
}

void copy_file(std::string filename,std::string newpath){

}

void copy_directory(std::string dirname, std::string newpath){

}

void move_file(std::string filename,std::string newpath){

}

void move_directory(std::string dirname, std::string newpath){

}