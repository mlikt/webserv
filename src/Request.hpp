#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;
class Request {
public:
	Request() {};
	~Request() {};
	Request(char *buf);
	std::string request;
	std::string METHOD;
	std::string URI;
	std::string HTTP_PROTOCOL;
	int parseStartLine(void);
};

#endif
