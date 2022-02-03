#ifndef WEBSERV_HEADERS_HPP
#define WEBSERV_HEADERS_HPP
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>

std::vector<std::string> customSplit (std::string s, std::string delimiter);
bool isFileExists (const std::string& name);
#endif
