#include "filesapi.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <experimental/filesystem>
#include "constants.h"

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
    char *program = new char[ filename.size() + 9];
    strcpy(program,"xdg-open ");
    strcat(program,filename.c_str());

    system(program);
    free(program);
}

std::vector<std::string> get_files_from_path(std::string path){
    std::vector<std::string> files;
    DIR * dir_pointer = opendir(path.c_str());
    struct dirent *dp;
    while((dp = readdir(dir_pointer)) != NULL){
        if(dp->d_name[0]!='.'){
            if(dp->d_type == DT_REG | DT_LNK ){
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
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP| S_IWGRP | S_IWOTH | S_IROTH;
    int src, dest,in ,out;
    char buffer[BUFFER_SIZE];
    src = open(filename.c_str(), O_RDONLY); if(src<0){std::cout<<"Error al abrir el archivo"<<std::endl; return;}
    dest = creat(newpath.c_str(),mode); if (dest < 0){ std::cout<<"Error al crear archivo"<<std::endl;return;}

    while (true)
    {
        in = read(src,buffer,BUFFER_SIZE);
        if (in <= 0) break;
        out = write(dest,buffer,in);
        if(out<=0) break;
    }

    close(src);
    close(dest);
}

void copy_directory(std::string dirname, std::string newpath){
    std::experimental::filesystem::copy(dirname,newpath, std::experimental::filesystem::copy_options::recursive | std::experimental::filesystem::copy_options::overwrite_existing );
    std::cout<<"Carpeta Copiada"<<std::endl;
}

void move_file(std::string filename,std::string newpath){
    if(rename(filename.c_str(),newpath.c_str())!= -1){
        std::cout<<"Archivo Movido"<<std::endl;
        return;
    }
    std::cout<<"Error al mover Archivo"<<std::endl;
}

void move_directory(std::string dirname, std::string newpath){
    std::experimental::filesystem::rename(dirname,newpath);
}