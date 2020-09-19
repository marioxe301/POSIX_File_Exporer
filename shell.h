#ifndef SHELL_H
#define SHELL_H

#include <vector>
#include <string>

class SHELL{
    public:
        SHELL(std::string initial_path);
        SHELL();
        ~SHELL();
        
        void ls(std::vector<std::string>&files,std::vector<std::string>&folder); // puede recibir de parametro dos vectores por referencia para llenarlos
        void touch(std::string filename);
        void rm_file(std::string filename);
        void rm_dir(std::string dirname);
        void cd(std::string path);
        void mkdir(std::string dir_name);
        void mkHlink(std::string source,std::string link);
        void mkSlink(std::string source,std::string link);

        void open(std::string filename);

        //Extra Functions
        std::string get_folder_name();
        std::string get_absolute_path();
        void cp_file(std::string from,std::string to);
        void cp_folder(std::string from,std::string to);
        void mv_file(std::string from,std::string to);
        void mv_folder(std::string from,std::string to);

    private:
        std::vector<std::string> navigation;
};

#endif