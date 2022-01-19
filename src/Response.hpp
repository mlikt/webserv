#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
class Response {
public:
	Response();
	~Response();
	std::string response;
	void formResponse(std::string staticPageFolder);
	std::string fullResponse;
};
#endif
