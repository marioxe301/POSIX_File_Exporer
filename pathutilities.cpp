#include "pathutilities.h"

std::string convert_stack_to_path(std::vector<std::string>&navigation){
    std::string path="";
    for (int i = 0; i < navigation.size(); i++)
    {
        path = path + "/"+ navigation[i];
    }
    path+="/";
    return path;
}

std::vector<std::string> convert_path_to_stack(std::string path){
    std::vector<std::string> naviagation;
    size_t pos = 0;
    std::string token;
    std::string temp = path;
    temp.erase(temp.begin());
    while ((pos = temp.find('/')) != std::string::npos)
    {
        token = temp.substr(0,pos);
        naviagation.push_back(token);
        temp.erase(0,pos+1);
    }

    return naviagation;
    
}

bool is_path(std::string path){
    if( path.find('/') != std::string::npos){
        return true;
    }else{
        return false;
    }
}