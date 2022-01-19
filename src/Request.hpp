#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include<dirent.h>
using namespace std;
class Request {
public:
	Request() {
		this->request = "";
	};
	~Request() {};
	Request(char *buf);
	int checkReqPath();
	std::string request;
	std::string METHOD;
	std::string URI;
	std::string HTTP_PROTOCOL;
	std::string staticPageFolder;
	int parseStartLine(void);
};

#endif
