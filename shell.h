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

        //Extra Functions
        std::string get_folder_name();

    private:
        std::vector<std::string> navigation;
};

#endif