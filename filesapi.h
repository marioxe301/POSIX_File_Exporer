#ifndef FILESAPI_H
#define FILESAPI_H

#include <string>
#include <vector>

void create_file(std::string filename);
void make_directory(std::string directory);
void remove_file(std::string filename);
void remove_directory(std::string directory);
void create_hard_link(std::string link_name,std::string source);
void create_symbolic_link(std:: string link_name, std::string source);
void open_file(std::string filename);
void copy_file(std::string filename,std::string newpath);
void copy_directory(std::string dirname, std::string newpath);
void move_file(std::string filename,std::string newpath);
void move_directory(std::string dirname, std::string newpath);


std::vector<std::string> get_files_from_path(std::string path);
std::vector<std::string> get_directories_from_path(std::string path);

#endif