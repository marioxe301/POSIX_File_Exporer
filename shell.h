#ifndef SHELL_H
#define SHELL_H

#include <vector>
#include <string>

class SHELL{
    public:
        SHELL(std::string initial_path);
        SHELL();
        
        void ls();
        void touch(std::string filename);
        void rm_file(std::string filename);
        void cd(std::string path);
        void mkdir(std::string dir_name);
        void mkHlink(std::string source,std::string link);
        void mkSlink(std::string source,std::string link);

    private:
        std::vector<std::string> navigation;
};

#endif