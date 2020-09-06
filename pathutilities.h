#ifndef PATHU_H
#define PATHU_H

#include <vector>
#include <string>

std::string convert_stack_to_path(std::vector<std::string>&navigation);
std::vector<std::string> convert_path_to_stack(std::string path);
bool is_path(std::string path);


#endif